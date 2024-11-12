#ifndef COMPUTER_H
#define COMPUTER_H

#include <random>
#include <vector>
#include "table.h"

using namespace std; // Dodano przestrzeń nazw std

// Struktura reprezentująca statek komputerowy
struct ComputerShip {
    int number;
    int length;
    bool isVertical;
    vector<Coordinates> coordinates;
};

// Struktura reprezentująca położenie statku
struct ShipPlacement {
    int letter;
    int number;
    bool isVertical;
};

class Computer {
private:
    random_device rd; // Generator liczb losowych
    mt19937 gen; // Silnik do generowania liczb losowych
    uniform_int_distribution<> dis; // Dystrybucja do generowania liczb całkowitych
    vector<ComputerShip> computerShips; // Lista statków komputerowych
    vector<Coordinates> hitStack; // Stos trafień
    Coordinates lastHit; // Ostatnie trafienie
    bool hunting; // Flaga informująca o trybie polowania

    // Metody pomocnicze do rozmieszczania statków
    void placeShip(Table &table, int length, int shipNumber);
    ShipPlacement generateRandomPlacement(int length);
    bool canPlaceShip(Table &table, const ShipPlacement &placement, int length);
    bool checkShipArea(Table &table, const ShipPlacement &placement, int length);
    void addShipToTable(Table &table, const ShipPlacement &placement, int length, int shipNumber);

    // Metody do wykonywania ataków
    Coordinates selectTargetCoordinates(Table &computerViewTable, vector<Coordinates> &potentialTargets);
    char getTargetFieldValue(Table &firstTable, Coordinates &shotCoords);
    bool isHit(char targetField);
    void handleHit(Table &firstTable, Table &computerViewTable, Coordinates &shotCoords, 
                   char targetField, vector<Coordinates> &potentialTargets, Table &tableVidmo);
    void markHit(Table &computerViewTable, Coordinates &shotCoords);
    void addAdjacentTargets(Table &computerViewTable, Coordinates &shotCoords, 
                            vector<Coordinates> &potentialTargets);
    bool isValidCoordinate(Coordinates &coord);
    bool isShipSunk(Table &firstTable, Table &computerViewTable, char targetField);
    void handleSunkShip(Table &firstTable, Table &computerViewTable, char targetField, Table &tableVidmo);
    void markSurroundingFields(Table &computerViewTable, int i, int j);
    void handleMiss(Table &computerViewTable, Coordinates &shotCoords);

    // Metody do oznaczania zatopionych statków
    void markSunkShip(Table &firstTable, Table &computerViewTable, char targetField);
    void hitAttTableChange(Table &table, Coordinates coords);
    void missAttTableChange(Table &table, Coordinates coords);
public:
    Computer();
    ~Computer();
    void set_Ship(Table &table);
    void att_Ship(Table &firstTable, Table &computerViewTable, Table &tableVidmo);
};

#endif