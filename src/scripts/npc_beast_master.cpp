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
#include "CPlayer.h"

enum
{
    CAT_MENU,
    CAT_TAME,
};

class npc_beast_master : public CreatureScript
{
public:
    npc_beast_master() : CreatureScript("npc_beast_master") {}

    /*
    Position 0 = category

    If category = CAT_MENU:
    Position 1 = menu ID
    If category = CAT_TAME
    Position 1 = creature ID
    */

    bool GossipHello(Player *pPlayer, Creature *pCreature) override
    {
        CPlayer* pCPlayer = pPlayer->ToCPlayer();

        if (pPlayer->isInCombat())
        {
            pCPlayer->BoxChat << sCustom.ChatNameWrapper(pCreature->GetName()) << " You are in combat!" << std::endl;
            return true;
        }
        else if (pPlayer->getClass() != CLASS_HUNTER)
        {
            pCPlayer->BoxChat << sCustom.ChatNameWrapper(pCreature->GetName()) << " You are no hunter!" << std::endl;
            return true;
        }

        pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Bat", { CAT_TAME, 16173 });
        pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Bear", { CAT_TAME, 23584 });
        pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Boar", { CAT_TAME, 21878 });
        pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Carrion Bird", { CAT_TAME, 18707 });
        pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Cat", { CAT_TAME, 20671 });
        pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Crab", { CAT_TAME, 18241 });
        pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Crocolisk", { CAT_TAME, 20773 });
        pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Dragonhawk", { CAT_TAME, 18155 });
        pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Gorilla", { CAT_TAME, 6585 });
        pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Hyena", { CAT_TAME, 8300 });
        pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Nether Ray", { CAT_TAME, 17731 });
        pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Raptor", { CAT_TAME, 20634 });
        pCPlayer->AddGossipMenuItem(Icon::GEAR, "Go to Next Page", { CAT_MENU, 2 });

        if (!pPlayer->PlayerTalkClass->GetGossipMenu().Empty())
            pPlayer->PlayerTalkClass->SendGossipMenu("Those beasts mean nothing to me!", pCreature->GetObjectGuid());
        return true;
    }

    bool GossipSelect(Player *pPlayer, Creature *pCreature, GossipActionList actionlist, std::string code) override
    {
        CPlayer* pCPlayer = pPlayer->ToCPlayer();
        const char* cName = pCreature->GetName();

        if (pPlayer->isInCombat())
        {
            pCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << " You are in combat!" << std::endl;
            return true;
        }

        uint32 category = actionlist[0];

        if (category == CAT_MENU)
        {
            uint32 menuID = actionlist[1];

            if (menuID == 1)
                GossipHello(pPlayer, pCreature);
            else if (menuID == 2)
            {
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Ravager", { CAT_TAME, 23326 }); // 23326
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Scorpid", { CAT_TAME, 21864 }); // 21864
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Serpent", { CAT_TAME, 19784 }); // 19784
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Spider", { CAT_TAME, 16170 }); // 16170
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Tallstrider", { CAT_TAME, 22807 }); // 22807
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Turtle", { CAT_TAME, 5431 }); // 5431
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Warp Stalker", { CAT_TAME, 23219 }); // 23219
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Wind Serpent", { CAT_TAME, 20673 }); // 20673
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Wolf", { CAT_TAME, 17280 }); // 17280
                pCPlayer->AddGossipMenuItem(Icon::GEAR, "Go to Previous Page", { CAT_MENU, 1 });
            }
        }
        else if (category == CAT_TAME)
        {
            uint32 creatureID = actionlist[1];

            pCPlayer->CreatePet(creatureID);

            if (auto pPet = pCPlayer->GetPet())
                pCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << "You have tamed a " << pPet->GetName() << std::endl;
        }

        if (!pPlayer->PlayerTalkClass->GetGossipMenu().Empty())
            pPlayer->PlayerTalkClass->SendGossipMenu("Those beasts mean nothing to me!", pCreature->GetObjectGuid());

        return true;
    }
};

void AddSC_beast_master()
{
    new npc_beast_master;
}