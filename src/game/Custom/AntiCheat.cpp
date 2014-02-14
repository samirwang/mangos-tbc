#include "Player.h"
#include "Timer.h"
#include "World.h"

void Player::HandleMovementCheat(MovementInfo& MoveInfo)
{
    bool SkipChecks = m_SkipAntiCheat;
    m_SkipAntiCheat = false;

    if (!SkipChecks)
    {
        HandleSpeedCheat(MoveInfo);
    }

    m_OldMoveInfo = MoveInfo;
    m_OldMoveTime = WorldTimer::getMSTime();
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

    if (!isGameMaster() && overtravel > 0 && m_OverTraveled.size() >= listsize)
    {
        m_SkipAntiCheat = true;
        m_OverTraveled.clear();

        CharacterDatabase.PExecute("INSERT INTO cheaters (guid, account, type, time) VALUES (%u, %u, 'Speed hack', %u)", GetGUIDLow(), GetSession()->GetAccountId(), sWorld.GetGameTime());

        std::ostringstream ss;
        ss << "Player " << GetName() << " was caught speedhacking";
        sWorld.SendGMMessage(ss.str());
    }
}