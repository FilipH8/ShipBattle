#include "player.h"
#include <string>
#include <iostream>

using namespace std;

Player::Player(string playerName)
{
    this->playerName = playerName;
}

Player::~Player()
{
    cout << "Player " << playerName << " leave the game" << endl;
}

void Player::setPlayerName()
{
    cin >> playerName;
    cout << "Player " << playerName << " is ready to play" << endl;
}

string Player::getPlayerName()
{
    return playerName;
}
