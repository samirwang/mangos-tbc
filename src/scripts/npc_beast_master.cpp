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

class npc_beast_master : public CreatureScript
{
public:
    npc_beast_master() : CreatureScript("npc_beast_master") {}

    bool GossipHello(Player *pPlayer, Creature *pCreature) override
    {
        CCPlayer* pCCPlayer = pPlayer->GetCCPlayer();

        if (pPlayer->isInCombat())
        {
            pCCPlayer->BoxChat << sCustom.ChatNameWrapper(pCreature->GetName()) << " You are in combat!" << std::endl;
            return true;
        }
        else if (pPlayer->getClass() != CLASS_HUNTER)
        {
            pCCPlayer->BoxChat << sCustom.ChatNameWrapper(pCreature->GetName()) << " You are no hunter!" << std::endl;
            return true;
        }

        pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Bat         ", GOSSIP_SENDER_MAIN, 1); // 16173
        pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Bear        ", GOSSIP_SENDER_MAIN, 2); // 23584
        pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Boar        ", GOSSIP_SENDER_MAIN, 3); // 21878
        pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Carrion Bird", GOSSIP_SENDER_MAIN, 4); // 18707
        pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Cat         ", GOSSIP_SENDER_MAIN, 5); // 20671
        pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Crab        ", GOSSIP_SENDER_MAIN, 6); // 18241
        pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Crocolisk   ", GOSSIP_SENDER_MAIN, 7); // 20773
        pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Dragonhawk  ", GOSSIP_SENDER_MAIN, 8); // 18155
        pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Gorilla     ", GOSSIP_SENDER_MAIN, 9); // 6585
        pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Hyena       ", GOSSIP_SENDER_MAIN, 10); // 8300
        pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Nether Ray  ", GOSSIP_SENDER_MAIN, 11); // 17731
        pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Raptor      ", GOSSIP_SENDER_MAIN, 12); // 20634
        pCCPlayer->AddGossipMenuItem(Icon::GEAR, "Go to Next Page          ", GOSSIP_SENDER_MAIN, 13);

        if (!pPlayer->PlayerTalkClass->GetGossipMenu().Empty())
            pPlayer->PlayerTalkClass->SendGossipMenu("Those beasts mean nothing to me!", pCreature->GetObjectGuid());
        return true;
    }

    bool GossipSelect(Player *pPlayer, Creature *pCreature, uint32 /*sender*/, uint32 action, std::string code) override
    {
        CCPlayer* pCCPlayer = pPlayer->GetCCPlayer();

        const char* cName = pCreature->GetName();
        pPlayer->PlayerTalkClass->CloseGossip();

        std::string tamedmsg = " You have tamed a ";

        if (pPlayer->isInCombat())
        {
            pCCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << " You are in combat!" << std::endl;
            return true;
        }
        else if (action == 0)
            GossipHello(pPlayer, pCreature);
        else if (action == 1) // Bat 16173
        {
            pCCPlayer->CreatePet(16173);

            if (pPlayer->GetPet())
            {
                pPlayer->GetPet()->learnSpell(27050); // Bite
                pPlayer->GetPet()->learnSpell(23148); // Dive
                pPlayer->GetPet()->learnSpell(27051); // Screech
                pCCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "bat" << std::endl;
            }
        }
        else if (action == 2) // Bear 23584
        {
            pCCPlayer->CreatePet(23584);

            if (pPlayer->GetPet())
            {
                pPlayer->GetPet()->learnSpell(27050); // Bite
                pPlayer->GetPet()->learnSpell(27049); // Claw
                pCCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "bear" << std::endl;
            }
        }
        else if (action == 3) // Boar 21878
        {
            pCCPlayer->CreatePet(21878);

            if (pPlayer->GetPet())
            {
                pPlayer->GetPet()->learnSpell(27050); // Bite
                pPlayer->GetPet()->learnSpell(27685); // Charge
                pPlayer->GetPet()->learnSpell(23110); // Dash
                pPlayer->GetPet()->learnSpell(35298); // Gore
                pCCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "boar" << std::endl;
            }
        }
        else if (action == 4) // Carrion Bird 18707
        {
            pCCPlayer->CreatePet(18707);

            if (pPlayer->GetPet())
            {
                pPlayer->GetPet()->learnSpell(27050); // Bite
                pPlayer->GetPet()->learnSpell(27049); // Claw
                pPlayer->GetPet()->learnSpell(23148); // Dive
                pPlayer->GetPet()->learnSpell(27051); // Screech
                pCCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "carrion bird" << std::endl;
            }
        }
        else if (action == 5) // Cat 20671
        {
            pCCPlayer->CreatePet(20671);

            if (pPlayer->GetPet())
            {
                pPlayer->GetPet()->learnSpell(27050); // Bite
                pPlayer->GetPet()->learnSpell(27049); // Claw
                pPlayer->GetPet()->learnSpell(23110); // Dash
                pPlayer->GetPet()->learnSpell(24453); // Prowl
                pCCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "cat" << std::endl;
            }
        }
        else if (action == 6) // Crab 18241
        {
            pCCPlayer->CreatePet(18241);

            if (pPlayer->GetPet())
            {
                pPlayer->GetPet()->learnSpell(27049); // Claw
                pCCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "crab" << std::endl;
            }
        }
        else if (action == 7) // Crocolisk 20773
        {
            pCCPlayer->CreatePet(20773);

            if (pPlayer->GetPet())
            {
                pPlayer->GetPet()->learnSpell(27050); // Bite
                pCCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "crocolisk" << std::endl;
            }
        }
        else if (action == 8) // Dragonhawk 18155
        {
            pCCPlayer->CreatePet(18155);

            if (pPlayer->GetPet())
            {
                pPlayer->GetPet()->learnSpell(27050); // Bite
                pPlayer->GetPet()->learnSpell(23148); // Dive
                pPlayer->GetPet()->learnSpell(35323); // Fire Breath
                pCCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "dragonhawk" << std::endl;
            }
        }
        else if (action == 9) // Gorilla 6585
        {
            pCCPlayer->CreatePet(6585);

            if (pPlayer->GetPet())
            {
                pPlayer->GetPet()->learnSpell(27050); // Bite
                pPlayer->GetPet()->learnSpell(27063); // Thunderstomp
                pCCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "gorilla" << std::endl;
            }
        }
        else if (action == 10) // Hyena 8300
        {
            pCCPlayer->CreatePet(8300);

            if (pPlayer->GetPet())
            {
                pPlayer->GetPet()->learnSpell(27050); // Bite
                pPlayer->GetPet()->learnSpell(23110); // Dash
                pCCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "hyena" << std::endl;
            }
        }
        else if (action == 11) // Nether Ray 17731
        {
            pCCPlayer->CreatePet(17731);

            if (pPlayer->GetPet())
            {
                pPlayer->GetPet()->learnSpell(27050); // Bite
                pPlayer->GetPet()->learnSpell(23148); // Dive
                pCCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "nether ray" << std::endl;
            }
        }
        else if (action == 12) // Raptor 20634
        {
            pCCPlayer->CreatePet(20634);

            if (pPlayer->GetPet())
            {
                pPlayer->GetPet()->learnSpell(27050); // Bite
                pPlayer->GetPet()->learnSpell(27049); // Claw
                pPlayer->GetPet()->learnSpell(23110); // Dash
                pCCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "raptor" << std::endl;
            }
        }
        else if (action == 13)
        {
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Ravager     ", GOSSIP_SENDER_MAIN, 14); // 23326
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Scorpid     ", GOSSIP_SENDER_MAIN, 15); // 21864
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Serpent     ", GOSSIP_SENDER_MAIN, 16); // 19784
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Spider      ", GOSSIP_SENDER_MAIN, 17); // 16170
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Tallstrider ", GOSSIP_SENDER_MAIN, 18); // 22807
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Turtle      ", GOSSIP_SENDER_MAIN, 19); // 5431
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Warp Stalker", GOSSIP_SENDER_MAIN, 20); // 23219
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Wind Serpent", GOSSIP_SENDER_MAIN, 21); // 20673
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Wolf        ", GOSSIP_SENDER_MAIN, 22); // 17280
            pCCPlayer->AddGossipMenuItem(Icon::GEAR, "Go to Previous Page      ", GOSSIP_SENDER_MAIN, 0);
        }
        else if (action == 14) // Ravager 23326
        {
            pCCPlayer->CreatePet(23326);

            if (pPlayer->GetPet())
            {
                pPlayer->GetPet()->learnSpell(27050); // Bite
                pPlayer->GetPet()->learnSpell(23110); // Dash
                pPlayer->GetPet()->learnSpell(35298); // Gore
                pCCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "ravager" << std::endl;
            }
        }
        else if (action == 15) // Scorpid 21864
        {
            pCCPlayer->CreatePet(21864);

            if (pPlayer->GetPet())
            {
                pPlayer->GetPet()->learnSpell(27049); // Claw
                pPlayer->GetPet()->learnSpell(27060); // Scorpid Poision
                pCCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "scorpid" << std::endl;
            }
        }
        else if (action == 16) // Serpent 19784
        {
            pCCPlayer->CreatePet(19784);

            if (pPlayer->GetPet())
            {
                pPlayer->GetPet()->learnSpell(27050); // Bite
                pPlayer->GetPet()->learnSpell(35392); // Poision Spit
                pCCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "serpent" << std::endl;
            }
        }
        else if (action == 17) // Spider 16170
        {
            pCCPlayer->CreatePet(16170);

            if (pPlayer->GetPet())
            {
                pPlayer->GetPet()->learnSpell(27050); // Bite
                pPlayer->GetPet()->learnSpell(4167);  // Web
                pCCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "spider" << std::endl;
            }
        }
        else if (action == 18) // Tallstrider 22807
        {
            pCCPlayer->CreatePet(22807);

            if (pPlayer->GetPet())
            {
                pPlayer->GetPet()->learnSpell(27050); // Bite
                pPlayer->GetPet()->learnSpell(23110); // Dash
                pCCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "tallstrider" << std::endl;
            }
        }
        else if (action == 19) // Turtle 5431
        {
            pCCPlayer->CreatePet(5431);

            if (pPlayer->GetPet())
            {
                pPlayer->GetPet()->learnSpell(27050); // Bite
                pPlayer->GetPet()->learnSpell(26064); // Spell Shield
                pCCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "turtle" << std::endl;
            }
        }
        else if (action == 20) // Warp Stalker 23219
        {
            pCCPlayer->CreatePet(23219);

            if (pPlayer->GetPet())
            {
                pPlayer->GetPet()->learnSpell(27050); // Bite
                pPlayer->GetPet()->learnSpell(27049); // Claw
                pPlayer->GetPet()->learnSpell(35346); // Warp
                pCCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "warp stalker" << std::endl;
            }
        }
        else if (action == 21) // Wind Serpent 20673
        {
            pCCPlayer->CreatePet(20673);

            if (pPlayer->GetPet())
            {
                pPlayer->GetPet()->learnSpell(27050); // Bite
                pPlayer->GetPet()->learnSpell(23148); // Dive
                pPlayer->GetPet()->learnSpell(25012); // Lightning Breath
                pCCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "wind serpent" << std::endl;
            }
        }
        else if (action == 22) // Wolf 17280
        {
            pCCPlayer->CreatePet(17280);

            if (pPlayer->GetPet())
            {
                pPlayer->GetPet()->learnSpell(27050); // Bite
                pPlayer->GetPet()->learnSpell(23110); // Dash
                pPlayer->GetPet()->learnSpell(27685); // Charge
                pPlayer->GetPet()->learnSpell(24604); // Furious Howl
                pCCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "wolf" << std::endl;
            }
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