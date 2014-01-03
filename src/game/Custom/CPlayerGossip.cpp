#include "Player.h"
#include "Custom.h"

void Player::PlayerGossip(uint32 sender, uint32 action, std::string code)
{
    PlayerTalkClass->CloseGossip();

    if (sender == GOSSIP_SENDER_FIRSTLOGIN)
    {
        if (action == 1)
        { // Warrior - Arms
            AddItemSet(750);
        }
        else if (action == 2)
        { // Warrior - Fury
            AddItemSet(750);
        }
        else if (action == 3)
        { // Warrior - Protection
            AddItemSet(750);
        }
        else if (action == 4)
        { // Paladin - Holy
            AddItemSet(751);
        }
        else if (action == 5)
        { // Paladin - Protection
            AddItemSet(752);
        }
        else if (action == 6)
        { // Paladin - Retribution
            AddItemSet(752);
        }
    }
}