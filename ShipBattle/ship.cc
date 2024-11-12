#include "ship.h"
#include <string>
#include <iostream>
#include "table.h"
#include <vector>
#include "player.h"
#include <unistd.h>

using namespace std;

Ship::Ship(int sizeShip, int ver_hor, int numberOfShip)
{
    this->sizeShip = sizeShip;
    this->ver_hor = ver_hor;
    this->numberOfShip = numberOfShip;
}

Ship::~Ship()
{
}

// Funkcja zwraca rozmiar statku.
int Ship::getSize() const {
    return sizeShip;
}

// Funkcja sprawdza, czy nie można umieścić statku w danej pozycji.
bool Ship::cannotPlaceShip(const Coordinates& startCoord, int sizeShip, const Table& table) const {
    return !canPlaceHorizontally(startCoord, sizeShip, table) &&
           !canPlaceVertically(startCoord, sizeShip, table);
}

// Funkcja sprawdza, czy statek można umieścić poziomo.
bool Ship::canPlaceHorizontally(const Coordinates& startCoord, int sizeShip, const Table& table) const {
    
    if (startCoord.number + sizeShip - 1 <= 10) { 
        bool canPlaceRight = true;
        for (int i = 0; i < sizeShip; i++) {
            Coordinates checkCoord = {startCoord.letter, startCoord.number + i};
            if (!canPlaceShipPart(checkCoord, table)) {
                canPlaceRight = false; 
                break;
            }
        }
        if (canPlaceRight) {
            return true; 
        }
    }

    
    if (startCoord.number - sizeShip + 1 >= 1) { 
        bool canPlaceLeft = true;
        for (int i = 0; i < sizeShip; i++) {
            Coordinates checkCoord = {startCoord.letter, startCoord.number - i};
            if (!canPlaceShipPart(checkCoord, table)) {
                canPlaceLeft = false; 
                break;
            }
        }
        if (canPlaceLeft) {
            return true; 
        }
    }

    return false; 
}

// Funkcja sprawdza, czy statek można umieścić pionowo.
bool Ship::canPlaceVertically(const Coordinates& startCoord, int sizeShip, const Table& table) const {
    
    if (startCoord.letter + sizeShip - 1 <= 10) { 
        bool canPlaceDown = true;
        for (int i = 0; i < sizeShip; i++) {
            Coordinates checkCoord = {startCoord.letter + i, startCoord.number};
            if (!canPlaceShipPart(checkCoord, table)) {
                canPlaceDown = false; 
                break;
            }
        }
        if (canPlaceDown) {
            return true; 
        }
    }

    
    if (startCoord.letter - sizeShip + 1 >= 1) { 
        bool canPlaceUp = true;
        for (int i = 0; i < sizeShip; i++) {
            Coordinates checkCoord = {startCoord.letter - i, startCoord.number};
            if (!canPlaceShipPart(checkCoord, table)) {
                canPlaceUp = false; 
                break;
            }
        }
        if (canPlaceUp) {
            return true; 
        }
    }

    return false; 
}

// Funkcja sprawdza, czy współrzędne są w dozwolonych granicach.
bool Ship::isWithinBounds(int letter, int number) const {
    return letter >= 1 && letter <= 10 && number >= 1 && number <= 10;
}

// Funkcja sprawdza, czy pola otaczające dany współrzędne są wolne.
bool Ship::isSurroundingClear(const Coordinates& coord, const Table& table) const {
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            int checkLetter = coord.letter + dy;
            int checkNumber = coord.number + dx;
            if (isWithinBounds(checkLetter, checkNumber)) {
                if (table.table[checkLetter][checkNumber] != '~' &&
                    table.table[checkLetter][checkNumber] != ('0' + numberOfShip + 1)) {
                    return false;
                }
            }
        }
    }
    return true;
}

// Funkcja sprawdza, czy można umieścić część statku w danej pozycji.
bool Ship::canPlaceShipPart(const Coordinates& coord, const Table& table) const {
    return isWithinBounds(coord.letter, coord.number) &&
           table.table[coord.letter][coord.number] == '~' &&
           isSurroundingClear(coord, table);
}

// Funkcja sprawdza, czy można zakończyć umieszczanie statku w danej pozycji.
bool Ship::canCompletePlacement(const Coordinates& newCoord, 
                              const Coordinates& firstCoord, 
                              int remainingLength,
                              const Table& table) const {
    bool isHorizontal = (newCoord.letter == firstCoord.letter);
    int direction = isHorizontal ? 
        (newCoord.number > firstCoord.number ? 1 : -1) : 
        (newCoord.letter > firstCoord.letter ? 1 : -1);

    Coordinates checkCoord = newCoord;
    for (int i = 0; i < remainingLength - 1; i++) {
        if (isHorizontal) {
            checkCoord.number += direction;
        } else {
            checkCoord.letter += direction;
        }

        if (!canPlaceShipPart(checkCoord, table)) {
            return false;
        }
    }
    return true;
}

// Funkcja waliduje, czy nowa część statku może być umieszczona w danej pozycji.
bool Ship::validateNextPart(const Coordinates& newCoord, 
                          const vector<Coordinates>& shipCoords,
                          const Table& table,
                          int remainingLength) const {
    const auto& firstCoord = shipCoords.front();
    const auto& lastCoord = shipCoords.back();

    
    if (shipCoords.size() == 1) {
        bool isHorizontal = (newCoord.letter == firstCoord.letter);
        bool isVertical = (newCoord.number == firstCoord.number);
        
        
        bool isAdjacent = (isHorizontal && abs(newCoord.number - firstCoord.number) == 1) ||
                         (isVertical && abs(newCoord.letter - firstCoord.letter) == 1);

        if (!isAdjacent) {
            cout << "Parts must be adjacent!" << endl;
            return false;
        }

        
        return canCompletePlacement(newCoord, firstCoord, remainingLength, table);
    }

    
    bool isHorizontal = (shipCoords[0].letter == shipCoords[1].letter);
    bool isValidPlacement = isHorizontal ? 
        (newCoord.letter == lastCoord.letter && abs(newCoord.number - lastCoord.number) == 1) :
        (newCoord.number == lastCoord.number && abs(newCoord.letter - lastCoord.letter) == 1);

    if (!isValidPlacement) {
        cout << "Parts must be in a straight line and adjacent!" << endl;
        return false;
    }

    return true;
}

// Funkcja umieszcza część statku w danej pozycji na planszy.
void Ship::placeShipPart(const Coordinates& coord, Table& table, Table &tableVidmo) {
    table.table[coord.letter][coord.number] = '0' + numberOfShip + 1;
    tableVidmo.table[coord.letter][coord.number] = '0' + numberOfShip + 1;
    displayPlacementInfo(coord);
}

// Funkcja aktualizuje informacje o statku na podstawie współrzędnych.
void Ship::updateShipInfo(const vector<Coordinates>& coords) {
    stern = coords.front();
    bow = coords.back();
    shipCoordinates = coords;
}

// Funkcja wyświetla informacje o umiejscowieniu statku.
void Ship::displayPlacementInfo(const Coordinates& coord) const {
    cout << "Successfully placed ship part at " 
         << (char)(coord.letter + 'A' - 1) << coord.number << endl;
}

// Funkcja umieszcza statek w danej pozycji na planszy.
void Ship::placeShipAt(const Coordinates& startCoord, Table& table, Table &tableVidmo)
{
    placeShipPart(startCoord, table, tableVidmo);
}

// Funkcja ustawia statek na planszy, prosząc gracza o podanie pozycji.
void Ship::setShip(Table& table, const Coordinates& startCoord, Table &tableVidmo) {
    cout << "\nPlacing ship number " << numberOfShip + 1 << " (size: " << sizeShip << ")" << endl;
    vector<Coordinates> coords;

    
    auto firstCoord = startCoord;
    placeShipAt(startCoord, table, tableVidmo);
    coords.push_back(firstCoord);
    cout << endl;
    table.showTable();

    
    for (int i = 1; i < sizeShip; i++) {
        cout << "Remaining fields to place: " << sizeShip - i << endl;
        while (true) {
            auto nextCoord = table.setPlace();
            if (!canPlaceShipPart(nextCoord, table)) {
                cout << "Invalid position. Try again." << endl;
                continue;
            }

            if (!validateNextPart(nextCoord, coords, table, sizeShip - i)) {
                cout << "Invalid placement. Try again." << endl;
                continue;
            }

            placeShipPart(nextCoord, table, tableVidmo);
            coords.push_back(nextCoord);
            cout << endl;
            system("cls");
            table.showTable();
            break;
        }
    }

    updateShipInfo(coords);
    cout << "Ship placement completed!" << endl;
}

// Funkcja wykonuje atak na przeciwnika.
void Ship::attack(Table &firstTable, Table &secondTable, Table &thrTable, Table &tableVidmo) {
    cout << "Attacking..." << endl;
    sleep(0.2);
    Coordinates shotCoords = getValidShotCoordinates(secondTable);
    
    char targetField = getTargetFieldValue(firstTable, shotCoords);
    
    if (isHit(targetField)) {
        handleHit(firstTable, secondTable, shotCoords, targetField, thrTable, tableVidmo);
        hitAttTableChange(tableVidmo, shotCoords);
    } else {
        handleMiss(secondTable, shotCoords);
        missAttTableChange(tableVidmo, shotCoords);
    }
    system("cls");
}

// Funkcja zmienia pole na planszy po trafieniu.
void Ship::hitAttTableChange(Table &table, Coordinates coords)
{
    table.table[coords.letter][coords.number] = 'H';
}

// Funkcja zmienia pole na planszy po nietrafieniu.
void Ship::missAttTableChange(Table &table, Coordinates coords)
{
    table.table[coords.letter][coords.number] = 'O';
}

// Funkcja pobiera współrzędne strzału od gracza.
Coordinates Ship::getValidShotCoordinates(Table &table) {
    Coordinates shotCoords;
    bool validShot = false;

    while (!validShot) {
        shotCoords = table.setPlace();
        if (table.getPlace(shotCoords.letter + 'A' - 1, shotCoords.number) == '~') {
            validShot = true;
        } else {
            cout << "You've already shot at this location. Choose another." << endl;
        }
    }

    return shotCoords;
}

// Funkcja zwraca wartość pola docelowego na planszy.
char Ship::getTargetFieldValue(Table &table, Coordinates coords) {
    return table.getPlace(coords.letter + 'A' - 1, coords.number);
}

// Funkcja sprawdza, czy strzał trafił w statek.
bool Ship::isHit(char targetField) {
    return targetField != '~' && targetField != 'O' && targetField != 'H';
}

// Funkcja obsługuje sytuację trafienia w statek.
void Ship::handleHit(Table &firstTable, Table &secondTable, Coordinates shotCoords, char targetField, Table &thrTable, Table &tableVidmo) {
    cout << "Hit!" << endl;
    markHit(secondTable, shotCoords);
    
    if (isShipSunk(firstTable, secondTable, targetField)) {
        handleSunkShip(firstTable, secondTable, targetField, tableVidmo);
    }

    cout << "You get an extra shot!" << endl;
    sleep(0.5);
    system("cls");
    displayTables(thrTable, secondTable);

    if(!checkWinConditionInShip(secondTable))
    {
    attack(firstTable, secondTable, thrTable, tableVidmo);
    }
}

// Funkcja oznacza pole jako trafione.
void Ship::markHit(Table &table, Coordinates coords) {
    table.table[coords.letter][coords.number] = 'H'; 
}

// Funkcja sprawdza, czy statek został zatopiony.
bool Ship::isShipSunk(Table &firstTable, Table &secondTable, char shipSymbol) {
    for (int i = 1; i <= 10; i++) {
        for (int j = 1; j <= 10; j++) {
            if (firstTable.getPlace(i + 'A' - 1, j) == shipSymbol) {
                if (secondTable.getPlace(i + 'A' - 1, j) != 'H') {
                    return false;
                }
            }
        }
    }
    return true;
}

// Funkcja obsługuje sytuację, gdy statek został zatopiony.
void Ship::handleSunkShip(Table &firstTable, Table &secondTable, char shipSymbol, Table &tableVidmo) {
    cout << "Ship sunk!" << endl;
    
    
    for (int i = 1; i <= 10; i++) {
        for (int j = 1; j <= 10; j++) {
            if (firstTable.getPlace(i + 'A' - 1, j) == shipSymbol) {
                secondTable.table[i][j] = 'S'; 
                markSurroundingFields(secondTable, i, j);
                markSurroundingFields(tableVidmo, i, j);
            }
        }
    }

    
}

// Funkcja obsługuje sytuację nietrafienia.
void Ship::handleMiss(Table &table, Coordinates coords) {
    cout << "Miss!" << endl;
    table.table[coords.letter][coords.number] = 'O'; 
}

// Funkcja oznacza pola wokół zatopionego statku.
void Ship::markSurroundingFields(Table &table, int i, int j) {
    for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
            markFieldIfValid(table, i + di, j + dj);
        }
    }
}

// Funkcja sprawdza, czy współrzędne są ważne.
void Ship::markFieldIfValid(Table &table, int i, int j) {
    if (isValidCoordinate(i, j)) {
        markFieldAsMiss(table, i, j);
    }
}

// Funkcja sprawdza, czy współrzędne są w dozwolonych granicach.
bool Ship::isValidCoordinate(int i, int j) {
    return i >= 1 && i <= 10 && j >= 1 && j <= 10;
}

// Funkcja oznacza pole jako nietrafione.
void Ship::markFieldAsMiss(Table &table, int i, int j) {
    if (table.table[i][j] == '~') {
        table.table[i][j] = 'O';
    }
}

// Funkcja oznacza statek jako zatopiony.
void Ship::markShipAsSunk(Table &table) {
    for (const auto& coord : shipCoordinates) {
        table.table[coord.letter][coord.number] = '~'; 
    }
}

// Funkcja sprawdza warunek wygranej.
bool Ship::checkWinConditionInShip(Table &table) {
    int sunkShipsCount = 0; 
    const int totalShips = 20; 

    for (int i = 1; i <= 10; i++) {
        for (int j = 1; j <= 10; j++) {
            char cell = table.getPlace(i + 'A' - 1, j);
            if (cell == 'S') {
                sunkShipsCount++;
            }
        }
    }

    cout << "Sink Ship: " << sunkShipsCount << " From " << totalShips << endl; 

    if(sunkShipsCount == totalShips)
    {
        return true;
    }
    else{
        return false;
    }

}

void Ship::displayTables(const Table &firstTable, const Table &secondTable) {
    for (int i = 0; i < 11; ++i) {
        
        for (int j = 0; j < 11; ++j) {
            cout << firstTable.table[i][j] << " "; 
        }
        cout << "\t"; 

        
        for (int j = 0; j < 11; ++j) {
            cout << secondTable.table[i][j] << " "; 
        }
        cout << endl; 
    }
}