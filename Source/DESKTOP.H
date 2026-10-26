class DeskTop{
  private:
    int NumbWind;                      // кол-во окон
    Window *Windows;                   // массив, задающий окна
    FilePicture* Pic;                  // картинка экрана
    Palette* Pal;                      // палитра экрана
    int WhatWindowClick(Coord Point);  // в каком окне нажата кнопка
    Coord LastPoint;
    int DefaultCursor,
        ButtonCursor;
  public:
    DeskTop(int NW, Window *W,char *FileName,char *PalName,
	    int DC);
    DeskTop(int NW, Window *W,char *FileName,char *PalName,
	    int DC, int BC);
    ~DeskTop();
    void Show(void);                   // вывод картинки
    void Hide(void);                   // гашение экрана
    int GetOption(int WindowCurs[MAX_Y][MAX_X]=NULL);  // возвращает номер окна
    Coord GetPoint(void);              // Точка последнего нажатия
};

DeskTop::DeskTop(int NW, Window *W,char *FileName,char *PalName,
                 int DC){
  NumbWind=NW;
  Windows=W;
  Pic=new FilePicture(FileName);
  Pal=new Palette(PalName);
  LastPoint=Coord(0,0);
  DefaultCursor=DC;
  ButtonCursor=DC;
}

DeskTop::DeskTop(int NW, Window *W,char *FileName,char *PalName,
                 int DC,int BC){
  NumbWind=NW;
  Windows=W;
  Pic=new FilePicture(FileName);
  Pal=new Palette(PalName);
  LastPoint=Coord(0,0);
  DefaultCursor=DC;
  ButtonCursor=BC;
}


DeskTop::~DeskTop(){
  delete Pic;
  delete Pal;
}

void DeskTop::Show(void){
  Pal->SetBlack();
  Pic->Show();
  Pal->Show();
}

void DeskTop::Hide(void){
  Pal->Hide();
  Pal->SetBlack();
}

int DeskTop::GetOption(int WindowCurs[MAX_Y][MAX_X]){
  int W,
      State=NOTHING,
      GameState=DefaultCursor,NewState=GameState;
  int CurCursor=DefaultCursor;

  MyMouse->SetCursor(CurCursor);
  MyMouse->ShowCursor();

  do{

    do{

      LastPoint=MyMouse->GetCoord();
      W=WhatWindowClick(LastPoint);

      if(State!=NOTHING && W==-1){
        State=NOTHING;
        GameState=DefaultCursor;
        MyMouse->SetCursor(DefaultCursor);
      }

      if(State!=BUTTON && W!=-1 && Windows[W].GetWindowType()==BUTTON){
        State=BUTTON;
        GameState=ButtonCursor;
	MyMouse->SetCursor(ButtonCursor);
      }

      if(W==MAP_WINDOW && WindowCurs!=NULL){
        NewState=WindowCurs[(LastPoint.GetMap()).GetY()]
                           [(LastPoint.GetMap()).GetX()];

	if(State!=WINDOW){
          State=WINDOW;
          MyMouse->SetCursor(NewState);
        }
        if(GameState!=NewState){
	  MyMouse->SetCursor(NewState);
        }
        GameState=NewState;
      }



    }while(!MyMouse->GetClick());

  }while(W==-1);

  MyMouse->HideCursor();

  Windows[W].PressButton();

  return W;
}


int DeskTop::WhatWindowClick(Coord Point){
  for(int i=0;i<NumbWind;i++)
    if(Windows[i].InWindow(Point))
      return i;
  return -1;
}

Coord DeskTop::GetPoint(void){
  return LastPoint;
}
