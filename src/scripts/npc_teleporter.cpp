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

#include "Precompiled.h"
#include "ArenaTeam.h"
#include "CPlusMgr.h"
#include "CPlayer.h"

enum
{
    CAT_MENU,
    CAT_MISC,
    CAT_TELEDB,
    CAT_TELECOORD,
    CAT_MULTIVENDOR,
};

class NPC_Teleporter : public CreatureScript
{
public:
    NPC_Teleporter() : CreatureScript("npc_teleporter") {}

    /*
    Position 0 = category

    If category = CAT_MENU
    Position 1 = menuID
    If category = CAT_MISC
    Position 1 = actionID
    If category = CAT_TELEDB
    Position 1 = teleID
    If category = CAT_TELECOORD
    Position 1 = mapID
    Position 2 = coordX
    Position 3 = coordY
    Position 4 = coordZ
    Position 5 = coordO
    If category = CAT_MULTIVENDOR
    Position 1 = creatureID
    */

    bool GossipHello(Player* pPlayer, Creature* pCreature) override
    {
        CPlayer* pCPlayer = pPlayer->ToCPlayer();

        if (pPlayer->isInCombat())
        {
            pCPlayer->BoxChat << sCustom.ChatNameWrapper(pCreature->GetName()) << " You are in combat!" << std::endl;
            return true;
        }

        // Add menu stuff here.

        if (!pPlayer->PlayerTalkClass->GetGossipMenu().Empty())
            pPlayer->PlayerTalkClass->SendGossipMenu("Your wish, my law!", pCreature->GetObjectGuid());
        return true;
    }

    bool GossipSelect(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action, std::string code) override
    {
        CPlayer* pCPlayer = pPlayer->ToCPlayer();

        const char* cName = pCreature->GetName();
        pPlayer->PlayerTalkClass->CloseGossip();

        if (pPlayer->isInCombat())
        {
            pCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << " You are in combat!" << std::endl;
            return true;
        }

        // Add actions here

        if (!pPlayer->PlayerTalkClass->GetGossipMenu().Empty())
            pPlayer->PlayerTalkClass->SendGossipMenu("Your wish, my law!", pCreature->GetObjectGuid());

        return true;
    }
};

void AddSC_npc_teleporter()
{
    new NPC_Teleporter;
}