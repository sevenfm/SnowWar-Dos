Map::Map():CFlag(0,MAX_Y-1), HFlag(MAX_X-1,0){
  randomize();
  CArmy=NULL;
  HArmy=NULL;
  PutSnow();      // снег
  PutIceCrumb();  // лед
  PutTrees();     // деревья
  PutPath();      // тропа
  PutBonuses();   // бонусы

}

Map::~Map(){
  for(int i=0;i<BONUS_TYPE;i++)
     if(Bonuses[i]!=NULL) delete Bonuses[i];
}

int Map::InMap(Coord Pos){
  return  Pos.GetY()>=0 && Pos.GetY()<MAX_Y &&
          Pos.GetX()>=0 && Pos.GetX()<MAX_X ;
}

void Map::SetArmy(ComputerArmy *CA, HumanArmy *HA){
  CArmy=CA;
  HArmy=HA;
}
void Map::PutSolid(int Type, int Width, int Direct, Coord StartPos){
  Coord Pos[2]={StartPos,StartPos};   // позиции концов контура
  Coord AllShift[8]={Coord(-1,0),Coord(-1,1),Coord(0,1),Coord(1,1),
                     Coord(1,0), Coord(1,-1),Coord(0,-1),Coord(-1,-1)};
  int  IndexShift;
  int  Shift[4][2]={{0,4},{1,3},{2,2},{3,1}};
  char Flag[2]={1,1}, fl=0;
  int i,j,s;

  SnowMap[StartPos.GetY()][StartPos.GetX()]=Type+SOLID;

  if(Direct){  // меняем сторону
     Shift[0][0]=0; Shift[1][0]=7; Shift[2][0]=6; Shift[3][0]=5;
     Shift[0][1]=4; Shift[1][1]=5; Shift[2][1]=6; Shift[3][1]=7;
  }

  do{
    for(s=0;s<2;s++){
      if(Flag[s] && (!Flag[!s] || !s && Pos[0].GetY()==Pos[1].GetY() ||
                                  s && Pos[0].GetY()!=Pos[1].GetY())){
        if(fl || (Pos[1].GetX()-Pos[0].GetX())>=Width){
          IndexShift=random(2)+2;
          fl=1; // достигли MAX ширины
        }else{
          IndexShift=random(2);
        }
        Pos[s]=Pos[s]+AllShift[Shift[IndexShift][s]];
        Flag[s]=InMap(Pos[s]);  // остались ли в карте
        if(Flag[s]) SnowMap[Pos[s].GetY()][Pos[s].GetX()]=Type+SOLID;
      }
    }
    if(Flag[0] && Flag[1] && Pos[0].GetY()==Pos[1].GetY() ||
       (Flag[0]+Flag[1])%2 ){
       if(Flag[0]) i=Pos[0].GetY();
       else        i=Pos[1].GetY();
       if(i<0) i=0;
       if(i>=MAX_Y) i=MAX_Y-1;
       if((j=Pos[0].GetX()+1)<0) j=0;
       if(j>=MAX_X) j=MAX_X-1;
       for(;j<Pos[1].GetX();j++) SnowMap[i][j]=Type+SOLID;  // заполняем
    }
  }while((Flag[0] || Flag[1]) && !(fl && Pos[0].Dist2(Pos[1])<=2));
}

void Map::PutIceCrumb(void){
  Coord CrumPos;
  int CrumX, CrumY,MaxL=MAX_X-1,MaxR=0,MaxU=MAX_Y-1,MaxD=0;
  int Direct,i,j;

  Direct=random(2);  // выбираем направление
  PutSolid(ICE,ICE_WIDTH,Direct,
              Coord(random(MAX_X), random(MAX_Y/2)+MAX_Y/4));

  for(i=0;i<MAX_Y;i++){
     for(j=0;j<MAX_X;j++){
        if(SnowMap[i][j]==ICE){
           if(i<MaxU) MaxU=i;
           if(i>MaxD) MaxD=i;
           if(j<MaxL) MaxL=j;
           if(j>MaxR) MaxR=j;
        }
     }
  }

  MaxR+=ICE_WIDTH+3;
  MaxL-=(ICE_WIDTH+3);
  MaxU-=2; MaxD+=2;

  CrumY=random(MAX_Y-6)+3; CrumX=random(MAX_X);
  for(i=0;i<50*MAX_REPIT &&
          CrumX>=MaxL && CrumX<=MaxR && CrumY>=MaxU && CrumY<=MaxD ;i++){
     CrumY=random(MAX_Y-6)+3; CrumX=random(MAX_X);
  }
  if(i<50*MAX_REPIT){
     if(CrumX>=MaxL && CrumX<=MaxR) Direct=!Direct;
     else Direct=random(2);

     PutSolid(CRUMB,CRUMB_WIDTH,Direct,Coord(CrumX,CrumY));
     PutBorder(CRUMB);
  }
  PutBorder(ICE);
}

void Map::PutBorder(int Type){
 int i, j, fl;
 for(i=0;i<MAX_Y;i++){   // обводим
     for(j=0;j<MAX_X;j++){
        fl=1;
        if(SnowMap[i][j]==SNOW){
          if(i>0){   // U..
             if(SnowMap[i-1][j]==Type+SOLID){
               if(j>0 && SnowMap[i][j-1]==Type+SOLID){
                 SnowMap[i][j]=Type+ULI; fl=0;
               }
               if(fl && j<MAX_X-1 && SnowMap[i][j+1]==Type+SOLID){
                 SnowMap[i][j]=Type+URI; fl=0;
               }
               if(fl){ SnowMap[i][j]=Type+U; fl=0; }
             }else{
               if(j>0 && SnowMap[i-1][j-1]==Type+SOLID
                      && SnowMap[i][j-1]!=Type+SOLID){
                 SnowMap[i][j]=Type+ULO; fl=0;
               }
               if(fl && j<MAX_X-1 && SnowMap[i-1][j+1]==Type+SOLID
                     && SnowMap[i][j+1]!=Type+SOLID){
                 SnowMap[i][j]=Type+URO; fl=0;
               }
             }
          }// U..

          if(fl && i<MAX_Y-1){   // D..
             if(SnowMap[i+1][j]==Type+SOLID){
               if(j>0 && SnowMap[i][j-1]==Type+SOLID){
                 SnowMap[i][j]=Type+DLI; fl=0;
               }
               if(fl && j<MAX_X-1 && SnowMap[i][j+1]==Type+SOLID){
                 SnowMap[i][j]=Type+DRI; fl=0;
               }
               if(fl){ SnowMap[i][j]=Type+D; fl=0; }
             }else{
               if(j>0 && SnowMap[i+1][j-1]==Type+SOLID
                      && SnowMap[i][j-1]!=Type+SOLID){
                 SnowMap[i][j]=Type+DLO; fl=0;
               }
               if(fl && j<MAX_X-1 && SnowMap[i+1][j+1]==Type+SOLID
                     && SnowMap[i][j+1]!=Type+SOLID){
                 SnowMap[i][j]=Type+DRO; fl=0;
               }
             }
          }// D..

          if(fl && j>0 &&  SnowMap[i][j-1]==Type+SOLID){ // L
            SnowMap[i][j]=Type+L; fl=0;
          }
          if(fl && j<MAX_X-1 &&  SnowMap[i][j+1]==Type+SOLID){ // R
            SnowMap[i][j]=Type+R;
          }
        } // if SNOW
     } // j
  } // i

}

void Map::PutPath(void){
   Coord Path[MAX_PATH],
         StartPos,EndPos,Cur,Prev,
         Shift[8]={Coord(0,-1),Coord(0,1),Coord(-1,0),Coord(1,0),
                  Coord(-1,-1), Coord(1,-1),Coord(-1,1),Coord(1,1)};
   int Side1, Side2,fl=1,i, Dir;
   do{
     Side1=random(4);

     do{
       Side2=random(4);
     }while(Side1==Side2);

     switch(Side1){
       case 0:
         StartPos=Coord(random(MAX_X),0);break;
       case 1:
         StartPos=Coord(random(MAX_X),MAX_Y-1);break;
       case 2:
         StartPos=Coord(MAX_X-1,random(MAX_Y));break;
       case 3:
         StartPos=Coord(0,random(MAX_Y));break;
     }
     switch(Side2){
       case 0:
         EndPos=Coord(random(MAX_X),0);break;
       case 1:
         EndPos=Coord(random(MAX_X),MAX_Y-1);break;
       case 2:
         EndPos=Coord(MAX_X-1,random(MAX_Y));break;
       case 3:
         EndPos=Coord(0,random(MAX_Y));break;
     }
   }while(EndPos.Distance(StartPos)<MIN_LENGTH);

   int Result;
   FillMap(StartPos,MAX_X+MAX_Y);
   Result=FindPath(Path,EndPos);

   if(Result){
     for(Cur=Path[0],i=0,Prev=StartPos;fl && Prev!=EndPos;
                              i++,Prev=Cur,Cur=Path[i]){

       for(Dir=0;(Prev+Shift[Dir])!=Cur && Dir<8;Dir++);
       if(Dir==8) fl=0;
       else{
         if(SnowMap[Prev.GetY()][Prev.GetX()]==SNOW)
            SnowMap[Prev.GetY()][Prev.GetX()]=Dir+PATH_U;
       }
     }
     if(fl && SnowMap[EndPos.GetY()][EndPos.GetX()]==SNOW)
        SnowMap[EndPos.GetY()][EndPos.GetX()]=Dir+PATH_U;
   }
}

void Map::PutSnow(void){
  for(int j,i=0; i<MAX_Y;i++)
    for(j=0;j<MAX_X;j++)
      SnowMap[i][j]=SNOW;
}

int Map::Check(Coord StartPos,Coord LastPos,Coord CurPos){
  Coord NewPos;

  for(int Direct=0;Direct<4;Direct++){
    NewPos=CurPos+Shift[Direct];
    if(NewPos!=LastPos){
      if(NewPos==StartPos) return FALSE;
      if((SnowMap[NewPos.GetY()][NewPos.GetX()]==TREE_SNOW) ||
         (SnowMap[NewPos.GetY()][NewPos.GetX()]==TREE_CRUMB)){
        if(!Check(StartPos,CurPos,NewPos))
          return FALSE;
      }
    }
  }
  return TRUE;
}

void Map::PutTrees(void){
  int i,Type,Rep;
  Coord Pos;
  char fl=FALSE;

  for(i=0;i<MAX_TREE;i++){
    if(random(6)>=3) Type=TREE_SNOW;
    else Type=TREE_CRUMB;
    fl=FALSE;
    for(Rep=0;!fl && Rep<MAX_REPIT; Rep++){
      Pos=Coord(random(MAX_X-2)+1,random(MAX_Y-2)+1);
      if(Pos!=HFlag && Pos!=CFlag)
        if((Type==TREE_SNOW) && (SnowMap[Pos.GetY()][Pos.GetX()]==SNOW) ||
           (Type==TREE_CRUMB) && (SnowMap[Pos.GetY()][Pos.GetX()]==CRUMB))
         // проверка на образование непроходимых участков
          fl=Check(Pos,Pos,Pos);
    }
    if(fl)
      SnowMap[Pos.GetY()][Pos.GetX()]=Type;
  }
}

void Map::PutBonuses(void){
  int i,Type,Rep;
  Coord Pos;
  char fl;
  for(i=0;i<MAX_BONUS;i++) Bonuses[i]=NULL;

  for(i=0;i<MAX_BONUS;i++){
    Type=random(BONUS_TYPE);

    fl=FALSE;
    for(Rep=0;!fl && Rep<MAX_REPIT; Rep++){
      Pos=Coord(random(MAX_X),random(MAX_Y));
      if(Pos!=HFlag && Pos!=CFlag && IsBonus(Pos)==-1){
         if(Type==ICE_HOLE && SnowMap[Pos.GetY()][Pos.GetX()]==ICE ||
            Type==SLEDGE && SnowMap[Pos.GetY()][Pos.GetX()]==SNOW ||
             Type==FIRE && SnowMap[Pos.GetY()][Pos.GetX()]==CRUMB)

            fl=TRUE;
      }
    }
    if(fl) Bonuses[i]=new Bonus(Type,Pos);
  }

}

void Map::RemoveBonus(Coord Pos){
  for(int fl=1,i=0; fl && (i<MAX_BONUS);i++)
     if(Bonuses[i]!=NULL && (Bonuses[i]->GetPos())==Pos){
        delete Bonuses[i];
        Bonuses[i]=NULL;
        fl=0;
     }
}

void Map::ShowMap(void){
  int i,j;
  for(i=0;i<MAX_Y;i++)     // ячейки
    for(j=0;j<MAX_X;j++)
      ShowCell(Coord(j,i));
}

void Map::ShowCell(Coord Pos){
  CellPic[SnowMap[Pos.GetY()][Pos.GetX()]]->Show(Pos.GetScreen());
//  for(int i=0,fl=TRUE;i<MAX_BONUS && fl;i++)
//    if(Bonuses[i]!=NULL && Bonuses[i]->GetPos()==Pos)
//       fl=FALSE;
//  if(!fl) BonusPic[Bonuses[i-1]->GetType()]->ShowShd(Pos.GetScreen());
  if(IsBonus(Pos)!=-1) BonusPic[IsBonus(Pos)]->ShowShd(Pos.GetScreen());
  if(Pos==CFlag) Flag[COMPUTER]->ShowShd(Pos.GetScreen());
  if(Pos==HFlag) Flag[HUMAN]->ShowShd(Pos.GetScreen());
}


int Map::CostToMove(Coord Pos){
  int type=SnowMap[Pos.GetY()][Pos.GetX()];
  if(type==SNOW) return CELL_MOVE[0];
  if(type>=ICE && type<CRUMB) return CELL_MOVE[1];
  if(type>=CRUMB && type<PATH_U) return CELL_MOVE[2];
  if(type>=PATH_U && type<TREE_SNOW) return CELL_MOVE[3];
  return CELL_MOVE[4];
}

int Map::IsSnowMan(Coord Pos){
  int i;
  if(CArmy!=NULL)
     for(i=0;i<CArmy->GetNumbSold();i++)
        if((CArmy->WhatSnowMan(Pos))!=NULL) return TRUE;
  if(HArmy!=NULL)
     for(i=0;i<HArmy->GetNumbSold();i++)
        if((HArmy->WhatSnowMan(Pos))!=NULL) return TRUE;
  return FALSE;
}

int Map::IsBonus(Coord Pos){
  for(int i=0;i<MAX_BONUS;i++)
     if(Bonuses[i]!=NULL && Bonuses[i]->GetPos()==Pos)
         return (Bonuses[i]->GetType());
  return -1;
}

int Map::GetFillCell(Coord NewPos){
  return Fill[NewPos.GetY()][NewPos.GetX()];

}

void Map::FillMap(Coord CurPos,int MaxRepeat){

  Pos=CurPos;
  int i,j,r,dx,dy,Cost;

  for(i=0;i<MAX_Y;i++)      // Инициализация массивов
    for(j=0;j<MAX_X;j++){
      Fill[i][j]=MAXINT;
    }

  Fill[Pos.GetY()][Pos.GetX()]=0;  // Обозначим начальную клетку

  for(r=MaxRepeat;r>=0;r--){  // max оценка длины пути: <=AP
  for(i=0;i<MAX_Y;i++)        //  Просматриваем весь
    for(j=0;j<MAX_X;j++){     //  массив Fill[][]
      for(dx=-1;dx<=1;dx++)    // Пройдем все
        for(dy=-1;dy<=1;dy++)  // соседние
          if(dx||dy){               // клетки
            if(InMap(Coord(j+dx,i+dy))){ // Клетка в пределах карты
              if(Fill[i][j]!=MAXINT){
                Cost=CostToMove(Coord(j+dx,i+dy));
                if((Cost>0) &&(dx!=0)&&(dy!=0)) Cost+=1;
                if(Cost>0 && Fill[i+dy][j+dx]>Fill[i][j]+Cost){
                  // Если клетка проходима и путь короче
                  Fill[i+dy][j+dx]=Fill[i][j]+Cost;
                }
              }
            }
          }
    }
  }


}


int Map::FindPath(Coord Path[MAX_PATH],Coord NewPos){
  Coord Temp[MAX_PATH];
  int Cost;
  int i,j,dx,dy;

  if(!CostToMove(NewPos)){
    return FALSE;  // Конечная клетка непроходима !!
  }

  int x=NewPos.GetX(),y=NewPos.GetY();  // координаты цели путешествия

  if(Fill[y][x]==MAXINT){
     return FALSE;    // нет пути до конечной цели!
  }

  int St=0;
  int NewX=x,
      NewY=y;
  int min;

  while(x!=Pos.GetX() || y!=Pos.GetY()){ // пока не дошли до начальной клетки
    min=MAXINT;
    Temp[St++]=Coord(NewX,NewY);  // координаты добавляем в путь
    for(dx=-1;dx<=1;dx++)          // просматриваем
      for(dy=-1;dy<=1;dy++)        // соседние
        if(dx||dy)                     // клетки
          if(InMap(Coord(x+dx,y+dy))){
            Cost=Fill[y+dy][x+dx];
            if(Cost<min){          // находим min из проходимых соседей
              min=Cost;
              NewX=x+dx;
              NewY=y+dy;
            }
          }
    x=NewX;
    y=NewY;
  }

  for(i=0;i<St;i++)
    Path[St-1-i]=Temp[i];

  return St;
}
