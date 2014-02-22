#ifndef _PLAYERGOSSIP_H
#define _PLAYERGOSSIP_H

#include "Player.h"

class PlayerGossip
{
public:
    PlayerGossip(Player* pPlayer);
    ~PlayerGossip() {};

    void PlayerGossipHello(uint32 sender);
    void PlayerGossipSelect(uint32 sender, uint32 action, std::string code);

private:
    Player* m_player;
};

#endif