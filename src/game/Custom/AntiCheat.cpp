#include "Player.h"
#include "Timer.h"
#include "World.h"
#include "AntiCheat.h"

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
    m_Lag = 0;

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
    {
        if (m_MoveInfo[i].HasMovementFlag(MOVEFLAG_FALLING) ||
            m_MoveInfo[i].HasMovementFlag(MOVEFLAG_FALLINGFAR) ||
            m_MoveInfo[i].HasMovementFlag(MOVEFLAG_SAFE_FALL))

            return true;
    }

    return false;
}

bool AntiCheat::IsSwimming()
{
    for (uint8 i = 0; i < 2; i++)
    {
        if (m_MoveInfo[0].HasMovementFlag(MOVEFLAG_SWIMMING) ||
            m_MoveInfo[1].HasMovementFlag(MOVEFLAG_SWIMMING))
            return true;
    }

    return false;
}

bool AntiCheat::IsRooted()
{
    for (uint8 i = 0; i < 2; i++)
    {
        if (m_MoveInfo[0].HasMovementFlag(MOVEFLAG_ROOT) ||
            m_MoveInfo[1].HasMovementFlag(MOVEFLAG_ROOT))
            return true;
    }

    return false;
}

void AntiCheat::HandleMovementCheat(MovementInfo& MoveInfo, Opcodes opcode, int32 lag)
{
    bool SkipChecks = m_SkipAntiCheat;
    m_SkipAntiCheat = false;

    m_MoveInfo[0] = MoveInfo;
    m_Opcode[0] = opcode;

    if (!SkipChecks && !m_player->GetTransport() && !m_player->IsTaxiFlying())
    {
        if ((m_Opcode[0] == MSG_MOVE_HEARTBEAT && WorldTimer::getMSTimeDiff(GetOldMoveTime(), WorldTimer::getMSTime()) >= 100) ||
            WorldTimer::getMSTimeDiff(GetOldMoveTime(), WorldTimer::getMSTime()) >= 1000)
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

    float mstime = WorldTimer::getMSTimeDiff(m_OldMoveTime, WorldTimer::getMSTime()) + m_Lag;

    float speedmod = mstime / 1000;

    float maxdist = highspeed * speedmod;

    const size_t listsize = 15;

    dist = floor(dist * 10.f) / 10.f;
    maxdist = ceil(maxdist * 10.f) / 10.f;

    if (maxdist / 2 > dist)
        dist = maxdist; // If player is standing still we do not subtract a lot.

    if (IsFalling())
    {
        float deltaZ = m_MoveInfo[1].GetPos()->z - m_MoveInfo[0].GetPos()->z;
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
    if (!IsFlying() && (!IsFalling() || m_Opcode[0] == MSG_MOVE_JUMP) && !IsRooted() && !IsSwimming())
    {
        float Size = m_player->GetObjectBoundingRadius();

        float x = m_MoveInfo[0].GetPos()->x;
        float y = m_MoveInfo[0].GetPos()->y;
        float z = m_MoveInfo[0].GetPos()->z;
        float o = m_MoveInfo[0].GetPos()->o;

        Map* pMap = m_player->GetMap();

        if (!pMap)
            return;

        float floor_z[5];

        // Forward
        float fx = x + cosf(o)*1;
        float fy = y + sinf(o)*1;
        floor_z[0] = pMap->GetHeight(fx, fy, z);

        // Backward
        float bx = x + cosf(o)*-1;
        float by = y + sinf(o)*-1;
        floor_z[1] = pMap->GetHeight(bx, by, z);

        // Right
        float rx = x + cos(o - (M_PI / 2))*1;
        float ry = y + sin(o - (M_PI / 2))*1;
        floor_z[2] = pMap->GetHeight(rx, ry, z);

        // Left
        float lx = x + cos(o - (M_PI / 2))*-1;
        float ly = y + sin(o - (M_PI / 2))*-1;
        floor_z[3] = pMap->GetHeight(lx, ly, z);

        // Current
        floor_z[4] = pMap->GetHeight(x, y, z);

        uint8 diffing = 0;
        for (uint8 i = 0; i < 5; i++)
            if (abs(z - floor_z[i]) > 1)
                ++diffing;

        if (diffing == 5 && m_player->getDeathState() == ALIVE)
            HandleCheatReport(m_Opcode[0] == MSG_MOVE_JUMP ? "jump hacking" : "height hacking");
    }
}

void AntiCheat::HandleClimbCheat()
{
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

    if (angle[0] > 1.9f && angle[1] > 1.9f && !IsFlying() && !IsFalling() && !IsSwimming())
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
        sWorld.SendGMMessage(ss.str());
        m_CheatReportTimer[1] = 1000;
        m_LastHack = hack;
    }
}
