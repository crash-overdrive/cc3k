#ifndef ITEM_H
#define ITEM_H

#include <iostream>
#include <string>
#include <memory>
#include "posn.h"

class Board;
class Character;
class Player;
class Dragon;


class Item {
    protected:
    Posn position;
    std::string displaySymbol;
    std::string name;
    
    public:
    Item(Posn&& position, std::string displaySymbol, std::string name);
    
    // getters
    Posn getPosition() const;
    std::string getDisplaySymbol() const;
    
    virtual bool isMoveable() const;
    
    virtual void see(Board& b);
    virtual void use(Board& b, Player& p) = 0;
};


std::ostream &operator<<(std::ostream &out, const Item &i);


class Treasure: public Item {
    protected:
    int gold;
    
    public:
    Treasure(Posn&& position, std::string name, int gold);
    virtual ~Treasure() = 0;
    
    bool isMoveable() const override;
    
    virtual void use(Board& b, Player& p) override;
};


class SmallGoldPile: public Treasure {
    public:
    SmallGoldPile(Posn&& position);
};


class NormalGoldPile: public Treasure {
    public:
    NormalGoldPile(Posn&& position);
};


class HumanGoldPile: public Treasure {
    public:
    HumanGoldPile(Posn&& position);
};


class MerchantHoard: public Treasure {
    public:
    MerchantHoard(Posn&& position);
};


class DragonHoard: public Treasure {
    std::shared_ptr<Dragon> dragon;
    
    public:
    DragonHoard(Posn&& p);
    
    void setDragon(std::shared_ptr<Dragon> d);
    
    void use(Board& b, Player& p) override;
};



class Potion: public Item {
    protected:
    std::string shortName;
    
    public:
    Potion(Posn&& position, std::string name, std::string shortName);
};


class RestoreHealth: public Potion {
    protected:
    static bool isKnown;
    
    public:
    RestoreHealth(Posn&& p);
    
    static void setUnknown();
    
    void see(Board& b) override;
    void use(Board& b, Player& p) override;
};


class PoisonHealth: public Potion {
    protected:
    static bool isKnown;
    
    public:
    PoisonHealth(Posn&& p);
    
    static void setUnknown();
    
    void see(Board& b) override;
    void use(Board& b, Player& p) override;
};


class BoostAtk: public Potion {
    protected:
    static bool isKnown;
    
    public:
    BoostAtk(Posn&& p);
    
    static void setUnknown();
    
    void see(Board& b) override;
    void use(Board& b, Player& p) override;
};


class WoundAtk: public Potion {
    protected:
    static bool isKnown;
    
    public:
    WoundAtk(Posn&& p);
    
    static void setUnknown();
    
    void see(Board& b) override;
    void use(Board& b, Player& p) override;
};


class BoostDef: public Potion {
    protected:
    static bool isKnown;
    
    public:
    BoostDef(Posn&& p);
    
    static void setUnknown();
    
    void see(Board& b) override;
    void use(Board& b, Player& p) override;
};


class WoundDef: public Potion {
    protected:
    static bool isKnown;
    
    public:
    WoundDef(Posn&& p);
    
    static void setUnknown();
    
    void see(Board& b) override;
    void use(Board& b, Player& p) override;
};

#endif
