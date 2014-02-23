#include "Precompiled.h"

bool GossipHello_npc_enchant(Player *pPlayer, Creature *pCreature)
{
    if (pPlayer->isInCombat())
    {
        pPlayer->BothChat << sCustom.ChatNameWrapper(pCreature->GetName()) << " You are in combat!" << std::endl;
        return true;
    }

    if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_HEAD))
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Head      ", SLOT_HEAD, 1);

    if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_SHOULDERS))
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Shoulders ", SLOT_SHOULDERS, 2);

    if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_BACK))
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Back      ", SLOT_BACK, 3);

    if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_CHEST))
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Chest     ", SLOT_CHEST, 4);

    if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_WRISTS))
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Wrists    ", SLOT_WRISTS, 5);

    if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_HANDS))
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Hands     ", SLOT_HANDS, 6);

    if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_LEGS))
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Legs      ", SLOT_LEGS, 7);

    if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_FEET))
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Feet      ", SLOT_FEET, 8);

    if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_FINGER1))
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Finger one", SLOT_FINGER1, 9);

    if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_FINGER2))
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Finger two", SLOT_FINGER2, 9);

    if (Item* item = pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_MAIN_HAND))
    {
        if (item->GetProto()->InventoryType ==  INVTYPE_2HWEAPON)
            pPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Two hand  ", SLOT_MAIN_HAND, 10);
        else
            pPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Main hand ", SLOT_MAIN_HAND, 11);
    }
    if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_OFF_HAND))
    {
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Offhand   ", SLOT_OFF_HAND, 11);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Shield    ", SLOT_OFF_HAND, 14);
    }
    if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_RANGED))
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Ranged    ", SLOT_RANGED, 13);

    if (!pPlayer->PlayerTalkClass->GetGossipMenu().Empty())
        pPlayer->PlayerTalkClass->SendGossipMenu("Don't you feel enchanted a day like this?", pCreature->GetObjectGuid());
    return true;
}

bool GossipSelect_npc_enchant(Player *pPlayer, Creature *pCreature, uint32 sender, uint32 action)
{
    const char* cName = pCreature->GetName();
    pPlayer->PlayerTalkClass->CloseGossip();

    if (pPlayer->isInCombat())
    {
        pPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << " You are in combat!" << std::endl;
        return true;
    }
    else if (action == 0)
        GossipHello_npc_enchant(pPlayer,pCreature);
    else if (action == 1)
    {
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "34 Attackpower 16 Hit          ", sender, 35452);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "17 Strength16 Intellect        ", sender, 37891);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "22 Spelldamage 14 Hit          ", sender, 35447);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "35 Healing 12 Spelldamage 7 mp5", sender, 35445);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "18 Stamina 20 Resilience       ", sender, 35453);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "16 Defense 17 Dodge            ", sender, 35443);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Fire Resistance             ", sender, 35456);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Arcane Resistance           ", sender, 35455);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Shadow Resistance           ", sender, 35458);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Nature Resistance           ", sender, 35454);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Frost Resistance            ", sender, 35457);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "8 Resist All                   ", sender, 37889);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back                        ", sender, 0    );
    }
    else if (action == 2)
    {
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "30 Attackpower 10 Crit         ", sender, 35417);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "26 Attackpower 14 Crit         ", sender, 29483);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Attackpower 15 Crit         ", sender, 35439);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "18 Spelldamage 10 Crit         ", sender, 35406);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Spelldamage 14 Crit         ", sender, 29467);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Spelldamage 15 Crit         ", sender, 35437);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "33 Healing 11 Spelldamage 4 mp5", sender, 35404);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "31 Healing 11 Spelldamage 5 mp5", sender, 29475);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "22 Healing 6 mp5               ", sender, 35435);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "16 Stamina 100 Armor           ", sender, 29480);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "10 Dodge 15 Defense            ", sender, 35433);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Dodge 10 Defense            ", sender, 35402);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back                        ", sender, 0    );
    }
    else if (action == 3)
    {
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Agility          ", sender, 34004);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Spell Penetration", sender, 34003);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Fire Resistance  ", sender, 25081);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Arcane Resistance", sender, 34005);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Shadow Resistance", sender, 34006);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Nature Resistance", sender, 25082);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "7 Resist All        ", sender, 27962);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Dodge            ", sender, 25086);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Defense          ", sender, 47051);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "120 Armor           ", sender, 27961);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Increase stealth    ", sender, 25083);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back             ", sender, 0    );
    }
    else if (action == 4)
    {
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Resilience", sender, 33992);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "6 All Stats  ", sender, 27960);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Spirit    ", sender, 33990);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Defense   ", sender, 46594);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "150 Health   ", sender, 27957);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "150 Mana     ", sender, 27958);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back      ", sender, 0    );
    }
    else if (action == 5)
    {
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "24 Attackpower           ", sender, 34002);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Spelldamage           ", sender, 27917);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "30 Healing 10 Spelldamage", sender, 27911);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Stamina               ", sender, 27914);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Defense               ", sender, 27906);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Strength              ", sender, 27899);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Intellect             ", sender, 34001);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "4 All Stats              ", sender, 27905);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "6 mp5                    ", sender, 27913);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "9 Spirit                 ", sender, 20009);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back                  ", sender, 0    );
    }
    else if (action == 6)
    {
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "26 Attackpower           ", sender, 33996);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Strength              ", sender, 33995);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Agility               ", sender, 25080);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Spelldamage           ", sender, 33997);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "35 Healing 12 Spelldamage", sender, 33999);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Spell Hit             ", sender, 33994);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "10 Spell Crit            ", sender, 33993);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back                  ", sender, 0    );
    }
    else if (action == 7)
    {
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "50 Attackpower 12 Crit              ", sender, 35490);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "40 Stamina 12 Agility               ", sender, 35495);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "35 Spelldamage 20 Stamina           ", sender, 31372);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "66 Healing 22 Spelldamage 20 Stamina", sender, 31370);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back                             ", sender, 0    );
    }
    else if (action == 8)
    {
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Minor Speed 6 Agility      ", sender,34007 );
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Minor Speed 9 Stamina      ", sender,34008 );
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "5% Root/Snare Resist 10 Hit", sender,27954 );
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Agility                 ", sender,27951 );
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Stamina                 ", sender,27950 );
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "4 hp/mp5                   ", sender,27948 );
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back                    ", sender,0     );
    }
    else if (action == 9)
    {
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "4 All Stats             ", sender, 27927);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Spelldamage          ", sender, 27924);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Healing 7 Spelldamage", sender, 27926);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "2 Weapon Damage         ", sender, 27920);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back                 ", sender, 0    );
    }
    else if (action == 10)
    {
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "70 Attackpower",sender, 27971);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "35 Agility    ",sender, 27977);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "9 Damage      ",sender, 20030);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back       ",sender, 0    );
    }
    else if (action == 11)
    {
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Mongoose                  ", sender, 27984);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Executioner               ", sender, 42974);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Strength               ", sender, 27972);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Agility                ", sender, 42620);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "40 Spelldamage            ", sender, 27975);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "81 Healing 27 Spelldamage ", sender, 34010);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Spirit                 ", sender, 23803);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "7 Weapon Damage           ", sender, 27967);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "50 Arcane/Fire Spelldamage", sender, 27981);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back                   ", sender, 0    );
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Next ->                   ", sender, 13   );
    }
    else if (action == 12)
    {
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "30 Intellect                              ", sender, 27968);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Battlemaster                              ", sender, 28004);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Lifestealing                              ", sender, 20032);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Crusader                                  ", sender, 20034);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Deathfrost                                ", sender, 46578);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Fieryweapon                               ", sender, 13898);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Icy Chill                                 ", sender, 20029);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "3% chance to restore 100 mana on spellcast", sender, 28003);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "-50% Disarm Duration 15 Parry             ", sender, 42687);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "54 Shadow/Frost Spelldamage               ", sender, 27982);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back                                   ", sender, 12   );
    }
    else if (action == 13)
    {
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "28 Crit         ", sender, 30260);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "30 Hit          ", sender, 22779);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Ranged Damage", sender, 30252);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back         ", sender, 0    );
    }
    else if (action == 14)
    {
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Resilience             ", sender, 44383);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "18 Stamina                ", sender, 34009);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Intellect              ", sender, 27945);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "5 Resist All              ", sender, 27947);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Shield Block           ", sender, 29454);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "9 Spirit                  ", sender, 20016);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "8 Frost Resistance        ", sender, 13933);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "30 Armor                  ", sender, 13464);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "Deal 26-38 damage on block", sender, 23530);
        pPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back                   ", sender, 0    );
    }
    else
    {
        pPlayer->EnchantItem(action,sender, cName);
        GossipHello_npc_enchant(pPlayer,pCreature);
    }

    if(pPlayer->PlayerTalkClass->GetGossipMenu().MenuItemCount() > 0)
        pPlayer->PlayerTalkClass->SendGossipMenu("Don't you feel enchanted a day like this?", pCreature->GetObjectGuid());

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