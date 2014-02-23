#include "Precompiled.h"
#include "ArenaTeam.h"
#include "Settings.h"

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
        pPlayer->AddGossipMenuItem(Icon::TAXI, "Teleport To: Shopping Mall", GOSSIP_SENDER_MAIN, 1  );

    if (pPlayer->GetAreaId() != 3539 || pPlayer->isGameMaster())
        pPlayer->AddGossipMenuItem(Icon::TAXI, "Teleport To: Neutral  Mall", GOSSIP_SENDER_MAIN, 2);

    if (pPlayer->GetAreaId() != 3738 || pPlayer->isGameMaster())
        pPlayer->AddGossipMenuItem(Icon::TAXI, "Teleport To: Stormspire", GOSSIP_SENDER_MAIN, 4);

    if ((pPlayer->GetAreaId() != 1741 && pPlayer->GetAreaId() != 2177) ||
        pPlayer->isGameMaster())
        pPlayer->AddGossipMenuItem(Icon::TAXI, "Teleport To: Gurubashi Arena", GOSSIP_SENDER_MAIN, 3);

    pPlayer->AddGossipMenuItem(Icon::GEAR, "Open Tools                  ", GOSSIP_SENDER_MAIN, 100);
    //pPlayer->AddGossipMenuItem(Icon::GEAR, "Change Your Level           ", GOSSIP_SENDER_MAIN, 200);
    pPlayer->AddGossipMenuItem(Icon::BAG, "Open Vendors                ", GOSSIP_SENDER_MAIN, 300);
    pPlayer->AddGossipMenuItem(Icon::SWORDS, "Queue for battle            ", GOSSIP_SENDER_MAIN, 400);

    if (pPlayer->isGameMaster())
        pPlayer->AddGossipMenuItem(Icon::BAG, "GM menu", GOSSIP_SENDER_MAIN, 1337);

    pPlayer->PlayerTalkClass->SendGossipMenu("Your wish, my law!", pCreature->GetObjectGuid());
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
        pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << " Welcome to The Shopping Mall\n";
        if (pPlayer->GetTeam() == HORDE)
            pPlayer->TeleportTo(530, 237.497f, 2696.65f, 90.755f, 3.25544f);
        else
            pPlayer->TeleportTo(530, -796.259f, 2625.9f, 124.095f, 3.70235f);
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
    else if (action == 4) // Teleport To: Stormspire
    {
        pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << " Welcome to The Stormspire\n";

        if (pPlayer->GetTeam() == ALLIANCE)
            pPlayer->TeleportTo(530, 4201.07f, 3100.86f, 335.821f, 4.02089f);
        else
            pPlayer->TeleportTo(530, 4118.95f, 2936.97f, 354.523f, 1.62542f);
    }
    else if (action == 100) // Open Tools
    {
        pPlayer->AddGossipMenuItem(Icon::TRAINER,   "Reset Talents     ", GOSSIP_SENDER_MAIN, 101);
        pPlayer->AddGossipMenuItem(Icon::TRAINER,   "Reset Cooldowns   ", GOSSIP_SENDER_MAIN, 104);
        pPlayer->AddGossipMenuItem(Icon::TRAINER,   "Max Weapon Skills ", GOSSIP_SENDER_MAIN, 103);
        pPlayer->AddGossipMenuItem(Icon::BAG,       "Open Bank         ", GOSSIP_SENDER_MAIN, 105);
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "Open Auction House", GOSSIP_SENDER_MAIN, 106);
        pPlayer->AddGossipMenuItem(Icon::TABARD,    "Create a guild    ", GOSSIP_SENDER_MAIN, 107);
        pPlayer->AddGossipMenuItem(Icon::TABARD,    "Design your tabard", GOSSIP_SENDER_MAIN, 108);

        if (pPlayer->GetSettings()->GetSetting(SETTING_UINT_HIDETEMPLATEMENU))
            pPlayer->AddGossipMenuItem(Icon::GEAR, "Show templates on login", GOSSIP_SENDER_MAIN, 109);

        pPlayer->AddGossipMenuItem(Icon::GEAR,      "Back              ", GOSSIP_SENDER_MAIN, 0  );
    }
    else if (action == 101) // Reset Talents
    {
        pPlayer->AddGossipMenuItem(Icon::GEAR, "Are you sure you want to reset your talents?", GOSSIP_SENDER_MAIN, 100);
        pPlayer->AddGossipMenuItem(Icon::GEAR, "Yes                                         ", GOSSIP_SENDER_MAIN, 102);
        pPlayer->AddGossipMenuItem(Icon::GEAR, "No                                          ", GOSSIP_SENDER_MAIN, 100);
        pPlayer->AddGossipMenuItem(Icon::GEAR, "Back                                        ", GOSSIP_SENDER_MAIN, 0);
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
    else if (action == 109)
    {
        pPlayer->GetSettings()->SetSetting(SETTING_UINT_HIDETEMPLATEMENU, false);
        pPlayer->BoxChat << sCustom.ChatNameWrapper(pCreature->GetName()) << " Template menu will be shown on next login\n";
    }
    else if (action == 200) // Change Level
    {
        if (pPlayer->getLevel() != 70)
            pPlayer->AddGossipMenuItem(Icon::GEAR, "Level: 70", GOSSIP_SENDER_MAIN, 201);
        if (pPlayer->getLevel() != 49)
            pPlayer->AddGossipMenuItem(Icon::GEAR, "Level: 49", GOSSIP_SENDER_MAIN, 202);
        if (pPlayer->getLevel() != 19)
            pPlayer->AddGossipMenuItem(Icon::GEAR, "Level: 19", GOSSIP_SENDER_MAIN, 203);
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
        pPlayer->AddGossipMenuItem(Icon::BAG,       "Starter Items   ", GOSSIP_SENDER_MAIN, 301);
        pPlayer->AddGossipMenuItem(Icon::BAG,       "Season Items    ", GOSSIP_SENDER_MAIN, 302);
        pPlayer->AddGossipMenuItem(Icon::BAG,       "PvE Items       ", GOSSIP_SENDER_MAIN, 303);
        pPlayer->AddGossipMenuItem(Icon::BAG,       "Gems            ", GOSSIP_SENDER_MAIN, 304);
        pPlayer->AddGossipMenuItem(Icon::BAG,       "Jewelry         ", GOSSIP_SENDER_MAIN, 305);
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "Cloaks          ", GOSSIP_SENDER_MULTIVENDOR, 81023);
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "Relics          ", GOSSIP_SENDER_MULTIVENDOR, 82016);
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "Offhands/Shields", GOSSIP_SENDER_MULTIVENDOR, 82034);
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "Tabards         ", GOSSIP_SENDER_MULTIVENDOR, 81024);
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "Profession Goods", GOSSIP_SENDER_MULTIVENDOR, 81034);
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "General Goods   ", GOSSIP_SENDER_MULTIVENDOR, 81027);

        if (pPlayer->getClass() == CLASS_ROGUE)
            pPlayer->AddGossipMenuItem(Icon::MONEY_BAG ,"Rogue Supplies ", GOSSIP_SENDER_MULTIVENDOR, 81026);
        else if (pPlayer->getClass() == CLASS_WARLOCK)
            pPlayer->AddGossipMenuItem(Icon::MONEY_BAG ,"Pet Spell books", GOSSIP_SENDER_MULTIVENDOR, 81037);

        pPlayer->AddGossipMenuItem(Icon::GEAR ,"Back            ", GOSSIP_SENDER_MAIN, 0  );
    }
    else if (action == 301)
    {
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG ,"Starter Weapons ", GOSSIP_SENDER_MULTIVENDOR, 82022);
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG ,"Starter Mainset ", GOSSIP_SENDER_MULTIVENDOR, 82020);
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG ,"Starter Offset  ", GOSSIP_SENDER_MULTIVENDOR, 82021);
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG ,"Starter Jewelery", GOSSIP_SENDER_MULTIVENDOR, 82015);
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG ,"Starter Gems    ", GOSSIP_SENDER_MULTIVENDOR, 82027);
        pPlayer->AddGossipMenuItem(Icon::GEAR ,"Back            ", GOSSIP_SENDER_MAIN, 300);
    }
    else if (action == 302)
    {
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG ,"S1: Gladiator ", GOSSIP_SENDER_MULTIVENDOR, 81000);
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG ,"S1: General   ", GOSSIP_SENDER_MULTIVENDOR, 81001);
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG ,"S2: Merciless ", GOSSIP_SENDER_MULTIVENDOR, 81002);
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG ,"S2: Veteran   ", GOSSIP_SENDER_MULTIVENDOR, 81003);
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG ,"S3: Vengeful  ", GOSSIP_SENDER_MULTIVENDOR, 81035);
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG ,"S3: Vindicator", GOSSIP_SENDER_MULTIVENDOR, 81005);
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG ,"S4: Brutal    ", GOSSIP_SENDER_MULTIVENDOR, 81006);
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG ,"S4: Guardian  ", GOSSIP_SENDER_MULTIVENDOR, 81029);
        pPlayer->AddGossipMenuItem(Icon::GEAR ,"Back          ", GOSSIP_SENDER_MAIN, 300);
    }
    else if (action == 303)
    {
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "T6: Mainset      ", GOSSIP_SENDER_MULTIVENDOR, 82035);
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "T6: Offset       ", GOSSIP_SENDER_MULTIVENDOR, 82037);
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "MISC: PvE Gear   ", GOSSIP_SENDER_MULTIVENDOR, 81036);
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "MISC: PvE Weapons", GOSSIP_SENDER_MULTIVENDOR, 81033);
        pPlayer->AddGossipMenuItem(Icon::GEAR ,"Back             ", GOSSIP_SENDER_MAIN, 300);
    }
    else if (action == 304)
    {
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG, MSG_COLOR_WHITE   "Meta  ", GOSSIP_SENDER_MULTIVENDOR, 81028);
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG, MSG_COLOR_RED     "Red   ", GOSSIP_SENDER_MULTIVENDOR, 81014);
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG, MSG_COLOR_ORANGE  "Orange", GOSSIP_SENDER_MULTIVENDOR, 81009);
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG, MSG_COLOR_YELLOW  "Yellow", GOSSIP_SENDER_MULTIVENDOR, 81012);
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG, MSG_COLOR_GREEN   "Green ", GOSSIP_SENDER_MULTIVENDOR, 81010);
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG, MSG_COLOR_BLUE    "Blue  ", GOSSIP_SENDER_MULTIVENDOR, 81013);
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG, MSG_COLOR_PURPLE  "Purple", GOSSIP_SENDER_MULTIVENDOR, 81011);
        pPlayer->AddGossipMenuItem(Icon::GEAR ,"Back             ", GOSSIP_SENDER_MAIN, 300);
    }
    else if (action == 305)
    {
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG ,"Trinkets ", GOSSIP_SENDER_MULTIVENDOR, 81022);
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG ,"Rings    ", GOSSIP_SENDER_MULTIVENDOR, 81025);
        pPlayer->AddGossipMenuItem(Icon::MONEY_BAG ,"Necklaces", GOSSIP_SENDER_MULTIVENDOR, 81021);
        pPlayer->AddGossipMenuItem(Icon::GEAR ,"Back     ", GOSSIP_SENDER_MAIN, 300);
    }
    else if (action == 400)
    {
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Warsong Gulch   ", GOSSIP_SENDER_MAIN, 401);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Arathi Basin    ", GOSSIP_SENDER_MAIN, 402);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Eye of the Storm", GOSSIP_SENDER_MAIN, 403);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Alterac Valley  ", GOSSIP_SENDER_MAIN, 404);

        if (Group* grp = pPlayer->GetGroup())
        {
            for (uint8 i = 0; i < 3; ++i)
            {
                if (ArenaTeam* at = sObjectMgr.GetArenaTeamById(pPlayer->GetArenaTeamId(i)))
                {
                    bool showrated = true;

                    for (Group::MemberSlotList::const_iterator itr = grp->GetMemberSlots().begin(); itr != grp->GetMemberSlots().end(); ++itr)
                    {
                        if (Player* member = sObjectMgr.GetPlayer(itr->guid))
                        {
                            if (sObjectMgr.GetArenaTeamById(member->GetArenaTeamId(i)) != at)
                                showrated = false;
                        }
                        else
                            showrated = false;
                    }

                    if (showrated)
                    {
                        if (i == 0)
                            pPlayer->AddGossipMenuItem(Icon::SWORDS, "Rated 2v2", GOSSIP_SENDER_MAIN, 405);
                        else if (i == 1)
                            pPlayer->AddGossipMenuItem(Icon::SWORDS, "Rated 3v3", GOSSIP_SENDER_MAIN, 406);
                        else if (i == 2)
                            pPlayer->AddGossipMenuItem(Icon::SWORDS, "Rated 5v5", GOSSIP_SENDER_MAIN, 407);
                    }
                }
            }
        }

        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Skirmish 2v2", GOSSIP_SENDER_MAIN, 408);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Skirmish 3v3", GOSSIP_SENDER_MAIN, 409);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Skirmish 5v5", GOSSIP_SENDER_MAIN, 410);
        pPlayer->AddGossipMenuItem(Icon::GEAR ,"Back        ", GOSSIP_SENDER_MAIN, 0  );
    }
    else if (action >= 401 && action <= 404)
    {
        BattleGroundTypeId bgtype = BATTLEGROUND_TYPE_NONE;

        switch (action)
        {
        case 401: bgtype = BATTLEGROUND_WS; break;
        case 402: bgtype = BATTLEGROUND_AB; break;
        case 403: bgtype = BATTLEGROUND_EY; break;
        case 404: bgtype = BATTLEGROUND_AV; break;
        }

        WorldPacket data;
        data.SetOpcode(CMSG_BATTLEMASTER_JOIN);
        data << ObjectGuid(pCreature->GetObjectGuid());
        data << uint32(bgtype);
        data << uint32(0);
        data << uint8(pPlayer->GetGroup());
        pPlayer->GetSession()->HandleBattlemasterJoinOpcode(data);
    }
    else if (action >= 405 && action <= 410)
    {
        uint8 arenaslot = 0; // 2v2 default
        bool rated = false;

        switch (action)
        {
        case 406:
        case 409:
            arenaslot = 1;
            break;
        case 407:
        case 410:
            arenaslot = 2;
            break;
        }

        if (action <= 407)
            rated = true;

        WorldPacket data;
        data.SetOpcode(CMSG_BATTLEMASTER_JOIN_ARENA);
        data << ObjectGuid(pCreature->GetObjectGuid());
        data << uint8(arenaslot);
        data << uint8(pPlayer->GetGroup());
        data << uint8(rated);
        pPlayer->GetSession()->HandleBattlemasterJoinArena(data);
    }
    else if (action == 1337)
    {
        pPlayer->AddGossipMenuItem(Icon::GEAR, "Send boxmessage",  CHAT_BOX,  1339);
        pPlayer->AddGossipMenuItem(Icon::GEAR, "Send widemessage", CHAT_WIDE, 1339);
        pPlayer->AddGossipMenuItem(Icon::GEAR, "Send bothmessage", CHAT_BOTH, 1339);
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
        pPlayer->PlayerTalkClass->SendGossipMenu("Your wish, my law!", pCreature->GetObjectGuid());

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