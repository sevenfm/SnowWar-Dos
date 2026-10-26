class Record;
class EmsDriver;
class UserInterface;
class DeskTop;
class Window;
class Graphics;
class Mouse;
class Palette;
class Picture;
class FilePicture;
class Font;
class TopTen;
class Army;
class HumanArmy;
class ComputerArmy;
class Battle;
class Map;
class Bonus;
class SnowMan;
class Coord;

void ErrorReport(char *String);
void InitErrorReport(void);

EmsDriver *Ems;
Mouse *MyMouse;
Map *MyMap;

Picture* BonusPic[BONUS_TYPE];      // картинки бонусов
Picture* PicHuman[SNOWMAN_TYPE];    // картинки для разных типов
Picture* PicComputer[SNOWMAN_TYPE];
Picture* Bullet[SNOWMAN_TYPE];      // картинки пуль
Picture* Expl[SNOWMAN_TYPE];        // картинки взрывов
Picture* CurPic;                    // картинка рамочки для текущего
Picture* Status[10];                // картинки статуса снеговика
Picture* CellPic[CELL_TYPE];        // картинки ячеек поля
Picture* Flag[2];                   // картики флагов
Picture* Cursor;                    // курсор для ввода текста
Picture* Button[2];                 // кнопки

char* CursorName[MOUSE_CURSOR]={"mmouse.raw","topmouse.raw","abmouse.raw",
                                "gcursor.raw","carrot.raw",
                                "targeton.raw","targetof.raw",
                                "moveon.raw","moveof.raw",
                                "selecton.raw","selectof.raw"};

