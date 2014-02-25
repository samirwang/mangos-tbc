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

#include "Player.h"
#include "Timer.h"
#include "World.h"
#include "AntiCheat.h"
#include "MapManager.h"
#include "Custom.h"

AntiCheat::AntiCheat(Player* pPlayer)
{
    m_player = pPlayer;

    m_OldMoveTime = 0;
    m_OldMoveSpeed = 0;
    m_SkipAntiCheat = 1;
    m_GmFly = 0;
    m_CheatReportTimer[0] = 0;
    m_CheatReportTimer[1] = 0;
    m_LastHack = "";
    m_ServerTime = 0;
    m_HeightDelay = 0;
    m_SpeedDelay = 0;

    for (uint8 i = 0; i < 2; i++)
        m_Opcode[i] = MSG_NULL_ACTION;
}

bool AntiCheat::IsFlying()
{
    return m_player->HasAuraType(SPELL_AURA_FLY) || m_GmFly;
}

bool AntiCheat::IsFalling()
{
    for (uint8 i = 0; i < 2; i++)
    if (m_MoveInfo[i].HasMovementFlag(MovementFlags(MOVEFLAG_FALLING | MOVEFLAG_FALLINGFAR | MOVEFLAG_SAFE_FALL)))
        return true;

    return false;
}

bool AntiCheat::IsSwimming()
{
    for (uint8 i = 0; i < 2; i++)
    if (m_MoveInfo[0].HasMovementFlag(MOVEFLAG_SWIMMING))
        return true;

    return false;
}

bool AntiCheat::IsRooted()
{
    for (uint8 i = 0; i < 2; i++)
    if (m_MoveInfo[i].HasMovementFlag(MOVEFLAG_ROOT))
        return true;

    return false;
}

void AntiCheat::HandleMovementCheat(MovementInfo& MoveInfo, Opcodes opcode)
{
    bool SkipChecks = m_SkipAntiCheat;
    m_SkipAntiCheat = false;

    m_MoveInfo[0] = MoveInfo;
    m_Opcode[0] = opcode;
    m_ServerTime = WorldTimer::getMSTimeDiff(GetOldMoveTime(), WorldTimer::getMSTime());

    if (m_ServerTime == 0)
        m_ServerTime = 1;

    if (!SkipChecks && !m_player->GetTransport() && !m_player->IsTaxiFlying())
    {
        HandleSpeedCheat();
        HandleHeightCheat();
        HandleClimbCheat();
    }

    m_MoveInfo[1] = MoveInfo;
    m_Opcode[1] = opcode;
    m_OldMoveTime = WorldTimer::getMSTime();
}

void AntiCheat::HandleSpeedCheat()
{
    m_SpeedDelay += m_ServerTime;

    if (!((m_Opcode[0] == MSG_MOVE_HEARTBEAT && m_SpeedDelay >= 100) || m_SpeedDelay >= 500))
        return;

    m_SpeedDelay = 0;

    bool back = m_MoveInfo[0].HasMovementFlag(MOVEFLAG_BACKWARD);

    float speed = 0;

    if (m_MoveInfo[0].HasMovementFlag(MOVEFLAG_WALK_MODE))
        speed = m_player->GetSpeed(MOVE_WALK);
    else if (m_MoveInfo[0].HasMovementFlag(MOVEFLAG_SWIMMING))
        speed = m_player->GetSpeed(back ? MOVE_SWIM_BACK : MOVE_SWIM);
    else if (m_MoveInfo[0].HasMovementFlag(MOVEFLAG_FLYING))
        speed = m_player->GetSpeed(back ? MOVE_FLIGHT_BACK : MOVE_FLIGHT);
    else
        speed = m_player->GetSpeed(back ? MOVE_RUN_BACK : MOVE_RUN);


    float highspeed = std::max(m_OldMoveSpeed, speed);

    m_OldMoveSpeed = speed;

    float dx = m_MoveInfo[1].GetPos()->x - m_MoveInfo[0].GetPos()->x;
    float dy = m_MoveInfo[1].GetPos()->y - m_MoveInfo[0].GetPos()->y;
    float dist = sqrt((dx * dx) + (dy * dy)); // Traveled distance

    float c_mstime = abs(int32(m_MoveInfo[0].GetTime() - m_MoveInfo[1].GetTime()));
    float s_mstime = WorldTimer::getMSTimeDiff(m_OldMoveTime, WorldTimer::getMSTime());
    float difftime = abs(c_mstime - s_mstime);

    if ((c_mstime + s_mstime) / 2 > s_mstime * 1.1)
        c_mstime = s_mstime;

    float speedmod = c_mstime / 1000;

    float maxdist = highspeed * speedmod;

    const size_t listsize = 15;

    dist = floor(dist * 10.f) / 10.f;
    maxdist = ceil(maxdist * 10.f) / 10.f;

    if (IsFalling())
    {
        float deltaZ = abs(m_MoveInfo[1].GetPos()->z - m_MoveInfo[0].GetPos()->z);
        float angle = MapManager::NormalizeOrientation(tan(deltaZ / dist));

        // Sliding the ground which can be really fast.
        if (angle > 1.9f)
            maxdist *= 3;
    }

    if (dist > maxdist)
        HandleCheatReport("speedhacking");
}

void AntiCheat::HandleHeightCheat()
{
    m_HeightDelay += m_ServerTime;

    if (!((!IsFlying() && !IsRooted() && !IsSwimming() && !IsFalling() && m_HeightDelay > 500) || (IsFalling() && m_Opcode[0] == MSG_MOVE_JUMP)))
        return;

    m_HeightDelay = 0;

    if (!m_player->getDeathState() == ALIVE)
        return;

    float Size = m_player->GetObjectBoundingRadius();

    float x = m_MoveInfo[0].GetPos()->x;
    float y = m_MoveInfo[0].GetPos()->y;
    float z = m_MoveInfo[0].GetPos()->z;
    float o = m_MoveInfo[0].GetPos()->o;

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

    if (!notcheat)
    {
        float cz = pMap->GetHeight(x, y, z);

        if (abs(cz - z) < heightrange)
            notcheat = true;
    }

    if (!notcheat)
        HandleCheatReport(m_Opcode[0] == MSG_MOVE_JUMP ? "jump hacking" : "height hacking");
}

void AntiCheat::HandleClimbCheat()
{

    if (IsFlying() || IsFalling() || IsSwimming())
        return;

    float dist[2];
    float deltaZ[2];
    float floor_z[2];
    float angle[2];

    float dx = m_MoveInfo[1].GetPos()->x - m_MoveInfo[0].GetPos()->x;
    float dy = m_MoveInfo[1].GetPos()->y - m_MoveInfo[0].GetPos()->y;
    dist[0] = sqrt((dx * dx) + (dy * dy)); // Traveled distance

    float Size = m_player->GetObjectBoundingRadius();
    dist[1] = Size * 2;


    float x = m_MoveInfo[0].GetPos()->x;
    float y = m_MoveInfo[0].GetPos()->y;
    float z = m_MoveInfo[0].GetPos()->z;
    float o = m_MoveInfo[0].GetPos()->o;

    Map* pMap = m_player->GetMap();

    if (!pMap)
        return;

    // Forward
    float fx = x + cosf(o)*1;
    float fy = y + sinf(o)*1;
    floor_z[0] = pMap->GetHeight(fx, fy, z);

    // Backward
    float bx = x + cosf(o)*-1;
    float by = y + sinf(o)*-1;
    floor_z[1] = pMap->GetHeight(bx, by, z);
    
    deltaZ[0] = fabs(m_MoveInfo[0].GetPos()->z - m_MoveInfo[1].GetPos()->z);
    deltaZ[1] = fabs(floor_z[0] - floor_z[1]);

    for (uint8 i = 0; i < 2; i++)
        angle[i] = MapManager::NormalizeOrientation(tan(deltaZ[i] / dist[i]));

    if (angle[0] > 1.9f && angle[1] > 1.9f)
        HandleCheatReport("wallclimbing");
}

void AntiCheat::HandleCheatReport(const char* hack)
{
    if (m_player->isGameMaster())
        return;

    bool newhack = hack != m_LastHack;

    if (m_CheatReportTimer[0] <= 0 || newhack)
    {
        CharacterDatabase.PExecute("INSERT INTO cheaters (guid, account, type, time) VALUES (%u, %u, '%s', %u)", m_player->GetGUIDLow(), m_player->GetSession()->GetAccountId(), hack, sWorld.GetGameTime());
        m_CheatReportTimer[0] = 5000;
    }

    if (m_CheatReportTimer[1] <= 0 || newhack)
    {
        std::ostringstream ss;
        ss << "Player " << m_player->GetName() << " was caught " << hack;
        sCustom.SendGMMessage(ss.str());
        m_CheatReportTimer[1] = 1000;
        m_LastHack = hack;
    }
}
