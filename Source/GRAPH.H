
// Графика
// *******

class Font{
  private:
    VariableFont* UserFont;
  public:
    Font(char* FontName);
    ~Font();
    void PutText(Coord Point,char* Text);
    void GetText(Coord Point,char* string);
};

class Image{
  private:
    void far *BitMap;
    Record *EmsBlock;
    int Width,Height;
    int MemType;
  public:
    Image(char *FileName,int MemFlag=EMS_MEMORY);
    Image(int Width,int Height,int MemFlag=EMS_MEMORY);
    ~Image();
    void *GetBitMap(void);
    int GetWidth(void);
    int GetHeight(void);
};


class Picture{                      // картинка
  private:
    Image *Pic;
    Image *BackGround;
    Coord CurPoint;
    int Visible;
  public:
    Picture(char *FileName,Coord Point); // создает из файла
    ~Picture();                     // освобождает память
    void Show(Coord Point);         // выводит на экран
    void ShowShd(Coord Point);      // выводит на экран без черного цвета
    void Show(void);                // выводит и запоминает, что под ней
    void ShowShd(void);             // Show без вывода черного цвета
    void Hide();                    // Убирает картинку с экрана
    void Move(Coord NewPoint);      // передвигает снеговика
    void MoveShd(Coord NewPoint);   // Move без вывода черного цвета
    void Slide(Coord NewPoint,int Speed);  // плавно передвигает
                                                    // картинку
};

class FilePicture{
  private:
    char* FileName;                     // имя файла с картинкой
  public:
    FilePicture(char* FileName);        // задать имя файла
    void Show(void);                    // вывести картинку из файла
};

class Palette{                      // палитра
  private:
    void *Pal;                      // указатель на палитру в памяти
    void WaitRetrace(void);
  public:
    Palette(char *FileName);        // создает из файла
    ~Palette();                     // освобождает память
    void Apply(void);               // делает палитру текущей
    void Show(void);
    void Hide(void);                // медленно гасит экран
    void SetBlack(void);
};

class Graphics{                         // графика
  public:
    Graphics(void);                  // инициализирует графику и мышь
    ~Graphics(void);                 // восстанавливает исходное состояние
};

class Mouse{                      // мышь
  private:
    Image *MouseCursor[MOUSE_CURSOR];                 // курсор
    int CurrentCursor;
    int Visible;
  public:
    Mouse();                      // создает мышь
    ~Mouse();                     // освобождает память
    void ShowCursor(void);        // показывать курсор
    void HideCursor(void);        // не показывать курсор
    int GetClick(void);           // TRUE - если была нажата кнопка
    Coord GetCoord(void);         // получить координаты курсора
    void SetCursor(int NewCursor); // изменить курсор
};

// Реализации

Graphics::Graphics(void) {

  if(loadGraphDriver("640x480.drv")!=TGE_SUCCESS){
    cout<<"\nError loading graphics driver 640x480.drv!\n";
    exit(EXIT_FAILURE);
  }else{
    atexit(unloadGraphDriver);
  }

  if(!initGraphics()){
    cout<<"\nError in graphics initialization!\n";
    exit(EXIT_FAILURE);
  }

  setViewports(0,0,OUTMAXX,OUTMAXY);

  if(resetMouse()){
    initNewMouse();
    atexit(deInitNewMouse);
    setHorizLimitsMouse(0,OUTMAXX);
    setVertLimitsMouse(0,OUTMAXY);
    setPosMouse(OUTMAXX/2,OUTMAXY/2);
  }
}

Graphics::~Graphics(void){
  deInitGraphics();
  deInitNewMouse();
}

Mouse::Mouse(){
  for(int i=0;i<MOUSE_CURSOR;i++)
    MouseCursor[i]=new Image(CursorName[i],CONV_MEMORY);
  Visible=INVISIBLE;
}

Mouse::~Mouse(){
  for(int i=0;i<MOUSE_CURSOR;i++)
    delete MouseCursor[i];
}

void Mouse::ShowCursor(void){
  if(Visible==INVISIBLE){
    showMouse();
    Visible=VISIBLE;
  }
}

void Mouse::HideCursor(void){
  if(Visible==VISIBLE){
    hideMouse();
    Visible=INVISIBLE;
  }
}

int Mouse::GetClick(void){
  Coord Point;
  int x,y;

  if(leftButtonMouse()){
    waitReleaseMouse(LEFTBUTTON);
    return TRUE;
  }
  return FALSE;
}

Coord Mouse::GetCoord(void){
   int x,y;
   getPosMouse(&x,&y);
   return Coord(x,y);
}

void Mouse::SetCursor(int NewCursor){
  CurrentCursor=NewCursor;
  HideCursor();
  setPointerMouse(0,0,MouseCursor[CurrentCursor]->GetBitMap());
  ShowCursor();
}

Picture::Picture(char *FileName,Coord Point=Coord(0,0)){

  Pic=new Image(FileName);
  BackGround=new Image(Pic->GetWidth(),
                       Pic->GetHeight());

  CurPoint=Point;
  Visible=INVISIBLE;

}

Picture::~Picture(){
  delete Pic;
  delete BackGround;
}

void Picture::Show(void){
  if(Visible==INVISIBLE){
      getImage(CurPoint.GetX(),
               CurPoint.GetY(),
               CurPoint.GetX()+Pic->GetWidth()-1,
               CurPoint.GetY()+Pic->GetHeight()-1,
               BackGround->GetBitMap());
    putImage(CurPoint.GetX(),
             CurPoint.GetY(),
             Pic->GetBitMap());
    Visible=VISIBLE;
  }
}

void Picture::ShowShd(void){
  if(Visible==INVISIBLE){
      getImage(CurPoint.GetX(),
               CurPoint.GetY(),
               CurPoint.GetX()+Pic->GetWidth()-1,
               CurPoint.GetY()+Pic->GetHeight()-1,
               BackGround->GetBitMap());
    putImageInv(CurPoint.GetX(),
                CurPoint.GetY(),
                Pic->GetBitMap());
    Visible=VISIBLE;
  }
}

void Picture::Show(Coord Point){
  putImage(Point.GetX(),
           Point.GetY(),
           Pic->GetBitMap());
}

void Picture::ShowShd(Coord Point){
  putImageInv(Point.GetX(),
              Point.GetY(),
              Pic->GetBitMap());
}

void Picture::Hide(){
  if(Visible==VISIBLE){
      putImage(CurPoint.GetX(),
               CurPoint.GetY(),
               BackGround->GetBitMap());
      Visible=INVISIBLE;
  }
}

void Picture::Move(Coord NewPoint){
  if(Visible==VISIBLE){
    Hide();
    CurPoint=NewPoint;
    Show();
  }else{
    CurPoint=NewPoint;
  }
}

void Picture::MoveShd(Coord NewPoint){
  if(Visible==VISIBLE){
    Hide();
    CurPoint=NewPoint;
    ShowShd();
  }else{
    CurPoint=NewPoint;
  }
}


FilePicture::FilePicture(char* FileName){
  FilePicture::FileName=FileName;
}

void FilePicture::Show(void){
char* PointPos;
int result;
  char Name[100];
  Name[0]=0;
  strcat(Name,"graph\\");
  strcat(Name,FileName);

  PointPos=strchr(FileName,'.');
  PointPos++;

  if(!strcmp(PointPos,"raw"))
    result=displayRawFile(0,0,Name);
  if(!strcmp(PointPos,"pcx"))
    result=displayPcxFile(0,0,Name);
  if(!result){
    ErrorReport(FileName);
    exit(EXIT_FAILURE);
  }
}

Palette::Palette(char *FileName){
  char Name[100];
  Name[0]=0;
  strcat(Name,"graph\\");
  strcat(Name,FileName);
  Pal=loadPalFile(Name,NULL);
  if(Pal==NULL){
    ErrorReport(FileName);
    exit(EXIT_FAILURE);
  }
}

Palette::~Palette(){
  free(Pal);
}

void Palette::Apply(void){
  WaitRetrace();
  setBlockPalette(0,255,Pal);
}

void Palette::Show(void){
  unsigned char BufPal[768],CurPal[768];
  memset(BufPal,0x00,768);
  memset(CurPal,0x00,768);
  setBlockPalette(0,255,BufPal);
  while(fadePalette(8,(void*)BufPal,(void*)CurPal,Pal)){
    WaitRetrace();
    setBlockPalette(0,255,CurPal);
    memcpy(BufPal,CurPal,768);
  }

}

void Palette::Hide(void){
  unsigned char BlackPal[768],CurPal[768],BufPal[768];
  memset(BlackPal,0x00,768);
  memcpy(CurPal,Pal,768);
  memcpy(BufPal,Pal,768);
  while(fadePalette(8,(void*)BufPal,(void*)CurPal,BlackPal)){
    WaitRetrace();
    setBlockPalette(0,255,CurPal);
    memcpy(BufPal,CurPal,768);
  }
}

void Palette::SetBlack(void){
  unsigned char BlackPal[768];
  memset(BlackPal,0x00,768);
  WaitRetrace();
  setBlockPalette(0,255,BlackPal);
}

void Palette::WaitRetrace(void)
{
  asm mov   dx,3DAh
          WaitVerticalRetrace1:
  asm in    al,dx                           // get status
  asm and   al,08h
  asm jnz   WaitVerticalRetrace1            // loop if necessary
  asm mov   dx,3DAh
          WaitVerticalRetrace2:
  asm in    al,dx                           // get status
  asm and   al,08h
  asm jz    WaitVerticalRetrace2            // loop if necessary
}

Font::Font(char* FontName){
  char Name[100];
  Name[0]=0;
  strcat(Name,"graph\\");
  strcat(Name,FontName);
  UserFont=new VariableFont();
  if(!UserFont->load(Name)){
    ErrorReport(FontName);
    exit(EXIT_FAILURE);
  }
}

Font::~Font(){
  delete UserFont;
}

void Font::PutText(Coord Point,char* Text){
  UserFont->matchColours();
  UserFont->put(Point.GetX(),Point.GetY(),Text);
}

void Font::GetText(Coord Point,char* string){
   int Ch,
       ChNum=0;
  UserFont->matchColours();
  do{
    Cursor->Move(Point+Coord(0,UserFont->maxHeight()));
    Cursor->ShowShd();
    do{
      Ch=getch();
    }while(!((Ch>='a' && Ch<='z') || (Ch>='A' && Ch<='Z') || Ch==' ' ||
             (Ch>='0' && Ch<='9') || Ch==13));
    Cursor->Hide();
    if(Ch!=13){
      UserFont->put(Point.GetX(),Point.GetY(),(char) Ch);
      string[ChNum++]=Ch;
    }
    Point=Point+Coord(UserFont->width((char) Ch)+UserFont->spacing(),0);
  }while(Ch!=13 && ChNum<MAX_NAME-1);
    string[ChNum]=0;
}

void Picture::Slide(Coord NewPoint,int Speed){
  if(CurPoint==NewPoint) return;

  ShowShd();

  int StartX=CurPoint.GetX(),
      StartY=CurPoint.GetY(),
      Dx=NewPoint.GetX()-StartX,
      Dy=NewPoint.GetY()-StartY;

  float DxDivDy,DyDivDx;


  if(abs(Dx)>abs(Dy)){
    DyDivDx=(float)Dy/(float)abs(Dx);
    for(int i=0;abs(i)<=abs(Dx);i+=Dx/abs(Dx)){
      delay(Speed);
      MoveShd(Coord(StartX+i,StartY+abs(i)*DyDivDx));
    }
  }else{
    DxDivDy=(float)Dx/(float)abs(Dy);
    for(int j=0;abs(j)<=abs(Dy);j+=Dy/abs(Dy)){
      delay(Speed);
      MoveShd(Coord(StartX+abs(j)*DxDivDy,StartY+j));
    }
  }

  Hide();
}

Image::Image(char *FileName,int MemFlag){
  char* PointPos;
  char far *Temp;

  MemType=MemFlag;
  char Name[100];
  Name[0]=0;
  strcat(Name,"graph\\");
  strcat(Name,FileName);

  PointPos=strchr(FileName,'.');
  PointPos++;

  BitMap=NULL;

  if(!strcmp(PointPos,"pcx"))
    BitMap=(char*) loadPcxFile(Name,NULL);

  if(!strcmp(PointPos,"raw"))
    BitMap=(char*) loadRawFile(Name);

  if(BitMap==NULL){
    ErrorReport(FileName);
    exit(EXIT_FAILURE);
  }
  Width=imageWidth(BitMap);
  Height=imageHeight(BitMap);

  if(MemType==EMS_MEMORY){
    EmsBlock=Ems->Alloc(long(Width)*long(Height)+4);
    if(EmsBlock==NULL){
      ErrorReport("EmsBlock==NULL");
      exit(EXIT_FAILURE);
    }
    Temp=Ems->Map(EmsBlock);
    memcpy(Temp,BitMap,Width*Height+4);
    free(BitMap);
  }
}

Image::Image(int Width,int Height,int MemFlag){
  char far *Temp;
  MemType=MemFlag;
  BitMap=makeVirtScreen(Width,Height);
  if(BitMap==NULL){
    ErrorReport("Error creating bitmap");
    exit(EXIT_FAILURE);
  }
  if(MemType==EMS_MEMORY){
    EmsBlock=Ems->Alloc(long(Width)*long(Height)+4);
    if(EmsBlock==NULL){
      ErrorReport("EmsBlock==NULL");
      exit(EXIT_FAILURE);
    }
    Temp=Ems->Map(EmsBlock);
    memcpy(Temp,BitMap,Width*Height+4);
    free(BitMap);
  }
}

Image::~Image(){
  if(MemType==EMS_MEMORY){
    if(EmsBlock!=NULL)
      Ems->Free(EmsBlock);
  }
  if(MemType==CONV_MEMORY){
    if(BitMap!=NULL)
      free(BitMap);
  }
}

void* Image::GetBitMap(void){
  if(MemType==EMS_MEMORY){
    BitMap=Ems->Map(EmsBlock);
  }
  return BitMap;
}

int Image::GetWidth(void){
  return Width;
}

int Image::GetHeight(void){
  return Height;
}
