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

    bool startcode = false;

    switch (m_Opcode[Cheat::NEW])
    {
    case MSG_MOVE_START_FORWARD:
    case MSG_MOVE_START_BACKWARD:
    case MSG_MOVE_START_STRAFE_LEFT:
    case MSG_MOVE_START_STRAFE_RIGHT:
    case MSG_MOVE_START_SWIM:
    case MSG_MOVE_START_ASCEND:
    case MSG_MOVE_START_DESCEND:
        startcode = true;
    default:
        break;
    }

    if (!startcode || Skipping())
    {
        SetOldValues();
        return;
    }

    bool cheat = false;

    if (GetDistance3D() > GetCurSpeed())
        cheat = true;

    auto opos = m_MoveInfo[Cheat::OLD].GetPos();
    auto npos = m_MoveInfo[Cheat::NEW].GetPos();

    if (!cheat && m_Player->GetMap()->IsInLineOfSight(npos->x, npos->y, npos->z + 2.0f, opos->x, opos->y, opos->z + 2.0f))
        cheat = true;

    if (cheat)
    {
        MapEntry const* mapEntry = sMapStore.LookupEntry(m_Player->GetMapId());
        uint32 ZoneID[2];
        uint32 AreaID[2];
        AreaTableEntry const* ZoneEntry[2];
        AreaTableEntry const* AreaEntry[2];

        m_Player->GetTerrain()->GetZoneAndAreaId(ZoneID[Cheat::OLD], AreaID[Cheat::OLD], opos->x, opos->y, opos->z);
        m_Player->GetTerrain()->GetZoneAndAreaId(ZoneID[Cheat::NEW], AreaID[Cheat::NEW], npos->x, npos->y, npos->z);

        for (uint8 i = 0; i < 2; ++i)
        {
            ZoneEntry[i] = GetAreaEntryByAreaID(ZoneID[i]);
            AreaEntry[i] = GetAreaEntryByAreaID(AreaID[i]);
        }

        auto locale = m_Player->GetSession()->GetSessionDbcLocale();

        std::ostringstream ss;
        ss << "Distance: " << GetDistance3D() << " Map: " << mapEntry->name[locale] << std::endl;
        ss << "Old Position: ";
        ss << "X: " << opos->x << " Y: " << opos->y << " Z: " << opos->z << std::endl;
        ss << "Zone: " << ZoneEntry[Cheat::OLD]->area_name[locale] << " (" << ZoneEntry[Cheat::OLD]->ID << ") ";
        ss << "Area: " << AreaEntry[Cheat::OLD]->area_name[locale] << " (" << AreaEntry[Cheat::OLD]->ID << ") " << std::endl;
        ss << "New Position: ";
        ss << "X: " << npos->x << " Y: " << npos->y << " Z: " << npos->z << std::endl;
        if (ZoneEntry[Cheat::OLD] != ZoneEntry[Cheat::NEW])
            ss << "Zone: " << ZoneEntry[Cheat::NEW]->area_name[locale] << " (" << ZoneEntry[Cheat::NEW]->ID << ") ";
        if (AreaEntry[Cheat::OLD] != AreaEntry[Cheat::NEW])
            ss << "Area: " << AreaEntry[Cheat::NEW]->area_name[locale] << " (" << AreaEntry[Cheat::NEW]->ID << ") " << std::endl;

        ReportCheat("Teleport", ss.str());

        TeleportBack();
    }

    SetOldValues();
}
