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

#include "NewPlayer.h"
#include "Custom.h"
#include "World.h"
#include "Mail.h"
#include "MapManager.h"
#include "Player.h"

void CPlayer::DetectHacks(MovementInfo& MoveInfo, Opcodes Opcode)
{
    if (!sWorld.getConfig(CONFIG_UINT32_ANTICHEAT_ENABLED))
        return;

    for (auto i = 0; i < 2; ++i)
    {
        m_MoveInfo[i][Cheats::NEW] = MoveInfo;
        m_Opcode[i][Cheats::NEW] = Opcode;
        m_ServerTime[i][Cheats::NEW] = WorldTimer::getMSTime();
    }

    if (m_FirstMoveInfo)
    {
        m_FirstMoveInfo = false;
        m_ClientBasedServerTime = MoveInfo.GetTime();
    }

    if (!m_SkipAntiCheat)
    {
        if (sCustom.HasFlag(sWorld.getConfig(CONFIG_UINT32_ANTICHEAT_ENABLED), Cheats::MSPEED))
            DetectSpeed();
        if (sCustom.HasFlag(sWorld.getConfig(CONFIG_UINT32_ANTICHEAT_ENABLED), Cheats::MTIME))
            DetectTime();
        if (sCustom.HasFlag(sWorld.getConfig(CONFIG_UINT32_ANTICHEAT_ENABLED), Cheats::MJUMP))
            DetectJump();
        if (sCustom.HasFlag(sWorld.getConfig(CONFIG_UINT32_ANTICHEAT_ENABLED), Cheats::MFLY))
            DetectFly();
    }
    else
        --m_SkipAntiCheat;

    m_MoveInfo[Cheats::COM][Cheats::OLD] = m_MoveInfo[Cheats::COM][Cheats::NEW];
    m_Opcode[Cheats::COM][Cheats::OLD] = m_Opcode[Cheats::COM][Cheats::NEW];
    m_ServerTime[Cheats::COM][Cheats::OLD] = m_ServerTime[Cheats::COM][Cheats::NEW];
}

void CPlayer::DetectSpeed()
{
    if (WorldTimer::getMSTimeDiff(m_LastSpeedCheck, WorldTimer::getMSTime()) < sWorld.getConfig(CONFIG_UINT32_SPEEDCHEAT_INTERVAL))
        return;

    std::ostringstream ss;
    float Traveled = GetDistance(IsFlying());
    float Allowed = GetSpeedRate() * 1.f + (float(sWorld.getConfig(CONFIG_UINT32_SPEEDCHEAT_TOLERANCE)) / 100.f);

    bool TooFast = Traveled > Allowed;

    ss << "Diff: " << Traveled - Allowed << " Traveled: " << Traveled << " Allowed: " << Allowed;

    bool Sliding = false;

    if (Map* pMap = GetMap())
    {
        const Position* pos = m_MoveInfo[Cheats::SPEED][Cheats::NEW].GetPos();

        float groundZ = pMap->GetHeight(pos->x, pos->y, pos->z);

        if (abs(groundZ - pos->z) < 1 && IsFalling())
            Sliding = true;
    }

    if (TooFast && !Sliding)
    {
        ReportCheat("Speed", ss.str());

        m_MoveInfo[Cheats::SPEED][Cheats::NEW] = m_MoveInfo[Cheats::SPEED][Cheats::OLD];
        const Position* pos = m_MoveInfo[Cheats::SPEED][Cheats::OLD].GetPos();

        TeleportTo(GetMapId(), pos->x, pos->y, pos->z, pos->o, 0, 0, true);
    }

    m_MoveInfo[Cheats::SPEED][Cheats::OLD] = m_MoveInfo[Cheats::SPEED][Cheats::NEW];
    m_Opcode[Cheats::SPEED][Cheats::OLD] = m_Opcode[Cheats::SPEED][Cheats::NEW];
    m_ServerTime[Cheats::SPEED][Cheats::OLD] = WorldTimer::getMSTime();

    m_LastSpeedCheck = WorldTimer::getMSTime();
}

void CPlayer::DetectTime()
{
    std::ostringstream ss;

    ss << "Client time: " << m_MoveInfo[Cheats::COM][Cheats::NEW].GetTime() << " Client based server time: " << m_ClientBasedServerTime;

    if (abs(int32(m_MoveInfo[Cheats::COM][Cheats::NEW].GetTime() - m_ClientBasedServerTime)) > 5000)
        ReportCheat("Time", ss.str());
}

void CPlayer::DetectJump()
{
    if (m_Opcode[Cheats::COM][Cheats::NEW] != MSG_MOVE_JUMP)
        return;

    std::ostringstream ss;

    bool Detected = false;

    if (m_Opcode[Cheats::COM][Cheats::OLD] == m_Opcode[Cheats::COM][Cheats::NEW])
    {
        Detected = true;
        ss << "Opcode";
    }
    else
    {
        const Position* pos = m_MoveInfo[Cheats::COM][Cheats::OLD].GetPos();
        float groundZ = pos->z;
        if (Map* pMap = GetMap())
            groundZ = pMap->GetHeight(pos->x, pos->y, pos->z);

        if (pos->z - groundZ > 1)
        {
            Detected = true;
            ss << "Height";
        }
    }

    if (Detected)
    {
        ReportCheat("Jump", ss.str());

        const Position* pos = m_MoveInfo[Cheats::COM][Cheats::OLD].GetPos();
        float newz = pos->z;

        if (Map* pMap = GetMap())
            newz = pMap->GetHeight(pos->x, pos->y, pos->z);

        TeleportTo(GetMapId(), pos->x, pos->y, newz, pos->o, 0, 0, true);
        m_Opcode[Cheats::COM][Cheats::NEW] = MSG_NULL_ACTION;
    }
}

void CPlayer::DetectFly()
{
    if (IsFlying() && !CanFly())
    {
        ReportCheat("Fly", "");
        SendFly(false);
    }
}

void CPlayer::ReportCheat(std::string cheat, std::string info)
{
    std::ostringstream ss;
    ss << "Name: " << GetName() << " Cheat: " << cheat << " Info: " << (info.empty() ? "No info available" : info) << std::endl;

    sCustom.SendGMMessage(ss.str());
}

float CPlayer::GetCurSpeed()
{
    float speed = 0;

    for (uint8 i = 0; i < 2; ++i)
    {
        float thisspeed = 0;

        bool back = m_MoveInfo[Cheats::COM][i].HasMovementFlag(MOVEFLAG_BACKWARD);

        if (m_MoveInfo[Cheats::COM][i].HasMovementFlag(MOVEFLAG_WALK_MODE))
            thisspeed = GetSpeed(MOVE_WALK);
        else if (m_MoveInfo[Cheats::COM][i].HasMovementFlag(MOVEFLAG_SWIMMING))
            thisspeed = GetSpeed(back ? MOVE_SWIM_BACK : MOVE_SWIM);
        else if (m_MoveInfo[Cheats::COM][i].HasMovementFlag(MOVEFLAG_FLYING))
            thisspeed = GetSpeed(back ? MOVE_FLIGHT_BACK : MOVE_FLIGHT);
        else
            thisspeed = GetSpeed(back ? MOVE_RUN_BACK : MOVE_RUN);

        if (thisspeed > speed)
            speed = thisspeed;
    }

    return speed;
}

float CPlayer::GetSpeedRate()
{
    return GetCurSpeed() * (GetClientDiff() / 1000.f);
}

float CPlayer::GetClientDiff()
{
    return m_MoveInfo[Cheats::COM][Cheats::NEW].GetTime() - m_MoveInfo[Cheats::COM][Cheats::OLD].GetTime();
}

float CPlayer::GetServerDiff()
{
    return m_ServerTime[Cheats::COM][Cheats::NEW] - m_ServerTime[Cheats::COM][Cheats::OLD];
}

float CPlayer::GetDistance(bool threeD)
{
    return (threeD ? GetDistance3D() : GetDistance2D());
}

float CPlayer::GetDistance2D()
{
    return sqrt(
        pow(m_MoveInfo[Cheats::COM][Cheats::OLD].GetPos()->x - m_MoveInfo[Cheats::COM][Cheats::NEW].GetPos()->x, 2) +
        pow(m_MoveInfo[Cheats::COM][Cheats::OLD].GetPos()->y - m_MoveInfo[Cheats::COM][Cheats::NEW].GetPos()->y, 2)
        );
}

float CPlayer::GetDistance3D()
{
    return sqrt(
        pow(m_MoveInfo[Cheats::COM][Cheats::OLD].GetPos()->x - m_MoveInfo[Cheats::COM][Cheats::NEW].GetPos()->x, 2) +
        pow(m_MoveInfo[Cheats::COM][Cheats::OLD].GetPos()->y - m_MoveInfo[Cheats::COM][Cheats::NEW].GetPos()->y, 2) +
        pow(m_MoveInfo[Cheats::COM][Cheats::OLD].GetPos()->z - m_MoveInfo[Cheats::COM][Cheats::NEW].GetPos()->z, 2)
        );
}

float CPlayer::GetDistanceZ()
{
    return m_MoveInfo[Cheats::COM][Cheats::NEW].GetPos()->z - m_MoveInfo[Cheats::COM][Cheats::OLD].GetPos()->z;
}

float CPlayer::GetMoveAngle()
{
    return MapManager::NormalizeOrientation(tan(GetDistanceZ() / GetDistance2D()));
}

bool CPlayer::IsFlying()
{
    return m_MoveInfo[Cheats::COM][Cheats::NEW].HasMovementFlag(MovementFlags(MOVEFLAG_FLYING | MOVEFLAG_FLYING2));
}

bool CPlayer::IsFalling()
{
    return m_MoveInfo[Cheats::COM][Cheats::NEW].HasMovementFlag(MovementFlags(MOVEFLAG_FALLING | MOVEFLAG_FALLINGFAR | MOVEFLAG_SAFE_FALL));
}

bool CPlayer::IsSwimming()
{
    return m_MoveInfo[Cheats::COM][Cheats::NEW].HasMovementFlag(MOVEFLAG_SWIMMING);
}

bool CPlayer::IsRooted()
{
    return m_MoveInfo[Cheats::COM][Cheats::NEW].HasMovementFlag(MOVEFLAG_ROOT);
}

bool CPlayer::CanFly()
{
    return HasAuraType(SPELL_AURA_FLY) || IsGMFly();
}

void CPlayer::SendFly(bool value)
{
    WorldPacket data;
    data.SetOpcode(value ? SMSG_MOVE_SET_CAN_FLY : SMSG_MOVE_UNSET_CAN_FLY);
    data << GetPackGUID();
    data << uint32(0);                                      // unknown
    SendMessageToSet(&data, true);
}