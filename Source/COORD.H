// **********
// Координаты
// **********

class Coord{
  private:
    int x;
    int y;
  public:
    Coord(int aX,int aY): x(aX),y(aY) {}
    Coord(void) {x=0;y=0;}
    Coord(Coord &aCoord) { x=aCoord.x; y=aCoord.y; }
    int GetX(void) {return x;}
    int GetY(void) {return y;}
    void SetX(int aX) {x=aX;}
    void SetY(int aY) {y=aY;}
    int operator ==(Coord c) { return x==c.x && y==c.y; }
    int operator !=(Coord c) { return x!=c.x || y!=c.y; }
    Coord operator =(Coord c) {x=c.x;y=c.y;return c;}
    Coord operator +(Coord c) {return Coord(x+c.x,y+c.y);}
    int Distance(Coord b)
        {return (int ) sqrt((b.x-x)*(b.x-x)+
                      (b.y-y)*(b.y-y));}
    int Dist2(Coord b)
        {return  (b.x-x)*(b.x-x)+
                      (b.y-y)*(b.y-y);}
    int IsDiag(Coord b){ if(Dist2(b)==1) return FALSE;
                          else  return TRUE;  }
    Coord GetMap(){
      return Coord(x/20-1,y/20-1);
    }
    Coord GetScreen(){
      return Coord(x*20+20,y*20+20);
    }
};




const Coord Shift[4]={Coord(0,-1),Coord(-1,0),Coord(0,1),Coord(1,0)};
