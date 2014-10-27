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

#pragma once

#include "CPlayer.h"

namespace Cheat
{
    enum Time
    {
        NEW,
        OLD,
        BOTH,
    };

    enum Detector
    {
        NONE,
        SPEED,
        JUMP,
        FLY,
        CLIMB,
        TELEPORT,
    };
}

class AntiCheat
{
public:
    AntiCheat(CPlayer* pPlayer);
    virtual void DetectHack(MovementInfo& MoveInfo, Opcodes Opcode);
    virtual void SetOldValues();
    void SetMoveInfo(MovementInfo& MoveInfo);

    void ReportCheat(std::string cheat, std::string info);
    void TeleportBack();

    float GetCurSpeed();
    float GetSpeedRate();
    float GetClientDiff(bool limit = true);
    float GetServerDiff();
    float GetDistance(bool is3D);
    float GetDistance2D();
    float GetDistance3D();
    float GetDistanceZ();
    float GetMoveAngle();

    bool IsFlying(uint8 when);
    bool IsFalling(uint8 when);
    bool IsSwimming(uint8 when);
    bool IsRooted(uint8 when);

    bool CanFly();

    void SendFly(bool value);
    bool Skipping() { return m_Player->SkipAC(); }

protected:
    CPlayer* m_Player;

    Cheat::Detector m_Detector;
    MovementInfo m_MoveInfo[2];
    Opcodes m_Opcode[2];
    uint32 m_ServerTime[2];
};

class AntiCheat_speed : public AntiCheat
{
public:
    AntiCheat_speed(CPlayer* pPlayer) : AntiCheat(pPlayer)
    {
        m_Detector = Cheat::SPEED;
        m_LastSpeedCheck = 0;
    }
    void DetectHack(MovementInfo& MoveInfo, Opcodes Opcode);
private:
    uint32 m_LastSpeedCheck;
};

class AntiCheat_jump : public AntiCheat
{
public:
    AntiCheat_jump(CPlayer* pPlayer) : AntiCheat(pPlayer) { m_Detector = Cheat::JUMP; }
    void DetectHack(MovementInfo& MoveInfo, Opcodes Opcode);
};

class AntiCheat_fly : public AntiCheat
{
public:
    AntiCheat_fly(CPlayer* pPlayer) : AntiCheat(pPlayer) { m_Detector = Cheat::FLY; }
    void DetectHack(MovementInfo& MoveInfo, Opcodes Opcode);
};

class AntiCheat_climb : public AntiCheat
{
public:
    AntiCheat_climb(CPlayer* pPlayer) : AntiCheat(pPlayer) { m_Detector = Cheat::CLIMB; }
    void DetectHack(MovementInfo& MoveInfo, Opcodes Opcode);
};

class AntiCheat_teleport : public AntiCheat
{
public:
    AntiCheat_teleport(CPlayer* pPlayer) : AntiCheat(pPlayer)
    {
        m_Detector = Cheat::TELEPORT;

        for (uint8 i = 0; i < 2; ++i)
            Moving[i] = false;
    }
    void DetectHack(MovementInfo& MoveInfo, Opcodes Opcode);
    void SetOldValues();
private:

    std::map<Opcodes, bool> Moves;
    bool Moving[2];
};
