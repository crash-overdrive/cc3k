#ifndef CELL_H
#define CELL_H

#include <string>
#include <iostream>
#include <memory>
#include "posn.h"

class Board;
class Character;
class Player;
class Enemy;
class Item;

std::ostream &operator<<(std::ostream &out, const Character &c);
std::ostream &operator<<(std::ostream &out, const Item &c);


class Cell {

    protected:
    std::string baseSymbol;
    Posn position;
    
    public:
    Cell(std::string baseSymbol, int x, int y);
    virtual ~Cell() = 0;
    
    // Getters
    std::string getBaseSymbol() const;
    Posn getPosition() const;
    virtual std::shared_ptr<Character> getCharacter() const;
    virtual std::shared_ptr<Item> getItem() const;
    
    // checks if the Character can move onto the Cell
    virtual bool isMoveable(Player &) const;
    virtual bool isMoveable(Enemy &) const;

    // called when player is in Cell
    virtual int notifyFriendly(Board &b);
    // called when player is in neighbouring cell
    virtual void notifyGreedy(Board &b);
    
    // Dummy methods needed so that these can be called on floors/passageways/doorways
    virtual void setCharacter(std::shared_ptr<Character> c);
    virtual void setItem(std::shared_ptr<Item> i);
    
    // blindly called on all cells when changing floors, and by an enemy when killed
    // We need to store another copy of each item and character in board vectors so 
    // that an object calling this doesn't delete itself 
    virtual void removeCharacter();
    virtual void removeItem();
};

class Blank: public Cell {
    public:
    Blank(int x, int y);
    ~Blank();
};

class WallVert: public Cell {
    public:
    WallVert(int x, int y);
    ~WallVert();
};

class WallHor: public Cell {
    public:
    WallHor(int x, int y);
    ~WallHor();
};

class Passageway: public Cell {
    std::shared_ptr<Character> PC;

    public:
    Passageway(int x, int y);
    ~Passageway();
    
    bool isMoveable(Player &) const override;
    
    std::shared_ptr<Character> getCharacter() const override;
    void setCharacter(std::shared_ptr<Character> c) override;
    void removeCharacter() override;
};

class Doorway: public Cell {
    std::shared_ptr<Character> PC;

    public:
    Doorway(int x, int y);
    ~Doorway();
    
    bool isMoveable(Player &) const override;
    
    std::shared_ptr<Character> getCharacter() const override;
    void setCharacter(std::shared_ptr<Character> c) override;
    void removeCharacter() override;
};

class Floor: public Cell {
    std::shared_ptr<Character> character;
    std::shared_ptr<Item> item;

    public:
    Floor(int x, int y);
    ~Floor();
    
    bool isMoveable(Player &) const override;
    bool isMoveable(Enemy &) const override;
    
    std::shared_ptr<Character> getCharacter() const override;
    void setCharacter(std::shared_ptr<Character> c) override;
    void removeCharacter() override;
    std::shared_ptr<Item> getItem() const override;
    void setItem(std::shared_ptr<Item> i) override;
    void removeItem() override;
};

class Stairway: public Cell {
    public:
    Stairway(int x, int y);
    ~Stairway();
    
    bool isMoveable(Player &) const override;
    int notifyFriendly(Board &b) override;
};


std::ostream &operator<<(std::ostream &out, const Cell &c);

#endif
