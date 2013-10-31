#include "Precompiled.h"

bool GossipHello_npc_enchant(Player *pPlayer, Creature *pCreature)
{
    if (pPlayer->isInCombat())
    {
        pPlayer->BothChat << sCustom.ChatNameWrapper(pCreature->GetName()) << " You are in combat!\n";
        return true;
    }

    if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_HEAD))
        pPlayer->AddGossipMenuItem(9, "Enchant Head      ", SLOT_HEAD, 1);

    if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_SHOULDERS))
        pPlayer->AddGossipMenuItem(9, "Enchant Shoulders ", SLOT_SHOULDERS, 2);

    if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_BACK))
        pPlayer->AddGossipMenuItem(9, "Enchant Back      ", SLOT_BACK, 3);

    if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_CHEST))
        pPlayer->AddGossipMenuItem(9, "Enchant Chest     ", SLOT_CHEST, 4);

    if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_WRISTS))
        pPlayer->AddGossipMenuItem(9, "Enchant Wrists    ", SLOT_WRISTS, 5);

    if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_HANDS))
        pPlayer->AddGossipMenuItem(9, "Enchant Hands     ", SLOT_HANDS, 6);

    if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_LEGS))
        pPlayer->AddGossipMenuItem(9, "Enchant Legs      ", SLOT_LEGS, 7);

    if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_FEET))
        pPlayer->AddGossipMenuItem(9, "Enchant Feet      ", SLOT_FEET, 8);

    if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_FINGER1))
        pPlayer->AddGossipMenuItem(9, "Enchant Finger one", SLOT_FINGER1, 9);

    if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_FINGER2))
        pPlayer->AddGossipMenuItem(9, "Enchant Finger two", SLOT_FINGER2, 9);

    if (Item* item = pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_MAIN_HAND))
    {
        if (item->GetProto()->InventoryType ==  INVTYPE_2HWEAPON)
            pPlayer->AddGossipMenuItem(9, "Enchant Two hand  ", SLOT_MAIN_HAND, 10);
        else
            pPlayer->AddGossipMenuItem(9, "Enchant Main hand ", SLOT_MAIN_HAND, 11);
    }
    if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_OFF_HAND))
    {
        pPlayer->AddGossipMenuItem(9, "Enchant Offhand   ", SLOT_OFF_HAND, 11);
        pPlayer->AddGossipMenuItem(9, "Enchant Shield    ", SLOT_OFF_HAND, 14);
    }
    if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_RANGED))
        pPlayer->AddGossipMenuItem(9, "Enchant Ranged    ", SLOT_RANGED, 13);

    pPlayer->PlayerTalkClass->SendGossipMenu(1,pCreature->GetObjectGuid());
    return true;
}

bool GossipSelect_npc_enchant(Player *pPlayer, Creature *pCreature, uint32 sender, uint32 action)
{
    const char* cName = pCreature->GetName();
    pPlayer->PlayerTalkClass->CloseGossip();

    if (pPlayer->isInCombat())
    {
        pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << " You are in combat!\n";
        return true;
    }
    else if (action == 0)
        GossipHello_npc_enchant(pPlayer,pCreature);
    else if (action == 1)
    {
        pPlayer->AddGossipMenuItem(9, "34 Attackpower 16 Hit          ", sender, 35452);
        pPlayer->AddGossipMenuItem(9, "17 Strength16 Intellect        ", sender, 37891);
        pPlayer->AddGossipMenuItem(9, "22 Spelldamage 14 Hit          ", sender, 35447);
        pPlayer->AddGossipMenuItem(9, "35 Healing 12 Spelldamage 7 mp5", sender, 35445);
        pPlayer->AddGossipMenuItem(9, "18 Stamina 20 Resilience       ", sender, 35453);
        pPlayer->AddGossipMenuItem(9, "16 Defense 17 Dodge            ", sender, 35443);
        pPlayer->AddGossipMenuItem(9, "20 Fire Resistance             ", sender, 35456);
        pPlayer->AddGossipMenuItem(9, "20 Arcane Resistance           ", sender, 35455);
        pPlayer->AddGossipMenuItem(9, "20 Shadow Resistance           ", sender, 35458);
        pPlayer->AddGossipMenuItem(9, "20 Nature Resistance           ", sender, 35454);
        pPlayer->AddGossipMenuItem(9, "20 Frost Resistance            ", sender, 35457);
        pPlayer->AddGossipMenuItem(9, "8 Resist All                   ", sender, 37889);
        pPlayer->AddGossipMenuItem(9, "<- Back                        ", sender, 0    );
    }
    else if (action == 2)
    {
        pPlayer->AddGossipMenuItem(9, "30 Attackpower 10 Crit         ", sender, 35417);
        pPlayer->AddGossipMenuItem(9, "26 Attackpower 14 Crit         ", sender, 29483);
        pPlayer->AddGossipMenuItem(9, "20 Attackpower 15 Crit         ", sender, 35439);
        pPlayer->AddGossipMenuItem(9, "18 Spelldamage 10 Crit         ", sender, 35406);
        pPlayer->AddGossipMenuItem(9, "15 Spelldamage 14 Crit         ", sender, 29467);
        pPlayer->AddGossipMenuItem(9, "12 Spelldamage 15 Crit         ", sender, 35437);
        pPlayer->AddGossipMenuItem(9, "33 Healing 11 Spelldamage 4 mp5", sender, 35404);
        pPlayer->AddGossipMenuItem(9, "31 Healing 11 Spelldamage 5 mp5", sender, 29475);
        pPlayer->AddGossipMenuItem(9, "22 Healing 6 mp5               ", sender, 35435);
        pPlayer->AddGossipMenuItem(9, "16 Stamina 100 Armor           ", sender, 29480);
        pPlayer->AddGossipMenuItem(9, "10 Dodge 15 Defense            ", sender, 35433);
        pPlayer->AddGossipMenuItem(9, "15 Dodge 10 Defense            ", sender, 35402);
        pPlayer->AddGossipMenuItem(9, "<- Back                        ", sender, 0    );
    }
    else if (action == 3)
    {
        pPlayer->AddGossipMenuItem(9, "12 Agility          ", sender, 34004);
        pPlayer->AddGossipMenuItem(9, "20 Spell Penetration", sender, 34003);
        pPlayer->AddGossipMenuItem(9, "15 Fire Resistance  ", sender, 25081);
        pPlayer->AddGossipMenuItem(9, "15 Arcane Resistance", sender, 34005);
        pPlayer->AddGossipMenuItem(9, "15 Shadow Resistance", sender, 34006);
        pPlayer->AddGossipMenuItem(9, "15 Nature Resistance", sender, 25082);
        pPlayer->AddGossipMenuItem(9, "7 Resist All        ", sender, 27962);
        pPlayer->AddGossipMenuItem(9, "12 Dodge            ", sender, 25086);
        pPlayer->AddGossipMenuItem(9, "12 Defense          ", sender, 47051);
        pPlayer->AddGossipMenuItem(9, "120 Armor           ", sender, 27961);
        pPlayer->AddGossipMenuItem(9, "Increase stealth    ", sender, 25083);
        pPlayer->AddGossipMenuItem(9, "<- Back             ", sender, 0    );
    }
    else if (action == 4)
    {
        pPlayer->AddGossipMenuItem(9, "15 Resilience", sender, 33992);
        pPlayer->AddGossipMenuItem(9, "6 All Stats  ", sender, 27960);
        pPlayer->AddGossipMenuItem(9, "15 Spirit    ", sender, 33990);
        pPlayer->AddGossipMenuItem(9, "15 Defense   ", sender, 46594);
        pPlayer->AddGossipMenuItem(9, "150 Health   ", sender, 27957);
        pPlayer->AddGossipMenuItem(9, "150 Mana     ", sender, 27958);
        pPlayer->AddGossipMenuItem(9, "<- Back      ", sender, 0    );
    }
    else if (action == 5)
    {
        pPlayer->AddGossipMenuItem(9, "24 Attackpower           ", sender, 34002);
        pPlayer->AddGossipMenuItem(9, "15 Spelldamage           ", sender, 27917);
        pPlayer->AddGossipMenuItem(9, "30 Healing 10 Spelldamage", sender, 27911);
        pPlayer->AddGossipMenuItem(9, "12 Stamina               ", sender, 27914);
        pPlayer->AddGossipMenuItem(9, "12 Defense               ", sender, 27906);
        pPlayer->AddGossipMenuItem(9, "12 Strength              ", sender, 27899);
        pPlayer->AddGossipMenuItem(9, "12 Intellect             ", sender, 34001);
        pPlayer->AddGossipMenuItem(9, "4 All Stats              ", sender, 27905);
        pPlayer->AddGossipMenuItem(9, "6 mp5                    ", sender, 27913);
        pPlayer->AddGossipMenuItem(9, "9 Spirit                 ", sender, 20009);
        pPlayer->AddGossipMenuItem(9, "<- Back                  ", sender, 0    );
    }
    else if (action == 6)
    {
        pPlayer->AddGossipMenuItem(9, "26 Attackpower           ", sender, 33996);
        pPlayer->AddGossipMenuItem(9, "15 Strength              ", sender, 33995);
        pPlayer->AddGossipMenuItem(9, "15 Agility               ", sender, 25080);
        pPlayer->AddGossipMenuItem(9, "20 Spelldamage           ", sender, 33997);
        pPlayer->AddGossipMenuItem(9, "35 Healing 12 Spelldamage", sender, 33999);
        pPlayer->AddGossipMenuItem(9, "15 Spell Hit             ", sender, 33994);
        pPlayer->AddGossipMenuItem(9, "10 Spell Crit            ", sender, 33993);
        pPlayer->AddGossipMenuItem(9, "<- Back                  ", sender, 0    );
    }
    else if (action == 7)
    {
        pPlayer->AddGossipMenuItem(9, "50 Attackpower 12 Crit              ", sender, 35490);
        pPlayer->AddGossipMenuItem(9, "40 Stamina 12 Agility               ", sender, 35495);
        pPlayer->AddGossipMenuItem(9, "35 Spelldamage 20 Stamina           ", sender, 31372);
        pPlayer->AddGossipMenuItem(9, "66 Healing 22 Spelldamage 20 Stamina", sender, 31370);
        pPlayer->AddGossipMenuItem(9, "<- Back                             ", sender, 0    );
    }
    else if (action == 8)
    {
        pPlayer->AddGossipMenuItem(9, "Minor Speed 6 Agility      ", sender,34007 );
        pPlayer->AddGossipMenuItem(9, "Minor Speed 9 Stamina      ", sender,34008 );
        pPlayer->AddGossipMenuItem(9, "5% Root/Snare Resist 10 Hit", sender,27954 );
        pPlayer->AddGossipMenuItem(9, "12 Agility                 ", sender,27951 );
        pPlayer->AddGossipMenuItem(9, "12 Stamina                 ", sender,27950 );
        pPlayer->AddGossipMenuItem(9, "4 hp/mp5                   ", sender,27948 );
        pPlayer->AddGossipMenuItem(9, "<- Back                    ", sender,0     );
    }
    else if (action == 9)
    {
        pPlayer->AddGossipMenuItem(9, "4 All Stats             ", sender, 27927);
        pPlayer->AddGossipMenuItem(9, "12 Spelldamage          ", sender, 27924);
        pPlayer->AddGossipMenuItem(9, "20 Healing 7 Spelldamage", sender, 27926);
        pPlayer->AddGossipMenuItem(9, "2 Weapon Damage         ", sender, 27920);
        pPlayer->AddGossipMenuItem(9, "<- Back                 ", sender, 0    );
    }
    else if (action == 10)
    {
        pPlayer->AddGossipMenuItem(9, "70 Attackpower",sender, 27971);
        pPlayer->AddGossipMenuItem(9, "35 Agility    ",sender, 27977);
        pPlayer->AddGossipMenuItem(9, "9 Damage      ",sender, 20030);
        pPlayer->AddGossipMenuItem(9, "<- Back       ",sender, 0    );
    }
    else if (action == 11)
    {
        pPlayer->AddGossipMenuItem(9, "Mongoose                  ", sender, 27984);
        pPlayer->AddGossipMenuItem(9, "Executioner               ", sender, 42974);
        pPlayer->AddGossipMenuItem(9, "20 Strength               ", sender, 27972);
        pPlayer->AddGossipMenuItem(9, "20 Agility                ", sender, 42620);
        pPlayer->AddGossipMenuItem(9, "40 Spelldamage            ", sender, 27975);
        pPlayer->AddGossipMenuItem(9, "81 Healing 27 Spelldamage ", sender, 34010);
        pPlayer->AddGossipMenuItem(9, "20 Spirit                 ", sender, 23803);
        pPlayer->AddGossipMenuItem(9, "7 Weapon Damage           ", sender, 27967);
        pPlayer->AddGossipMenuItem(9, "50 Arcane/Fire Spelldamage", sender, 27981);
        pPlayer->AddGossipMenuItem(9, "<- Back                   ", sender, 0    );
        pPlayer->AddGossipMenuItem(9, "Next ->                   ", sender, 13   );
    }
    else if (action == 12)
    {
        pPlayer->AddGossipMenuItem(9, "30 Intellect                              ", sender, 27968);
        pPlayer->AddGossipMenuItem(9, "Battlemaster                              ", sender, 28004);
        pPlayer->AddGossipMenuItem(9, "Lifestealing                              ", sender, 20032);
        pPlayer->AddGossipMenuItem(9, "Crusader                                  ", sender, 20034);
        pPlayer->AddGossipMenuItem(9, "Deathfrost                                ", sender, 46578);
        pPlayer->AddGossipMenuItem(9, "Fieryweapon                               ", sender, 13898);
        pPlayer->AddGossipMenuItem(9, "Icy Chill                                 ", sender, 20029);
        pPlayer->AddGossipMenuItem(9, "3% chance to restore 100 mana on spellcast", sender, 28003);
        pPlayer->AddGossipMenuItem(9, "-50% Disarm Duration 15 Parry             ", sender, 42687);
        pPlayer->AddGossipMenuItem(9, "54 Shadow/Frost Spelldamage               ", sender, 27982);
        pPlayer->AddGossipMenuItem(9, "<- Back                                   ", sender, 12   );
    }
    else if (action == 13)
    {
        pPlayer->AddGossipMenuItem(9, "28 Crit         ", sender, 30260);
        pPlayer->AddGossipMenuItem(9, "30 Hit          ", sender, 22779);
        pPlayer->AddGossipMenuItem(9, "12 Ranged Damage", sender, 30252);
        pPlayer->AddGossipMenuItem(9, "<- Back         ", sender, 0    );
    }
    else if (action == 14)
    {
        pPlayer->AddGossipMenuItem(9, "12 Resilience             ", sender, 44383);
        pPlayer->AddGossipMenuItem(9, "18 Stamina                ", sender, 34009);
        pPlayer->AddGossipMenuItem(9, "12 Intellect              ", sender, 27945);
        pPlayer->AddGossipMenuItem(9, "5 Resist All              ", sender, 27947);
        pPlayer->AddGossipMenuItem(9, "15 Shield Block           ", sender, 29454);
        pPlayer->AddGossipMenuItem(9, "9 Spirit                  ", sender, 20016);
        pPlayer->AddGossipMenuItem(9, "8 Frost Resistance        ", sender, 13933);
        pPlayer->AddGossipMenuItem(9, "30 Armor                  ", sender, 13464);
        pPlayer->AddGossipMenuItem(9, "Deal 26-38 damage on block", sender, 23530);
        pPlayer->AddGossipMenuItem(9, "<- Back                   ", sender, 0    );
    }
    else
    {
        pPlayer->EnchantItem(action,sender, cName);
        GossipHello_npc_enchant(pPlayer,pCreature);
    }

    if(pPlayer->PlayerTalkClass->GetGossipMenu().MenuItemCount() > 0)
        pPlayer->PlayerTalkClass->SendGossipMenu(1,pCreature->GetObjectGuid());

    return true;
}

void AddSC_npc_enchant()
{
    Script *newscript;

    newscript                   = new Script;
    newscript->Name             = "npc_enchant";
    newscript->pGossipHello     = &GossipHello_npc_enchant;
    newscript->pGossipSelect    = &GossipSelect_npc_enchant;
    newscript->RegisterSelf();
}