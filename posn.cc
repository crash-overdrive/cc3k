#include "posn.h"
using namespace std;

Posn::Posn(int x , int y) : x{x} , y{y}{}
Posn::Posn() : x{0} , y{0}{};

vector<Posn> Posn::getNeighbours() {
    vector<Posn> neighbours(8);
    neighbours[0] = Posn(x-1 , y-1);
    neighbours[1] = Posn(x-1 , y);
    neighbours[2] = Posn(x-1 , y+1);
    neighbours[3] = Posn(x , y-1);
    neighbours[4] = Posn(x , y+1);
    neighbours[5] = Posn(x+1 , y-1);
    neighbours[6] = Posn(x+1 , y);
    neighbours[7] = Posn(x+1 , y+1);
    return neighbours;
}


bool Posn::operator==(const Posn& other) const {
  return x == other.x && y == other.y;
}

