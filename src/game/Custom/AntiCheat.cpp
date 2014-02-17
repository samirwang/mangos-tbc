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
        {
            HandleSpeedCheat(MoveInfo);
            HandleHeightCheat(MoveInfo);
            HandleClimbCheat(MoveInfo);
        }
        HandleJumpCheat(MoveInfo, opcode);
    }

    m_OldMoveInfo = MoveInfo;
    m_OldMoveTime = WorldTimer::getMSTime();
    m_LastOpcode = opcode;
}

void Player::HandleSpeedCheat(MovementInfo& MoveInfo)
{
    float speed = GetSpeed(MOVE_RUN);

    if (HasMovementFlag(MOVEFLAG_WALK_MODE))
        speed = GetSpeed(MOVE_WALK);
    else if (HasMovementFlag(MOVEFLAG_SWIMMING))
        speed = GetSpeed(MOVE_SWIM);
    else if (HasMovementFlag(MOVEFLAG_FLYING))
        speed = GetSpeed(MOVE_FLIGHT);

    float highspeed = m_OldMoveSpeed > speed ? m_OldMoveSpeed : speed;

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

    m_OverTraveled.push_back(dist - maxdist);

    while (m_OverTraveled.size() > listsize)
        m_OverTraveled.pop_front();

    float overtravel = 0;

    for (AntiCheatTicks::const_iterator itr = m_OverTraveled.begin(); itr != m_OverTraveled.end(); ++itr)
        overtravel += *itr;

    if (isGameMaster())
        BothChat << maxdist << " " << dist << " " << dist - maxdist << " " << overtravel;

    if (overtravel > 0 && m_OverTraveled.size() >= listsize)
    {
        m_SkipAntiCheat = true;
        m_OverTraveled.clear();

        HandleCheatReport("speedhacking");
    }
}

void Player::HandleHeightCheat(MovementInfo& MoveInfo)
{
    if (!IsFlying() && !IsFalling(MoveInfo) && !IsRooted(MoveInfo))
    {
        float floor_z = GetMap()->GetHeight(GetPositionX(), GetPositionY(), GetPositionZ());

        float diff = abs(floor_z - MoveInfo.GetPos()->z);

        if (diff > 1.5f && getDeathState() == ALIVE)
            HandleCheatReport(floor_z < MoveInfo.GetPos()->z ? "flyhacking" : "planehacking");
    }
}

void Player::HandleClimbCheat(MovementInfo& MoveInfo)
{
    float dx = m_OldMoveInfo.GetPos()->x - MoveInfo.GetPos()->x;
    float dy = m_OldMoveInfo.GetPos()->y - MoveInfo.GetPos()->y;
    float dist = sqrt((dx * dx) + (dy * dy)); // Traveled distance

    float deltaZ = fabs(MoveInfo.GetPos()->z - m_OldMoveInfo.GetPos()->z);

    float angle = MapManager::NormalizeOrientation(tan(deltaZ / dist));

    if (angle > 1.9f && dist > 0.1f && !IsFlying() && !IsFalling(MoveInfo) && !IsSwimming(MoveInfo))
        HandleCheatReport("wallclimbing");
}

void Player::HandleJumpCheat(MovementInfo& MoveInfo, Opcodes opcode)
{
    if (opcode == MSG_MOVE_JUMP)
    {
        float floor_z = GetMap()->GetHeight(GetPositionX(), GetPositionY(), GetPositionZ());

        float diff = abs(floor_z - MoveInfo.GetPos()->z);

        if (diff > 1.5f && getDeathState() == ALIVE)
            HandleCheatReport("jumphacking");
    }
}

void Player::HandleCheatReport(const char* hack)
{
    if (isGameMaster())
        return;

    if (m_CheatDatabaseReportTimer <= 0)
    {
        CharacterDatabase.PExecute("INSERT INTO cheaters (guid, account, type, time) VALUES (%u, %u, '%s', %u)", GetGUIDLow(), GetSession()->GetAccountId(), hack, sWorld.GetGameTime());
        m_CheatDatabaseReportTimer = 5000;
    }

    std::ostringstream ss;
    ss << "Player " << GetName() << " was caught " << hack;
    sWorld.SendGMMessage(ss.str());
}
