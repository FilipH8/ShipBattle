#ifndef SHIP_H
#define SHIP_H

#include <string>
#include <vector>
#include "table.h"

using namespace std;

class Table;
class Player;

class Ship
{
private:
    int sizeShip;
    int ver_hor;
    int numberOfShip;
    Coordinates stern;  // Rufa
    Coordinates bow;    // Dziób
    vector<Coordinates> shipCoordinates;
    vector<Coordinates> hitCoordinates;

    // Private methods for ship placement
    bool isWithinBounds(int letter, int number) const;
    bool isSurroundingClear(const Coordinates& coord, const Table& table) const;
    bool canPlaceShipPart(const Coordinates& coord, const Table& table) const;
    bool canCompletePlacement(const Coordinates& newCoord, 
                              const Coordinates& firstCoord, 
                              int remainingLength,
                              const Table& table) const;
    void placeShipPart(const Coordinates& coord, Table& table, Table &tableVidmo);
    void updateShipInfo(const vector<Coordinates>& coords);
    void displayPlacementInfo(const Coordinates& coord) const;

    // Private methods for attacking
    void markSurroundingFields(Table &table, int i, int j);
    Coordinates getValidShotCoordinates(Table &table);
    char getTargetFieldValue(Table &table, Coordinates coords);
    bool isHit(char targetField);
    void handleHit(Table &firstTable, Table &secondTable, Coordinates shotCoords, char targetField, Table &thrTable, Table &tableVidmo);
    void markHit(Table &table, Coordinates coords);
    bool isShipSunk(Table &firstTable, Table &secondTable, char shipSymbol);
    void handleSunkShip(Table &firstTable, Table &secondTable, char shipSymbol, Table &tableVidmo);
    void handleMiss(Table &table, Coordinates coords);
    void markFieldIfValid(Table &table, int i, int j);
    bool isValidCoordinate(int i, int j);
    void markFieldAsMiss(Table &table, int i, int j);
    void markShipAsSunk(Table &table);
    bool checkWinConditionInShip(Table &table);
    bool canPlaceHorizontally(const Coordinates& startCoord, int sizeShip, const Table& table) const;
    bool canPlaceVertically(const Coordinates& startCoord, int sizeShip, const Table& table) const;

    // Private methods for ship fitting
    bool shipFitsOnBoard(const Coordinates& start, bool horizontal, int size) const;
    bool isFieldClear(int letter, int number, const Table& table) const;
    bool isSurroundingClear(int letter, int number, const Table& table) const;
    bool canFitShip(const Coordinates& start, bool horizontal, int size, const Table& table);
    Coordinates placeFirstPart(Table& table);
    bool canPlaceShip(const Coordinates& startCoord, int sizeShip, int orientation, const Table& table) const;
    bool canFitShipAnywhere(int sizeShip, const Table& table) const;

    void hitAttTableChange(Table &table, Coordinates coords);
    void missAttTableChange(Table &table, Coordinates coords);

    void displayTables(const Table &firstTable, const Table &secondTable);

public:
    Ship(int sizeShip, int ver_hor, int numberOfShip);
    ~Ship();

    void setShip(Table &table, const Coordinates& startCoord, Table &tableVidmo);
    int getSize() const;
    bool cannotPlaceShip(const Coordinates& startCoord, int sizeShip, const Table& table) const;
    void placeShipAt(const Coordinates& startCoord, Table& table, Table &tableVidmo);
    void attack(Table &firstTable, Table &secondTable, Table &thrTable, Table &tableVidmo);
    bool validateNextPart(const Coordinates& newCoord, 
                          const vector<Coordinates>& shipCoords,
                          const Table& table,
                          int remainingLength) const;
};

#endif