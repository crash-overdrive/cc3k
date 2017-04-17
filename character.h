#ifndef CHARACTER_H
#define CHARACTER_H

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <cmath>

#include "posn.h"

class Cell;
class Board;
class DragonHoard;

class Character {
    protected:
    Posn position;
    const std::string displaySymbol;
    int HP;
    const int ATK;
    const int DEF;
    int gold;
    
    public:
    Character(Posn&& p, std::string displaySymbol, int HP, int ATK, int DEF, int gold);
    virtual ~Character() = 0;
    
    Posn getPosition();
    std::string getDisplaySymbol() const;
    virtual int getATK() const;
    
    void setPosition(Posn& p);
    void addGold(int amount);
    
    virtual void takeTurn(Board& b) = 0;
    virtual bool dodges();
    virtual void successfulAttack(Board& b, Character& target);
    virtual void getHit(Board& b, Character& attacker)=0;
    bool isDead() const;
};

std::ostream &operator<<(std::ostream &out, const Character &c);


class Player: public Character {
    protected:
    const std::string race;
    const int maxHP;
    int modifiedATK;
    int modifiedDEF;
    
    // utility functions
    Posn dirToPosn(std::string dir);
    std::string dirToFullName(std::string dir);
    
    public:
    Player(std::string race, int HP, int ATK, int DEF);
    
    // Getters
    std::string getRace() const;
    int getHP() const;
    int getATK() const override;
    int getDEF() const;
    int getGold() const;
    
    virtual void addHP(int amount);
    virtual void addATK(int amount);
    virtual void addDEF(int amount);
    void resetStats();
    
    virtual void takeTurn(Board& b);
    
    virtual int move(Board& b, std::string dir);
    
    virtual void attemptAttack(Board& b, std::string dir);
    bool dodges() override;
    virtual void getHit(Board& b, Character& attacker);
    
    virtual void usePotion(Board& b, std::string dir);
    
    virtual int getScore() const;
};


class Shade: public Player {
    public:
    Shade();
    ~Shade() override;
    int getScore() const override;
};


class Drow: public Player {
    public:
    Drow();
    ~Drow() override;
    void addHP(int amount) override;
    void addATK(int amount) override;
    void addDEF(int amount) override;
};


class Vampire: public Player {
    public:
    Vampire();
    ~Vampire() override;
    void addHP(int amount) override;
    void successfulAttack(Board& b, Character& target) override;
};


class Troll: public Player {
    public:
    Troll();
    ~Troll() override;
    int move(Board& b, std::string dir) override;
    void attemptAttack(Board& b, std::string dir) override;
    void usePotion(Board& b, std::string dir) override;
};


class Goblin: public Player {
    public:
    Goblin();
    ~Goblin() override;
    void successfulAttack(Board& b, Character& target) override;
    
    void getHit(Board& b, Character& attacker) override;
};



class Enemy: public Character {
    public:
    Enemy(Posn&& p, std::string displaySymbol, int HP, int ATK, int DEF, int gold);
    
    virtual void takeTurn(Board& b);
    
    virtual void attemptAttack(Board& b, Player& defender);
    virtual void getHit(Board& b, Character& attacker);
};



class Human: public Enemy {
    public:
    Human(Posn&& p);
    ~Human() override;
    
    void getHit(Board& b, Character& attacker) override;
};


class Dwarf: public Enemy {
    public:
    Dwarf(Posn&& p);
    ~Dwarf() override;
    
    void getHit(Board& b, Character& attacker) override;
};


class Elf: public Enemy {
    public:
    Elf(Posn&& p);
    ~Elf() override;
    
    void attemptAttack(Board& b, Player& defender) override;
};


class Orc: public Enemy {
    public:
    Orc(Posn&& p);
    ~Orc() override;
};


class Merchant: public Enemy {
    protected:
    static bool isHostile;
    
    public:
    Merchant(Posn&& p);
    ~Merchant() override;
    
    static void setFriendly();
    
    void takeTurn(Board& b) override;
    void getHit(Board& b, Character& attacker) override;
};


class Dragon: public Enemy {
    std::shared_ptr<DragonHoard> hoard;
    
    public:
    Dragon(Posn&& p);
    ~Dragon() override;
    
    void setHoard(std::shared_ptr<DragonHoard> h);
    
    void takeTurn(Board& b) override;
    void attemptAttack(Board& b, Player& defender) override;
    void getHit(Board& b, Character& attacker) override;
};


class Halfling: public Enemy {
    public:
    Halfling(Posn&& p);
    ~Halfling() override;
    
    bool dodges() override;
};

#endif
