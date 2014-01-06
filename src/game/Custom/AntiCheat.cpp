#include "Player.h"
#include "Timer.h"

void Player::HandleMovementCheat(MovementInfo& MoveInfo)
{
    if (!isGameMaster() && !m_SkipAntiCheat)
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

    dist = (floor(dist * 10.f) / 10.f); // Round to lowest 1/10
    maxdist = (ceil(maxdist * 10.f) / 10.f); // Round to highest 1/10 and add 0.1 for failsafe

    const size_t listsize = 15;
    const float detectpercent = 0.5f;

    m_SpeedHackTicks.push_back(dist > maxdist);

    while (m_SpeedHackTicks.size() > listsize)
        m_SpeedHackTicks.pop_front();

    float count = std::count(m_SpeedHackTicks.begin(), m_SpeedHackTicks.end(), true);

    // BothChat << maxdist << " " << dist << " " << mstime << " " << (dist > maxdist ? "true" : "false") << " " << float(float(count) / float(m_SpeedHackTicks.size()));

    if (m_SpeedHackTicks.size() >= listsize && count / float(m_SpeedHackTicks.size()) >= detectpercent)
        BothChat << "You was detected speedhacking";
}