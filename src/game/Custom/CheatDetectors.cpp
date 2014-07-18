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
#include "AntiCheat.h"
#include "World.h"
#include "MapManager.h"

void AntiCheat_speed::DetectHack(MovementInfo& MoveInfo, Opcodes Opcode)
{
    if (WorldTimer::getMSTimeDiff(m_LastSpeedCheck, WorldTimer::getMSTime()) < sWorld.getConfig(CONFIG_UINT32_SPEEDCHEAT_INTERVAL) && GetDistance(IsFlying(Cheat::BOTH)) < GetCurSpeed())
        return;

    AntiCheat::DetectHack(MoveInfo, Opcode);

    std::ostringstream ss;
    auto Traveled = GetDistance(IsFlying(Cheat::BOTH));
    auto Allowed = GetSpeedRate() * 1.f + (float(sWorld.getConfig(CONFIG_UINT32_SPEEDCHEAT_TOLERANCE)) / 100.f);

    auto TooFast = Traveled > Allowed;

    ss << "Diff: " << Traveled - Allowed << " Traveled: " << Traveled << " Allowed: " << Allowed;

    auto Sliding = false;

    if (Map* pMap = m_Player->GetMap())
    {
        auto pos = m_MoveInfo[Cheat::NEW].GetPos();

        auto groundZ = pMap->GetHeight(pos->x, pos->y, pos->z);

        if (abs(groundZ - pos->z) < 1 && IsFalling(Cheat::BOTH))
            Sliding = true;
    }

    if (TooFast && !Sliding)
    {
        ReportCheat("Speed", ss.str());

        m_Player->SetAntiCheatMoveInfo(m_MoveInfo[Cheat::OLD]);
        auto pos = m_MoveInfo[Cheat::OLD].GetPos();

        m_Player->TeleportTo(m_Player->GetMapId(), pos->x, pos->y, pos->z, pos->o, 0, 0, true);
    }

    SetOldValues();

    m_LastSpeedCheck = WorldTimer::getMSTime();
}


void AntiCheat_jump::DetectHack(MovementInfo& MoveInfo, Opcodes Opcode)
{
    AntiCheat::DetectHack(MoveInfo, Opcode);

    if (m_Opcode[Cheat::NEW] != MSG_MOVE_JUMP)
    {
        SetOldValues();
        return;
    }

    std::ostringstream ss;

    auto Detected = false;

    if (m_Opcode[Cheat::OLD] == m_Opcode[Cheat::NEW])
    {
        Detected = true;
        ss << "Opcode";
    }
    else
    {
        auto pos = m_MoveInfo[Cheat::OLD].GetPos();
        auto groundZ = pos->z;
        if (Map* pMap = m_Player->GetMap())
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

        auto pos = m_MoveInfo[Cheat::OLD].GetPos();
        auto newz = pos->z;

        if (Map* pMap = m_Player->GetMap())
            newz = pMap->GetHeight(pos->x, pos->y, pos->z);

        m_Player->TeleportTo(m_Player->GetMapId(), pos->x, pos->y, newz, pos->o, 0, 0, true);
        m_Opcode[Cheat::NEW] = MSG_NULL_ACTION;
    }

    SetOldValues();
}

void AntiCheat_fly::DetectHack(MovementInfo& MoveInfo, Opcodes Opcode)
{
    AntiCheat::DetectHack(MoveInfo, Opcode);

    if (IsFlying(Cheat::NEW) && !CanFly())
    {
        std::ostringstream ss;

        if (Map* pMap = m_Player->GetMap())
        {
            const Position* pos = m_MoveInfo[Cheat::NEW].GetPos();
            float groundZ = pMap->GetHeight(pos->x, pos->y, pos->z);
            ss << "Height: " << pos->z - groundZ;
        }

        ReportCheat("Fly", ss.str());
        SendFly(false);
    }

    SetOldValues();
}

void AntiCheat_climb::DetectHack(MovementInfo& MoveInfo, Opcodes Opcode)
{
    AntiCheat::DetectHack(MoveInfo, Opcode);

    if (IsFlying(Cheat::BOTH) || IsFalling(Cheat::BOTH) || IsSwimming(Cheat::BOTH))
    {
        SetOldValues();
        return;
    }

    if (GetDistance3D() < 1)
        return;

    auto angle = std::atan2(GetDistanceZ(), GetDistance2D()) * 180.f / M_PI_F;

    if (angle > 50)
    {
        std::ostringstream ss;
        ss << "Angle: " << angle;
        ReportCheat("Climb", ss.str());

        m_Player->SetAntiCheatMoveInfo(m_MoveInfo[Cheat::OLD]);
        auto pos = m_MoveInfo[Cheat::OLD].GetPos();

        m_Player->TeleportTo(m_Player->GetMapId(), pos->x, pos->y, pos->z, pos->o, 0, 0, true);
    }

    SetOldValues();
}