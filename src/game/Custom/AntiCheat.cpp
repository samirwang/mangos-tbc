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

#include "AntiCheat.h"
#include "Custom.h"
#include "World.h"
#include "Mail.h"
#include "MapManager.h"
#include "Player.h"
#include "CPlayer.h"

AntiCheat::AntiCheat(Player* pPlayer)
{
    m_player = pPlayer;

    m_SkipAntiCheat = 5;
    m_GmFly = false;

    m_FirstMoveInfo = true;
    m_ClientBasedServerTime = 0;
    m_LastSpeedCheck = 0;
}

void AntiCheat::DetectHacks(MovementInfo& MoveInfo, Opcodes Opcode)
{
    if (!sWorld.getConfig(CONFIG_UINT32_ANTICHEAT_ENABLED))
        return;

    for (auto i = 0; i < 2; ++i)
    {
        m_MoveInfo[i][NEW] = MoveInfo;
        m_Opcode[i][NEW] = Opcode;
        m_ServerTime[i][NEW] = WorldTimer::getMSTime();
    }

    if (m_FirstMoveInfo)
    {
        m_FirstMoveInfo = false;
        m_ClientBasedServerTime = MoveInfo.GetTime();
    }

    if (!m_SkipAntiCheat)
    {
        if (sCustom.HasFlag(sWorld.getConfig(CONFIG_UINT32_ANTICHEAT_ENABLED), Cheats::SPEED))
            DetectSpeed();
        if (sCustom.HasFlag(sWorld.getConfig(CONFIG_UINT32_ANTICHEAT_ENABLED), Cheats::TIME))
            DetectTime();
        if (sCustom.HasFlag(sWorld.getConfig(CONFIG_UINT32_ANTICHEAT_ENABLED), Cheats::JUMP))
            DetectJump();
        if (sCustom.HasFlag(sWorld.getConfig(CONFIG_UINT32_ANTICHEAT_ENABLED), Cheats::FLY))
            DetectFly();
    }
    else
        --m_SkipAntiCheat;

    m_MoveInfo[COM][OLD] = m_MoveInfo[COM][NEW];
    m_Opcode[COM][OLD] = m_Opcode[COM][NEW];
    m_ServerTime[COM][OLD] = m_ServerTime[COM][NEW];
}

void AntiCheat::DetectSpeed()
{
    if (WorldTimer::getMSTimeDiff(m_LastSpeedCheck, WorldTimer::getMSTime()) < sWorld.getConfig(CONFIG_UINT32_SPEEDCHEAT_INTERVAL))
        return;

    m_LastSpeedCheck = WorldTimer::getMSTime();

    std::ostringstream ss;
    float Traveled = GetDistance(IsFlying());
    float Allowed = GetSpeedRate() * 1.f + (float(sWorld.getConfig(CONFIG_UINT32_SPEEDCHEAT_TOLERANCE)) / 100);

    bool TooFast = Traveled > Allowed;

    ss << "Diff: " << Traveled - Allowed << " Traveled: " << Traveled << " Allowed: " << Allowed;

    bool Sliding = false;

    if (Map* pMap = m_player->GetMap())
    {
        const Position* pos = m_MoveInfo[SPEED][NEW].GetPos();

        float groundZ = pMap->GetHeight(pos->x, pos->y, pos->z);

        if (abs(groundZ - pos->z) < 1 && IsFalling())
            Sliding = true;
    }

    if (TooFast && !Sliding)
    {
        ReportCheat("Speed", ss.str());

        m_MoveInfo[SPEED][NEW] = m_MoveInfo[SPEED][OLD];
        const Position* pos = m_MoveInfo[SPEED][OLD].GetPos();

        m_player->TeleportTo(m_player->GetMapId(), pos->x, pos->y, pos->z, pos->o, 0, 0, true);
    }

    m_MoveInfo[SPEED][OLD] = m_MoveInfo[SPEED][NEW];
    m_Opcode[SPEED][OLD] = m_Opcode[SPEED][NEW];
    m_ServerTime[SPEED][OLD] = WorldTimer::getMSTime();
}

void AntiCheat::DetectTime()
{
    std::ostringstream ss;

    ss << "Client time: " << m_MoveInfo[COM][NEW].GetTime() << " Client based server time: " << m_ClientBasedServerTime;

    if (abs(int32(m_MoveInfo[COM][NEW].GetTime() - m_ClientBasedServerTime)) > 5000)
        ReportCheat("Time", ss.str());
}

void AntiCheat::DetectJump()
{
    if (m_Opcode[COM][NEW] != MSG_MOVE_JUMP)
        return;

    std::ostringstream ss;

    bool Detected = false;

    if (m_Opcode[COM][OLD] == m_Opcode[COM][NEW])
    {
        Detected = true;
        ss << "Opcode";
    }
    else
    {
        const Position* pos = m_MoveInfo[COM][OLD].GetPos();
        float groundZ = pos->z;
        if (Map* pMap = m_player->GetMap())
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

        const Position* pos = m_MoveInfo[COM][OLD].GetPos();
        float newz = pos->z;

        if (Map* pMap = m_player->GetMap())
            newz = pMap->GetHeight(pos->x, pos->y, pos->z);

        m_player->TeleportTo(m_player->GetMapId(), pos->x, pos->y, newz, pos->o, 0, 0, true);
        m_Opcode[COM][NEW] = MSG_NULL_ACTION;
    }
}

void AntiCheat::DetectFly()
{
    if (IsFlying() && !CanFly())
    {
        ReportCheat("Fly", "");
        SendFly(false);
    }
}

void AntiCheat::ReportCheat(std::string cheat, std::string info)
{
    std::ostringstream ss;
    ss << "Name: " << m_player->GetName() << "Cheat: " << cheat << " Info: " << info << std::endl;

    sCustom.SendGMMessage(ss.str());
}

float AntiCheat::GetSpeed()
{
    bool back = m_MoveInfo[COM][NEW].HasMovementFlag(MOVEFLAG_BACKWARD);

    float speed = 0;

    if (m_MoveInfo[COM][NEW].HasMovementFlag(MOVEFLAG_WALK_MODE))
        speed = m_player->GetSpeed(MOVE_WALK);
    else if (m_MoveInfo[COM][NEW].HasMovementFlag(MOVEFLAG_SWIMMING))
        speed = m_player->GetSpeed(back ? MOVE_SWIM_BACK : MOVE_SWIM);
    else if (m_MoveInfo[COM][NEW].HasMovementFlag(MOVEFLAG_FLYING))
        speed = m_player->GetSpeed(back ? MOVE_FLIGHT_BACK : MOVE_FLIGHT);
    else
        speed = m_player->GetSpeed(back ? MOVE_RUN_BACK : MOVE_RUN);

    return speed;
}

float AntiCheat::GetSpeedRate()
{
    return GetSpeed() * (GetClientDiff() / 1000.f);
}

float AntiCheat::GetClientDiff()
{
    return m_MoveInfo[COM][NEW].GetTime() - m_MoveInfo[COM][OLD].GetTime();
}

float AntiCheat::GetServerDiff()
{
    return m_ServerTime[COM][NEW] - m_ServerTime[COM][OLD];
}

float AntiCheat::GetDistance(bool threeD)
{
    return (threeD ? GetDistance3D() : GetDistance2D());
}

float AntiCheat::GetDistance2D()
{
    return sqrt(
        pow(m_MoveInfo[COM][OLD].GetPos()->x - m_MoveInfo[COM][NEW].GetPos()->x, 2) +
        pow(m_MoveInfo[COM][OLD].GetPos()->y - m_MoveInfo[COM][NEW].GetPos()->y, 2)
        );
}

float AntiCheat::GetDistance3D()
{
    return sqrt(
        pow(m_MoveInfo[COM][OLD].GetPos()->x - m_MoveInfo[COM][NEW].GetPos()->x, 2) +
        pow(m_MoveInfo[COM][OLD].GetPos()->y - m_MoveInfo[COM][NEW].GetPos()->y, 2) +
        pow(m_MoveInfo[COM][OLD].GetPos()->z - m_MoveInfo[COM][NEW].GetPos()->z, 2)
        );
}

float AntiCheat::GetDistanceZ()
{
    return m_MoveInfo[COM][NEW].GetPos()->z - m_MoveInfo[COM][OLD].GetPos()->z;
}

float AntiCheat::GetMoveAngle()
{
    return MapManager::NormalizeOrientation(tan(GetDistanceZ() / GetDistance2D()));
}

bool AntiCheat::IsFlying()
{
    return m_MoveInfo[COM][NEW].HasMovementFlag(MovementFlags(MOVEFLAG_FLYING | MOVEFLAG_FLYING2));
}

bool AntiCheat::IsFalling()
{
    return m_MoveInfo[COM][NEW].HasMovementFlag(MovementFlags(MOVEFLAG_FALLING | MOVEFLAG_FALLINGFAR | MOVEFLAG_SAFE_FALL));
}

bool AntiCheat::IsSwimming()
{
    return m_MoveInfo[COM][NEW].HasMovementFlag(MOVEFLAG_SWIMMING);
}

bool AntiCheat::IsRooted()
{
    return m_MoveInfo[COM][NEW].HasMovementFlag(MOVEFLAG_ROOT);
}

bool AntiCheat::CanFly()
{
    return m_player->HasAuraType(SPELL_AURA_FLY) || m_player->GetAntiCheat()->IsGMFly();
}

void AntiCheat::SendFly(bool value)
{
    WorldPacket data;
    data.SetOpcode(value ? SMSG_MOVE_SET_CAN_FLY : SMSG_MOVE_UNSET_CAN_FLY);
    data << m_player->GetPackGUID();
    data << uint32(0);                                      // unknown
    m_player->SendMessageToSet(&data, true);
}