#ifndef PLAYER_H
#define PLAYER_H
#include <string>

using namespace std;

class Player
{
    private:

    public:
    Player(string playerName);
    ~Player();
    string playerName;
    void setPlayerName();
    string getPlayerName();
};

#endif