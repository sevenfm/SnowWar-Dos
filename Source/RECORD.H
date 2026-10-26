// Record.h

class Record{
  private:
    long Start;
    long Size;
    Boolean Free;
    Record *Prev,*Next;
  public:
    Record(void);
    Record(long Start,long Size, Boolean Free,
           Record *Prev,Record *Next);
    void Split(long Size);
    long GetStart(void);
    long GetSize(void);
    void FreeMem(void);
    Boolean IsFree(void);
    void Empty(void);
    Record *Find(long Size);
}

Record::Record(void){
  Start=0;
  Size=0;
  Free=FALSE;
  Prev=Next=this;
}

Record::Record(long Start,long Size, Boolean Free,
               Record *Prev,Record *Next){
  Record::Start=Start;
  Record::Size=Size;
  Record::Free=Free;
  Record::Prev=Prev;
  Record::Next=Next;
  Prev->Next=this;
  Next->Prev=this;
}

void Record::Split(long Size){
  Record *Temp;
  if(Record::Size-Size>0){
    Temp=new Record(Start+Size,Record::Size-Size,TRUE,this,Next);
    if(Temp==NULL){
      ErrorReport("Temp=NULL");
      exit(EXIT_FAILURE);
    }
    Next=Temp;
  }

  Free=FALSE;
  Record::Size=Size;
}

long Record::GetStart(void){
  return Start;
}

long Record::GetSize(void){
  return Size;
}

void Record::FreeMem(void){
  Record *Temp;

  Free=TRUE;
  while(Next->IsFree()==TRUE){
    Temp=Next;
    Size+=Next->Size;
    Next=Next->Next;
    delete Temp;
  }
}

void Record::Empty(void){
  Record *Temp;
  while(Next->Size!=0){
    Temp=Next;
    Next=Next->Next;
    delete Temp;
  }
}

Boolean Record::IsFree(void){
  return Free;
}

Record *Record::Find(long Size){

  Record *Cur=this->Next;

  while(Cur->Size!=0 && (!Cur->Free || Cur->Size<Size)){
    Cur=Cur->Next;
  }

  if(Cur->Size>=Size){
    return Cur;
  }
  return NULL;
}