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

#ifndef _NEWPLAYER_H
#define _NEWPLAYER_H

#include "Player.h"

enum NewOld
{
    NEW = 0,
    OLD,
};

enum Detector
{
    COM = 0,
    SPEED,
};

namespace Cheats
{
    enum DetectBitmasks
    {
        SPEED = 1,
        TIME = 2,
        JUMP = 4,
        FLY = 8,
    };

    const uint32 ALL_DETECTORS = SPEED | TIME | JUMP | FLY;
}

class CPlayer : public Player
{
public:
    explicit CPlayer(WorldSession* session);

    ~CPlayer() {}

    Player* ToPlayer() { return static_cast<Player*>(this); }

    /************************************************************************/
    /**********************************ANTICHEAT*****************************/
    /************************************************************************/
public:
    void DetectHacks(MovementInfo& MoveInfo, Opcodes Opcode);
    void DetectSpeed();
    void DetectTime();
    void DetectJump();
    void DetectFly();

    void ReportCheat(std::string cheat, std::string info);

    float GetCurSpeed();
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

    bool CanFly();

    void SetGMFly(bool value) { m_GmFly = value; }
    void SkipAntiCheat() { ++m_SkipAntiCheat; }

    bool Skipping() { return m_SkipAntiCheat; }
    bool IsGMFly() { return m_GmFly; }

    void SendFly(bool value);

    void IncClientBasedServerTime(uint32 diff) { if (!m_FirstMoveInfo) m_ClientBasedServerTime += diff; }

    uint32 GetLastServerTime() { return m_ServerTime[COM][NEW]; }
    uint32 GetClientBasedServerTime() { return m_ClientBasedServerTime; }
    bool HadFirstMovementSent() { return !m_FirstMoveInfo; }

private:
    MovementInfo m_MoveInfo[2][2];
    Opcodes m_Opcode[2][2];
    uint32 m_ServerTime[2][2];

    uint32 m_SkipAntiCheat;
    bool m_GmFly;
    bool m_FirstMoveInfo;

    uint32 m_LastSpeedCheck;
    uint32 m_ClientBasedServerTime;

    /************************************************************************/
    /*************************************CFBG*******************************/
    /************************************************************************/
public:
    typedef std::vector<ObjectGuid> FakedPlayers;

    bool NativeTeam() const;
    uint8 getFRace() const { return m_fRace; }
    uint8 getORace() const { return m_oRace; }
    uint32 getOFaction() const { return m_oFaction; }
    uint32 getFFaction() const { return m_fFaction; }
    uint32 getOPlayerBytes() const { return m_oPlayerBytes; }
    uint32 getFPlayerBytes() const { return m_fPlayerBytes; }
    uint32 getOPlayerBytes2() const { return m_oPlayerBytes2; }
    uint32 getFPlayerBytes2() const { return m_fPlayerBytes2; }

    void SetFakeValues();
    void RecachePlayersFromList();
    void RecachePlayersFromBG();
    WorldPacket BuildNameQuery();
    bool GetRecache() { return m_Recache; }
    void SetRecache() { m_Recache = true; }
    void SetFakedPlayers(FakedPlayers guidlist) { m_FakedPlayers = guidlist; }

    void JoinBattleGround(BattleGround* bg);
    void LeaveBattleGround(BattleGround* bg);

    void FakeDisplayID();

    void SetFakeOnNextTick(bool value = true) { m_FakeOnNextTick = value; }
    bool GetFakeOnNextTick() { return m_FakeOnNextTick; }

    bool SendBattleGroundChat(ChatMsg msgtype, std::string message);

private:
    uint8 m_fRace;
    uint8 m_oRace;
    uint32 m_fFaction;
    uint32 m_oFaction;
    uint32 m_oPlayerBytes;
    uint32 m_oPlayerBytes2;
    uint32 m_fPlayerBytes;
    uint32 m_fPlayerBytes2;

    FakedPlayers m_FakedPlayers;

    bool m_Recache;
    bool m_FakeOnNextTick;
};

#endif
