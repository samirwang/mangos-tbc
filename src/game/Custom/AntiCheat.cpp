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

AntiCheat::AntiCheat(Player* pPlayer)
{
    m_player = pPlayer;

    m_SkipAntiCheat = true;
    m_GmFly = false;

    m_SpeedCheat = new AntiCheat_speed(pPlayer);
    m_HeightCheat = new AntiCheat_height(pPlayer);
    m_ClimbCheat = new AntiCheat_climb(pPlayer);
}

AntiCheat::~AntiCheat()
{
    delete m_SpeedCheat;
    delete m_HeightCheat;
    delete m_ClimbCheat;
}


bool AntiCheat_module::IsFlying()
{
    return m_player->HasAuraType(SPELL_AURA_FLY) || m_player->GetAntiCheat()->IsGMFly();
}

bool AntiCheat_module::IsFalling()
{
    return m_CurMoveInfo.HasMovementFlag(MovementFlags(MOVEFLAG_FALLING | MOVEFLAG_FALLINGFAR | MOVEFLAG_SAFE_FALL));
}

bool AntiCheat_module::IsSwimming()
{
    return m_CurMoveInfo.HasMovementFlag(MOVEFLAG_SWIMMING);
}

bool AntiCheat_module::IsRooted()
{
    return m_CurMoveInfo.HasMovementFlag(MOVEFLAG_ROOT);
}

void AntiCheat::DetectHacks(MovementInfo& MoveInfo, Opcodes Opcode)
{
    m_SpeedCheat->DetectHack(MoveInfo, Opcode);
    m_HeightCheat->DetectHack(MoveInfo, Opcode);
    m_ClimbCheat->DetectHack(MoveInfo, Opcode);

    m_SkipAntiCheat = false;
}

void AntiCheat_module::DetectHack(MovementInfo& MoveInfo, Opcodes Opcode)
{
    m_CurMoveInfo = MoveInfo;
    m_CurOpcode = Opcode;
    m_CurServerTime = WorldTimer::getMSTime();

    m_CTimeDiff = WorldTimer::getMSTimeDiff(m_OldServerTime, m_CurServerTime);
    m_STimeDiff = WorldTimer::getMSTimeDiff(m_OldMoveInfo.GetTime(), m_CurMoveInfo.GetTime());
    m_CSTimeDiff = m_CTimeDiff - m_STimeDiff;

    m_DetectionDelay += WorldTimer::getMSTimeDiff(m_PrevPacketTime, m_CurServerTime);
    m_PrevPacketTime = m_CurServerTime;

    float dx = m_OldMoveInfo.GetPos()->x - m_CurMoveInfo.GetPos()->x;
    float dy = m_OldMoveInfo.GetPos()->y - m_CurMoveInfo.GetPos()->y;
    m_MoveDist = sqrt((dx * dx) + (dy * dy)); // Traveled distance

   m_DeltaZ = abs(m_OldMoveInfo.GetPos()->z - m_CurMoveInfo.GetPos()->z);
}

void AntiCheat_module::SetOldValues()
{
    m_OldMoveInfo = m_CurMoveInfo;
    m_OldOpcode = m_CurOpcode;
    m_OldServerTime = m_CurServerTime;

    m_DetectionDelay = 0;
}

void AntiCheat_module::ReportPlayer(std::string hack, std::string misc)
{
    if (m_player->isGameMaster() || m_player->GetAntiCheat()->SkippingAntiCheat())
        return;

    CharacterDatabase.PExecute("INSERT INTO cheaters (guid, account, type, time) VALUES (%u, %u, '%s', %u)", m_player->GetGUIDLow(), m_player->GetSession()->GetAccountId(), hack, sWorld.GetGameTime());

    std::ostringstream ss;
    ss << "Player " << m_player->GetName() << " was caught " << hack << " " << misc;

    sCustom.SendGMMessage(ss.str());
}

void AntiCheat_speed::DetectHack(MovementInfo& MoveInfo, Opcodes Opcode)
{
    AntiCheat_module::DetectHack(MoveInfo, Opcode);

    if (!(m_DetectionDelay >= 500))
        return;

    bool back = m_CurMoveInfo.HasMovementFlag(MOVEFLAG_BACKWARD) &&
        m_OldMoveInfo.HasMovementFlag(MOVEFLAG_BACKWARD);

    float speed = 0;

    if (m_CurMoveInfo.HasMovementFlag(MOVEFLAG_WALK_MODE))
        speed = m_player->GetSpeed(MOVE_WALK);
    else if (m_CurMoveInfo.HasMovementFlag(MOVEFLAG_SWIMMING))
        speed = m_player->GetSpeed(back ? MOVE_SWIM_BACK : MOVE_SWIM);
    else if (m_CurMoveInfo.HasMovementFlag(MOVEFLAG_FLYING))
        speed = m_player->GetSpeed(back ? MOVE_FLIGHT_BACK : MOVE_FLIGHT);
    else
        speed = m_player->GetSpeed(back ? MOVE_RUN_BACK : MOVE_RUN);


    float highspeed = std::max(m_OldMoveSpeed, speed);

    m_OldMoveSpeed = speed;

    float speedmod = m_CTimeDiff / 1000.f;

    float maxdist = highspeed * speedmod;

    const size_t listsize = 15;

    float dist = floor(m_MoveDist * 10.f) / 10.f;
    maxdist = ceil(maxdist * 10.f) / 10.f;

    if (IsFalling())
    {
        float angle = MapManager::NormalizeOrientation(tan(m_DeltaZ / dist));

        // Sliding the ground which can be really fast.
        if (angle > 1.9f)
            maxdist *= 3;
    }

    if (dist > maxdist)
    {
        ++m_DetectStreak;

        if (m_DetectStreak >= 2)
        {
            std::ostringstream ss;
            ss << std::endl;
            ss << "TimeDiff: " << m_STimeDiff << " Distance Traveled: " << dist << " Distance Allowed: " << maxdist << std::endl;
            ss << "DetectStreak: " << m_DetectStreak;

            ReportPlayer("speedhacking", ss.str());
        }
    }
    else
        m_DetectStreak = 0;

    SetOldValues();
}

void AntiCheat_height::DetectHack(MovementInfo& MoveInfo, Opcodes Opcode)
{
    AntiCheat_module::DetectHack(MoveInfo, Opcode);

    if (!((!IsFlying() && !IsRooted() && !IsSwimming() && !IsFalling() && m_DetectionDelay > 500) || (IsFalling() && m_CurOpcode == MSG_MOVE_JUMP)))
        return;

    if (!m_player->getDeathState() == ALIVE)
        return;

    float Size = m_player->GetObjectBoundingRadius();
    
    float x = m_CurMoveInfo.GetPos()->x;
    float y = m_CurMoveInfo.GetPos()->y;
    float z = m_CurMoveInfo.GetPos()->z;
    float o = m_CurMoveInfo.GetPos()->o;

    Map* pMap = m_player->GetMap();

    if (!pMap)
        return;

    float heightrange = 1.f;
    const float firstrange = 0.5f;
    const uint8 ranges = 2;
    const uint8 points = 8;

    bool notcheat = false;

    for (uint8 i = 1; i <= ranges; i++)
    {
        for (uint8 j = 1; j <= points; ++j)
        {
            float angle = MapManager::NormalizeOrientation((M_PI_F / points) * j);
            float radius = float(firstrange * i);

            float cx = ((cos(angle)*radius) + x);
            float cy = ((sin(angle)*radius) + y);
            float cz = pMap->GetHeight(cx, cy, z);

            if (abs(cz - z) < heightrange)
                notcheat = true;
        }
    }

    float cz = pMap->GetHeight(x, y, z);

    if (!notcheat)
    if (abs(cz - z) < heightrange)
        notcheat = true;

    if (!notcheat)
    {
        std::ostringstream ss;
        ss << std::endl;
        ss << "Distance to ground: " << abs(cz - z) << " " << (z > cz ? "above" : "under");

        ReportPlayer((m_CurOpcode == MSG_MOVE_JUMP ? "jumphacking" : (z > cz ? "flyhacking" : "planehacking")), ss.str());
    }

    SetOldValues();
}

void AntiCheat_climb::DetectHack(MovementInfo& MoveInfo, Opcodes Opcode)
{
    AntiCheat_module::DetectHack(MoveInfo, Opcode);

    if (IsFlying() || IsFalling() || IsSwimming())
        return;

    float dist[2];
    float deltaZ[2];
    float floor_z[2];
    float angle[2];

    dist[0] = m_MoveDist;

    float Size = m_player->GetObjectBoundingRadius();
    dist[1] = Size * 2;


    float x = m_CurMoveInfo.GetPos()->x;
    float y = m_CurMoveInfo.GetPos()->y;
    float z = m_CurMoveInfo.GetPos()->z;
    float o = m_CurMoveInfo.GetPos()->o;

    Map* pMap = m_player->GetMap();

    if (!pMap)
        return;

    // Forward
    float fx = x + cosf(o) * 1;
    float fy = y + sinf(o) * 1;
    floor_z[0] = pMap->GetHeight(fx, fy, z);

    // Backward
    float bx = x + cosf(o)*-1;
    float by = y + sinf(o)*-1;
    floor_z[1] = pMap->GetHeight(bx, by, z);

    deltaZ[0] = m_DeltaZ;
    deltaZ[1] = fabs(floor_z[0] - floor_z[1]);

    for (uint8 i = 0; i < 2; i++)
        angle[i] = MapManager::NormalizeOrientation(tan(deltaZ[i] / dist[i]));

    if (angle[0] > 1.9f && angle[1] > 1.9f)
    {
        std::ostringstream ss;
        ss << std::endl;
        ss << "Distance: " << m_MoveDist;

        ReportPlayer("climbhacking", ss.str());
    }

    SetOldValues();
}