// *********
// Константы
// *********

const int MAX_X=30,
          MAX_Y=20;            // размер карты
const int CELL_WIDTH=20,
          CELL_HEIGHT=20;      // размер ячейки
const int MAX_NAME=16;         // длина имени в таб. рекордов
const int MAX_BONUS=6;         // начальное число бонусов на карте
const int BONUS_TYPE=3;        // число типов бонусов
const int CELL_TYPE=37;         // кол-во типов ячеек
const int NUM_REC=10;          // 10 лучших
const int SNOWMAN_TYPE=4;      // типы снеговиков
const int MAX_SOLD=5;       // начальное число снеговиков
const int MAX_PATH=100;
const int ICE_WIDTH=14,CRUMB_WIDTH=13;
const int MAX_REPIT=5;
const int MAX_TREE=100;
const int TREE_TYPE=2;
const int MIN_LENGTH=10;
const int CELL_MOVE[5]={2,3,3,1,0};
const int MOUSE_CURSOR=11;

enum Boolean {FALSE, TRUE};
enum {EMS_MEMORY,CONV_MEMORY};
enum {GAME_GO_ON, VICTORY, ABORT}; // возвращают HumanArmy и ComputerArmy
enum {ICE_HOLE,SLEDGE,FIRE};   // типы бонусов
enum {VISIBLE,INVISIBLE};
enum {COMPUTER, HUMAN};
enum {BUTTON,WINDOW,NOTHING};

enum {SOLID,U,D,L,R,DLI,DLO,DRI,DRO,ULI,ULO,URI,URO};

enum {SNOW, ICE, CRUMB=14,
      PATH_U=27,PATH_D,PATH_L,PATH_R,PATH_UL,PATH_UR,PATH_DL,PATH_DR,
      TREE_SNOW,TREE_CRUMB};

enum{SNOW_MAN,ICE_MAN,FAST_MAN,FIRE_MAN};
enum{SNOW_MAN_NAME,ICE_MAN_NAME,FAST_MAN_NAME,FIRE_MAN_NAME,EMPTY_NAME,
     AP_PIC,HEALTH_PIC,COST_PIC,DAMAGE_PIC,STATUS_EMPTY};
enum{QUIT_GAME,END_TURN,MAP_WINDOW};
enum{HUMAN_WIN,COMPUTER_WIN,NOBODY_WIN};

enum{STANDART_CURSOR,TOPTEN_CURSOR,ABOUT_CURSOR,GAME_CURSOR,CARROT_CURSOR,
     TARGETON_CURSOR,TARGETOFF_CURSOR,MOVEON_CURSOR,MOVEOFF_CURSOR,
     SELECTON_CURSOR,SELECTOFF_CURSOR};


const struct{
  int AP;
  int Health;
  int Range;
  int CostToFire;
  int Damage;
}SnowType[4]={{6,3,4,3,1},
              {6,5,3,2,2},
              {8,3,4,3,1},
              {6,3,3,3,3}};

const int EMM_INT=0x67;
const int PAGE_SIZE=16384;
