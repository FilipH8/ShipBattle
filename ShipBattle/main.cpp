#include <iostream>
#include "player.cc"
#include <string>
#include "table.cc"
#include "ship.cc"
#include <vector>
#include "computer.cc"
#include <ctime>
#include <unistd.h>

using namespace std;

void setPlayerName(Player &player)
{
    player.setPlayerName();
    cout << player.getPlayerName() << endl;
}

void setComputerShip(Table &table)
{
    srand(static_cast<unsigned int>(time(0)));
    Computer computer;
    computer.set_Ship(table);
}


bool chooseGameMode() {
    int choice;
    cout << "Choose game mode:" << endl;
    cout << "1. Play against computer" << endl;
    cout << "2. Play against another player" << endl;
    cout << "Enter 1 or 2: ";

    while (true) {
        cin >> choice;

        
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "Invalid choice. Enter 1 or 2: ";
            continue; 
        }

        
        if (choice == 1 || choice == 2) {
            break; 
        } else {
            cout << "Invalid choice. Enter 1 or 2: ";
        }
    }

    return choice == 1; 
}


void setupComputerShips(Table &table) {
    Computer computer;
    computer.set_Ship(table); 
}


void setupPlayerShips(Table &table, Player &player, Table &tableVidmo) {
    cout << "Player " << player.getPlayerName() << ", set your ships:" << endl;

    
    Ship ships[] = {
        Ship(4, 0, 0), 
        Ship(3, 0, 1), 
        Ship(3, 0, 2), 
        Ship(2, 0, 3), 
        Ship(2, 0, 4), 
        Ship(2, 0, 5), 
        Ship(1, 0, 6), 
        Ship(1, 0, 7), 
        Ship(1, 0, 8), 
        Ship(1, 0, 9)  
    };

    for (Ship &ship : ships) {
        bool placed = false;
        while (!placed) {
            
            cout << "Enter coordinates for " << ship.getSize() << "-cell ship (e.g., A1): ";
            char letter;
                    
            int number;
            cin >> letter >> number;

            
            if (letter >= 'a' && letter <= 'z') {
                letter = letter - 'a' + 'A'; 
            }

            
            if (cin.fail() || number < 1 || number > 10) {
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                cout << "Invalid coordinates. Enter a letter A-J and a number from 1 to 10." << endl;
                continue; 
            }

            
            Coordinates startCoord = { letter - 'A' + 1, number };

            
            if (ship.cannotPlaceShip(startCoord, ship.getSize(), table)) {
                cout << "Cannot place ship in this position. Try again." << endl;
            } else {
                system("cls");
                ship.setShip(table, startCoord, tableVidmo); 
                placed = true; 
            }
        }
    }
}


bool checkWinCondition(const Table &table) {
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

    cout << "Sunk ships: " << sunkShipsCount << " out of " << totalShips << endl; 

    return sunkShipsCount == totalShips; 
}


bool askToPlayAgain() {
    char choice;
    cout << "Do you want to play again? (y/n): ";
    cin >> choice;

    while (choice != 'y' && choice != 'n') {
        cout << "Invalid choice. Enter 'y' to play again or 'n' to quit: ";
        cin >> choice;
    }

    return choice == 'y'; 
}


void updateWinCount(Player &player, int &winCount) {
    winCount++;
    cout << player.getPlayerName() << " now has " << winCount << " wins!" << endl;
}

void space()
{
    cout << endl;
    cout << endl;
    cout << endl;
}

void change()
{
    system("cls");
    fflush(stdin);
    cout << "Click enter to continue: ";
    getchar();
    getchar();
    system("cls");
}

void displayTables(const Table &firstTable, const Table &secondTable) {
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

int main() {
    system("cls");

    int firstPlayerWins = 0; 
    int secondPlayerWins = 0; 

    
    Table playerTable;         
    Table playerAttackTable;
    Table playerTableVidmo;  

    Table secondPlayerTable;         
    Table secondPlayerAttackTable;
    Table secondPlayerTableVidmo;    

    
    Table computerTable;         
    Table computerAttackTable;   

    Player secondPlayer(""); 
    Player firstPlayer("");

    Ship attack(0, 0, 0);
    Ship attack2(0, 0, 0);


    
    cout << "Enter the name of the first player: ";
    firstPlayer.setPlayerName();

    sleep(1);
    system("cls");

    
    bool isPlayingWithComputer = chooseGameMode();

    if (!isPlayingWithComputer) {
        sleep(1);
        change();
        cout << "Enter the name of the second player: ";
        secondPlayer.setPlayerName();
        sleep(1);
        change();
    }

    do {

        sleep(1);
        system("cls");

        playerTable.setTable();
        playerAttackTable.setTable();
        playerTableVidmo.setTable();
        secondPlayerTable.setTable();
        secondPlayerAttackTable.setTable();
        secondPlayerTableVidmo.setTable();
        computerTable.setTable();
        computerAttackTable.setTable();

        
        setupPlayerShips(playerTable, firstPlayer, playerTableVidmo);


        if (isPlayingWithComputer) {
            sleep(1.5);
            setupComputerShips(computerTable);
        } else {
            
            sleep(1);
            change();
            setupPlayerShips(secondPlayerTable, secondPlayer, secondPlayerTableVidmo);
        }
        
        bool gameOver = false;
        while (!gameOver) {
            
            sleep(1);
            change();

            displayTables(playerTableVidmo, playerAttackTable);
            cout << endl;
            cout << firstPlayer.getPlayerName() << ", make your attack!" << endl;
            attack.attack(isPlayingWithComputer ? computerTable : secondPlayerTable, playerAttackTable, playerTable, secondPlayerTableVidmo);

            sleep(0.5);

            system("cls");

            sleep(0.5);

            
            if (checkWinCondition(playerAttackTable)) {
                sleep(1.5);
                cout << firstPlayer.getPlayerName() << " won the game!" << endl;
                updateWinCount(firstPlayer, firstPlayerWins);
                gameOver = true;
                break;
            }

            
            if (isPlayingWithComputer) {
                
                Computer computer;
                computer.att_Ship(playerTable, computerAttackTable, playerTableVidmo); 

                space();

                
                if (checkWinCondition(computerAttackTable)) {
                    sleep(1.5);
                    cout << "Computer won the game!" << endl;
                    updateWinCount(secondPlayer, secondPlayerWins);
                    gameOver = true;
                    break;
                }
            } else {
                system("cls");
                change();

                displayTables(secondPlayerTableVidmo, secondPlayerAttackTable);
                cout << endl;
                cout << secondPlayer.getPlayerName() << ", make your attack!" << endl;
                attack2.attack(playerTable, secondPlayerAttackTable, secondPlayerTable, playerTableVidmo);
                cout << endl;
                displayTables(secondPlayerTableVidmo, secondPlayerAttackTable);

                
                if (checkWinCondition(secondPlayerAttackTable)) {
                    sleep(1.5);
                    cout << secondPlayer.getPlayerName() << " won the game!" << endl;
                    updateWinCount(secondPlayer, secondPlayerWins);
                    gameOver = true;
                    break;
                }
            }
            sleep(1.5);
        }

        change();

    } while (askToPlayAgain()); 

    cout << "Thank you for playing!" << endl;
    return 0;
}