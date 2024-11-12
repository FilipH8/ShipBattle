#ifndef TABLE_H
#define TABLE_H
#include <string>

using namespace std;

struct Coordinates {
    int letter;
    int number;
};

class Ship;
class Computer;

class Table
{
    private:
    
    public:
    friend class Ship;
    friend class Computer;
    
    char** table;

    Table();
    ~Table();
    void setTable();
    int getTable();
    void showTable();
    Coordinates setPlace();
    char getPlace(char letter, int number) const;
};

#endif
