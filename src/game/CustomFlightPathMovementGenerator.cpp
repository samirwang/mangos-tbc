/*
 * This file is part of the CMaNGOS Project. See AUTHORS file for Copyright information
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

#include "CustomFlightPathMovementGenerator.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "Creature.h"
#include "AI/CreatureAI.h"
#include "WaypointManager.h"
#include "ScriptMgr.h"
#include "movement/MoveSplineInit.h"
#include "movement/MoveSpline.h"
#include "CPlayer.h"

#include <cassert>

void CustomFlightPathMovementGenerator::Initialize(Player& player)
{
    Reset(player);
}

void CustomFlightPathMovementGenerator::Finalize(Player& player)
{
    // remove flag to prevent send object build movement packets for flight state and crash (movement generator already not at top of stack)
    player.clearUnitState(UNIT_STAT_TAXI_FLIGHT);

    player.Unmount();
    player.RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_TAXI_FLIGHT);
    player.SetClientControl(&player, 1);

    if (player.m_taxi.empty())
    {
        player.getHostileRefManager().setOnlineOfflineState(true);
        if (player.pvpInfo.inHostileArea)
            player.CastSpell(&player, 2479, TRIGGERED_OLD_TRIGGERED);

        // update z position to ground and orientation for landing point
        // this prevent cheating with landing  point at lags
        // when client side flight end early in comparison server side
        player.StopMoving(true);
    }
}

void CustomFlightPathMovementGenerator::Interrupt(Player& player)
{
    player.clearUnitState(UNIT_STAT_TAXI_FLIGHT);
}

#define PLAYER_FLIGHT_SPEED        10.0f

void CustomFlightPathMovementGenerator::Reset(Player& player)
{
    player.Mount(22724);
    player.getHostileRefManager().setOnlineOfflineState(false);
    player.addUnitState(UNIT_STAT_TAXI_FLIGHT);
    player.SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_TAXI_FLIGHT);
    player.SetClientControl(&player, 0);

    Movement::MoveSplineInit init(player);

    for (auto& i : m_Nodes)
        init.Path().push_back(i.pos);

    init.SetFly();
    init.SetVelocity(PLAYER_FLIGHT_SPEED);
    init.Launch();
}

bool CustomFlightPathMovementGenerator::Update(Player& player, const uint32& /*diff*/)
{
    return !player.movespline->Finalized();
}
