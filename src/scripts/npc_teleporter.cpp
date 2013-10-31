#include "Precompiled.h"

bool GossipHello_teleporter(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->isInCombat())
    {
        pPlayer->BoxChat << sCustom.ChatNameWrapper(pCreature->GetName()) << " You are in combat!\n";
        return true;
    }

    if ((pPlayer->GetAreaId() != 3536 && pPlayer->GetTeam() == HORDE) ||
        (pPlayer->GetAreaId() != 3538 && pPlayer->GetTeam() == ALLIANCE) ||
        pPlayer->isGameMaster())
        pPlayer->AddGossipMenuItem(2, "Teleport To: Shopping Mall  ", GOSSIP_SENDER_MAIN, 1  );

    if (pPlayer->GetAreaId() != 3539 || pPlayer->isGameMaster())
        pPlayer->AddGossipMenuItem(2, "Teleport To: Neutral  Mall  ", GOSSIP_SENDER_MAIN, 2  );

    pPlayer->AddGossipMenuItem(2, "Teleport To: Gurubashi Arena", GOSSIP_SENDER_MAIN, 3  );
    pPlayer->AddGossipMenuItem(5, "Open Tools                  ", GOSSIP_SENDER_MAIN, 100);
    //pPlayer->AddGossipMenuItem(5, "Change Your Level           ", GOSSIP_SENDER_MAIN, 200);
    pPlayer->AddGossipMenuItem(1, "Open Vendors                ", GOSSIP_SENDER_MAIN, 300);

    if (pPlayer->isGameMaster())
        pPlayer->AddGossipMenuItem(1, "GM menu", GOSSIP_SENDER_MAIN, 1337);

    pPlayer->PlayerTalkClass->SendGossipMenu(1,pCreature->GetObjectGuid());
    return true;
}

bool GossipSelect_teleporter(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action)
{
    const char* cName = pCreature->GetName();
    pPlayer->PlayerTalkClass->CloseGossip();

    if (pPlayer->isInCombat())
    {
        pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << " You are in combat!\n";
        return true;
    }

    if (action == 0)
        GossipHello_teleporter(pPlayer,pCreature);
    else if (sender == GOSSIP_SENDER_MULTIVENDOR)
        pPlayer->SendMultiVendorInventory(action, pCreature->GetObjectGuid());
    else if (action == 1) // Teleport To: Shopping Mall
    {
        if (pPlayer->GetTeam() == HORDE)
        {
            pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << " Welcome to Horde shopping mall\n";
            pPlayer->TeleportTo(530, 237.497f, 2696.65f, 90.755f, 3.25544f);
        }
        else
        {
            pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << " Welcome to Alliance shopping mall\n";
            pPlayer->TeleportTo(530, -796.259f, 2625.9f, 124.095f, 3.70235f);
        }
    }
    else if (action == 2) // Teleport To: Neutral Mall
    {
        pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << " Welcome to Neutral shopping mall\n";
        pPlayer->TeleportTo(530, -248.403f, 939.569f, 84.3788f, 1.53148f);
    }
    else if (action == 3) // Teleport To: Gurubashi Arena
    {
        pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << " Welcome to Gurubashi Arena\n";
        pPlayer->TeleportTo(0, -13235.7f, 213.741f, 31.2181f, 1.14919f);
    }
    else if (action == 100) // Open Tools
    {
        pPlayer->AddGossipMenuItem(5 ,"Reset Talents     ", GOSSIP_SENDER_MAIN, 101);
        pPlayer->AddGossipMenuItem(5 ,"Reset Cooldowns   ", GOSSIP_SENDER_MAIN, 104);
        pPlayer->AddGossipMenuItem(5 ,"Max Weapon Skills ", GOSSIP_SENDER_MAIN, 103);
        pPlayer->AddGossipMenuItem(5 ,"Open Bank         ", GOSSIP_SENDER_MAIN, 105);
        pPlayer->AddGossipMenuItem(5 ,"Open Auction House", GOSSIP_SENDER_MAIN, 106);
        pPlayer->AddGossipMenuItem(5 ,"Create a guild    ", GOSSIP_SENDER_MAIN, 107);
        pPlayer->AddGossipMenuItem(5 ,"Design your tabard", GOSSIP_SENDER_MAIN, 108);
        pPlayer->AddGossipMenuItem(5 ,"Back to Main Menu ", GOSSIP_SENDER_MAIN, 0  );
    }
    else if (action == 101) // Reset Talents
    {
        pPlayer->AddGossipMenuItem(5 ,"Are you sure you want to reset your talents?", GOSSIP_SENDER_MAIN, 100);
        pPlayer->AddGossipMenuItem(5 ,"Yes                                         ", GOSSIP_SENDER_MAIN, 102);
        pPlayer->AddGossipMenuItem(5 ,"No                                          ", GOSSIP_SENDER_MAIN, 100);
        pPlayer->AddGossipMenuItem(5 ,"Back to Main Menu                           ", GOSSIP_SENDER_MAIN, 0  );
    }
    else if (action == 102) // Talents Reset
    {
        pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << " Talents Reset\n";
        pPlayer->resetTalents(true);
    }
    else if (action == 103) // Weapon skills maxed
    {
        pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << " Weapon skills maxed\n";
        pPlayer->UpdateSkillsToMaxSkillsForLevel();
    }
    else if (action == 104) // Cooldowns reset
    {
        pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << " Cooldowns reset\n";
        pPlayer->RemoveArenaSpellCooldowns();
    }
    else if (action == 105) // Show bank
        pPlayer->GetSession()->SendShowBank(pCreature->GetObjectGuid());
    else if (action == 106) // Auctioneer
        pPlayer->GetSession()->SendAuctionHello(pCreature);
    else if (action == 107) // Guild Master
        pPlayer->GetSession()->SendPetitionShowList(pCreature->GetObjectGuid());
    else if (action == 108) // Tabard Petitioner
        pPlayer->GetSession()->SendTabardVendorActivate(pCreature->GetObjectGuid());
    else if (action == 200) // Change Level
    {
        if (pPlayer->getLevel() != 70)
            pPlayer->AddGossipMenuItem(5 ,"Level: 70", GOSSIP_SENDER_MAIN, 201);
        if (pPlayer->getLevel() != 49)
            pPlayer->AddGossipMenuItem(5 ,"Level: 49", GOSSIP_SENDER_MAIN, 202);
        if (pPlayer->getLevel() != 19)
            pPlayer->AddGossipMenuItem(5 ,"Level: 19", GOSSIP_SENDER_MAIN, 203);
    }
    else if (action >= 201 && action <= 203)
    {
        for (uint8 i = SLOT_HEAD; i < EQUIPMENT_SLOT_END; ++i)
        {
            if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
            {
                pPlayer->BoxChat << sCustom.ChatNameWrapper(pCreature->GetName()) << " You cannot have any gear equipped while changing level\n";
                return true;
            }
        }

        if (action == 201)
            pPlayer->SetLevel(70);
        else if (action == 202)
            pPlayer->SetLevel(49);
        else if (action == 203)
            pPlayer->SetLevel(19);
    }
    else if (action == 300)
    {
        pPlayer->AddGossipMenuItem(1 ,"Starter Items   ", GOSSIP_SENDER_MAIN, 301);
        pPlayer->AddGossipMenuItem(1 ,"Season Items    ", GOSSIP_SENDER_MAIN, 302);
        pPlayer->AddGossipMenuItem(1 ,"PvE Items       ", GOSSIP_SENDER_MAIN, 303);
        pPlayer->AddGossipMenuItem(1 ,"Gems            ", GOSSIP_SENDER_MAIN, 304);
        pPlayer->AddGossipMenuItem(1 ,"Jewelry         ", GOSSIP_SENDER_MAIN, 305);
        pPlayer->AddGossipMenuItem(6 ,"Cloaks          ", GOSSIP_SENDER_MULTIVENDOR, 81023);
        pPlayer->AddGossipMenuItem(6 ,"Relics          ", GOSSIP_SENDER_MULTIVENDOR, 82016);
        pPlayer->AddGossipMenuItem(6 ,"Offhands/Shields", GOSSIP_SENDER_MULTIVENDOR, 82034);
        pPlayer->AddGossipMenuItem(6 ,"Tabards         ", GOSSIP_SENDER_MULTIVENDOR, 81024);
        pPlayer->AddGossipMenuItem(6 ,"Profession Goods", GOSSIP_SENDER_MULTIVENDOR, 81034);
        pPlayer->AddGossipMenuItem(6 ,"General Goods   ", GOSSIP_SENDER_MULTIVENDOR, 81027);

        if (pPlayer->getClass() == CLASS_ROGUE)
            pPlayer->AddGossipMenuItem(6 ,"Rogue Supplies ", GOSSIP_SENDER_MULTIVENDOR, 81026);
        else if (pPlayer->getClass() == CLASS_WARLOCK)
            pPlayer->AddGossipMenuItem(6 ,"Pet Spell books", GOSSIP_SENDER_MULTIVENDOR, 81037);

        pPlayer->AddGossipMenuItem(5 ,"Back            ", GOSSIP_SENDER_MAIN, 0  );
    }
    else if (action == 301)
    {
        pPlayer->AddGossipMenuItem(6 ,"Starter Weapons ", GOSSIP_SENDER_MULTIVENDOR, 82022);
        pPlayer->AddGossipMenuItem(6 ,"Starter Mainset ", GOSSIP_SENDER_MULTIVENDOR, 82020);
        pPlayer->AddGossipMenuItem(6 ,"Starter Offset  ", GOSSIP_SENDER_MULTIVENDOR, 82021);
        pPlayer->AddGossipMenuItem(6 ,"Starter Jewelery", GOSSIP_SENDER_MULTIVENDOR, 82015);
        pPlayer->AddGossipMenuItem(6 ,"Starter Gems    ", GOSSIP_SENDER_MULTIVENDOR, 82027);
        pPlayer->AddGossipMenuItem(5 ,"Back            ", GOSSIP_SENDER_MAIN, 300);
    }
    else if (action == 302)
    {
        pPlayer->AddGossipMenuItem(6 ,"S1: Gladiator ", GOSSIP_SENDER_MULTIVENDOR, 81000);
        pPlayer->AddGossipMenuItem(6 ,"S1: General   ", GOSSIP_SENDER_MULTIVENDOR, 81001);
        pPlayer->AddGossipMenuItem(6 ,"S2: Merciless ", GOSSIP_SENDER_MULTIVENDOR, 81002);
        pPlayer->AddGossipMenuItem(6 ,"S2: Veteran   ", GOSSIP_SENDER_MULTIVENDOR, 81003);
        pPlayer->AddGossipMenuItem(6 ,"S3: Vengeful  ", GOSSIP_SENDER_MULTIVENDOR, 81035);
        pPlayer->AddGossipMenuItem(6 ,"S3: Vindicator", GOSSIP_SENDER_MULTIVENDOR, 81005);
        pPlayer->AddGossipMenuItem(6 ,"S4: Brutal    ", GOSSIP_SENDER_MULTIVENDOR, 81006);
        pPlayer->AddGossipMenuItem(6 ,"S4: Guardian  ", GOSSIP_SENDER_MULTIVENDOR, 81029);
        pPlayer->AddGossipMenuItem(5 ,"Back          ", GOSSIP_SENDER_MAIN, 300);
    }
    else if (action == 303)
    {
        pPlayer->AddGossipMenuItem(6, "T6: Mainset      ", GOSSIP_SENDER_MULTIVENDOR, 82035);
        pPlayer->AddGossipMenuItem(6, "T6: Offset       ", GOSSIP_SENDER_MULTIVENDOR, 82037);
        pPlayer->AddGossipMenuItem(6, "MISC: PvE Gear   ", GOSSIP_SENDER_MULTIVENDOR, 81036);
        pPlayer->AddGossipMenuItem(6, "MISC: PvE Weapons", GOSSIP_SENDER_MULTIVENDOR, 81033);
        pPlayer->AddGossipMenuItem(5 ,"Back             ", GOSSIP_SENDER_MAIN, 300);
    }
    else if (action == 304)
    {
        pPlayer->AddGossipMenuItem(6, MSG_COLOR_WHITE   "Meta  ", GOSSIP_SENDER_MULTIVENDOR, 81028);
        pPlayer->AddGossipMenuItem(6, MSG_COLOR_RED     "Red   ", GOSSIP_SENDER_MULTIVENDOR, 81014);
        pPlayer->AddGossipMenuItem(6, MSG_COLOR_ORANGE  "Orange", GOSSIP_SENDER_MULTIVENDOR, 81009);
        pPlayer->AddGossipMenuItem(6, MSG_COLOR_YELLOW  "Yellow", GOSSIP_SENDER_MULTIVENDOR, 81012);
        pPlayer->AddGossipMenuItem(6, MSG_COLOR_GREEN   "Green ", GOSSIP_SENDER_MULTIVENDOR, 81010);
        pPlayer->AddGossipMenuItem(6, MSG_COLOR_BLUE    "Blue  ", GOSSIP_SENDER_MULTIVENDOR, 81013);
        pPlayer->AddGossipMenuItem(6, MSG_COLOR_PURPLE  "Purple", GOSSIP_SENDER_MULTIVENDOR, 81011);
        pPlayer->AddGossipMenuItem(5 ,"Back             ", GOSSIP_SENDER_MAIN, 300);
    }
    else if (action == 305)
    {
        pPlayer->AddGossipMenuItem(6 ,"Trinkets ", GOSSIP_SENDER_MULTIVENDOR, 81022);
        pPlayer->AddGossipMenuItem(6 ,"Rings    ", GOSSIP_SENDER_MULTIVENDOR, 81025);
        pPlayer->AddGossipMenuItem(6 ,"Necklaces", GOSSIP_SENDER_MULTIVENDOR, 81021);
        pPlayer->AddGossipMenuItem(5 ,"Back     ", GOSSIP_SENDER_MAIN, 300);
    }
    else if (action == 1337)
    {
        pPlayer->AddGossipMenuItem(5, "Check how icons look", GOSSIP_SENDER_MAIN, 1338);
        pPlayer->AddGossipMenuItem(5, "Send boxmessage",  CHAT_BOX,  1339);
        pPlayer->AddGossipMenuItem(5, "Send widemessage", CHAT_WIDE, 1339);
        pPlayer->AddGossipMenuItem(5, "Send bothmessage", CHAT_BOTH, 1339);
    }
    else if (action == 1338)
    {
        pPlayer->AddGossipMenuItem(GOSSIP_ICON_CHAT         ,"0  GOSSIP_ICON_CHAT",        GOSSIP_SENDER_MAIN, 0);
        pPlayer->AddGossipMenuItem(GOSSIP_ICON_VENDOR       ,"1  GOSSIP_ICON_VENDOR",      GOSSIP_SENDER_MAIN, 0);
        pPlayer->AddGossipMenuItem(GOSSIP_ICON_TAXI         ,"2  GOSSIP_ICON_TAXI",        GOSSIP_SENDER_MAIN, 0);
        pPlayer->AddGossipMenuItem(GOSSIP_ICON_TRAINER      ,"3  GOSSIP_ICON_TRAINER",     GOSSIP_SENDER_MAIN, 0);
        pPlayer->AddGossipMenuItem(GOSSIP_ICON_INTERACT_1   ,"4  GOSSIP_ICON_INTERACT_1",  GOSSIP_SENDER_MAIN, 0);
        pPlayer->AddGossipMenuItem(GOSSIP_ICON_INTERACT_2   ,"5  GOSSIP_ICON_INTERACT_2",  GOSSIP_SENDER_MAIN, 0);
        pPlayer->AddGossipMenuItem(GOSSIP_ICON_MONEY_BAG    ,"6  GOSSIP_ICON_MONEY_BAG",   GOSSIP_SENDER_MAIN, 0);
        pPlayer->AddGossipMenuItem(GOSSIP_ICON_TALK         ,"7  GOSSIP_ICON_TALK",        GOSSIP_SENDER_MAIN, 0);
        pPlayer->AddGossipMenuItem(GOSSIP_ICON_TABARD       ,"8  GOSSIP_ICON_TABARD",      GOSSIP_SENDER_MAIN, 0);
        pPlayer->AddGossipMenuItem(GOSSIP_ICON_BATTLE       ,"9  GOSSIP_ICON_BATTLE",      GOSSIP_SENDER_MAIN, 0);
        pPlayer->AddGossipMenuItem(GOSSIP_ICON_DOT          ,"10 GOSSIP_ICON_DOT",         GOSSIP_SENDER_MAIN, 0);
    }
    else if (action == 1339)
    {
        if (sender == CHAT_BOX)
            pPlayer->BoxChat << "Boxchat\n";
        else if (sender == CHAT_WIDE)
            pPlayer->WideChat << "WideChat\n";
        else if (sender == CHAT_BOTH)
            pPlayer->BothChat << "BothChat\n";
    }

    if(pPlayer->PlayerTalkClass->GetGossipMenu().MenuItemCount() > 0)
        pPlayer->PlayerTalkClass->SendGossipMenu(1,pCreature->GetObjectGuid());

    return true;
}

void AddSC_npc_teleporter()
{
    Script *newscript;

    newscript                   = new Script;
    newscript->Name             = "npc_teleporter";
    newscript->pGossipHello     = &GossipHello_teleporter;
    newscript->pGossipSelect    = &GossipSelect_teleporter;
    newscript->RegisterSelf();
}