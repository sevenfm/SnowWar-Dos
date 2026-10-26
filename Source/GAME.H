// ****
// Игра
// ****

class SnowMan{                   // снеговик
  private:
    int Side;
    int Type;                    // тип снеговика
    int AP;                      // Action Points
    int Health;                  // здоровье
    Coord Pos;                   // позиция снеговика (x,y)
    void TakeBonus(int BonusType);  // изменяет тип снеговика
  public:
    SnowMan(Coord Pos,int SnowManSide);          // создает снеговика
    void Show(void);          // вывести снеговика на экран
    void ShowStatus(void);    // выводит статус снеговика
    void HideStatus(void);       // выводит пустой статус
    void Hide(void);             // стереть с экрана
    void Move(Coord NewPos);     // двигает снеговика
    void Fire(SnowMan *Target);  // стреляет
    void SetCurrent(void);       // сделять текущим,т.е. показать рамку
    void ResetCurrent(void);     // сделать не текущим,т.е. вывести заново все
    int GetHealth(void);         // возвращает здоровье
    int GetAP(void);             // возвращает Action Points
    void ResetAP(void);
    int CanMove(Coord NewPos);      // может ли идти
    int CanFire(Coord TargetPos);    // или стрелять
    Coord GetPos(void);             // возвращает позицию снеговика
    int GetType(void) { return Type; }  // тип снеговика
};

class Army{                              // армия - базовый класс
  protected:
    SnowMan *Soldiers[MAX_SOLD];         // снеговики в армии
    int NumbSoldiers;                    // кол-во снеговиков в армии
  public:
    Army(int Side);                      // располагает снеговиков
    ~Army();                             // уничтожает снеговиков
    void Show(void);                     // показывает всех снеговиков
    void KillSoldier(SnowMan *Soldier);  // уничтожает снеговика
    int GetNumbSold(void);               // сколько солдат в армии
    void ResetAP(void);
    SnowMan *WhatSnowMan(Coord Pos);
};

class ComputerArmy:public Army{          // армия компьютера
  friend class HumanArmy;
  private:
    HumanArmy *EnemyArmy;
    Coord FindNearest();
  public:
    ComputerArmy();
    int Move();             // 0 - игра продолжается, 1 - победа
    void SetEnemy(HumanArmy *Enemy);
};

class HumanArmy:public Army{             // армия человека
  friend class ComputerArmy;
  private:
    DeskTop *GameDeskTop;
    int GetSoldNumb(Coord Pos);
    ComputerArmy *EnemyArmy;
  public:
    HumanArmy(DeskTop *aDeskTop);
    void SetEnemy(ComputerArmy *Enemy);
    int Move();    // 0 - игра продолжается, 1 - победа, 2 - прервано
};

class Battle{             // сражение
  private:
    HumanArmy    *HArmy;   // армия человека
    ComputerArmy *CArmy;   // армия комьютера
    DeskTop *GameDeskTop;
    int NumbTurn;          // номер хода
    int Turn(void);        // одна итерация игры
  public:
    Battle();             // создает армии
    ~Battle();
    int Game(int *NumbHuman,int *NumbComputer);   // игра
    int GetTurn();        // возвращает номер хода
};


// Реализации

SnowMan::SnowMan(Coord Pos,int SnowManSide){
  Type=SNOW_MAN;
  Side=SnowManSide;
  AP=SnowType[Type].AP;
  Health=SnowType[Type].Health;
  SnowMan::Pos=Pos;
}

void SnowMan::Show(void){
  if(Side==HUMAN)
    PicHuman[Type]->ShowShd(Pos.GetScreen());
  else
    PicComputer[Type]->ShowShd(Pos.GetScreen());
}

void SnowMan::ShowStatus(void){
  Status[Type]->Show(Coord(80,440));
  for(int i=0;i<AP;i++) Status[AP_PIC]->ShowShd(Coord(480+i*10,440));
  for(;i<8;i++) Status[STATUS_EMPTY]->Show(Coord(480+i*10,440));

  for(i=0;i<Health;i++) Status[HEALTH_PIC]->ShowShd(Coord(320+i*10,440));
  for(;i<8;i++) Status[STATUS_EMPTY]->Show(Coord(320+i*10,440));

  for(i=0;i<SnowType[Type].CostToFire;i++)
    Status[COST_PIC]->ShowShd(Coord(480+i*10,460));
  for(;i<8;i++) Status[STATUS_EMPTY]->Show(Coord(480+i*10,460));

  for(i=0;i<SnowType[Type].Damage;i++)
    Status[DAMAGE_PIC]->ShowShd(Coord(320+i*10,460));
  for(;i<8;i++) Status[STATUS_EMPTY]->Show(Coord(320+i*10,460));
}

void SnowMan::HideStatus(void){
  Status[EMPTY_NAME]->Show(Coord(80,440));
  for(int i=0;i<8;i++) Status[STATUS_EMPTY]->Show(Coord(320+i*10,440));
  for(i=0;i<8;i++) Status[STATUS_EMPTY]->Show(Coord(320+i*10,460));
  for(i=0;i<8;i++) Status[STATUS_EMPTY]->Show(Coord(480+i*10,440));
  for(i=0;i<8;i++) Status[STATUS_EMPTY]->Show(Coord(480+i*10,460));
}

void SnowMan::Hide(void){
  MyMap->ShowCell(Pos);
}

void SnowMan::Move(Coord NewPos){
  Coord Path[MAX_PATH];
  int BonusType;

  int Steps=MyMap->FindPath(Path,NewPos);
  int Cost;
  int i=0;

  Hide();

  while(i<Steps && (MyMap->CostToMove(Path[i])+Pos.IsDiag(Path[i]))<=AP &&
        !MyMap->IsSnowMan(Path[i])) {

   if(Side==HUMAN)
      PicHuman[Type]->Move(Pos.GetScreen());
   else
      PicComputer[Type]->Move(Pos.GetScreen());

   AP-=MyMap->CostToMove(Path[i])+Pos.IsDiag(Path[i]);
    Pos=Path[i];
    if(Side==HUMAN)
      PicHuman[Type]->Slide(Pos.GetScreen(),15-(SnowType[Type].AP-6)*2);
    else
      PicComputer[Type]->Slide(Pos.GetScreen(),15-(SnowType[Type].AP-6)*2);
    BonusType=MyMap->IsBonus(Pos);
    if(BonusType!=-1){
      MyMap->RemoveBonus(Pos);
      TakeBonus(BonusType);
      MyMap->ShowCell(Pos);
    }


    i++;
  }
  Show();
}

void SnowMan::Fire(SnowMan *Target){
  int Distance=Pos.Distance(Target->GetPos()),
      Range=SnowType[Type].Range;
  if(random(Range)>=(Range+Distance)/3){
    Bullet[Type]->Move(Pos.GetScreen()+Coord(5,5));
    Bullet[Type]->Slide(Target->GetPos().GetScreen()+Coord(5,5),10);
    Expl[Type]->Move(Target->GetPos().GetScreen());
    Expl[Type]->ShowShd();
    delay(60);
    Expl[Type]->Hide();
    Target->Health-=SnowType[Type].Damage;
    if(Target->Health<0) Target->Health=0;
  }else{
    Bullet[Type]->Move(Pos.GetScreen()+Coord(5,5));
    Bullet[Type]->Slide(Target->GetPos().GetScreen()+Coord(5,5)+
       Coord(5*( random(2)*2-1),5*(random(2)*2-1)),10);
  }
  AP-=SnowType[Type].CostToFire;
  if(AP<0) AP=0;

}

void SnowMan::SetCurrent(void){
  CurPic->ShowShd(Pos.GetScreen());
}

void SnowMan::ResetCurrent(void){
  MyMap->ShowCell(Pos);
  Show();
}

int SnowMan::GetHealth(void){
  return Health;
}

int SnowMan::GetAP(void){
  return AP;
}

void SnowMan::ResetAP(void){
  AP=SnowType[Type].AP;
}

int SnowMan::CanMove(Coord NewPos){
//  Coord Path[MAX_PATH];
//  int PathLength=MyMap->FindPath(Path,NewPos);
//  if(!PathLength) return MAXINT;
//  for(int AP=0,i=0;i<PathLength;i++) AP+=MyMap->CostToMove(Path[i]);
//  return AP;
    return MyMap->GetFillCell(NewPos);
}

int SnowMan::CanFire(Coord TargetPos){
  return (Pos.Distance(TargetPos)<=SnowType[Type].Range) &&
         (SnowType[Type].CostToFire<=AP);
}

Coord SnowMan::GetPos(void){
  return Pos;
}

void SnowMan::TakeBonus(int BonusType){
  Type=BonusType+1;
  AP=0;
  Health=SnowType[Type].Health;
}

Battle::Battle():NumbTurn(0){
  GameDeskTop=new DeskTop(3,GameWindows,
                          "game.raw","game.pal",
                          GAME_CURSOR,CARROT_CURSOR);
}

Battle::~Battle(){
  delete GameDeskTop;
}

int Battle::Game(int *NumbHuman, int *NumbComputer){

  MyMap=new Map;
  HArmy=new HumanArmy(GameDeskTop);
  CArmy=new ComputerArmy;
  HArmy->SetEnemy(CArmy);
  CArmy->SetEnemy(HArmy);
  MyMap->SetArmy(CArmy,HArmy);

  GameDeskTop->Show();
  MyMap->ShowMap();
  CArmy->Show();
  HArmy->Show();
  int Result;

  do{
    Result=Turn();
  }while(Result==NOBODY_WIN);

  GameDeskTop->Hide();

  *NumbHuman=HArmy->GetNumbSold();
  *NumbComputer=CArmy->GetNumbSold();
  delete HArmy;
  delete CArmy;

  delete MyMap;

  return Result;
}

int Battle::Turn(void){
  int Result;
  while(TRUE){
    Result=HArmy->Move();
    HArmy->ResetAP();
    if(Result==VICTORY)
      return HUMAN_WIN;
    if(Result==ABORT)
      return COMPUTER_WIN;
    Result=CArmy->Move();
    CArmy->ResetAP();
    if(Result==VICTORY)
      return COMPUTER_WIN;
  }
}

HumanArmy::HumanArmy(DeskTop *aDeskTop):Army(HUMAN){
  GameDeskTop=aDeskTop;
}

void HumanArmy::SetEnemy(ComputerArmy *Enemy){
  EnemyArmy=Enemy;
}

int HumanArmy::Move(void){
  SnowMan *Current=NULL;
  SnowMan *Target;
  Coord Pos;
  int i,j,s;
  int Decision,Fire;
  int WindowCurs[MAX_Y][MAX_X];

  while(TRUE){
    if(Current!=NULL)
      MyMap->FillMap(Current->GetPos(),Current->GetAP());
    for(i=0;i<MAX_X;i++){
      for(j=0;j<MAX_Y;j++){
        WindowCurs[j][i]=SELECTOFF_CURSOR;
      }  // for j
    } // for i
    for(s=0;s<NumbSoldiers;s++)
      WindowCurs[Soldiers[s]->GetPos().GetY()]
                [Soldiers[s]->GetPos().GetX()]=SELECTON_CURSOR;
    if(Current!=NULL){
      for(i=0;i<MAX_X;i++){
        for(j=0;j<MAX_Y;j++){
          if(WindowCurs[j][i]==SELECTOFF_CURSOR){
            if(MyMap->GetFillCell(Coord(i,j))<=Current->GetAP())
              WindowCurs[j][i]=MOVEON_CURSOR;
            else
              WindowCurs[j][i]=MOVEOFF_CURSOR;
            Target=EnemyArmy->WhatSnowMan(Coord(i,j));
            if(Target!=NULL){
              if(Current->CanFire(Coord(i,j)))
                WindowCurs[j][i]=TARGETON_CURSOR;
              else
                WindowCurs[j][i]=TARGETOFF_CURSOR;
            }
          }
        }
      }

    }

    Decision=GameDeskTop->GetOption(WindowCurs);
    if(Decision==QUIT_GAME) return ABORT;
    if(Decision==END_TURN) {
      if(Current!=NULL){
        Current->ResetCurrent();
        Current->HideStatus();
      }
      return GAME_GO_ON;
    }

    if(Decision==MAP_WINDOW){
      Fire=FALSE;
      Pos=(GameDeskTop->GetPoint()).GetMap();
      if(WhatSnowMan(Pos)!=NULL){
        if(Current!=NULL)
          Current->ResetCurrent();
        Current=WhatSnowMan(Pos);
        Current->SetCurrent();
        Current->ShowStatus();
        MyMap->FillMap(Current->GetPos(),Current->GetAP());
      }
      Target=EnemyArmy->WhatSnowMan(Pos);
      if(Target!=NULL){
        if(Current!=NULL)
          if(Current->CanFire(Pos)){
            Fire=TRUE;
            Current->Fire(Target);
            Current->ShowStatus();
            if(Target->GetHealth()==0) EnemyArmy->KillSoldier(Target);
            if(EnemyArmy->GetNumbSold()==0) return VICTORY;
          }
      }
      if(!Fire && Current!=NULL){
        if(Current->CanMove(Pos)!=MAXINT && !MyMap->IsSnowMan(Pos)){
          Current->ResetCurrent();
          Current->Move(Pos);
          Current->SetCurrent();
          Current->ShowStatus();
          MyMap->FillMap(Current->GetPos(),Current->GetAP());
          if(Current->GetPos()==MyMap->CFlag) return VICTORY;
        }
      }
    }
  }
}


Army::Army(int Side){
  Coord Pos;
  char fl;
  int i;

  NumbSoldiers=0;
  for(int s=0;s<MAX_SOLD;s++){
    do{
      fl=0;
      Pos=Coord(random(MAX_X/2)+(MAX_X/2)*(Side==HUMAN),random(MAX_Y));
      for(i=0;!fl && i<NumbSoldiers;i++)
         fl=Soldiers[i]->GetPos() == Pos;
    }while((MyMap->CostToMove(Pos))==0 || fl || MyMap->IsBonus(Pos)!=-1);
    Soldiers[s]=new SnowMan(Pos,Side);
    NumbSoldiers++;
  }
}

Army::~Army(){
  for(int s=0;s<NumbSoldiers;s++)
    delete Soldiers[s];
}

SnowMan* Army::WhatSnowMan(Coord Pos){
  for(int i=0;i<NumbSoldiers;i++)
   if(Soldiers[i]->GetPos()==Pos) return Soldiers[i];
  return NULL;
}

void Army::ResetAP(void){
 for(int i=0;i<NumbSoldiers;i++)
   Soldiers[i]->ResetAP();
}

void Army::Show(void){
  for(int s=0;s<NumbSoldiers;s++)
    Soldiers[s]->Show();
}

void Army::KillSoldier(SnowMan *Soldier){
  Soldier->Hide();
  for(int s=0;s<NumbSoldiers;s++)
    if(Soldiers[s]==Soldier){
      delete Soldiers[s];
      for(int i=s;i<NumbSoldiers-1;i++)
        Soldiers[i]=Soldiers[i+1];
        NumbSoldiers--;
    }

}

int Army::GetNumbSold(void){
  return NumbSoldiers;
}

ComputerArmy::ComputerArmy():Army(COMPUTER){

}

void ComputerArmy::SetEnemy(HumanArmy *Enemy){
  EnemyArmy=Enemy;
}

int ComputerArmy::Move(void){
  Coord EnemyPos,NewPos;
  SnowMan *Enemy;
  Coord Target;
  int CanStep=TRUE;
  int es,b;
  Coord Path[MAX_PATH];
  int FoundPath;
  int FlagLive,FlagFire;
  int MinDist,NewDist;

  for(int s=0;s<NumbSoldiers;s++){
    CanStep=TRUE;

    // Строим карту движения
    MyMap->FillMap(Soldiers[s]->GetPos(),MAX_Y/2);

    do{
      // Можно ли захватить флаг противника
      if(Soldiers[s]->CanMove(MyMap->HFlag)<=Soldiers[s]->GetAP()){
        Soldiers[s]->Move(MyMap->HFlag);
        if( (Soldiers[s]->GetPos()) == (MyMap->HFlag) )
          return VICTORY;
      }

      do{
        // Стреляем в ближайшего, пока хватает AP
        // Ищем самого близкого
        EnemyPos=(EnemyArmy->Soldiers[0])->GetPos();
        MinDist=(Soldiers[s]->GetPos()).Dist2(EnemyPos);

        for(es=0;es<EnemyArmy->GetNumbSold();es++){
          NewPos=(EnemyArmy->Soldiers[es])->GetPos();
          NewDist=(Soldiers[s]->GetPos()).Dist2(NewPos);
          if(NewDist<=MinDist){
            EnemyPos=NewPos;
            MinDist=NewDist;
          }
        }

        FlagFire=Soldiers[s]->CanFire(EnemyPos);

        FlagLive=TRUE;  // то есть цель еще жива
        while(FlagLive && Soldiers[s]->CanFire(EnemyPos)){
          Enemy=EnemyArmy->WhatSnowMan(EnemyPos);
          Soldiers[s]->Fire(Enemy);
          if(Enemy->GetHealth()==0){
            EnemyArmy->KillSoldier(Enemy);
            if(EnemyArmy->NumbSoldiers==0) return VICTORY;
            FlagLive=FALSE;
          }
        }
      }while(FlagFire);

     // Если тип - снеговик, то ищем вокруг себя бонус
     if(Soldiers[s]->GetType()==SNOW_MAN)
      for(b=0;Soldiers[s]->GetAP()>0 && b<MAX_BONUS;b++){
        if(MyMap->Bonuses[b]!=NULL){
          if(Soldiers[s]->CanMove((MyMap->Bonuses[b])->GetPos())<=
             Soldiers[s]->GetAP()){
            Soldiers[s]->Move((MyMap->Bonuses[b])->GetPos());
            MyMap->FillMap(Soldiers[s]->GetPos(),MAX_Y/2);
          }
	}
      }

      // Идем к ближайшему снеговику
      if(Soldiers[s]->GetAP()>0){
	Target=FindNearest();
	FoundPath=MyMap->FindPath(Path,Target);
	if(FoundPath){
	  if(Soldiers[s]->CanMove(Path[0])<=Soldiers[s]->GetAP() &&
	     !MyMap->IsSnowMan(Path[0]) ){
	    Soldiers[s]->Move(Path[0]);
	    MyMap->FillMap(Soldiers[s]->GetPos(),MAX_Y/2);
	  }else{
	    CanStep=FALSE;
	  }
	}else{
	  CanStep=FALSE;
	}
      }else
	CanStep=FALSE;
    }while(CanStep);
  }
  return GAME_GO_ON;
}

Coord ComputerArmy::FindNearest(){
  int MinDist=MAXINT;
  int Dist;
  Coord Target;
  Coord MinCoord;
  for(int s=0;s<EnemyArmy->NumbSoldiers;s++){
   Target=(EnemyArmy->Soldiers[s])->GetPos();
   Dist=MyMap->Fill[Target.GetY()][Target.GetX()];
   if(Dist<MinDist){
     MinCoord=Target;
     MinDist=Dist;
   }
  }
  if(MyMap->Fill[(MyMap->HFlag).GetY()][(MyMap->HFlag).GetX()]<=MinDist+4){
    MinCoord=MyMap->HFlag;
  }
  return MinCoord;
}

int Battle::GetTurn(){
  return NumbTurn;
}


