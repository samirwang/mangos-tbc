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

class AntiCheat_module // Must only be used as parent
{
public:
    explicit AntiCheat_module(Player* pPlayer);

    virtual ~AntiCheat_module() { }

    virtual void DetectHack(MovementInfo& MoveInfo, Opcodes Opcode);
    void SetOldValues();
    void ReportPlayer(std::string hack, std::string misc = "");

    bool IsFlying();
    bool IsFalling();
    bool IsSwimming();
    bool IsRooted();

    bool Skipping();

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

    uint32 m_PrevPacketTime;
    uint32 m_DetectionDelay;

    float m_MoveDist;
    float m_DeltaZ;
};

class AntiCheat_speed : public AntiCheat_module
{
public:
    explicit AntiCheat_speed(Player* pPlayer) : AntiCheat_module(pPlayer) { }
    ~AntiCheat_speed() { }

    virtual void DetectHack(MovementInfo& MoveInfo, Opcodes Opcode) override;

private:
    float m_OldMoveSpeed;
    uint32 m_DetectStreak;

#ifdef LILLECARL_DEBUG
    std::vector<std::pair<float, float>> m_HighDiffs;
#endif
};

class AntiCheat_height : public AntiCheat_module
{
public:
    explicit AntiCheat_height(Player* pPlayer) : AntiCheat_module(pPlayer) { }
    ~AntiCheat_height() { }

    virtual void DetectHack(MovementInfo& MoveInfo, Opcodes Opcode) override;
};

class AntiCheat_climb : public AntiCheat_module
{
public:
    explicit AntiCheat_climb(Player* pPlayer) : AntiCheat_module(pPlayer) { }

    ~AntiCheat_climb() { }

    virtual void DetectHack(MovementInfo& MoveInfo, Opcodes Opcode) override;
};

class AntiCheat
{
public:
    explicit AntiCheat(Player* pPlayer);
    ~AntiCheat();

    void DetectHacks(MovementInfo& MoveInfo, Opcodes Opcode);

    void SetGMFly(bool value) { m_GmFly = value; }
    void SkipAntiCheat() { ++m_SkipAntiCheat; }

    bool SkippingAntiCheat() { return m_SkipAntiCheat; }
    bool IsGMFly() { return m_GmFly; }

private:
    AntiCheat_speed* m_SpeedCheat;
    AntiCheat_height* m_HeightCheat;
    AntiCheat_climb* m_ClimbCheat;

    Player* m_player;
    uint32 m_SkipAntiCheat;
    bool m_GmFly;
};

#endif
