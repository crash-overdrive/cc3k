#include "character.h"
#include "cell.h"
#include "board.h"

using namespace std;

Character::Character(Posn&& p, 
                     string displaySymbol,
                     int HP, 
                     int ATK, 
                     int DEF, 
                     int gold)
    : position{Posn{p}}, displaySymbol{displaySymbol}, HP{HP}, ATK{ATK}, DEF{DEF}, gold{gold} {
}

Character::~Character() {}


string Character::getDisplaySymbol() const {
    return displaySymbol;
}

int Character::getATK() const {
    return ATK;
}

Posn Character::getPosition() {
    return position;
}


void Character::addGold(int amount) {
    gold += amount;
}


void Character::setPosition(Posn& p) {
    position.x = p.x;
    position.y = p.y;
}

bool Character::dodges() {
    return false;
}

void Character::successfulAttack(Board& b, Character& target) {
    target.getHit(b, *this);
}

bool Character::isDead() const {
    return HP < 1;
}



ostream &operator<<(ostream &out, const Character &c) {
    out << c.getDisplaySymbol();
    return out;
}



Player::Player(string race, int HP, int ATK, int DEF)
    : Character{Posn{0,0}, "@", HP, ATK, DEF, 0}, // True position will be modified after intialization
    race{race},
    maxHP{HP}, 
    modifiedATK{ATK}, 
    modifiedDEF{DEF} {
}

string Player::getRace() const {
    return race;
}

int Player::getHP() const {
    return HP;
}

int Player::getATK() const {
    return modifiedATK;
}

int Player::getDEF() const {
    return modifiedDEF;
}

int Player::getGold() const {
    return gold;
}

void Player::addHP(int amount) {
    HP += amount;
    if (HP < 0) {
        HP = 0;
    } else if (HP > maxHP) {
        HP = maxHP;
    }
}

void Player::addATK(int amount) {
    modifiedATK += amount;
}

void Player::addDEF(int amount) {
    modifiedDEF += amount;
}

void Player::resetStats() {
    modifiedATK = ATK;
    modifiedDEF = DEF;
}


Posn Player::dirToPosn(string dir) {
    if (dir == "no") {
        return Posn(position.x - 1, position.y);
    } else if (dir == "ne") {
        return Posn(position.x - 1, position.y + 1);
    } else if (dir == "ea") {
        return Posn(position.x, position.y + 1);
    } else if (dir == "se") {
        return Posn(position.x + 1, position.y + 1);
    } else if (dir == "so") {
        return Posn(position.x + 1, position.y);
    } else if (dir == "sw") {
        return Posn(position.x + 1, position.y - 1);
    } else if (dir == "we") {
        return Posn(position.x, position.y - 1);
    } else if (dir == "nw") {
        return Posn(position.x - 1, position.y - 1);
    } else {
        throw out_of_range("Invalid direction");
    }
}

string Player::dirToFullName(string dir) {
    if (dir == "no") {
        return "North";
    } else if (dir == "ne") {
        return "NorthEast";
    } else if (dir == "ea") {
        return "East";
    } else if (dir == "se") {
        return "SouthEast";
    } else if (dir == "so") {
        return "South";
    } else if (dir == "sw") {
        return "SouthWest";
    } else if (dir == "we") {
        return "West";
    } else if (dir == "nw") {
        return "NorthWest";
    } else {
        throw out_of_range("Invalid direction");
    }
}


int Player::move(Board& b, string dir) {
    Posn targetPosition = dirToPosn(dir);
    string fullDirName = dirToFullName(dir);
    shared_ptr<Cell> targetCell = b.getCell(targetPosition);
    if (targetCell->isMoveable(*(this))) {
        b.moveCharacterInGrid(this->position, targetPosition);
        setPosition(targetPosition);
        b.addAction("PC moves " + fullDirName + ".");
        // Return -1 if changed floors, 1 if won game, 0 by default
        return targetCell->notifyFriendly(b);
    } else {
        b.addAction("Cannot move " + fullDirName + "!");
        return 0;
    }
}


void Player::attemptAttack(Board& b, string dir) {
    Posn targetPosition = dirToPosn(dir);
    shared_ptr<Cell> targetCell = b.getCell(targetPosition);
    shared_ptr<Character> target = targetCell->getCharacter();
    
    if (target == nullptr) {
        b.addAction("PC swings at nothing.");
    } else {
        if (target->dodges()) {
            b.addAction("PC swings at " + target->getDisplaySymbol() + " and misses." );
        } else {
            b.addAction("PC");
            successfulAttack(b, *target);
        }
    }
}

bool Player::dodges() {
    return rand()%2 == 0;
}

void Player::getHit(Board& b, Character& attacker) {
    double attackerATK = (double) attacker.getATK();
    double defenderDEF = (double) modifiedDEF;
    
    int dmg = (int) ceil((100.0 * attackerATK / (100.0 + defenderDEF)));
    int dmg_dealt = min(dmg, HP); // actual damage dealt is limited by remaining HP
    
    // We don't want to display monster hits if the player is already dead, so don't print 0 dmg hits
    // This is okay because monsters always attack for at least 1 dmg (non-0 ATK combined with ceil function)
    if (dmg_dealt != 0) {
        HP = HP - dmg_dealt;
        b.addAction(" " + attacker.getDisplaySymbol() + " deals " + to_string(dmg_dealt) + " damage to PC.");
    }
    
    // Player doesn't check if they're dead; board does
}


void Player::usePotion(Board& b, string dir) {
    Posn targetPosition = dirToPosn(dir);
    shared_ptr<Item> item_ptr = b.getCell(targetPosition)->getItem();
     // Check if the item is moveable to to make sure it's not gold
    if (item_ptr == nullptr || item_ptr->isMoveable()) {
        b.addAction("No potion there!");
    } else {
        item_ptr->use(b, *this);
    }
}


int Player::getScore() const {
    return gold;
}

void Player::takeTurn(Board& b) {}



Shade::Shade()
    : Player{"Shade", 125, 25, 25} {
}

Shade::~Shade() {}

int Shade::getScore() const {
    return 3*gold/2 + gold%2; // round up
}


Drow::Drow()
    : Player{"Drow", 150, 25, 15} {
}

Drow::~Drow() {}

void Drow::addHP(int amount) {
    HP += 3*amount/2 + amount%2; // round up
    if (HP < 0) {
        HP = 0;
    } else if (HP > maxHP) {
        HP = maxHP;
    }
}

void Drow::addATK(int amount) {
    modifiedATK += 3*amount/2 + amount%2; // round up
}

void Drow::addDEF(int amount) {
    modifiedDEF += 3*amount/2 + amount%2; // round up
}


Vampire::Vampire()
    : Player{"Vampire", 50, 25, 25} {
}

Vampire::~Vampire() {}

void Vampire::addHP(int amount) {
    HP += amount;
    if (HP < 0) {
        HP = 0;
    }
}

void Vampire::successfulAttack(Board& b, Character& target) {
    HP += 5;
    target.getHit(b, *this);
}


Troll::Troll()
    : Player{"Troll", 120, 25, 15} {
}

Troll::~Troll() {}

int Troll::move(Board& b, string dir) {
    addHP(5);
    Posn targetPosition = dirToPosn(dir);
    string fullDirName = dirToFullName(dir);
    shared_ptr<Cell> targetCell = b.getCell(targetPosition);
    if (targetCell->isMoveable(*(this))) {
        b.moveCharacterInGrid(this->position, targetPosition);
        setPosition(targetPosition);
        b.addAction("PC moves " + fullDirName + ".");
        // Return -1 if changed floors, 1 if won game, 0 by default
        return targetCell->notifyFriendly(b);
    } else {
        b.addAction("Cannot move " + fullDirName + "!");
        return 0;
    }
}

void Troll::attemptAttack(Board& b, string dir) {
    addHP(5);
    Posn targetPosition = dirToPosn(dir);
    shared_ptr<Cell> targetCell = b.getCell(targetPosition);
    shared_ptr<Character> target = targetCell->getCharacter();
    
    b.addAction("PC");
    if (target == nullptr) {
        b.addAction(" swings at nothing.");
    } else {
        if (target->dodges()) {
            b.addAction(" swings at " + target->getDisplaySymbol() + " and misses." );
        } else {
            successfulAttack(b, *target);
        }
    }
}

void Troll::usePotion(Board& b, string dir) {
    addHP(5);
    Posn targetPosition = dirToPosn(dir);
    shared_ptr<Item> item_ptr = b.getCell(targetPosition)->getItem();
     // Check if the item is moveable to to make sure it's not gold
    if (item_ptr == nullptr || item_ptr->isMoveable()) {
        b.addAction("No potion there!");
    } else {
        item_ptr->use(b, *this);
    }
}


Goblin::Goblin()
    : Player{"Goblin", 110, 15, 20} {
}

Goblin::~Goblin() {}

void Goblin::successfulAttack(Board& b, Character& target) {
    target.getHit(b, *this);
    if (target.isDead()) {
        gold += 5;
    }
}


void Goblin::getHit(Board& b, Character& attacker) {
    double attackerATK = (double) attacker.getATK();
    double defenderDEF = (double) modifiedDEF;
    
    double dmg_factor = 1.0;
    // Goblin takes 50% more damage from Orcs
    try {
        dynamic_cast<Orc&>(attacker);
        dmg_factor = 1.5;
    } catch (bad_cast) {
        // If not an Orc, take normal damage
    }
    
    int dmg = (int) ceil(dmg_factor * 100.0 * attackerATK / (100.0 + defenderDEF));
    int dmg_dealt = min(dmg, HP); // actual damage dealt is limited by remaining HP
    
    // We don't want to display monster hits if the player is already dead, so don't print 0 dmg hits
    // This is okay because monsters always attack for at least 1 dmg (non-0 ATK combined with ceil function)
    if (dmg_dealt != 0) {
        HP = HP - dmg_dealt;
        b.addAction(" " + attacker.getDisplaySymbol() + " deals " + to_string(dmg_dealt) + " damage to PC.");
    }
    
    // Player doesn't check if they're dead; board does
}



Enemy::Enemy(Posn&& p, string displaySymbol, int HP, int ATK, int DEF, int gold)
    : Character{Posn{p}, displaySymbol, HP, ATK, DEF, gold} {
}


void Enemy::takeTurn(Board& b) {
    vector<Posn> neighbours = position.getNeighbours();
    vector<Posn> candidateDestinations;
    for (int i = 0; i < (int)neighbours.size(); ++i) {
        if (neighbours[i] == b.getPlayer()->getPosition()) {
            // Do an attack turn and end
            attemptAttack(b, *(b.getPlayer()));
            return;
        }
        // While we're here, check if we'll be allowed to move to this location
        if (b.getCell(neighbours[i])->isMoveable(*(this))) {
            candidateDestinations.emplace_back(neighbours[i]);
        }
    }
    // If player not found, do a move turn
    if (candidateDestinations.size() != 0) {
        Posn targetPosition = candidateDestinations[rand()%candidateDestinations.size()];
        b.moveCharacterInGrid(this->position, targetPosition);
        setPosition(targetPosition);
    }
}

void Enemy::attemptAttack(Board& b, Player& defender) {
    if (defender.dodges()) {
        b.addAction(" " + displaySymbol + " swings at PC and misses." );
    } else {
        successfulAttack(b, defender);
    }
}

void Enemy::getHit(Board& b, Character& attacker) {
    double attackerATK = (double) attacker.getATK();
    double defenderDEF = (double) DEF;
    
    int dmg = (int) ceil((100.0 * attackerATK / (100.0 + defenderDEF)));
    int dmg_dealt = min(dmg, HP); // actual damage dealt is limited by remaining HP
    
    HP = HP - dmg_dealt;
    
    b.addAction(" deals " + to_string(dmg_dealt) + " damage to " + displaySymbol);
    
    if (isDead()) {
        attacker.addGold(gold);
        b.removeCharacter(position);
        b.addAction(" and kills it.");
    } else {
        b.addAction(" (" + to_string(HP) + " HP).");
    }
}


Human::Human(Posn&& p)
    : Enemy{Posn{p}, "H", 140, 20, 20, 0} {
}

Human::~Human() {}

void Human::getHit(Board& b, Character& attacker) {
    double attackerATK = (double) attacker.getATK();
    double defenderDEF = (double) DEF;
    
    int dmg = (int) ceil((100.0 * attackerATK / (100.0 + defenderDEF)));
    int dmg_dealt = min(dmg, HP); // actual damage dealt is limited by remaining HP
    
    HP = HP - dmg_dealt;
    
    b.addAction(" deals " + to_string(dmg_dealt) + " damage to " + displaySymbol);
    
    if (isDead()) {
        shared_ptr<HumanGoldPile> g = make_shared<HumanGoldPile>(Posn{position.x, position.y});
        b.addItem(g);
        b.removeCharacter(position);
        b.addAction(" and kills it.");
    } else {
        b.addAction(" (" + to_string(HP) + " HP).");
    }
}


Dwarf::Dwarf(Posn&& p)
    : Enemy{Posn{p}, "W", 100, 20, 30, rand()%2+1} {
}

Dwarf::~Dwarf() {}

void Dwarf::getHit(Board& b, Character& attacker) {
    // Vampires are allergic to dwarves; give them an additional -10 HP to get a net -5 HP 
    // (counteracting the +5 HP they've already given themselves)
    try {
        Vampire& v = dynamic_cast<Vampire&>(attacker);
        v.addHP(-10);
    } catch (bad_cast) {
        // Nothing special for non-Vampires
    }
    
    double attackerATK = (double) attacker.getATK();
    double defenderDEF = (double) DEF;
    
    int dmg = (int) ceil((100.0 * attackerATK / (100.0 + defenderDEF)));
    int dmg_dealt = min(dmg, HP); // actual damage dealt is limited by remaining HP
    
    HP = HP - dmg_dealt;
    
    b.addAction(" deals " + to_string(dmg_dealt) + " damage to " + displaySymbol);
    
    if (isDead()) {
        shared_ptr<HumanGoldPile> g = make_shared<HumanGoldPile>(Posn{position.x, position.y});
        b.addItem(g);
        b.removeCharacter(position);
        b.addAction(" and kills it.");
    } else {
        b.addAction(" (" + to_string(HP) + " HP).");
    }
}


Elf::Elf(Posn&& p)
    : Enemy{Posn{p}, "E", 140, 30, 10, rand()%2+1} {
}

Elf::~Elf() {}

void Elf::attemptAttack(Board& b, Player& defender) {
    if (defender.dodges()) {
        b.addAction(" " + displaySymbol + " swings at PC and misses." );
    } else {
        successfulAttack(b, defender);
    }
    
    try {
        dynamic_cast<Drow&>(defender);
    } catch (bad_cast) {
        // If not a Drow, do a second attack
        if (defender.dodges()) {
            b.addAction(" " + displaySymbol + " swings at PC and misses." );
        } else {
            successfulAttack(b, defender);
        }
    }
}


Orc::Orc(Posn&& p)
    : Enemy{Posn{p}, "O", 180, 30, 25, rand()%2+1} {
}

Orc::~Orc() {}


Merchant::Merchant(Posn&& p)
    : Enemy{Posn{p}, "M", 30, 70, 5, 0} {
}

Merchant::~Merchant() {}

bool Merchant::isHostile = false;

void Merchant::setFriendly() { isHostile = false; }

void Merchant::takeTurn(Board& b) {
    vector<Posn> neighbours = position.getNeighbours();
    vector<Posn> candidateDestinations;
    
    for (int i = 0; i < (int)neighbours.size(); ++i) {
        if (isHostile) { // Only attack if hostile
            if (neighbours[i] == b.getPlayer()->getPosition()) {
                // Do an attack turn and end
                attemptAttack(b, *(b.getPlayer()));
                return;
            }
        }
        // While we're here, check if we'll be allowed to move to this location
        if (b.getCell(neighbours[i])->isMoveable(*(this))) {
            candidateDestinations.emplace_back(neighbours[i]);
        }
    }
    
    // If not hostile or player not found, do a move turn
    if (candidateDestinations.size() != 0) {
        Posn targetPosition = candidateDestinations[rand()%candidateDestinations.size()];
        b.moveCharacterInGrid(this->position, targetPosition);
        setPosition(targetPosition);
    }
}

void Merchant::getHit(Board& b, Character& attacker) {
    // If getting hit, become hostile to Player
    isHostile = true;
    
    double attackerATK = (double) attacker.getATK();
    double defenderDEF = (double) DEF;
    
    int dmg = (int) ceil((100.0 * attackerATK / (100.0 + defenderDEF)));
    int dmg_dealt = min(dmg, HP); // actual damage dealt is limited by remaining HP
    
    HP = HP - dmg_dealt;
    
    b.addAction(" deals " + to_string(dmg_dealt) + " damage to " + displaySymbol);
    
    if (isDead()) {
        shared_ptr<MerchantHoard> g = make_shared<MerchantHoard>(Posn{position.x, position.y});
        b.addItem(g);
        b.removeCharacter(position);
        b.addAction(" and kills it.");
    } else {
        b.addAction(" (" + to_string(HP) + " HP).");
    }
}


Dragon::Dragon(Posn&& p)
    : Enemy{Posn{p}, "D", 150, 20, 20, 0}, hoard{nullptr} {
}

Dragon::~Dragon() {}

void Dragon::setHoard(shared_ptr<DragonHoard> h) {
    hoard = h;
}

void Dragon::takeTurn(Board& b) {
    vector<Posn> dragonNeighbours = position.getNeighbours();
    vector<Posn> hoardNeighbours = hoard->getPosition().getNeighbours();
    
    // Attack the player if they are next to this dragon or its hoard
    for (Posn p : dragonNeighbours) {
        if (p == b.getPlayer()->getPosition()) {
            // Do an attack turn and end
            attemptAttack(b, *(b.getPlayer()));
            return;
        }
    }
    
    for (Posn p : hoardNeighbours) {
        if (p == b.getPlayer()->getPosition()) {
            // Do an attack turn and end
            attemptAttack(b, *(b.getPlayer()));
            return;
        }
    }
}

void Dragon::attemptAttack(Board& b, Player& defender) {
    if (defender.dodges()) {
        b.addAction(" PC dodges " + displaySymbol + "'s fire breath." );
    } else {
        successfulAttack(b, defender);
    }
}

void Dragon::getHit(Board& b, Character& attacker) {
    double attackerATK = (double) attacker.getATK();
    double defenderDEF = (double) DEF;
    
    int dmg = (int) ceil((100.0 * attackerATK / (100.0 + defenderDEF)));
    int dmg_dealt = min(dmg, HP); // actual damage dealt is limited by remaining HP
    
    HP = HP - dmg_dealt;
    
    b.addAction(" deals " + to_string(dmg_dealt) + " damage to " + displaySymbol);
    
    if (isDead()) {
        hoard->setDragon(nullptr); // Remove its Hoard's reference to itself
        b.removeCharacter(position);
        b.addAction(" and kills it.");
    } else {
        b.addAction(" (" + to_string(HP) + " HP).");
    }
}


Halfling::Halfling(Posn&& p)
    : Enemy{Posn{p}, "L", 100, 15, 20, rand()%2+1} {
}

Halfling::~Halfling() {}

bool Halfling::dodges() {
    return rand()%2 == 0;
}

