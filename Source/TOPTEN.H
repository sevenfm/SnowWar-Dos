class TopTen{                    // таблица рекордов
  private:
    const int KM1,KM2;                // коэф. масштаба для NumbPoints
    Coord LUName;                // лев. верх. угол поля 1-го имени
    Coord LUPoint;               // и столбца очков
    const int Height;            // высота поля
    DeskTop *aDeskTop;
    int Handle;                  // файл таблицы
    int LoadTopTen(void);        // загрузка таблицы ( True-успешно )
    int SaveTopTen(void);        // запись таблицы ( True-успешно )
    Font TabFont;                // шрифт для таблицы
    struct Record{
      char Name[MAX_NAME];       // имя игрока
      int Points;                // кол-во очков
    }Rec[NUM_REC];               // массив записей
  public:
    TopTen();                    // загружает с диска или создает новую
    ~TopTen();                   // записывает на диск
    int NumbPoints(int NumbTurn,int NumbEnemy,int NumbOwn);
    int PlaceInTen(int Points);  // возвращает место в таблице
    void ShowTen();              // выводит таблицу на экран
    void InsertNew(int Place,int Points);  // добавляет в таблицу
};

TopTen::TopTen() : KM1(10), KM2(50),LUName(180,100),
                   LUPoint(420,100), Height(30),TabFont("bigtext"){

  // открываем или создаем новый

  Handle=open("hiscore.tbl",O_RDWR|O_CREAT|O_BINARY, S_IREAD|S_IWRITE);

  if(Handle==-1){      // проверяем успешность
     cout<<"\nОшибка открытия файла таблицы рекордов!\n";
     exit(EXIT_FAILURE);
  }

  if(!LoadTopTen())            // если не загрузили то обнуляем
    for(int i=0, j;i<NUM_REC;i++){
       for(j=0;j<MAX_NAME;j++) Rec[i].Name[j]=0;
       Rec[i].Points=0;
    }


  aDeskTop=new DeskTop(1,TopTenWindows,
                       "topten.raw","topten.pal",TOPTEN_CURSOR);
}

TopTen::~TopTen(){

  if(!SaveTopTen()){     // сохраняем и проверяем
     cout<<"\nОшибка при сохранении таблицы рекордов!\n";
     exit(EXIT_FAILURE);
  }

  if(close(Handle)!=0){   // закрываем файл и проверяем
     cout<<"\nОшибка закрытия файла таблицы рекордов!\n";
     exit(EXIT_FAILURE);
  }
  delete aDeskTop;
}

int TopTen::NumbPoints(int NumbTurn,int NumbEnemy,int NumbOwn){
   if(NumbEnemy!=0) return int(KM1*(NumbOwn+NumbEnemy) +KM2/(NumbTurn+1));
   return  int(NumbOwn*KM1+KM2/(NumbTurn+1));
}

int TopTen::PlaceInTen(int Points){
   for(int i=0;i<NUM_REC && Rec[i].Points>=Points;i++);
   return i+1;
}

void TopTen::ShowTen(){
   Coord ColumName=LUName, ColumPoint=LUPoint;
   char StrPoints[25];

   aDeskTop->Show();    // показываем фон

   for(int i=0;i<NUM_REC;i++){    // печать таблицы
      TabFont.PutText(ColumName, Rec[i].Name);

      if(Rec[i].Points) // переводим очки в char* и печатаем
         TabFont.PutText(ColumPoint, itoa(Rec[i].Points, StrPoints, 10));

      ColumName.SetY(ColumName.GetY()+Height);
      ColumPoint.SetY(ColumPoint.GetY()+Height);
   }

   aDeskTop->GetOption();   // ожидаем щелчка
   aDeskTop->Hide();
}

void TopTen::InsertNew(int Place,int Points){
   Coord ColumName=LUName, ColumPoint=LUPoint;
   char StrPoints[25];


   aDeskTop->Show();                  // показываем фон
   for(int i=NUM_REC-1;i>Place-1;i--){
       strcpy(Rec[i].Name,Rec[i-1].Name);
       Rec[i].Points=Rec[i-1].Points;
   }

   Rec[Place-1].Points=Points;   // заносим количество очков

   for(i=0;i<NUM_REC;i++){  // печать таблицы кроме Place
      if(Place-1!=i){
         TabFont.PutText(ColumName, Rec[i].Name);
       // переводим очки в char* и печатаем
      }
      if(Rec[i].Points)
         TabFont.PutText(ColumPoint, itoa(Rec[i].Points, StrPoints, 10));

      ColumName.SetY(ColumName.GetY()+Height);
      ColumPoint.SetY(ColumPoint.GetY()+Height);
   }

   // устанавливаем координаты записи

   ColumName.SetY(LUName.GetY()+(Place-1)*Height);

   // заносим и печатаем имя

   TabFont.GetText(ColumName, Rec[Place-1].Name);

//   Rec[Place-1].Points=Points;   // заносим количество очков

   aDeskTop->GetOption();   // ожидаем щелчка
   aDeskTop->Hide();
}

int TopTen::LoadTopTen(void){

   // считываем и возращаем успешность чтения

   return (sizeof(Rec)==read(Handle, (void*)Rec, sizeof(Rec)));
}

int TopTen::SaveTopTen(void){

   lseek(Handle, 0, SEEK_SET);  // позиционируем на начало

   // записываем и возвращаем успешность записи

   return (sizeof(Rec)==write(Handle, (void*)Rec, sizeof(Rec)));
}

