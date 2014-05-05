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

class Player;

enum NewOld
{
    NEW = 0,
    OLD = 1,
};

class AntiCheat
{
public:
    explicit AntiCheat(Player* pPlayer);
    ~AntiCheat() { };

    void DetectHacks(MovementInfo& MoveInfo, Opcodes Opcode);
    void DetectSpeed();

    float GetSpeed();
    float GetSpeedRate();
    float GetClientDiff();
    float GetServerDiff();
    float GetDistance(bool threeD);
    float GetDistance2D();
    float GetDistance3D();
    float GetDistanceZ();
    float GetMoveAngle();

    bool IsFlying();
    bool IsFalling();
    bool IsSwimming();
    bool IsRooted();

    void SetGMFly(bool value) { m_GmFly = value; }
    void SkipAntiCheat() { ++m_SkipAntiCheat; }

    bool Skipping() { return m_SkipAntiCheat; }
    bool IsGMFly() { return m_GmFly; }

    uint32 GetMoveDeltaT() { return m_MoveDeltaT; }

private:
    Player* m_player;

    MovementInfo m_MoveInfo[2];
    Opcodes m_Opcode[2];
    uint32 m_ServerTime[2];

    uint32 m_SkipAntiCheat;
    bool m_GmFly;
    bool m_FirstMoveInfo;
    int64 m_MoveDeltaT;
};

#endif
