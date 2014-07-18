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
#include "AntiCheat.h"

void CPlayer::DetectHacks(MovementInfo& MoveInfo, Opcodes Opcode)
{
    for (auto& i : m_CheatDetectors)
        i->DetectHack(MoveInfo, Opcode);

    if (SkipAC())
        --m_SkipAntiCheat;
}

void CPlayer::SetAntiCheatMoveInfo(MovementInfo& MoveInfo)
{
    for (auto& i : m_CheatDetectors)
        i->SetMoveInfo(MoveInfo);
}

AntiCheat::AntiCheat(CPlayer* pPlayer)
{
    m_Player = pPlayer;
    for (auto i = 0; i < 2; ++i)
    {
        m_MoveInfo[i] = MovementInfo();
        m_Opcode[i] = MSG_NULL_ACTION;
        m_ServerTime[i] = 0;
    }
}

void AntiCheat::DetectHack(MovementInfo& MoveInfo, Opcodes Opcode)
{
    m_MoveInfo[Cheat::NEW] = MoveInfo;
    m_Opcode[Cheat::NEW] = Opcode;
    m_ServerTime[Cheat::NEW] = WorldTimer::getMSTime();
}

void AntiCheat::SetOldValues()
{
    m_MoveInfo[Cheat::OLD] = m_MoveInfo[Cheat::NEW];
    m_Opcode[Cheat::OLD] = m_Opcode[Cheat::NEW];
    m_ServerTime[Cheat::OLD] = WorldTimer::getMSTime();
}

void AntiCheat::SetMoveInfo(MovementInfo& MoveInfo)
{
    for (auto i = 0; i < 2; ++i)
        m_MoveInfo[i] = MoveInfo;
}

void AntiCheat::ReportCheat(std::string cheat, std::string info)
{
    std::ostringstream ss;
    ss << "Name: " << m_Player->GetName() << " Cheat: " << cheat << " Info: " << (info.empty() ? "No info available" : info) << std::endl;

    sCustom.SendGMMessage(ss.str());
}

float AntiCheat::GetCurSpeed()
{
    float speed = 0;

    for (uint8 i = 0; i < 2; ++i)
    {
        float thisspeed = 0;
 
        bool back = m_MoveInfo[i].HasMovementFlag(MOVEFLAG_BACKWARD);

        if (m_MoveInfo[i].HasMovementFlag(MOVEFLAG_WALK_MODE))
            thisspeed = m_Player->GetSpeed(MOVE_WALK);
        else if (m_MoveInfo[i].HasMovementFlag(MOVEFLAG_SWIMMING))
            thisspeed = m_Player->GetSpeed(back ? MOVE_SWIM_BACK : MOVE_SWIM);
        else if (m_MoveInfo[i].HasMovementFlag(MOVEFLAG_FLYING))
            thisspeed = m_Player->GetSpeed(back ? MOVE_FLIGHT_BACK : MOVE_FLIGHT);
        else
            thisspeed = m_Player->GetSpeed(back ? MOVE_RUN_BACK : MOVE_RUN);

        if (thisspeed > speed)
            speed = thisspeed;
    }

    return speed;
}

float AntiCheat::GetSpeedRate()
{
    return GetCurSpeed() * (GetClientDiff() / 1000.f);
}

float AntiCheat::GetClientDiff(bool limit)
{
    auto diff = m_MoveInfo[Cheat::NEW].GetTime() - m_MoveInfo[Cheat::OLD].GetTime();
    return diff > 2500 ? 2500 : diff;
}

float AntiCheat::GetServerDiff()
{
    return m_ServerTime[Cheat::NEW] - m_ServerTime[Cheat::OLD];
}

float AntiCheat::GetDistance(bool is3D)
{
    return (is3D ? GetDistance3D() : GetDistance2D());
}

float AntiCheat::GetDistance2D()
{
    return sqrt(
        pow(m_MoveInfo[Cheat::OLD].GetPos()->x - m_MoveInfo[Cheat::NEW].GetPos()->x, 2) +
        pow(m_MoveInfo[Cheat::OLD].GetPos()->y - m_MoveInfo[Cheat::NEW].GetPos()->y, 2)
        );
}

float AntiCheat::GetDistance3D()
{
    return sqrt(
        pow(m_MoveInfo[Cheat::OLD].GetPos()->x - m_MoveInfo[Cheat::NEW].GetPos()->x, 2) +
        pow(m_MoveInfo[Cheat::OLD].GetPos()->y - m_MoveInfo[Cheat::NEW].GetPos()->y, 2) +
        pow(m_MoveInfo[Cheat::OLD].GetPos()->z - m_MoveInfo[Cheat::NEW].GetPos()->z, 2)
        );
}

float AntiCheat::GetDistanceZ()
{
    return m_MoveInfo[Cheat::NEW].GetPos()->z - m_MoveInfo[Cheat::OLD].GetPos()->z;
}

float AntiCheat::GetMoveAngle()
{
    return MapManager::NormalizeOrientation(tan(GetDistanceZ() / GetDistance2D()));
}

bool AntiCheat::IsFlying(uint8 when)
{
    if (when < Cheat::BOTH)
        return m_MoveInfo[when].HasMovementFlag(MovementFlags(MOVEFLAG_FLYING | MOVEFLAG_FLYING2));

    for (auto i = 0; i < 2; ++i)
    if (m_MoveInfo[i].HasMovementFlag(MovementFlags(MOVEFLAG_FLYING | MOVEFLAG_FLYING2)))
        return true;

    return false;
}

bool AntiCheat::IsFalling(uint8 when)
{
    if (when < Cheat::BOTH)
        return m_MoveInfo[when].HasMovementFlag(MovementFlags(MOVEFLAG_FALLING | MOVEFLAG_FALLINGFAR | MOVEFLAG_SAFE_FALL));

    for (auto i = 0; i < 2; ++i)
    if (m_MoveInfo[i].HasMovementFlag(MovementFlags(MOVEFLAG_FALLING | MOVEFLAG_FALLINGFAR | MOVEFLAG_SAFE_FALL)))
        return true;

    return false;
}

bool AntiCheat::IsSwimming(uint8 when)
{
    if (when < Cheat::BOTH)
        return m_MoveInfo[when].HasMovementFlag(MOVEFLAG_SWIMMING);

    for (auto i = 0; i < 2; ++i)
    if (m_MoveInfo[i].HasMovementFlag(MOVEFLAG_SWIMMING))
        return true;

    return false;
}

bool AntiCheat::IsRooted(uint8 when)
{
    if (when < Cheat::BOTH)
        return m_MoveInfo[when].HasMovementFlag(MOVEFLAG_ROOT);

    for (auto i = 0; i < 2; ++i)
    if (m_MoveInfo[i].HasMovementFlag(MOVEFLAG_ROOT))
        return true;

    return false;
}

bool AntiCheat::CanFly()
{
    return m_Player->HasAuraType(SPELL_AURA_FLY) || m_Player->IsGMFly();
}

void AntiCheat::SendFly(bool value)
{
    WorldPacket data;
    data.SetOpcode(value ? SMSG_MOVE_SET_CAN_FLY : SMSG_MOVE_UNSET_CAN_FLY);
    data << m_Player->GetPackGUID();
    data << uint32(0);                                      // unknown
    m_Player->SendMessageToSet(&data, true);
}