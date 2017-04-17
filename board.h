#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>

#include "posn.h"
#include "cell.h"
#include "character.h"
#include "item.h"

class Board {
    std::vector<std::vector<std::shared_ptr<Cell>>> grid;
    std::vector<std::shared_ptr<Posn>> CHAMBER_1;
    std::vector<std::shared_ptr<Posn>> CHAMBER_2;
    std::vector<std::shared_ptr<Posn>> CHAMBER_3;
    std::vector<std::shared_ptr<Posn>> CHAMBER_4;
    std::vector<std::shared_ptr<Posn>> CHAMBER_5;
    std::shared_ptr<Player> PC;
    int PCChamber;
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::vector<std::shared_ptr<Item>> items;
    int floor;
    int maxFloor;
    bool isPreset;
    std::string presetFile;
    bool testMode;
    std::shared_ptr<Posn> stairsPosn;
    
    // stores the string that will be printed for "Action:"
    std::string actionResult;
    
    void setChambers();
    void readBaseGrid(std::string fileName = "blank_floor.txt");
    
    //note: only works when specified capitalized (e.g. "Shade")
    void createPlayer(std::string type);
    
    void readPresetGrid(std::string fileName);
    void spawnGrid();
    void takeEnemyTurns();
    bool playerDied();
    std::shared_ptr<Cell> getSpawnCell(bool isStairs, bool isPC, bool isDragonHoard);
    
    public:
    Board(std::string presetFile, std::string race);

    // getters
    int getFloor() const;
    int getMaxFloor() const;
    std::shared_ptr<Player> getPlayer() const;
    
    std::vector<std::vector<std::shared_ptr<Cell>>> getGrid() const;
    std::string getActionResult() const;
    
    // other methods
    
    void addAction(std::string action);
    void clearAction();
    
    std::shared_ptr<Cell> getCell(const Posn &position) const;
    
    void moveCharacterInGrid(const Posn &fromPosn, const Posn &toPosn);
    void removeCharacter(Posn &position);
    void addItem(std::shared_ptr<Item> item);
    void removeItem(Posn &position);
    
    bool move(const std::string dir);
    bool attack(const std::string dir);
    bool usePotion(const std::string dir);
    
    void changeFloor();
    void endGame();
   
    // toggle testing mode (changes whether enemies will attack)
    void toggleEnemyMode();
};

std::ostream &operator<<(std::ostream &out, Board &b);

#endif
