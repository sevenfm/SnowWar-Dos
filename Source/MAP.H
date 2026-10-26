class Bonus{                            // бонус
  private:
    Coord Pos;                            // Позиция на карте
    int Type;                             // тип бонуса
  public:
    Bonus(int BType,Coord BPos);           // создает бонус
    Coord GetPos(void);                    // возвращает позицию
    int GetType(void);
};

class Map{                              // карта
  friend class ComputerArmy;
  private:
    int SnowMap[MAX_Y][MAX_X];          // игровое поле
    ComputerArmy *CArmy;
    HumanArmy *HArmy;
    int Fill[MAX_Y][MAX_X];
    Coord Pos;
    void PutBorder(int Type);
    void PutSolid(int Type, int Width,int Direct, Coord StartPos);
    void PutIceCrumb(void);  // лед
    void PutPath(void);      // тропа
    void PutSnow(void);      // снег
    void PutTrees(void);     // деревья
    void PutBonuses(void);   // бонусы
    int  Check(Coord StartPos,Coord LastPos,Coord CurPos);

  public:
    Coord CFlag,                        // координаты флагов
          HFlag;
    Bonus *Bonuses[MAX_BONUS];          // бонусы
    Map();// создает случайную карту
    ~Map();
    void ShowMap(void);                 // выводит всю карту
    void ShowCell(Coord Pos);           // выводит одну ячейку
    int CostToMove(Coord Pos);          // возвращает проходимость
                                        // и проверяет наличие снеговика
    int IsBonus(Coord Pos);             // -1 - не бонус, иначе-тип бонуса
    void RemoveBonus(Coord Pos);
    int InMap(Coord Pos);
    int IsSnowMan(Coord Pos);
    void FillMap(Coord CurPos,int MaxRepeat);
    int GetFillCell(Coord NewPos);
    int FindPath(Coord Path[],Coord NewPos);
    void SetArmy(ComputerArmy *CA, HumanArmy *HA);
};


Bonus::Bonus(int BType,Coord BPos){
  Type=BType;
  Pos=BPos;
}

Coord Bonus::GetPos(void){
  return Pos;
}

int Bonus::GetType(void){
  return Type;
}

