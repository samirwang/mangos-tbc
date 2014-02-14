#include "Player.h"
#include "Timer.h"

void Player::HandleMovementCheat(MovementInfo& MoveInfo)
{
    if (!m_SkipAntiCheat)
    {
        HandleSpeedCheat(MoveInfo);
    }

    m_SkipAntiCheat = false;
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
    float dist = sqrt((dx * dx) + (dy * dy));

    float mstime = WorldTimer::getMSTimeDiff(m_OldMoveTime, WorldTimer::getMSTime());
    if (!mstime)
        mstime = 1;

    float speedmod = mstime / 1000.f;

    float maxdist = highspeed * speedmod;

    const size_t listsize = 15;

    if (maxdist / 2 > dist)
        dist = maxdist;

    m_OverTraveled.push_back(dist - maxdist);

    while (m_OverTraveled.size() > listsize)
        m_OverTraveled.pop_front();

    float overtravel = 0;

    for (AntiCheatTicks::const_iterator itr = m_OverTraveled.begin(); itr != m_OverTraveled.end(); ++itr)
        overtravel += *itr;

    m_DistTraveled.push_back(dist);

    while (m_DistTraveled.size() > listsize)
        m_DistTraveled.pop_front();

    float totaltravel = 0;

    for (AntiCheatTicks::const_iterator itr = m_DistTraveled.begin(); itr != m_DistTraveled.end(); ++itr)
        totaltravel += *itr;

    if (isGameMaster())
        BothChat << maxdist << " " << dist << " " << mstime << " " << (dist > maxdist ? "true" : "false") << " " << overtravel << " " << totaltravel;
}