#ifndef _ANTICHEAT_H
#define _ANTICHEAT_H

#include "Player.h"

class AntiCheat
{
public:
    AntiCheat(Player* pPlayer);
    ~AntiCheat() {}

    void HandleMovementCheat(MovementInfo& MoveInfo, Opcodes opcode);
    void SetGMFly(bool value) { m_GmFly = value; }
    void SkipAntiCheat() { m_SkipAntiCheat = true; }
    int32 GetCheatReportTimer(uint8 index) { return m_CheatReportTimer[index]; }
    void AlterCheatReportTimer(uint8 index, int32 change) { m_CheatReportTimer[index] += change; }

private:
    void HandleSpeedCheat();
    void HandleHeightCheat();
    void HandleClimbCheat();
    uint32 GetOldMoveTime() { return m_OldMoveTime; }

    void HandleCheatReport(const char* hack);

    bool IsFlying();
    bool IsFalling();
    bool IsSwimming();
    bool IsRooted();


    Player* m_player;

    MovementInfo m_MoveInfo[2];
    Opcodes m_Opcode[2];

    uint32 m_OldMoveTime;
    float m_OldMoveSpeed;
    bool m_SkipAntiCheat;
    bool m_GmFly;
    int32 m_CheatReportTimer[2];
    const char* m_LastHack;
};
#endif
