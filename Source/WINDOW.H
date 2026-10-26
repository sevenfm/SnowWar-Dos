class Window{
  private:
    Coord LeftUp, RightDown;
    Picture **PressPic;
    int WindowType;
  public:
    Window(Coord LU, Coord RD,Picture **Pic);
    Window(Coord LU, Coord RD,int WT=BUTTON);
    int InWindow(Coord Point);
    void PressButton(void);
    int GetWindowType(void);
};

Window::Window(Coord LU, Coord RD,Picture **Pic){
  LeftUp=LU;
  RightDown=RD;
  PressPic=Pic;
  WindowType=BUTTON;
}

Window::Window(Coord LU, Coord RD,int WT){
  LeftUp=LU;
  RightDown=RD;
  PressPic=NULL;
  WindowType=WT;
}


int Window::InWindow(Coord Point){
  if(LeftUp.GetX()<=Point.GetX() && LeftUp.GetY()<=Point.GetY() &&
     RightDown.GetX()>=Point.GetX() && RightDown.GetY()>=Point.GetY())
    return TRUE;
  return FALSE;
}

void Window::PressButton(void){
  if(PressPic==NULL) return;
  if(*PressPic==NULL) return;
  (*PressPic)->Move(LeftUp);
  (*PressPic)->Show();
  delay(300);
  (*PressPic)->Hide();
}

int Window::GetWindowType(void){
  return WindowType;
}

Window TopTenWindows[1]={Window(Coord(0,0),Coord(639,479),WINDOW)};

Window  MainWindows[4]={Window(Coord(480,300),Coord(639,479)),
   Window(Coord(350,90),Coord(519,189)),
   Window(Coord(190,150),Coord(299,219)),
   Window(Coord(0,420),Coord(199,479))};

Window AboutWindows[1]={Window(Coord(0,0),Coord(639,479),WINDOW)};

Window GameWindows[3]={Window(Coord(0,440),Coord(79,479),&Button[0]),
   Window(Coord(560,440),Coord(639,479),&Button[1]),
   Window(Coord(20,20),Coord(619,419),WINDOW)};
