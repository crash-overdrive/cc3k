#include <fstream>

#include "board.h"
#include "cell.h"
#include "character.h"
#include "item.h"

using namespace std;


void Board::setChambers() {
    // chamber 1
    for(int ii = 3; ii <= 6; ++ii) {
        for(int jj = 3; jj <= 28; ++jj) {
            CHAMBER_1.emplace_back(make_shared<Posn>(ii, jj));
        }
    }

    // chamber 2
    for(int ii = 3; ii <= 4; ++ii) {
        for(int jj = 39; jj <= 61; ++jj) {
            CHAMBER_2.emplace_back(make_shared<Posn>(ii, jj));
        }
    }
    for(int ii = 5; ii <= 5; ++ii) {
        for(int jj = 39; jj <= 70; ++jj) {
            CHAMBER_2.emplace_back(make_shared<Posn>(ii, jj));
        }
    }

    for(int ii = 6; ii <= 6; ++ii) {
        for(int jj = 39; jj <= 72; ++jj) {
            CHAMBER_2.emplace_back(make_shared<Posn>(ii, jj));
        }
    }

    for(int ii = 7; ii <= 12; ++ii) {
        for(int jj = 61; jj <= 74; ++jj) {
            CHAMBER_2.emplace_back(make_shared<Posn>(ii, jj));
        }
    }

    // chamber 3
    for(int ii = 10; ii <= 12; ++ii) {
        for(int jj = 38; jj <= 49; ++jj) {
            CHAMBER_3.emplace_back(make_shared<Posn>(ii, jj));
        }
    }

    // chamber 4
    for(int ii = 15; ii <= 21; ++ii) {
        for(int jj = 4; jj <= 16; ++jj) {
            CHAMBER_4.emplace_back(make_shared<Posn>(ii, jj));
        }
    }

    // chamber 5
    for(int ii = 16; ii <= 18; ++ii) {
        for(int jj = 65; jj <= 75; ++jj) {
            CHAMBER_5.emplace_back(make_shared<Posn>(ii, jj));
        }
    }

    for(int ii = 19; ii <= 21; ++ii) {
        for(int jj = 38; jj <= 75; ++jj) {
            CHAMBER_5.emplace_back(make_shared<Posn>(ii, jj));
        }
    }
}

Board::Board(string fileName, string race) {

    floor = 1;
    maxFloor = 5;
    actionResult = "";
    testMode = false;
    presetFile = fileName;

    // if no fileName, we're using a pregenerated grid    
    if(presetFile == "") isPreset = false;
    else isPreset = true;

    readBaseGrid();

    // store locations of chambers in the appropriate vector field
    setChambers();

    // create the player
    createPlayer(race);

    // spawn everything (pregenerated or random)
    if(isPreset) readPresetGrid(presetFile);
    else spawnGrid();

    // print grid
    cout << *this;
   
    actionResult = "";
}

shared_ptr<Player> Board::getPlayer() const {
    return PC;
}

vector<vector<shared_ptr<Cell>>> Board::getGrid() const {
    return grid;
}

int Board::getFloor() const { return floor; }

int Board::getMaxFloor() const { return maxFloor; }

string Board::getActionResult() const { return actionResult; }

void Board::createPlayer(std::string type) {
    if (type == "Shade") {
        PC = make_shared<Shade>();
    } else if (type == "Drow") {
        PC = make_shared<Drow>();
    } else if (type == "Vampire") {
        PC = make_shared<Vampire>();
    } else if (type == "Troll") {
        PC = make_shared<Troll>();
    } else if (type == "Goblin") {
        PC = make_shared<Goblin>();
    } 
    addAction("Player character has spawned.");
}

void Board::readBaseGrid(std::string fileName) {
    
    ifstream file{fileName};
    string readRow;
    int rowNum = 0;

    // read in line by line
    while(getline(file, readRow)) {

        // initialize the row
        vector<shared_ptr<Cell>> row;
        grid.emplace_back(row);

        int colNum = 0;

        // read line char by char
        for(char c : readRow) {
            if(c == ' ') {
                auto cell = make_shared<Blank>(rowNum, colNum);
                grid[grid.size() - 1].emplace_back(cell);
            } else if (c == '|') {
                auto cell = make_shared<WallVert>(rowNum, colNum);
                grid[grid.size() - 1].emplace_back(cell);
            } else if (c == '-') {
                auto cell = make_shared<WallHor>(rowNum, colNum);
                grid[grid.size() - 1].emplace_back(cell);
            } else if (c == '#') {
                auto cell = make_shared<Passageway>(rowNum, colNum);
                grid[grid.size() - 1].emplace_back(cell);
            } else if (c == '+') {
                auto cell = make_shared<Doorway>(rowNum, colNum);
                grid[grid.size() - 1].emplace_back(cell);
            // assume it's a floor cell - perhaps should throw an exception
            } else { 
                auto cell = make_shared<Floor>(rowNum, colNum);
                grid[grid.size() - 1].emplace_back(cell);
            }
            
            ++colNum;
        }

        ++rowNum;
    }
}

void Board::readPresetGrid(std::string fileName) {
    // Read in a pre-set file, place the already-existant PC where specified,
    // and create the stairway, monsters and items wherever else specified.
    
    // Decided to go with a line by line approach to more
    // easily identify when a single grid is done (since one file will
    // contain five concatenated floor maps)
    ifstream file(fileName);
    string s;
    char c;
    int floorcount = 2*floor; // decrementer to track which floor we're looking at
    int row = 0; // Track which row of the grid we're on
    Posn cur_position = Posn{0,0};
    while(getline(file, s)) {
        // Use the header and footer rows of the map to count floors
        if (s == "|-----------------------------------------------------------------------------|") {
            if (floorcount == 1) { // return if we're at the end of our floor
                return;
            } else {
                floorcount -= 1;
                row = 0; // reset our row count at start of each floor
            }
        } else if (floorcount == 1) { // Only perform cell operations when we're on the right floor
            for(int col = 0; col < (int)s.length(); col++) {
                c = s[col];
                cur_position.x = row;
                cur_position.y = col;
                
                getCell(cur_position)->removeCharacter();
                getCell(cur_position)->removeItem();
                // 0 - RH, 1 - BA, 2 - BD, 3 - PH, 4 - WA, 5 - WD, 6 - normal gold pile, 7 - small hoard, 8 - merchant hoard, 9 - dragon hoard.
                if (c == '@') { // Player
                    // Player already exists at this point, just store it and tell it where it is
                    getCell(cur_position)->setCharacter(PC);
                    PC->setPosition(cur_position);
                } else if (c == '\\') { // Stairway
                    // Since the previously existing cell was a shared ptr, no need to worry about
                    // cleaning it up
                    grid[row][col] = make_shared<Stairway>(row, col);
                    // update position of stairs
                    stairsPosn = make_shared<Posn>(row, col);
                } else if (c == '0') { // Potions
                    shared_ptr<RestoreHealth> potion = make_shared<RestoreHealth>(Posn{row, col});
                    items.emplace_back(potion);
                    getCell(cur_position)->setItem(potion);
                } else if (c == '1') { // Potions
                    shared_ptr<BoostAtk> potion = make_shared<BoostAtk>(Posn{row, col});
                    items.emplace_back(potion);
                    getCell(cur_position)->setItem(potion);
                } else if (c == '2') { // Potions
                    shared_ptr<BoostDef> potion = make_shared<BoostDef>(Posn{row, col});
                    items.emplace_back(potion);
                    getCell(cur_position)->setItem(potion);
                } else if (c == '3') { // Potions
                    shared_ptr<PoisonHealth> potion = make_shared<PoisonHealth>(Posn{row, col});
                    items.emplace_back(potion);
                    getCell(cur_position)->setItem(potion);
                } else if (c == '4') { // Potions
                    shared_ptr<WoundAtk> potion = make_shared<WoundAtk>(Posn{row, col});
                    items.emplace_back(potion);
                    getCell(cur_position)->setItem(potion);
                } else if (c == '5') { // Potions
                    shared_ptr<WoundDef> potion = make_shared<WoundDef>(Posn{row, col});
                    items.emplace_back(potion);
                    getCell(cur_position)->setItem(potion);
                } else if (c == '6') { // Treasure
                    shared_ptr<NormalGoldPile> gold = make_shared<NormalGoldPile>(Posn{row, col});
                    items.emplace_back(gold);
                    getCell(cur_position)->setItem(gold);
                } else if (c == '7') {
                    shared_ptr<SmallGoldPile> gold = make_shared<SmallGoldPile>(Posn{row, col});
                    items.emplace_back(gold);
                    getCell(cur_position)->setItem(gold);
                } else if (c == 'H') { // Enemies
                    shared_ptr<Human> enemy = make_shared<Human>(Posn{row, col});
                    enemies.emplace_back(enemy);
                    getCell(cur_position)->setCharacter(enemy);
                } else if (c == 'W') {
                    shared_ptr<Dwarf> enemy = make_shared<Dwarf>(Posn{row, col});
                    enemies.emplace_back(enemy);
                    getCell(cur_position)->setCharacter(enemy);
                } else if (c == 'E') {
                    shared_ptr<Elf> enemy = make_shared<Elf>(Posn{row, col});
                    enemies.emplace_back(enemy);
                    getCell(cur_position)->setCharacter(enemy);
                } else if (c == 'O') {
                    shared_ptr<Orc> enemy = make_shared<Orc>(Posn{row, col});
                    enemies.emplace_back(enemy);
                    getCell(cur_position)->setCharacter(enemy);
                } else if (c == 'M') {
                    shared_ptr<Merchant> enemy = make_shared<Merchant>(Posn{row, col});
                    enemies.emplace_back(enemy);
                    getCell(cur_position)->setCharacter(enemy);
                } else if (c == 'L') {
                    shared_ptr<Halfling> enemy = make_shared<Halfling>(Posn{row, col});
                    enemies.emplace_back(enemy);
                    getCell(cur_position)->setCharacter(enemy); 
                } else if (c == 'D') {
                    // Create and add the dragon
                    shared_ptr<Dragon> dragon = make_shared<Dragon>(Posn{row, col});
                    enemies.emplace_back(dragon);
                    getCell(cur_position)->setCharacter(dragon);
                    
                    // Search the neighbouring cells for an unpaired DragonHoard. (We really only
                    // care about the four cells that may have already had objects added, but this
                    // simplifies the code). If one is found, pair it to this.
                    for (Posn p : cur_position.getNeighbours()) {
                        shared_ptr<Item> item = getCell(p)->getItem();
                        shared_ptr<DragonHoard> hoard = dynamic_pointer_cast<DragonHoard>(item);
                        if (hoard) {
                            // Pair these two up and move on to the next input
                            dragon->setHoard(hoard);
                            hoard->setDragon(dragon);
                            break;
                        }
                    }
                } else if (c == '9') {
                    // Create and add the Dragon Hoard
                    shared_ptr<DragonHoard> hoard = make_shared<DragonHoard>(Posn{row, col});
                    items.emplace_back(hoard);
                    getCell(cur_position)->setItem(hoard);
                    
                    // Search the neighbouring cells for an unpaired Dragon. (We really only
                    // care about the four cells that may have already had objects added, but this
                    // simplifies the code). If one is found, pair it to this.
                    for (Posn p : cur_position.getNeighbours()) {
                        shared_ptr<Character> character = getCell(p)->getCharacter();
                        shared_ptr<Dragon> dragon = dynamic_pointer_cast<Dragon>(character);
                        if (dragon) {
                            // Pair these two up and move on to the next input
                            hoard->setDragon(dragon);
                            dragon->setHoard(hoard);
                            break;
                        }
                    }
                }
            }
        }
        ++row;
    }
}

shared_ptr<Cell> Board::getSpawnCell(bool isStairs, bool isPC, bool isDragonHoard) {
    // pick a chamber
    vector<int> possibleChambers;
    for(int ii = 1; ii <= 5; ++ii) {
         if(! (isStairs && PCChamber == ii) ) {
            possibleChambers.emplace_back(ii);
         }
    }
 
    // keep trying chambers until you get one with a free cell
    while(possibleChambers.size() > 0) {
        int randNum = rand() % possibleChambers.size();
        int chamber = possibleChambers[randNum];
        vector<int>::iterator chamberPos = possibleChambers.begin() + randNum;

        vector<shared_ptr<Posn>> possiblePosns;

        if(chamber == 1) {
            possiblePosns = CHAMBER_1;
        } else if(chamber == 2) {
            possiblePosns = CHAMBER_2;
        } else if(chamber == 3) {
            possiblePosns = CHAMBER_3;
        } else if(chamber == 4) {
            possiblePosns = CHAMBER_4;
        } else if(chamber == 5) {
            possiblePosns = CHAMBER_5;
        } else {
            throw out_of_range("invalid chamber");
        }

        // make dummyEnemy so that we can call isMoveable
        Orc dummyEnemy{Posn(0,0)};

        vector<shared_ptr<Cell>> freeCells;

        // check which cells are free in this chamber
        for(auto posn : possiblePosns) {
            if(getCell(*posn)->isMoveable(dummyEnemy)) {
                freeCells.emplace_back(getCell(*posn));
            }
        }

        // if there is at least one free cell, pick a random one
        if(freeCells.size() > 0) {
            if(isPC) PCChamber = chamber;
            if(! isDragonHoard) return freeCells[rand() % freeCells.size()];
            
            // it is a dragon hoard so need to make sure there's room for a dragon
            int randNum = rand() % freeCells.size();
            while(freeCells.size() > 0) {
                vector<shared_ptr<Cell>>::iterator cellPos = freeCells.begin() + randNum;
                shared_ptr<Cell> cell = freeCells[randNum];

                // check that there's at least one neighbour free
                for(auto neighbourPosn : cell->getPosition().getNeighbours()) {
                     shared_ptr<Cell> neighbour = getCell(neighbourPosn);
                     if(neighbour->isMoveable(dummyEnemy)) return cell;
                }
                
                // neighbour is not free so remove and try a different neighbour
                freeCells.erase(cellPos);
                randNum = rand() % freeCells.size();
            }
        } 

        // remove the current chamber from the list of possible
        // chambers and try again
        possibleChambers.erase(chamberPos);
    }

    // no more chambers to pick from
    throw out_of_range("no more free chambers");
}

void Board::spawnGrid() {
    // spawn player
    auto spawnCell = getSpawnCell(false, true, false);
    spawnCell->setCharacter(PC);
    Posn playerPosn = spawnCell->getPosition();
    PC->setPosition(playerPosn);

    // spawn stairs
    spawnCell = getSpawnCell(true, false, false);
    stairsPosn = make_shared<Posn>(spawnCell->getPosition());
    int x = spawnCell->getPosition().x;
    int y = spawnCell->getPosition().y;
    grid[x][y] = make_shared<Stairway>(x, y);

    // spawn potions
    for(int ii = 0; ii < 10; ++ii) {
        spawnCell = getSpawnCell(false, false, false);
        int row = spawnCell->getPosition().x;
        int col = spawnCell->getPosition().y;

        shared_ptr<Potion> potion;

        // pick potion type
        int randNum = rand() % 6;
        if(randNum == 0) {
            potion = make_shared<RestoreHealth>(Posn{row,col});
        } else if (randNum == 1) {
            potion = make_shared<PoisonHealth>(Posn{row,col});
        } else if (randNum == 2) {
            potion = make_shared<BoostAtk>(Posn{row,col});
        } else if (randNum == 3) {
            potion = make_shared<WoundAtk>(Posn{row,col});
        } else if (randNum == 4) {
            potion = make_shared<BoostDef>(Posn{row,col});
        } else if (randNum == 5) {
            potion = make_shared<WoundDef>(Posn{row,col});
        } else {
            throw out_of_range("Not a valid potion type");
        }

        spawnCell->setItem(potion);
        items.emplace_back(potion);
    } 

    // spawn gold
    for(int ii = 0; ii < 10; ++ii) {
        spawnCell = getSpawnCell(false, false, false);
        int row = spawnCell->getPosition().x;
        int col = spawnCell->getPosition().y;

        shared_ptr<Treasure> treasure;

        // pick treasure type
        int randNum = rand() % 8;
        if(randNum >= 0 && randNum <= 4) {
            treasure = make_shared<NormalGoldPile>(Posn{row,col});
        } else if (randNum == 5) {
            spawnCell = getSpawnCell(false, false, true);
            row = spawnCell->getPosition().x;
            col = spawnCell->getPosition().y;
            auto hoard = make_shared<DragonHoard>(Posn{row,col});
            vector<Posn> neighbours = spawnCell->getPosition().getNeighbours();
            // check which neighbours are free
            vector<shared_ptr<Cell>> freeCells;
            
            for (Posn neighbour : neighbours) {
                shared_ptr<Cell> neighbourCell = getCell(neighbour);
                Orc dummyEnemy{Posn{0,0}};
                if(neighbourCell->isMoveable(dummyEnemy)) {
                    freeCells.emplace_back(neighbourCell);
                }
            }

            // pick a cell for the dragon
            auto dragonCell = freeCells[rand() % freeCells.size()];
            int dragonRow = dragonCell->getPosition().x;
            int dragonCol = dragonCell->getPosition().y;
            shared_ptr<Dragon> dragon = make_shared<Dragon>(Posn{dragonRow,dragonCol});
            dragonCell->setCharacter(dragon);
            // link together dragon and dragon hoard
            dragon->setHoard(hoard);
            hoard->setDragon(dragon);
            treasure = hoard;
            enemies.emplace_back(dragon);
        } else if (randNum == 6 || randNum == 7) {
            treasure = make_shared<SmallGoldPile>(Posn{row,col});
        } else {
            throw out_of_range("Not a valid treasure type");
        }

        spawnCell->setItem(treasure);
        items.emplace_back(treasure);
    }

    // spawn enemies
    for(int ii = 0; ii < 20; ++ii) {
        spawnCell = getSpawnCell(false, false, false);
        int row = spawnCell->getPosition().x;
        int col = spawnCell->getPosition().y;

        shared_ptr<Enemy> enemy;

        // pick potion type
        int randNum = rand() % 18;
        if(randNum >= 0 && randNum <= 3) {
            enemy = make_shared<Human>(Posn{row,col});
        } else if (randNum >= 4 && randNum <= 6) {
            enemy = make_shared<Dwarf>(Posn{row, col});
        } else if (randNum >= 7 && randNum <= 11) {
           enemy = make_shared<Halfling>(Posn{row,col});
        } else if (randNum == 12 || randNum == 13) {
            enemy = make_shared<Elf>(Posn{row,col});
        } else if (randNum == 14 || randNum == 15) {
            enemy = make_shared<Orc>(Posn{row,col});
        } else if (randNum == 16 || randNum == 17) {
            enemy = make_shared<Merchant>(Posn{row,col});
        } else {
            throw out_of_range("Not a valid treasure type");
        }

        spawnCell->setCharacter(enemy);
        enemies.emplace_back(enemy);
    }


}

void Board::addAction(string action) {
    actionResult += action;
} 

void Board::clearAction() {
    actionResult = "";
}

shared_ptr<Cell> Board::getCell(const Posn &position) const {
    int x = position.x;
    int y = position.y;
    return grid[x][y];
}

void Board::moveCharacterInGrid(const Posn &fromPosn, const Posn &toPosn) {
    if (fromPosn == toPosn) return;
    // Note: Only moves pointers, doesn't check if move is valid
    getCell(toPosn)->setCharacter(getCell(fromPosn)->getCharacter());
    getCell(fromPosn)->setCharacter(nullptr);
}

void Board::changeFloor() {
    for(auto enemy : enemies) {
        Posn enemyPosn = enemy->getPosition();
        removeCharacter(enemyPosn);
    }

    // get rid of all the enemies
    enemies.clear();
    for(auto item : items) {
        Posn itemPosn = item->getPosition();
        removeItem(itemPosn);
    }

    // get rid of all the times
    items.clear();
    // remove PC from grid
    Posn pcPosn = PC->getPosition();
    removeCharacter(pcPosn);

    // reset ATK and DEF modifiers
    PC->resetStats();
    // remove location of stairs
    int x = stairsPosn->x;
    int y = stairsPosn->y;
    grid[x][y] = make_shared<Floor>(x,y);
    stairsPosn = nullptr;
    ++floor;

    // generate the new floor
    if(isPreset) readPresetGrid(presetFile);
    else spawnGrid();
}

void Board::endGame() {
    // reset static variables
    RestoreHealth::setUnknown();
    PoisonHealth::setUnknown();
    BoostAtk::setUnknown();
    WoundAtk::setUnknown();
    BoostDef::setUnknown();
    WoundDef::setUnknown();
    Merchant::setFriendly();
}

void Board::takeEnemyTurns() {
    // enemies won't take a turn if in testMode
    if(testMode) return;
   
    // store the old state of the grid in order not to call
    //     Enemy::takeTurn() more than once per enemy
   vector<vector<shared_ptr<Cell>>> gridCopy;

   for(auto row : grid) {

        // initialize the row
        vector<shared_ptr<Cell>> rowCopy;
        gridCopy.emplace_back(rowCopy);

        // copy each cell
        for(auto cell : row) {
            // note cellCopy doesn't need to know its position so giving
            // dummy position (0,0)
            auto cellCopy = make_shared<Floor>(0,0);
            cellCopy->setCharacter(cell->getCharacter());
            gridCopy[gridCopy.size() - 1].emplace_back(cellCopy);
        }
    }


    // go through every cell in gridCopy and call takeTurn()
    //     this won't do anything if there isn't any enemy in
    //     cell
    for(auto copyRow : gridCopy) {
        for(auto copyCell : copyRow) {
            auto character = copyCell->getCharacter();
            // check for nullptr (no character in the cell)
            if(character != nullptr) {
                // will do nothing if called on a Player
                copyCell->getCharacter()->takeTurn(*this);
            }
        }
    }
}

void Board::removeCharacter(Posn &position) {
    getCell(position)->setCharacter(nullptr);
}

void Board::addItem(shared_ptr<Item> item) {
    // For adding gold piles dropped by enemies to the board
    items.emplace_back(item);
    getCell(item->getPosition())->setItem(item);
}

void Board::removeItem(Posn &position) {
    getCell(position)->setItem(nullptr);
}

bool Board::playerDied() {
    addAction(" You have died! Better luck next time.");
    endGame();

    // print the board
    cout << *this;
    cout << endl;
    cout << "Final score: " << PC->getScore() << endl;
    return true;
}

bool Board::move(const string dir) {
    int result = PC->move(*this, dir);

    // game was won
    if(result == 1) {
        // print the board
        cout << *this;
        cout << endl;
        cout << "Final score: " << PC->getScore() << endl;
        return true;
    } 

    if(result == 0) {
        takeEnemyTurns();
    }

    if(PC->isDead()) {
        return playerDied();
    }

    // print board
    cout << *this;
    return false; 
}

bool Board::attack(const string dir) {
    PC->attemptAttack(*this, dir);

    if(PC->isDead()) {
        return playerDied();
    }

    takeEnemyTurns();

    if(PC->isDead()) {
        return playerDied();
    }

    // print the board
    cout << *this;
    return false;
}

bool Board::usePotion(const string dir) {
    PC->usePotion(*this, dir);

    if(PC->isDead()) return playerDied();

    takeEnemyTurns();

    if(PC->isDead()) return playerDied();

    // print the board
    cout << *this;
    return false;
}

void Board::toggleEnemyMode() {
    testMode = (! testMode);

    cout << "You have toggled the testing mode. ";

    if(testMode) cout << "Enemies will not move or attack you." << endl;
    else cout << "Enemies will move and attack you again." << endl;
}

ostream &operator<<(ostream &out, Board &b) {

    for(auto row : b.getGrid()) {
        for (auto cell : row) {
            out << *cell;
        }
        out << endl;
    }

    shared_ptr<Player> PC = b.getPlayer();

    // bottom stats below the grid

    // keep track of max width to right align floor number
    const int MAX_CHAR = 79;
    int charRemaining = MAX_CHAR;

    // print out race and gold
    string raceAndGoldString = "Race: " + PC->getRace() + " Gold: ";
    raceAndGoldString += to_string(PC->getGold());
    charRemaining -= raceAndGoldString.length();
    out << raceAndGoldString;

    // create floor string
    string floorString = "Floor: " + to_string(b.getFloor()) + "   ";
    charRemaining -= floorString.length();
    
    // fill middle with spaces
    out << string(charRemaining, ' ');

    // print out floor number
    out << floorString << endl;

    // print out all other stats
    out << "HP: " << PC->getHP() << endl;
    out << "Atk: " << PC->getATK() << endl;
    out << "Def: " << PC->getDEF() << endl;
    out << "Action: " << b.getActionResult() << endl;

    // clear actionResult string
    b.clearAction();
    return out;
}
