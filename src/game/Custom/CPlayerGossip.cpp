#include "Player.h"

void Player::PlayerGossip(uint32 sender, uint32 action, std::string code)
{
    PlayerTalkClass->CloseGossip();
    if (sender == 1)
    {
        if (action == 1)
            BoxChat << "You're a fag now\n";
        else if (action == 2)
            BoxChat << "You're a Malcrom now\n";
    }
}