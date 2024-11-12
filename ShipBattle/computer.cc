#include "computer.h"
#include <iostream>
#include <algorithm>
#include "table.h"

using namespace std;

Computer::Computer() : 
    gen(rd()),
    dis(1, 10),
    hunting(false)
{
    lastHit = {0, 0};
}

Computer::~Computer() {}

// Funkcja ustawia statek na planszy komputera.
void Computer::set_Ship(Table &table) {
    srand(static_cast<unsigned int>(time(0))); 
    cout << "Computer is placing ships..." << endl;

    const int shipLengths[] = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
    int shipNumber = 0;

    
    table.setTable();

    for (int length : shipLengths) {
        placeShip(table, length, shipNumber);
        shipNumber++;
    }

    cout << "All ships placed successfully!" << endl;
}

// Funkcja umieszcza statek na planszy w losowej pozycji.
void Computer::placeShip(Table &table, int length, int shipNumber) {
    bool shipPlaced = false;

    while (!shipPlaced) {
        ShipPlacement placement = generateRandomPlacement(length);
        
        if (canPlaceShip(table, placement, length)) {
            addShipToTable(table, placement, length, shipNumber);
            shipPlaced = true;
        }
    }
}

// Funkcja generuje losową pozycję dla statku.
ShipPlacement Computer::generateRandomPlacement(int length) {
    ShipPlacement placement;
    placement.letter = 1 + (rand() % 10);  
    placement.number = 1 + (rand() % 10);  
    placement.isVertical = rand() % 2;

    return placement;
}

// Funkcja sprawdza, czy statek można umieścić w danej pozycji.
bool Computer::canPlaceShip(Table &table, const ShipPlacement &placement, int length) {
    
    if (placement.isVertical && (placement.letter + length > 10)) return false;
    if (!placement.isVertical && (placement.number + length > 10)) return false;

    
    return checkShipArea(table, placement, length);
}

// Funkcja sprawdza, czy obszar, gdzie ma być umieszczony statek, jest wolny.
bool Computer::checkShipArea(Table &table, const ShipPlacement &placement, int length) {
    for (int i = -1; i <= length; i++) {
        for (int j = -1; j <= 1; j++) {
            int checkL = placement.isVertical ? placement.letter + i : placement.letter + j;
            int checkN = placement.isVertical ? placement.number + j : placement.number + i;

            if (checkL >= 1 && checkL <= 10 && checkN >= 1 && checkN <= 10) {
                if (table.table[checkL][checkN] != '~') {
                    return false;
                }
            }
        }
    }
    return true;
}

// Funkcja dodaje statek do planszy.
void Computer::addShipToTable(Table &table, const ShipPlacement &placement, int length, int shipNumber) {
    for (int i = 0; i < length; i++) {
        int placeL = placement.isVertical ? placement.letter + i : placement.letter;
        int placeN = placement.isVertical ? placement.number : placement.number + i;
        table.table[placeL][placeN] = '0' + (shipNumber % 10);
    }
}

// Funkcja obsługuje atak komputera na przeciwnika.
void Computer::att_Ship(Table &firstTable, Table &computerViewTable, Table &tableVidmo) {
    srand(static_cast<unsigned int>(time(0))); 
    cout << "Computer is attacking..." << endl;
    
    static vector<Coordinates> potentialTargets;
    bool hitOccurred = false;
    
    do {
        Coordinates shotCoords = selectTargetCoordinates(computerViewTable, potentialTargets);
        
        char targetField = getTargetFieldValue(firstTable, shotCoords);
        
        if (isHit(targetField)) {
            handleHit(firstTable, computerViewTable, shotCoords, targetField, potentialTargets, tableVidmo);
            hitAttTableChange(tableVidmo, shotCoords);
            hitOccurred = true;
        } else {
            handleMiss(computerViewTable, shotCoords);
            missAttTableChange(tableVidmo, shotCoords);
            hitOccurred = false;
        }
        
        
    } while (hitOccurred);
}

// Funkcja wybiera współrzędne strzału.
Coordinates Computer::selectTargetCoordinates(Table &computerViewTable, vector<Coordinates> &potentialTargets) {
    Coordinates shotCoords;
    bool validShot = false;
    
    
    if (!potentialTargets.empty()) {
        shotCoords = potentialTargets.back();
        potentialTargets.pop_back();
        return shotCoords;
    }
    
    
    do {
        shotCoords.letter = 1 + (rand() % 10);  
        shotCoords.number = 1 + (rand() % 10);  
        
        if (computerViewTable.table[shotCoords.letter][shotCoords.number] == '~') {
            validShot = true;
        }
    } while (!validShot);
    
    return shotCoords;
}

// Funkcja zwraca wartość pola docelowego na planszy.
char Computer::getTargetFieldValue(Table &firstTable, Coordinates &shotCoords) {
    return firstTable.table[shotCoords.letter][shotCoords.number];
}

// Funkcja sprawdza, czy strzał trafił w statek.
bool Computer::isHit(char targetField) {
    return targetField != '~' && targetField != 'O' && targetField != 'H';
}

// Funkcja obsługuje sytuację trafienia w statek.
void Computer::handleHit(Table &firstTable, Table &computerViewTable, Coordinates &shotCoords, 
                         char targetField, vector<Coordinates> &potentialTargets, Table &tableVidmo) {
    cout << "Computer hit at " 
              << (char)(shotCoords.letter + 'A' - 1) 
              << shotCoords.number << "!" << endl;
    
    markHit(computerViewTable, shotCoords);
    addAdjacentTargets(computerViewTable, shotCoords, potentialTargets);
    
    if (isShipSunk(firstTable, computerViewTable, targetField)) {
        handleSunkShip(firstTable, computerViewTable, targetField, tableVidmo);
        potentialTargets.clear();
    }
}

// Funkcja aktualizuje planszę przeciwnika, oznaczając pole jako trafione.
void Computer::hitAttTableChange(Table &table, Coordinates coords)
{
    table.table[coords.letter][coords.number] = 'H';
}

// Funkcja aktualizuje planszę przeciwnika, oznaczając pole jako nietrafione.
void Computer::missAttTableChange(Table &table, Coordinates coords)
{
    table.table[coords.letter][coords.number] = 'O';
}

// Funkcja oznacza pole jako trafione na planszy komputera.
void Computer::markHit(Table &computerViewTable, Coordinates &shotCoords) {
    computerViewTable.table[shotCoords.letter][shotCoords.number] = 'H';
}

// Funkcja dodaje potencjalne cele do listy na podstawie strzału.
void Computer::addAdjacentTargets(Table &computerViewTable, Coordinates &shotCoords, 
                                   vector<Coordinates> &potentialTargets) {
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    
    for (auto& dir : directions) {
        Coordinates newCoord = {
            shotCoords.letter + dir[0], 
            shotCoords.number + dir[1]
        };
        
        
        if (isValidCoordinate(newCoord)) {
            
            if (computerViewTable.table[newCoord.letter][newCoord.number] == '~') {
                potentialTargets.push_back(newCoord);
            }
        }
    }
}

// Funkcja sprawdza, czy współrzędne są w dozwolonych granicach.
bool Computer::isValidCoordinate(Coordinates &coord) {
    return coord.letter >= 1 && coord.letter <= 10 && 
           coord.number >= 1 && coord.number <= 10;
}

// Funkcja sprawdza, czy statek został zatopiony.
bool Computer::isShipSunk(Table &firstTable, Table &computerViewTable, char targetField) {
    for (int i = 1; i <= 10; i++) {
        for (int j = 1; j <= 10; j++) {
            if (firstTable.table[i][j] == targetField) {
                if (computerViewTable.table[i][j] != 'H') {
                    return false;
                }
            }
        }
    }
    return true;
}

// Funkcja obsługuje sytuację, gdy statek został zatopiony.
void Computer::handleSunkShip(Table &firstTable, Table &computerViewTable, char targetField, Table &tableVidmo) {
    cout << "Computer sunk a ship!" << endl;
    
    

    for (int i = 1; i <= 10; i++) {
        for (int j = 1; j <= 10; j++) {
            if (firstTable.table[i][j] == targetField) {
                computerViewTable.table[i][j] = 'S';
                markSurroundingFields(computerViewTable, i, j);
                markSurroundingFields(tableVidmo, i, j);
            }
        }
    }
}

// Funkcja oznacza otaczające pola jako nietrafione.
void Computer::markSurroundingFields(Table &computerViewTable, int i, int j) {
    for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
            int newI = i + di;
            int newJ = j + dj;
            
            if (newI >= 1 && newI <= 10 && newJ >= 1 && newJ <= 10) {
                if (computerViewTable.table[newI][newJ] == '~') {
                    computerViewTable.table[newI][newJ] = 'O';
                }
            }
        }
    }
}

// Funkcja obsługuje sytuację, gdy strzał nie trafił w statek.
void Computer::handleMiss(Table &computerViewTable, Coordinates &shotCoords) {
    cout << "Computer missed at " 
              << (char)(shotCoords.letter + 'A' - 1) 
              << shotCoords.number << "." << endl;
    
    computerViewTable.table[shotCoords.letter][shotCoords.number] = 'O';
}

// Funkcja oznacza zatopiony statek na planszy przeciwnika.
void Computer::markSunkShip(Table &firstTable, Table &computerViewTable, char targetField) {
    cout << "Computer sunk a ship!" << endl;

    for (int i = 1; i <= 10; i++) {
        for (int j = 1; j <= 10; j++) {
            if (firstTable.table[i][j] == targetField) {
                computerViewTable.table[i][j] = '~'; 
            }
        }
    }
}
