#include "Precompiled.h"

bool GossipHello_beast_master(Player *pPlayer, Creature *pCreature)
{
    if (pPlayer->isInCombat())
    {
        pPlayer->BoxChat << sCustom.ChatNameWrapper(pCreature->GetName()) << " You are in combat!\n";
        return true;
    }
    else if (pPlayer->getClass() != CLASS_HUNTER)
    {
        pPlayer->BoxChat << sCustom.ChatNameWrapper(pCreature->GetName()) << " You are no hunter!\n";
        return true;
    }

    pPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Bat         ", GOSSIP_SENDER_MAIN, 1 ); // 16173
    pPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Bear        ", GOSSIP_SENDER_MAIN, 2 ); // 23584
    pPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Boar        ", GOSSIP_SENDER_MAIN, 3 ); // 21878
    pPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Carrion Bird", GOSSIP_SENDER_MAIN, 4 ); // 18707
    pPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Cat         ", GOSSIP_SENDER_MAIN, 5 ); // 20671
    pPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Crab        ", GOSSIP_SENDER_MAIN, 6 ); // 18241
    pPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Crocolisk   ", GOSSIP_SENDER_MAIN, 7 ); // 20773
    pPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Dragonhawk  ", GOSSIP_SENDER_MAIN, 8 ); // 18155
    pPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Gorilla     ", GOSSIP_SENDER_MAIN, 9 ); // 6585
    pPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Hyena       ", GOSSIP_SENDER_MAIN, 10); // 8300
    pPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Nether Ray  ", GOSSIP_SENDER_MAIN, 11); // 17731
    pPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Raptor      ", GOSSIP_SENDER_MAIN, 12); // 20634
    pPlayer->AddGossipMenuItem(Icon::GEAR, "Go to Next Page          ", GOSSIP_SENDER_MAIN, 13);
    pPlayer->PlayerTalkClass->SendGossipMenu(1,pCreature->GetObjectGuid());
    return true;
}

bool GossipSelect_beast_master(Player *pPlayer, Creature *pCreature, uint32 /*sender*/, uint32 action)
{
    const char* cName = pCreature->GetName();
    pPlayer->PlayerTalkClass->CloseGossip();

    std::string tamedmsg = " You have tamed a ";

    if (pPlayer->isInCombat())
    {
        pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << " You are in combat!\n";
        return true;
    }
    else if (action == 0)
        GossipHello_beast_master(pPlayer,pCreature);
    else if (action == 1) // Bat 16173
    {
        pPlayer->CreatePet(16173);

        if (pPlayer->GetPet())
        {
            pPlayer->GetPet()->learnSpell(27050); // Bite
            pPlayer->GetPet()->learnSpell(23148); // Dive
            pPlayer->GetPet()->learnSpell(27051); // Screech
            pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "bat\n";
        }
    }
    else if (action == 2) // Bear 23584
    {
        pPlayer->CreatePet(23584);

        if (pPlayer->GetPet())
        {
            pPlayer->GetPet()->learnSpell(27050); // Bite
            pPlayer->GetPet()->learnSpell(27049); // Claw
            pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "bear\n";
        }
    }
    else if (action == 3) // Boar 21878
    {
        pPlayer->CreatePet(21878);

        if (pPlayer->GetPet())
        {
            pPlayer->GetPet()->learnSpell(27050); // Bite
            pPlayer->GetPet()->learnSpell(27685); // Charge
            pPlayer->GetPet()->learnSpell(23110); // Dash
            pPlayer->GetPet()->learnSpell(35298); // Gore
            pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "boar\n";
        }
    }
    else if (action == 4) // Carrion Bird 18707
    {
        pPlayer->CreatePet(18707);

        if (pPlayer->GetPet())
        {
            pPlayer->GetPet()->learnSpell(27050); // Bite
            pPlayer->GetPet()->learnSpell(27049); // Claw
            pPlayer->GetPet()->learnSpell(23148); // Dive
            pPlayer->GetPet()->learnSpell(27051); // Screech
            pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "carrion bird\n";
        }
    }
    else if (action == 5) // Cat 20671
    {
        pPlayer->CreatePet(20671);

        if (pPlayer->GetPet())
        {
            pPlayer->GetPet()->learnSpell(27050); // Bite
            pPlayer->GetPet()->learnSpell(27049); // Claw
            pPlayer->GetPet()->learnSpell(23110); // Dash
            pPlayer->GetPet()->learnSpell(24453); // Prowl
            pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "cat\n";
        }
    }
    else if (action == 6) // Crab 18241
    {
        pPlayer->CreatePet(18241);

        if (pPlayer->GetPet())
        {
            pPlayer->GetPet()->learnSpell(27049); // Claw
            pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "crab\n";
        }
    }
    else if (action == 7) // Crocolisk 20773
    {
        pPlayer->CreatePet(20773);

        if (pPlayer->GetPet())
        {
            pPlayer->GetPet()->learnSpell(27050); // Bite
            pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "crocolisk\n";
        }
    }
    else if (action == 8) // Dragonhawk 18155
    {
        pPlayer->CreatePet(18155);

        if (pPlayer->GetPet())
        {
            pPlayer->GetPet()->learnSpell(27050); // Bite
            pPlayer->GetPet()->learnSpell(23148); // Dive
            pPlayer->GetPet()->learnSpell(35323); // Fire Breath
            pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "dragonhawk\n";
        }
    }
    else if (action == 9) // Gorilla 6585
    {
        pPlayer->CreatePet(6585);

        if (pPlayer->GetPet())
        {
            pPlayer->GetPet()->learnSpell(27050); // Bite
            pPlayer->GetPet()->learnSpell(27063); // Thunderstomp
            pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "gorilla\n";
        }
    }
    else if (action == 10) // Hyena 8300
    {
        pPlayer->CreatePet(8300);

        if (pPlayer->GetPet())
        {
            pPlayer->GetPet()->learnSpell(27050); // Bite
            pPlayer->GetPet()->learnSpell(23110); // Dash
            pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "hyena\n";
        }
    }
    else if (action == 11) // Nether Ray 17731
    {
        pPlayer->CreatePet(17731);

        if (pPlayer->GetPet())
        {
            pPlayer->GetPet()->learnSpell(27050); // Bite
            pPlayer->GetPet()->learnSpell(23148); // Dive
            pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "nether ray\n";
        }
    }
    else if (action == 12) // Raptor 20634
    {
        pPlayer->CreatePet(20634);

        if (pPlayer->GetPet())
        {
            pPlayer->GetPet()->learnSpell(27050); // Bite
            pPlayer->GetPet()->learnSpell(27049); // Claw
            pPlayer->GetPet()->learnSpell(23110); // Dash
            pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "raptor\n";
        }
    }
    else if (action == 13)
    {
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Ravager     ", GOSSIP_SENDER_MAIN, 14); // 23326
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Scorpid     ", GOSSIP_SENDER_MAIN, 15); // 21864
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Serpent     ", GOSSIP_SENDER_MAIN, 16); // 19784
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Spider      ", GOSSIP_SENDER_MAIN, 17); // 16170
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Tallstrider ", GOSSIP_SENDER_MAIN, 18); // 22807
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Turtle      ", GOSSIP_SENDER_MAIN, 19); // 5431
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Warp Stalker", GOSSIP_SENDER_MAIN, 20); // 23219
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Wind Serpent", GOSSIP_SENDER_MAIN, 21); // 20673
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Summon me a: Wolf        ", GOSSIP_SENDER_MAIN, 22); // 17280
        pPlayer->AddGossipMenuItem(Icon::GEAR, "Go to Previous Page      ", GOSSIP_SENDER_MAIN, 0 );
        pPlayer->PlayerTalkClass->SendGossipMenu(1,pCreature->GetObjectGuid());
    }
    else if (action == 14) // Ravager 23326
    {
        pPlayer->CreatePet(23326);

        if (pPlayer->GetPet())
        {
            pPlayer->GetPet()->learnSpell(27050); // Bite
            pPlayer->GetPet()->learnSpell(23110); // Dash
            pPlayer->GetPet()->learnSpell(35298); // Gore
            pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "ravager\n";
        }
    }
    else if (action == 15) // Scorpid 21864
    {
        pPlayer->CreatePet(21864);

        if (pPlayer->GetPet())
        {
            pPlayer->GetPet()->learnSpell(27049); // Claw
            pPlayer->GetPet()->learnSpell(27060); // Scorpid Poision
            pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "scorpid\n";
        }
    }
    else if (action == 16) // Serpent 19784
    {
        pPlayer->CreatePet(19784);

        if (pPlayer->GetPet())
        {
            pPlayer->GetPet()->learnSpell(27050); // Bite
            pPlayer->GetPet()->learnSpell(35392); // Poision Spit
            pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "serpent\n";
        }
    }
    else if (action == 17) // Spider 16170
    {
        pPlayer->CreatePet(16170);

        if (pPlayer->GetPet())
        {
            pPlayer->GetPet()->learnSpell(27050); // Bite
            pPlayer->GetPet()->learnSpell(4167);  // Web
            pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "spider\n";
        }
    }
    else if (action == 18) // Tallstrider 22807
    {
        pPlayer->CreatePet(22807);

        if (pPlayer->GetPet())
        {
            pPlayer->GetPet()->learnSpell(27050); // Bite
            pPlayer->GetPet()->learnSpell(23110); // Dash
            pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "tallstrider\n";
        }
    }
    else if (action == 19) // Turtle 5431
    {
        pPlayer->CreatePet(5431);

        if (pPlayer->GetPet())
        {
            pPlayer->GetPet()->learnSpell(27050); // Bite
            pPlayer->GetPet()->learnSpell(26064); // Spell Shield
            pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "turtle\n";
        }
    }
    else if (action == 20) // Warp Stalker 23219
    {
        pPlayer->CreatePet(23219);

        if (pPlayer->GetPet())
        {
            pPlayer->GetPet()->learnSpell(27050); // Bite
            pPlayer->GetPet()->learnSpell(27049); // Claw
            pPlayer->GetPet()->learnSpell(35346); // Warp
            pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "warp stalker\n";
        }
    }
    else if (action == 21) // Wind Serpent 20673
    {
        pPlayer->CreatePet(20673);

        if (pPlayer->GetPet())
        {
            pPlayer->GetPet()->learnSpell(27050); // Bite
            pPlayer->GetPet()->learnSpell(23148); // Dive
            pPlayer->GetPet()->learnSpell(25012); // Lightning Breath
            pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "wind serpent\n";
        }
    }
    else if (action == 22) // Wolf 17280
    {
        pPlayer->CreatePet(17280);

        if (pPlayer->GetPet())
        {
            pPlayer->GetPet()->learnSpell(27050); // Bite
            pPlayer->GetPet()->learnSpell(23110); // Dash
            pPlayer->GetPet()->learnSpell(27685); // Charge
            pPlayer->GetPet()->learnSpell(24604); // Furious Howl
            pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << tamedmsg << "wolf\n";
        }
    }

    return true;
}

void AddSC_beast_master()
{
    Script *newscript;

    newscript                   = new Script;
    newscript->Name             = "npc_beast_master";
    newscript->pGossipHello     = &GossipHello_beast_master;
    newscript->pGossipSelect    = &GossipSelect_beast_master;
    newscript->RegisterSelf();
}