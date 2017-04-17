#include "cell.h"
#include "character.h"
#include "item.h"
#include "board.h"

using namespace std;

Cell::Cell(string baseSymbol, int x, int y): baseSymbol{baseSymbol} {
    position = Posn{x, y};
}

Cell::~Cell() {}

string Cell::getBaseSymbol() const {
    return baseSymbol;
}

Posn Cell::getPosition() const { return position; }

//bool Cell::isMoveable(Character &) const { return false; }
bool Cell::isMoveable(Player &) const { return false; }
bool Cell::isMoveable(Enemy &) const { return false; }


int Cell::notifyFriendly(Board &b) {
    // Notify neighbours
    vector<Posn> neighbours = position.getNeighbours();
    for (int i = 0; i < (int)neighbours.size(); ++i) {
        b.getCell(neighbours[i])->notifyGreedy(b);
    }
    // Use any item we're standing on
    if (getItem() != nullptr) {
        getItem()->use(b, *(b.getPlayer()));
    }
    return 0;
}

void Cell::notifyGreedy(Board &b) {
     // See any item here
     if (getItem() != nullptr) {
        getItem()->see(b);
     }
}

shared_ptr<Character> Cell::getCharacter() const { return nullptr; }

shared_ptr<Item> Cell::getItem() const { return nullptr; }

void Cell::removeCharacter() {} // Does nothing by default
void Cell::removeItem() {} // Does nothing by default

void Cell::setCharacter(std::shared_ptr<Character> c) {} // Does nothing by default
void Cell::setItem(std::shared_ptr<Item> i) {} // Does nothing by default


Blank::Blank(int x, int y): Cell{" ", x, y} {
}

Blank::~Blank() {}



WallVert::WallVert(int x, int y): Cell{"|", x, y} {}

WallVert::~WallVert() {}



WallHor::WallHor(int x, int y): Cell("-", x, y) {}

WallHor::~WallHor() {}



Passageway::Passageway(int x, int y): Cell{"#", x, y}, PC{nullptr} {}

Passageway::~Passageway() {}

bool Passageway::isMoveable(Player &) const { return true; }

shared_ptr<Character> Passageway::getCharacter() const { return PC; }

void Passageway::removeCharacter() {
    PC = nullptr;
}
void Passageway::setCharacter(std::shared_ptr<Character> c) {
    PC = c;
}


Doorway::Doorway(int x, int y): Cell{"+", x, y}, PC{nullptr} {}

Doorway::~Doorway() {}

bool Doorway::isMoveable(Player &) const { return true; }

shared_ptr<Character> Doorway::getCharacter() const { return PC; }

void Doorway::removeCharacter() {
    PC = nullptr;
}
void Doorway::setCharacter(std::shared_ptr<Character> c) {
    PC = c;
}


Floor::Floor(int x, int y): Cell{".", x, y}, character{nullptr},
    item{nullptr} {}

Floor::~Floor() {}

bool Floor::isMoveable(Player &) const { 
    return character == nullptr && (item == nullptr || item->isMoveable());
 }

bool Floor::isMoveable(Enemy &) const { return character == nullptr && item == nullptr; } // Monsters can't move onto items

shared_ptr<Character> Floor::getCharacter() const { return character; }

shared_ptr<Item> Floor::getItem() const { return item; }


void Floor::removeCharacter() {
    character = nullptr;
}

void Floor::setCharacter(std::shared_ptr<Character> c) {
    character = c;
}

void Floor::removeItem() {
    item = nullptr;
}

void Floor::setItem(std::shared_ptr<Item> i) {
    item = i;
}



Stairway::Stairway(int x, int y): Cell{"/", x, y} {}

Stairway::~Stairway() {}

bool Stairway::isMoveable(Player &) const { return true; }

int Stairway::notifyFriendly(Board &b) {
    if (b.getFloor() == b.getMaxFloor()) {
        b.addAction(" PC finds the exit; you win!");
        b.endGame();
        return 1; // 1 == game over
    } else {
        b.changeFloor();
        b.addAction(" PC descends to floor " + to_string(b.getFloor()) + ".");
        return -1; // -1 == changed floors
    }
}



ostream &operator<<(ostream &out, const Cell &c) {
    if (c.getCharacter() != nullptr) {
        out << *(c.getCharacter());
    } else if (c.getItem() != nullptr) {
        out << *(c.getItem());
    } else {
        out << c.getBaseSymbol();
    }
    return out;
}



