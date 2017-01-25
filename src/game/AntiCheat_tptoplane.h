#pragma once

#include "AntiCheat.h"

class AntiCheat_tptoplane : public AntiCheat
{
public:
    AntiCheat_tptoplane(Player* player);

    bool HandleMovement(MovementInfo& moveInfo, Opcodes opcode, bool cheat) override;
};
