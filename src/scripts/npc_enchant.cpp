#include "Precompiled.h"
#include "CPlayer.h"

class npc_enchant : public CreatureScript
{
public:
    npc_enchant() : CreatureScript("npc_enchant") {}

    bool GossipHello(Player* pPlayer, Creature* pCreature) override
    {
        CCPlayer* pCCPlayer = pPlayer->GetCCPlayer();

        if (pPlayer->isInCombat())
        {
            pCCPlayer->BothChat << sCustom.ChatNameWrapper(pCreature->GetName()) << " You are in combat!" << std::endl;
            return true;
        }

        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_HEAD))
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Head      ", SLOT_HEAD, 1);

        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_SHOULDERS))
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Shoulders ", SLOT_SHOULDERS, 2);

        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_BACK))
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Back      ", SLOT_BACK, 3);

        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_CHEST))
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Chest     ", SLOT_CHEST, 4);

        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_WRISTS))
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Wrists    ", SLOT_WRISTS, 5);

        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_HANDS))
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Hands     ", SLOT_HANDS, 6);

        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_LEGS))
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Legs      ", SLOT_LEGS, 7);

        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_FEET))
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Feet      ", SLOT_FEET, 8);

        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_FINGER1))
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Finger one", SLOT_FINGER1, 9);

        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_FINGER2))
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Finger two", SLOT_FINGER2, 9);

        if (Item* item = pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_MAIN_HAND))
        {
            if (item->GetProto()->InventoryType == INVTYPE_2HWEAPON)
                pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Two hand  ", SLOT_MAIN_HAND, 10);
            else
                pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Main hand ", SLOT_MAIN_HAND, 11);
        }
        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_OFF_HAND))
        {
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Offhand   ", SLOT_OFF_HAND, 11);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Shield    ", SLOT_OFF_HAND, 14);
        }
        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_RANGED))
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Ranged    ", SLOT_RANGED, 13);

        if (!pPlayer->PlayerTalkClass->GetGossipMenu().Empty())
            pPlayer->PlayerTalkClass->SendGossipMenu("Don't you feel enchanted a day like this?", pCreature->GetObjectGuid());
        else
            pPlayer->PlayerTalkClass->SendGossipMenu("I've got nothing for you!", pCreature->GetObjectGuid());

        return true;
    }

    bool GossipSelect(Player *pPlayer, Creature *pCreature, uint32 sender, uint32 action, std::string code) override
    {
        CCPlayer* pCCPlayer = pPlayer->GetCCPlayer();

        const char* cName = pCreature->GetName();
        pPlayer->PlayerTalkClass->CloseGossip();

        if (pPlayer->isInCombat())
        {
            pCCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << " You are in combat!" << std::endl;
            return true;
        }
        else if (action == 0)
            GossipHello(pPlayer, pCreature);
        else if (action == 1)
        {
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "34 Attackpower 16 Hit          ", sender, 35452);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "17 Strength16 Intellect        ", sender, 37891);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "22 Spelldamage 14 Hit          ", sender, 35447);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "35 Healing 12 Spelldamage 7 mp5", sender, 35445);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "18 Stamina 20 Resilience       ", sender, 35453);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "16 Defense 17 Dodge            ", sender, 35443);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Fire Resistance             ", sender, 35456);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Arcane Resistance           ", sender, 35455);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Shadow Resistance           ", sender, 35458);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Nature Resistance           ", sender, 35454);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Frost Resistance            ", sender, 35457);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "8 Resist All                   ", sender, 37889);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back                        ", sender, 0);
        }
        else if (action == 2)
        {
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "30 Attackpower 10 Crit         ", sender, 35417);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "26 Attackpower 14 Crit         ", sender, 29483);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Attackpower 15 Crit         ", sender, 35439);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "18 Spelldamage 10 Crit         ", sender, 35406);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Spelldamage 14 Crit         ", sender, 29467);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Spelldamage 15 Crit         ", sender, 35437);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "33 Healing 11 Spelldamage 4 mp5", sender, 35404);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "31 Healing 11 Spelldamage 5 mp5", sender, 29475);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "22 Healing 6 mp5               ", sender, 35435);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "16 Stamina 100 Armor           ", sender, 29480);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "10 Dodge 15 Defense            ", sender, 35433);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Dodge 10 Defense            ", sender, 35402);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back                        ", sender, 0);
        }
        else if (action == 3)
        {
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Agility          ", sender, 34004);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Spell Penetration", sender, 34003);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Fire Resistance  ", sender, 25081);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Arcane Resistance", sender, 34005);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Shadow Resistance", sender, 34006);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Nature Resistance", sender, 25082);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "7 Resist All        ", sender, 27962);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Dodge            ", sender, 25086);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Defense          ", sender, 47051);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "120 Armor           ", sender, 27961);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Increase stealth    ", sender, 25083);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back             ", sender, 0);
        }
        else if (action == 4)
        {
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Resilience", sender, 33992);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "6 All Stats  ", sender, 27960);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Spirit    ", sender, 33990);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Defense   ", sender, 46594);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "150 Health   ", sender, 27957);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "150 Mana     ", sender, 27958);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back      ", sender, 0);
        }
        else if (action == 5)
        {
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "24 Attackpower           ", sender, 34002);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Spelldamage           ", sender, 27917);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "30 Healing 10 Spelldamage", sender, 27911);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Stamina               ", sender, 27914);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Defense               ", sender, 27906);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Strength              ", sender, 27899);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Intellect             ", sender, 34001);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "4 All Stats              ", sender, 27905);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "6 mp5                    ", sender, 27913);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "9 Spirit                 ", sender, 20009);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back                  ", sender, 0);
        }
        else if (action == 6)
        {
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "26 Attackpower           ", sender, 33996);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Strength              ", sender, 33995);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Agility               ", sender, 25080);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Spelldamage           ", sender, 33997);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "35 Healing 12 Spelldamage", sender, 33999);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Spell Hit             ", sender, 33994);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "10 Spell Crit            ", sender, 33993);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back                  ", sender, 0);
        }
        else if (action == 7)
        {
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "50 Attackpower 12 Crit              ", sender, 35490);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "40 Stamina 12 Agility               ", sender, 35495);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "35 Spelldamage 20 Stamina           ", sender, 31372);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "66 Healing 22 Spelldamage 20 Stamina", sender, 31370);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back                             ", sender, 0);
        }
        else if (action == 8)
        {
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Minor Speed 6 Agility      ", sender, 34007);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Minor Speed 9 Stamina      ", sender, 34008);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "5% Root/Snare Resist 10 Hit", sender, 27954);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Agility                 ", sender, 27951);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Stamina                 ", sender, 27950);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "4 hp/mp5                   ", sender, 27948);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back                    ", sender, 0);
        }
        else if (action == 9)
        {
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "4 All Stats             ", sender, 27927);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Spelldamage          ", sender, 27924);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Healing 7 Spelldamage", sender, 27926);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "2 Weapon Damage         ", sender, 27920);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back                 ", sender, 0);
        }
        else if (action == 10)
        {
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "70 Attackpower", sender, 27971);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "35 Agility    ", sender, 27977);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "9 Damage      ", sender, 20030);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back       ", sender, 0);
        }
        else if (action == 11)
        {
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Mongoose                  ", sender, 27984);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Executioner               ", sender, 42974);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Strength               ", sender, 27972);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Agility                ", sender, 42620);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "40 Spelldamage            ", sender, 27975);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "81 Healing 27 Spelldamage ", sender, 34010);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Spirit                 ", sender, 23803);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "7 Weapon Damage           ", sender, 27967);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "50 Arcane/Fire Spelldamage", sender, 27981);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back                   ", sender, 0);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Next ->                   ", sender, 13);
        }
        else if (action == 12)
        {
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "30 Intellect                              ", sender, 27968);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Battlemaster                              ", sender, 28004);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Lifestealing                              ", sender, 20032);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Crusader                                  ", sender, 20034);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Deathfrost                                ", sender, 46578);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Fieryweapon                               ", sender, 13898);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Icy Chill                                 ", sender, 20029);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "3% chance to restore 100 mana on spellcast", sender, 28003);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "-50% Disarm Duration 15 Parry             ", sender, 42687);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "54 Shadow/Frost Spelldamage               ", sender, 27982);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back                                   ", sender, 12);
        }
        else if (action == 13)
        {
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "28 Crit         ", sender, 30260);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "30 Hit          ", sender, 22779);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Ranged Damage", sender, 30252);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back         ", sender, 0);
        }
        else if (action == 14)
        {
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Resilience             ", sender, 44383);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "18 Stamina                ", sender, 34009);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Intellect              ", sender, 27945);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "5 Resist All              ", sender, 27947);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Shield Block           ", sender, 29454);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "9 Spirit                  ", sender, 20016);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "8 Frost Resistance        ", sender, 13933);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "30 Armor                  ", sender, 13464);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "Deal 26-38 damage on block", sender, 23530);
            pCCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back                   ", sender, 0);
        }
        else
        {
            pCCPlayer->EnchantItem(action, sender, cName);
            GossipHello(pPlayer, pCreature);
        }

        if (pPlayer->PlayerTalkClass->GetGossipMenu().MenuItemCount() > 0)
            pPlayer->PlayerTalkClass->SendGossipMenu("Don't you feel enchanted a day like this?", pCreature->GetObjectGuid());

        return true;
    }
};


void AddSC_npc_enchant()
{
    new npc_enchant;
}