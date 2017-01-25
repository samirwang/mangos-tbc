#pragma once

#include "AntiCheat.h"

class AntiCheat_test : public AntiCheat
{
public:
    AntiCheat_test(Player* player);

    bool HandleMovement(MovementInfo& moveInfo, Opcodes opcode, bool cheat) override;
    void HandleRelocate(float x, float y, float z, float o) override;
    void HandleTeleport(uint32 map, float x, float y, float z, float o) override;
};
