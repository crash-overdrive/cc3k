#include <iostream>
#include "item.h"
#include "character.h"
#include "cell.h"
#include "board.h"

using namespace std;


Item::Item(Posn&& p, string displaySymbol, string name)
    : position{Posn{p.x, p.y}},
    displaySymbol{displaySymbol},
    name{name} {
}

Posn Item::getPosition() const {
    return position;
}

string Item::getDisplaySymbol() const {
    return displaySymbol;
}

bool Item::isMoveable() const { return false; }

void Item::see(Board& b) {
    b.addAction(" PC sees a " + name + ".");
}


std::ostream &operator<<(std::ostream &out, const Item &i) {
    out << i.getDisplaySymbol();
    return out;
}


Treasure::Treasure(Posn&& p, string name, int gold)
    : Item(Posn{p.x, p.y}, "G", name), 
    gold{gold} {
}

Treasure::~Treasure() {}

bool Treasure::isMoveable() const { 
    return true; 
}

void Treasure::use(Board& b, Player& p) {
    p.addGold(gold);
    b.removeItem(position);
    b.addAction(" PC picks up " + to_string(gold) + " gold.");
}


SmallGoldPile::SmallGoldPile(Posn&& p)
    : Treasure(Posn{p.x, p.y}, "Small Gold Pile", 1) {
}


NormalGoldPile::NormalGoldPile(Posn&& p)
    : Treasure(Posn{p.x, p.y}, "Normal Gold Pile", 2) {
}


HumanGoldPile::HumanGoldPile(Posn&& p)
    : Treasure(Posn{p.x, p.y}, "pair of Normal Gold Piles", 4) {
}


MerchantHoard::MerchantHoard(Posn&& p)
    : Treasure(Posn{p.x, p.y}, "Merchant Hoard", 4) {
}


DragonHoard::DragonHoard(Posn&& p)
    : Treasure(Posn{p.x, p.y}, "Dragon Hoard", 6), dragon{nullptr} {
}

void DragonHoard::setDragon(shared_ptr<Dragon> d) {
    dragon = d;
}

void DragonHoard::use(Board& b, Player& p) {
    if (dragon == nullptr) {
        p.addGold(gold);
        b.removeItem(position);
        b.addAction(" PC picks up " + to_string(gold) + " gold.");
    }
}



Potion::Potion(Posn&& p, string name, string shortName)
    : Item(Posn{p.x, p.y}, "P", name),
    shortName{shortName} {
}


RestoreHealth::RestoreHealth(Posn&& p)
    : Potion{Posn{p.x, p.y}, "Restore Health potion", "RH"} {
}

bool RestoreHealth::isKnown = false;

void RestoreHealth::setUnknown() { isKnown = false; }

void RestoreHealth::see(Board& b) {
    if (isKnown) {
        b.addAction(" PC sees a " + shortName + " potion.");
    } else {
        b.addAction(" PC sees an unknown potion.");
    }
}

void RestoreHealth::use(Board& b, Player& p) {
    p.addHP(10);
    b.removeItem(position);
    if (not isKnown) {
        isKnown = true;
        b.addAction("PC uses and discovers the effects of a " + name + ".");
    } else {
        b.addAction("PC uses " + shortName + ".");
    }
}


PoisonHealth::PoisonHealth(Posn&& p)
    : Potion{Posn{p.x, p.y}, "Poison Health potion", "PH"} {
}

bool PoisonHealth::isKnown = false;

void PoisonHealth::setUnknown() { isKnown = false; }

void PoisonHealth::see(Board& b) {
    if (isKnown) {
        b.addAction(" PC sees a " + shortName + " potion.");
    } else {
        b.addAction(" PC sees an unknown potion.");
    }
}

void PoisonHealth::use(Board& b, Player& p) {
    p.addHP(-10);
    b.removeItem(position);
    if (not isKnown) {
        isKnown = true;
        b.addAction("PC uses and discovers the effects of a " + name + ".");
    } else {
        b.addAction("PC uses " + shortName + ".");
    }
}


BoostAtk::BoostAtk(Posn&& p)
    : Potion{Posn{p.x, p.y}, "Boost Attack potion", "BA"} {
}

bool BoostAtk::isKnown = false;

void BoostAtk::setUnknown() { isKnown = false; }

void BoostAtk::see(Board& b) {
    if (isKnown) {
        b.addAction(" PC sees a " + shortName + " potion.");
    } else {
        b.addAction(" PC sees an unknown potion.");
    }
}

void BoostAtk::use(Board& b, Player& p) {
    p.addATK(5);
    b.removeItem(position);
    if (not isKnown) {
        isKnown = true;
        b.addAction("PC uses and discovers the effects of a " + name + ".");
    } else {
        b.addAction("PC uses " + shortName + ".");
    }
}


WoundAtk::WoundAtk(Posn&& p)
    : Potion{Posn{p.x, p.y}, "Wound Attack potion", "WA"} {
}

bool WoundAtk::isKnown = false;

void WoundAtk::setUnknown() { isKnown = false; }

void WoundAtk::see(Board& b) {
    if (isKnown) {
        b.addAction(" PC sees a " + shortName + " potion.");
    } else {
        b.addAction(" PC sees an unknown potion.");
    }
}

void WoundAtk::use(Board& b, Player& p) {
    p.addATK(-5);
    b.removeItem(position);
    if (not isKnown) {
        isKnown = true;
        b.addAction("PC uses and discovers the effects of a " + name + ".");
    } else {
        b.addAction("PC uses " + shortName + ".");
    }
}


BoostDef::BoostDef(Posn&& p)
    : Potion{Posn{p.x, p.y}, "Boost Defense potion", "BD"} {
}

bool BoostDef::isKnown = false;

void BoostDef::setUnknown() { isKnown = false; }

void BoostDef::see(Board& b) {
    if (isKnown) {
        b.addAction(" PC sees a " + shortName + " potion.");
    } else {
        b.addAction(" PC sees an unknown potion.");
    }
}

void BoostDef::use(Board& b, Player& p) {
    p.addDEF(5);
    b.removeItem(position);
    if (not isKnown) {
        isKnown = true;
        b.addAction("PC uses and discovers the effects of a " + name + ".");
    } else {
        b.addAction("PC uses " + shortName + ".");
    }
}


WoundDef::WoundDef(Posn&& p)
    : Potion{Posn{p.x, p.y}, "Wound Defense potion", "WD"} {
}

bool WoundDef::isKnown = false;

void WoundDef::setUnknown() { isKnown = false; }

void WoundDef::see(Board& b) {
    if (isKnown) {
        b.addAction(" PC sees a " + shortName + " potion.");
    } else {
        b.addAction(" PC sees an unknown potion.");
    }
}

void WoundDef::use(Board& b, Player& p) {
    p.addDEF(-5);
    b.removeItem(position);
    if (not isKnown) {
        isKnown = true;
        b.addAction("PC uses and discovers the effects of a " + name + ".");
    } else {
        b.addAction("PC uses " + shortName + ".");
    }
}
