#include "Player.h"
#include "Timer.h"
#include "World.h"

bool Player::IsFlying()
{
    return HasAuraType(SPELL_AURA_FLY) || m_GmFly;
}

bool Player::IsFalling(MovementInfo& MoveInfo)
{
        return  MoveInfo.HasMovementFlag(MOVEFLAG_FALLING) ||
                MoveInfo.HasMovementFlag(MOVEFLAG_FALLINGFAR) ||
                MoveInfo.HasMovementFlag(MOVEFLAG_SAFE_FALL) ||
                m_OldMoveInfo.HasMovementFlag(MOVEFLAG_FALLING) ||
                m_OldMoveInfo.HasMovementFlag(MOVEFLAG_FALLINGFAR) ||
                m_OldMoveInfo.HasMovementFlag(MOVEFLAG_SAFE_FALL);
}

bool Player::IsSwimming(MovementInfo& MoveInfo)
{
    return  MoveInfo.HasMovementFlag(MOVEFLAG_SWIMMING) ||
            m_OldMoveInfo.HasMovementFlag(MOVEFLAG_SWIMMING);
}

bool Player::IsRooted(MovementInfo& MoveInfo)
{
    return  MoveInfo.HasMovementFlag(MOVEFLAG_ROOT) ||
            m_OldMoveInfo.HasMovementFlag(MOVEFLAG_ROOT);
}

void Player::HandleMovementCheat(MovementInfo& MoveInfo, Opcodes opcode)
{
    bool SkipChecks = m_SkipAntiCheat;
    m_SkipAntiCheat = false;

    if (!SkipChecks && !GetTransport() && !IsTaxiFlying())
    {
        if (WorldTimer::getMSTimeDiff(GetOldMoveTime(), WorldTimer::getMSTime()) >= 500)
            HandleSpeedCheat(MoveInfo);

        HandleHeightCheat(MoveInfo, opcode);
        HandleClimbCheat(MoveInfo);
    }

    m_OldMoveInfo = MoveInfo;
    m_OldMoveTime = WorldTimer::getMSTime();
    m_LastOpcode = opcode;
}

void Player::HandleSpeedCheat(MovementInfo& MoveInfo)
{
    bool back = HasMovementFlag(MOVEFLAG_BACKWARD);

    float speed = 0;

    if (HasMovementFlag(MOVEFLAG_WALK_MODE))
        speed = GetSpeed(MOVE_WALK);
    else if (HasMovementFlag(MOVEFLAG_SWIMMING))
        speed = GetSpeed(back ? MOVE_SWIM_BACK : MOVE_SWIM);
    else if (HasMovementFlag(MOVEFLAG_FLYING))
        speed = GetSpeed(back ? MOVE_FLIGHT_BACK : MOVE_FLIGHT);
    else
        speed = GetSpeed(back ? MOVE_RUN_BACK : MOVE_RUN);


    float highspeed = std::max(m_OldMoveSpeed, speed);

    m_OldMoveSpeed = speed;

    float dx = m_OldMoveInfo.GetPos()->x - MoveInfo.GetPos()->x;
    float dy = m_OldMoveInfo.GetPos()->y - MoveInfo.GetPos()->y;
    float dist = sqrt((dx * dx) + (dy * dy)); // Traveled distance

    float mstime = WorldTimer::getMSTimeDiff(m_OldMoveTime, WorldTimer::getMSTime());

    float speedmod = mstime / 1000.f;

    float maxdist = highspeed * speedmod;

    const size_t listsize = 15;

    dist = floor(dist * 10.f) / 10.f;
    maxdist = ceil(maxdist * 10.f) / 10.f;

    if (maxdist / 2 > dist)
        dist = maxdist; // If player is standing still we do not subtract a lot.

    if (IsFalling(MoveInfo))
    {
        float deltaZ = m_OldMoveInfo.GetPos()->z - MoveInfo.GetPos()->z;
        float angle = MapManager::NormalizeOrientation(tan(deltaZ / dist));

        // Sliding the ground which can be really fast.
        if (angle > 1.9f)
            maxdist *= 3;
    }

    if (dist > maxdist)
        HandleCheatReport("speedhacking");
}

void Player::HandleHeightCheat(MovementInfo& MoveInfo, Opcodes opcode)
{
    if (!IsFlying() && (!IsFalling(MoveInfo) || opcode == MSG_MOVE_JUMP) && !IsRooted(MoveInfo) && !IsSwimming(MoveInfo))
    {
        float Size = GetObjectBoundingRadius();

        float o = GetOrientation();
        float x, y, z;
        GetPosition(x, y, z);

        float floor_z[5];

        // Forward
        float fx = x + cosf(o)*1;
        float fy = y + sinf(o)*1;
        floor_z[0] = GetMap()->GetHeight(fx, fy, z);

        // Backward
        float bx = x + cosf(o)*-1;
        float by = y + sinf(o)*-1;
        floor_z[1] = GetMap()->GetHeight(bx, by, z);

        // Right
        float rx = x + cos(o - (M_PI / 2))*1;
        float ry = y + sin(o - (M_PI / 2))*1;
        floor_z[2] = GetMap()->GetHeight(rx, ry, z);

        // Left
        float lx = x + cos(o - (M_PI / 2))*-1;
        float ly = y + sin(o - (M_PI / 2))*-1;
        floor_z[3] = GetMap()->GetHeight(lx, ly, z);

        // Current
        floor_z[4] = GetMap()->GetHeight(x, y, z);

        uint8 diffing = 0;
        for (uint8 i = 0; i < 5; i++)
            if (abs(z - floor_z[i]) > 1)
                ++diffing;

        if (diffing == 5 && getDeathState() == ALIVE)
            HandleCheatReport(opcode == MSG_MOVE_JUMP ? "jump hacking" : "height hacking");
    }
}

void Player::HandleClimbCheat(MovementInfo& MoveInfo)
{
    float dist[2];
    float deltaZ[2];
    float floor_z[2];
    float angle[2];

    float dx = m_OldMoveInfo.GetPos()->x - MoveInfo.GetPos()->x;
    float dy = m_OldMoveInfo.GetPos()->y - MoveInfo.GetPos()->y;
    dist[0] = sqrt((dx * dx) + (dy * dy)); // Traveled distance

    float Size = GetObjectBoundingRadius();
    dist[1] = Size * 2;

    float o = GetOrientation();
    float x, y, z;
    GetPosition(x, y, z);

    // Forward
    float fx = x + cosf(o)*1;
    float fy = y + sinf(o)*1;
    floor_z[0] = GetMap()->GetHeight(fx, fy, z);

    // Backward
    float bx = x + cosf(o)*-1;
    float by = y + sinf(o)*-1;
    floor_z[1] = GetMap()->GetHeight(bx, by, z);
    
    deltaZ[0] = fabs(MoveInfo.GetPos()->z - m_OldMoveInfo.GetPos()->z);
    deltaZ[1] = fabs(floor_z[0] - floor_z[1]);

    for (uint8 i = 0; i < 2; i++)
        angle[i] = MapManager::NormalizeOrientation(tan(deltaZ[i] / dist[i]));

    if (angle[0] > 1.9f && angle[1] > 1.9f && !IsFlying() && !IsFalling(MoveInfo) && !IsSwimming(MoveInfo))
        HandleCheatReport("wallclimbing");
}

void Player::HandleCheatReport(const char* hack)
{
    if (isGameMaster())
        return;

    bool newhack = hack != m_LastHack;

    if (m_CheatReportTimer[0] <= 0 || newhack)
    {
        CharacterDatabase.PExecute("INSERT INTO cheaters (guid, account, type, time) VALUES (%u, %u, '%s', %u)", GetGUIDLow(), GetSession()->GetAccountId(), hack, sWorld.GetGameTime());
        m_CheatReportTimer[0] = 5000;
    }

    if (m_CheatReportTimer[1] <= 0 || newhack)
    {
        std::ostringstream ss;
        ss << "Player " << GetName() << " was caught " << hack;
        sWorld.SendGMMessage(ss.str());
        m_CheatReportTimer[1] = 1000;
        m_LastHack = hack;
    }
}
