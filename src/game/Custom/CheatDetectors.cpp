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
#include "World.h"

void AntiCheat_speed::DetectHack(MovementInfo& MoveInfo, Opcodes Opcode)
{
    AntiCheat::DetectHack(MoveInfo, Opcode);

    if (GetDistance(IsFlying(Cheat::BOTH)) < GetCurSpeed())
        return;

    auto Traveled = GetDistance(IsFlying(Cheat::BOTH));
    auto Allowed = GetSpeedRate() * (1.f + (float(sWorld.getConfig(CONFIG_UINT32_SPEEDCHEAT_TOLERANCE)) / 100.f));

    auto TooFast = Traveled > Allowed;

    auto Sliding = false;

    if (Map* pMap = m_Player->GetMap())
    {
        auto pos = m_MoveInfo[Cheat::NEW].GetPos();

        auto groundZ = pMap->GetHeight(pos->x, pos->y, pos->z);

        if (abs(groundZ - pos->z) < 1 && IsFalling(Cheat::BOTH))
            Sliding = true;
    }

    if (TooFast && !Sliding && !Skipping())
    {
        std::ostringstream ss;
        ss << "Traveled: " << Traveled << " Allowed: " << Allowed << std::endl;
        ss << "InCombat: " << (m_Player->isInCombat() ? "Yes" : "No") << std::endl;

        ReportCheat("Speed", ss.str());

        TeleportBack();
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

        if (pos->z - groundZ > 1.f)
        {
            Detected = true;
            ss << "Height";
        }
    }

    if (Detected && !Skipping())
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

    if (IsFlying(Cheat::NEW) && !CanFly() && !Skipping())
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

    if (IsFlying(Cheat::BOTH) || IsFalling(Cheat::BOTH) || IsSwimming(Cheat::BOTH) || Skipping())
    {
        SetOldValues();
        return;
    }

    if (GetDistance3D() < 1.f)
        return;

    auto angle = std::atan2(GetDistanceZ(), GetDistance2D()) * 180.f / M_PI_F;

    if (angle > 50 && !Skipping())
    {
        std::ostringstream ss;
        ss << "Angle: " << angle;
        ReportCheat("Climb", ss.str());

        TeleportBack();
    }

    SetOldValues();
}

void AntiCheat_teleport::DetectHack(MovementInfo& MoveInfo, Opcodes Opcode)
{
    AntiCheat::DetectHack(MoveInfo, Opcode);


    switch (Opcode)
    {
    case MSG_MOVE_START_FORWARD:
    case MSG_MOVE_START_BACKWARD:
    case MSG_MOVE_START_STRAFE_LEFT:
    case MSG_MOVE_START_STRAFE_RIGHT:
    case MSG_MOVE_JUMP:
    case MSG_MOVE_START_SWIM:
        Moves[Opcode] = true;
        break;

    case MSG_MOVE_STOP:
        Moves[MSG_MOVE_START_FORWARD] = false;
        Moves[MSG_MOVE_START_BACKWARD] = false;
        break;
    case MSG_MOVE_STOP_STRAFE:
        Moves[MSG_MOVE_START_STRAFE_LEFT] = false;
        Moves[MSG_MOVE_START_STRAFE_RIGHT] = false;
        break;
    case MSG_MOVE_FALL_LAND:
        Moves[MSG_MOVE_JUMP] = false;
        break;
    case MSG_MOVE_STOP_SWIM:
        Moves[MSG_MOVE_START_SWIM] = false;
    default:
        break;
    }

    bool MovingNow = false;

    for (auto& i : Moves)
        if (i.second)
            MovingNow = true;

    Moving[Cheat::NEW] = MovingNow;

    if (!Moving[Cheat::OLD] && GetDistance3D() > 0.f)
    {
        ReportCheat("Teleport", "");
        TeleportBack();
    }

    SetOldValues();
    Moving[Cheat::OLD] = Moving[Cheat::NEW];
}
