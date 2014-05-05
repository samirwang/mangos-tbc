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

    m_SkipAntiCheat = 1;
    m_GmFly = false;

    m_FirstMoveInfo = true;
    m_MoveDeltaT = 0;
}

void AntiCheat::DetectHacks(MovementInfo& MoveInfo, Opcodes Opcode)
{
    m_MoveInfo[NEW] = MoveInfo;
    m_Opcode[NEW] = Opcode;

    if (m_FirstMoveInfo)
    {
        m_FirstMoveInfo = false;
        m_MoveDeltaT = MoveInfo.GetTime() - WorldTimer::getMSTime();
    }

    if (!m_SkipAntiCheat)
    {
        DetectSpeed();
    }
    else
        --m_SkipAntiCheat;

    m_MoveInfo[OLD] = m_MoveInfo[NEW];
    m_Opcode[OLD] = m_Opcode[NEW];
}

void AntiCheat::DetectSpeed()
{
    std::ostringstream ss;
    ss << "SpeedRate: " << GetSpeedRate() << " Distance: " << GetDistance(IsFlying()) << std::endl;

    // Rounded to closest 100ish downwards.
    float RoundedTraveledDistance = floor(GetDistance(IsFlying()) * 100.f) / 100.f;
    // Rounded to closest 100ish upwards
    float RoundedAllowedDistance = ceil(GetSpeedRate() * 100.f) / 100.f;

    bool TooFast = RoundedTraveledDistance > RoundedAllowedDistance;

    if (TooFast)
        ss << "Cheat" << std::endl;

    m_player->GetCPlayer()->BoxChat << ss.str();
}

float AntiCheat::GetSpeed()
{
    bool back = m_MoveInfo[NEW].HasMovementFlag(MOVEFLAG_BACKWARD);

    float speed = 0;

    if (m_MoveInfo[NEW].HasMovementFlag(MOVEFLAG_WALK_MODE))
        speed = m_player->GetSpeed(MOVE_WALK);
    else if (m_MoveInfo[NEW].HasMovementFlag(MOVEFLAG_SWIMMING))
        speed = m_player->GetSpeed(back ? MOVE_SWIM_BACK : MOVE_SWIM);
    else if (m_MoveInfo[NEW].HasMovementFlag(MOVEFLAG_FLYING))
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
    return m_MoveInfo[NEW].GetTime() - m_MoveInfo[OLD].GetTime();
}

float AntiCheat::GetServerDiff()
{
    return m_ServerTime[NEW] - m_ServerTime[OLD];
}

float AntiCheat::GetDistance(bool threeD)
{
    return (threeD ? GetDistance3D() : GetDistance2D());
}

float AntiCheat::GetDistance2D()
{
    return sqrt(
        pow(m_MoveInfo[OLD].GetPos()->x - m_MoveInfo[NEW].GetPos()->x, 2) +
        pow(m_MoveInfo[OLD].GetPos()->y - m_MoveInfo[NEW].GetPos()->y, 2)
        );
}

float AntiCheat::GetDistance3D()
{
    return sqrt(
        pow(m_MoveInfo[OLD].GetPos()->x - m_MoveInfo[NEW].GetPos()->x, 2) +
        pow(m_MoveInfo[OLD].GetPos()->y - m_MoveInfo[NEW].GetPos()->y, 2) +
        pow(m_MoveInfo[OLD].GetPos()->z - m_MoveInfo[NEW].GetPos()->z, 2)
        );
}

float AntiCheat::GetDistanceZ()
{
    return m_MoveInfo[NEW].GetPos()->z - m_MoveInfo[OLD].GetPos()->z;
}

float AntiCheat::GetMoveAngle()
{
    return MapManager::NormalizeOrientation(tan(GetDistanceZ() / GetDistance2D()));
}

bool AntiCheat::IsFlying()
{
    //return m_player->HasAuraType(SPELL_AURA_FLY) || m_player->GetAntiCheat()->IsGMFly();
    return m_MoveInfo[NEW].HasMovementFlag(MovementFlags(MOVEFLAG_FLYING | MOVEFLAG_FLYING2));
}

bool AntiCheat::IsFalling()
{
    return m_MoveInfo[NEW].HasMovementFlag(MovementFlags(MOVEFLAG_FALLING | MOVEFLAG_FALLINGFAR | MOVEFLAG_SAFE_FALL));
}

bool AntiCheat::IsSwimming()
{
    return m_MoveInfo[NEW].HasMovementFlag(MOVEFLAG_SWIMMING);
}

bool AntiCheat::IsRooted()
{
    return m_MoveInfo[NEW].HasMovementFlag(MOVEFLAG_ROOT);
}
