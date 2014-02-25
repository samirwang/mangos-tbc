/*
* See AUTHORS file for Copyright information
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

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
    uint32 m_ServerTime;
    uint32 m_HeightDelay;
    uint32 m_SpeedDelay;
};
#endif
