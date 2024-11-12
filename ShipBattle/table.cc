#include "table.h"
#include <iostream>
#include <string>

using namespace std;

Table::Table()
{
    table = new char*[11];
    for (int i = 0; i < 11; ++i) {
        table[i] = new char[11];
    }

    
    for (int i = 0; i < 11; ++i) {
        for (int j = 0; j < 11; ++j) {
            table[i][j] = '0'; 
        }
    }
}

Table::~Table()
{
    cout << "Table is being destroyed." << endl;
    
    for (int i = 0; i < 11; ++i) {
        delete[] table[i];
    }
    delete[] table;
}

void Table::setTable()
{
    
    for (int j = 0; j < 10; ++j) {
        table[0][j] = '0' + j;
    }
    table[0][10] = '0';  

    
    table[0][0] = '#';
    for (int i = 1; i < 11; ++i) {
        table[i][0] = 'A' + (i - 1);
    }

    
    for (int i = 1; i < 11; ++i) {
        for (int j = 1; j < 11; ++j) {  
            table[i][j] = '~';
        }
    }
}

int Table::getTable()
{
    return 1;
}

void Table::showTable()
{
    for (int i = 0; i < 11; ++i) {
        for (int j = 0; j < 11; ++j) {
            cout << table[i][j] << " "; 
        }
        cout << endl; 
    }
}

Coordinates Table::setPlace()
{
    Coordinates coords;
    char A;
    string numberInput;  
    int t = 0;

    while (t < 1) {
        cout << "Write letters a to j: ";
        cin >> A;

        if (!isalpha(A)) {
            cout << "Invalid character, please enter a letter." << endl;
            continue;
        }

        cout << "Write number 1 to 10: ";
        cin >> numberInput;

        
        int numb;
        try {
            numb = stoi(numberInput);  
            if (numb < 1 || numb > 10) {
                cout << "Invalid number. Please enter a number between 1 and 10." << endl;
                continue;
            }
        } catch (...) {
            cout << "Invalid input. Please enter a number between 1 and 10." << endl;
            continue;
        }

        if (A >= 'a' && A <= 'j') {
            coords.letter = A - 'a' + 1;
        } else if (A >= 'A' && A <= 'J') {
            coords.letter = A - 'A' + 1;
        } else {
            cout << "Wrong letter" << endl;
            continue;
        }

        coords.number = numb;
        t++;
    }

    return coords;
}

char Table::getPlace(char letter, int number) const {
    int letterIndex = letter - 'A' + 1;
    return table[letterIndex][number];  
}
