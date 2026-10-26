// Ems Driver

class EmsDriver{
  private:
    int MaxPages;
    int Handle;
    char far *Frame;
    Record *Head;
    Boolean DetectEms(void);
    char far *GetEmsFrame(void);
    int GetEmsPages(void);
    int EmsAlloc(int Pages);
    Boolean EmsMap(int Handle,char LogicNum,int PageNum);
    Boolean EmsFree(unsigned int Handle);
  public:
    EmsDriver(void);
    ~EmsDriver();
    Record *Alloc(long Size);
    char *Map(Record *Rec);
    void Free(Record *Rec);
}

// Code

EmsDriver::EmsDriver(void){
  Record *First;

  if(!DetectEms()){
    ErrorReport("Cannot find EMM386");
    exit(EXIT_FAILURE);
  }
  MaxPages=GetEmsPages();
  if(!MaxPages){
    ErrorReport("No free ems pages");
    exit(EXIT_FAILURE);
  }
  Handle=EmsAlloc(MaxPages);
  if(!Handle){
    ErrorReport("Cannot allocate ems pages");
    exit(EXIT_FAILURE);
  }
  Frame=GetEmsFrame();
  if(Frame==NULL){
    ErrorReport("Cannot get frame address");
    exit(EXIT_FAILURE);
  }


  Head=new Record();
  if(Head==NULL){
    ErrorReport("Head==NULL!");
    exit(EXIT_FAILURE);
  }

  First=new Record(long(0),long(MaxPages)*long(PAGE_SIZE),TRUE,Head,Head);
  if(First==NULL){
    ErrorReport("First==NULL");
    exit(EXIT_FAILURE);
  }

}

EmsDriver::~EmsDriver(){
  if(EmsFree(Handle)==FALSE){
    ErrorReport("EmsFree(Handle) fails");
  }

  Head->Empty();

  delete Head;

}

Record *EmsDriver::Alloc(long Size){
  Record *Rec;
  Rec=Head->Find(Size);
  if(Rec==NULL)
    return NULL;

  Rec->Split(Size);
  return Rec;
}

char *EmsDriver::Map(Record *Rec){
  int FirstSeg=Rec->GetStart()/PAGE_SIZE+1;
  int LastSeg=(Rec->GetStart()+Rec->GetSize()-1)/PAGE_SIZE+1;
  int SegLen=LastSeg-FirstSeg+1;
  int SegOffset=Rec->GetStart()-(FirstSeg-1)*PAGE_SIZE;
  if(SegLen>4){
    ErrorReport("Data length is > than 4 segments");
    exit(EXIT_FAILURE);
  }
  for(int i=0;i<SegLen;i++)
    if(!EmsMap(Handle,i,FirstSeg+i)){
      ErrorReport("Cannot map segment");
      exit(EXIT_FAILURE);
    }
  return (char far*)(Frame+SegOffset);
}

void EmsDriver::Free(Record *Rec){
  if(Rec!=NULL) Rec->FreeMem();
}



Boolean EmsDriver::DetectEms(void){
  unsigned int EmsVect;
  char far *EmsDriver;

  EmsVect=FP_SEG(getvect(0x67));

  if(!EmsVect) return FALSE;  // NULL interrupt vector

  EmsDriver=(char *) MK_FP(EmsVect,0x0a);

  if(memcmp(EmsDriver,"EMMXXXX0",8))  // EMM386 not loaded!
    return FALSE;

  return TRUE;
} // end DetectEms

char far *EmsDriver::GetEmsFrame(void){
  _AH=0x41;
  geninterrupt(0x67);
  if(_AH!=0) return NULL;
  return ((char far*)MK_FP(_BX,0));
}  // end GetEmsFrame

int EmsDriver::GetEmsPages(void){
  _AH=0x42;
  geninterrupt(0x67);
  if(_AH!=0) return 0;
  return _BX;
}

int EmsDriver::EmsAlloc(int Pages){
  _BX=Pages;
  _AH=0x43;
  geninterrupt(0x67);
  if(_AH!=0) return 0;
  return _DX;
}

Boolean EmsDriver::EmsMap(int Handle,char LogicNum,int PageNum){
  _AH=0x44;
  _DX=Handle;
  _AL=LogicNum;
  _BX=PageNum;
  geninterrupt(0x67);
  if(_AH!=0) return FALSE;
  return TRUE;
}

Boolean EmsDriver::EmsFree(unsigned int Handle){
  _DX=Handle;
  _AH=0x45;
  geninterrupt(0x67);
  if(_AH!=0) return FALSE;
  return TRUE;
}
