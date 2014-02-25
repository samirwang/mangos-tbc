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

class AntiCheat_speed;
class AntiCheat_height;
class AntiCheat_climb;

class AntiCheat
{
public:
    AntiCheat(Player* pPlayer);
    ~AntiCheat() {}

    void DetectHacks(MovementInfo& MoveInfo, Opcodes Opcode);

    void SetGMFly(bool value) { m_GmFly = value; }
    void SkipAntiCheat(bool value = true) { m_SkipAntiCheat = value; }

    bool IsGMFly() { return m_GmFly; }

private:
    AntiCheat_speed* m_SpeedCheat;
    AntiCheat_height* m_HeightCheat;
    AntiCheat_climb* m_ClimbCheat;

    Player* m_player;
    bool m_SkipAntiCheat;
    bool m_GmFly;
};

class AntiCheat_module // Must only be used as parent
{
public:
    AntiCheat_module()
    {
        m_CurOpcode = MSG_NULL_ACTION;
        m_OldOpcode = MSG_NULL_ACTION;

        m_CurServerTime = WorldTimer::getMSTime();
        m_OldServerTime = WorldTimer::getMSTime();

        m_CTimeDiff = 0;
        m_STimeDiff = 0;
        m_CSTimeDiff = 0;

        m_PrevPacketTime = 0;
        m_DetectionDelay = 0;

        m_MoveDist = 0;
        m_DeltaZ = 0;
        m_LastCheat = false;
    }

    ~AntiCheat_module() { }

    virtual void DetectHack(MovementInfo& MoveInfo, Opcodes Opcode);
    void SetOldValues();

    bool IsFlying();
    bool IsFalling();
    bool IsSwimming();
    bool IsRooted();

protected:
    Player* m_player;
    
    MovementInfo m_CurMoveInfo;
    MovementInfo m_OldMoveInfo;

    Opcodes m_CurOpcode;
    Opcodes m_OldOpcode;

    uint32 m_CurServerTime;
    uint32 m_OldServerTime;

    uint32 m_CTimeDiff;
    uint32 m_STimeDiff;
    int32 m_CSTimeDiff;

    uint32 m_PrevPacketTime;
    uint32 m_DetectionDelay;

    float m_MoveDist;
    float m_DeltaZ;

    bool m_LastCheat;
};

class AntiCheat_speed : public AntiCheat_module
{
public:
    AntiCheat_speed(Player* pPlayer)
    {
        m_player = pPlayer;
        m_OldMoveSpeed = 0;
    }

    virtual void DetectHack(MovementInfo& MoveInfo, Opcodes Opcode) override;

private:
    float m_OldMoveSpeed;
};

class AntiCheat_height : public AntiCheat_module
{
public:
    AntiCheat_height(Player* pPlayer) { m_player = pPlayer; }

    virtual void DetectHack(MovementInfo& MoveInfo, Opcodes Opcode) override;
};

class AntiCheat_climb : public AntiCheat_module
{
public:
    AntiCheat_climb(Player* pPlayer) { m_player = pPlayer; }

    virtual void DetectHack(MovementInfo& MoveInfo, Opcodes Opcode) override;
};

#endif
