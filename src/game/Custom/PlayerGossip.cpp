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

#include "PlayerGossip.h"
#include "Log.h"

PlayerGossip::~PlayerGossip()
{
    for (auto& itr : m_ScriptContainer)
        delete itr.second;
}

void PlayerGossip::RegisterScript(uint32 sender, PlayerGossipScript* pScript)
{
    if (m_ScriptContainer.find(sender) == m_ScriptContainer.end())
        m_ScriptContainer.insert(std::make_pair(sender, pScript));
    else
        sLog.outError("PlayerGossip: Trying to register playergossip script %u which was laready registered!", sender);
}

bool PlayerGossip::GossipHello(Player* pPlayer, uint32 sender)
{
    auto itr = m_ScriptContainer.find(sender);

    if (itr != m_ScriptContainer.end())
    {
        itr->second->GossipHello(pPlayer, sender);
        return true;
    }
    else
    {
        sLog.outError("PlayerGossip: Player %s trying to call playergossip select script %u which is not registered!", pPlayer->GetName(), sender);
        return false;
    }
}

bool PlayerGossip::GossipSelect(Player* pPlayer, uint32 sender, uint32 action, std::string code)
{
    auto itr = m_ScriptContainer.find(sender);

    if (itr != m_ScriptContainer.end())
    {
        itr->second->GossipSelect(pPlayer, sender, action, code);
        return true;
    }
    else
    {
        sLog.outError("PlayerGossip: Player %s trying to call playergossip select script %u which is not registered!", pPlayer->GetName(), sender);
        return false;
    }
}