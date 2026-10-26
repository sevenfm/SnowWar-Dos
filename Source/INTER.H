// **********************
// Интерфейс пользователя
// **********************



class UserInterface{
  private:
    Graphics *Driver;
    void LoadArray(Picture *Array[],const char *BaseName,
                   const char *Ext,int Number);
    void FreeArray(Picture *Array[],int Number);
  public:
    UserInterface();
    ~UserInterface();
    void Run(void);              // запуск программы
};



// Реализации

UserInterface::UserInterface(){
  InitErrorReport();

  Ems=new EmsDriver;
  Driver=new Graphics;
  MyMouse=new Mouse;

  LoadArray(PicHuman,"pic_h",".raw",SNOWMAN_TYPE);
  LoadArray(PicComputer,"pic_c",".raw",SNOWMAN_TYPE);
  LoadArray(Status,"status",".raw",10);
  LoadArray(Button,"button",".raw",2);
  LoadArray(BonusPic,"bonus",".raw",BONUS_TYPE);
  LoadArray(Bullet,"bullet",".raw",SNOWMAN_TYPE);
  LoadArray(Expl,"expl",".raw",SNOWMAN_TYPE);
  CurPic=new Picture("curpic.raw");
  Cursor=new Picture("cursor.raw");
  LoadArray(CellPic,"cell",".raw",CELL_TYPE);
  LoadArray(Flag,"flag",".raw",2);

}

UserInterface::~UserInterface(){

  FreeArray(BonusPic,BONUS_TYPE);
  FreeArray(PicHuman,SNOWMAN_TYPE);
  FreeArray(PicComputer,SNOWMAN_TYPE);
  FreeArray(Bullet,SNOWMAN_TYPE);
  FreeArray(Expl,SNOWMAN_TYPE);
  delete CurPic;
  delete Cursor;
  FreeArray(Status,10);
  FreeArray(CellPic,CELL_TYPE);
  FreeArray(Flag,2);
  FreeArray(Button,2);

  delete MyMouse;  // мышеловка :-)
  delete Driver;
  delete Ems;

}

void UserInterface::LoadArray(Picture *Array[],const char *BaseName,
                              const char *Ext,int Number){
  char FileName[20];
  char Buf[20];
  for(int i=0;i<Number;i++){
    FileName[0]=0;
    Buf[0]=0;
    strcat(FileName,BaseName);
    itoa(i,Buf,10);
    strcat(FileName,Buf);
    strcat(FileName,Ext);
    Array[i]=new Picture(FileName);
  }
}

void UserInterface::FreeArray(Picture *Array[],int Number){
  for(int i=0;i<Number;i++)
    if(Array[i]!=NULL)
      delete Array[i];
}

void UserInterface::Run(void){

  DeskTop MainMenu(4,MainWindows,"main.raw","main.pal",
                  STANDART_CURSOR);
  DeskTop AboutUs(1,AboutWindows,"about.raw","about.pal",ABOUT_CURSOR);

    TopTen TopScore;
    Battle Game;

  int Points,Place,NumbHuman,NumbComputer;

  enum{GAME,TOPTEN,ABOUTUS,QUIT};

  int UserChoice;
  do{
  MainMenu.Show();
  UserChoice=MainMenu.GetOption();

    if(UserChoice==GAME){
      MainMenu.Hide();
      if(Game.Game(&NumbHuman,&NumbComputer)==HUMAN_WIN){
         Points=TopScore.NumbPoints(Game.GetTurn(),NumbHuman,NumbComputer);
         Place=TopScore.PlaceInTen(Points);
         if(Place<=NUM_REC){
           TopScore.InsertNew(Place,Points);
         }
      }

    }

    if(UserChoice==TOPTEN){
      MainMenu.Hide();
      TopScore.ShowTen();
    }

    if(UserChoice==ABOUTUS){
      MainMenu.Hide();
      AboutUs.Show();
      AboutUs.GetOption();
      AboutUs.Hide();
    }
  }while(UserChoice!=QUIT);

   MainMenu.Hide();
}





