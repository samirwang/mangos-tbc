#include "Precompiled.h"
#include "NewPlayer.h"

class npc_enchant : public CreatureScript
{
public:
    npc_enchant() : CreatureScript("npc_enchant") {}

    bool GossipHello(Player* pPlayer, Creature* pCreature) override
    {
        CPlayer* pCPlayer = pPlayer->ToCPlayer();

        if (pPlayer->isInCombat())
        {
            pCPlayer->BothChat << sCustom.ChatNameWrapper(pCreature->GetName()) << " You are in combat!" << std::endl;
            return true;
        }

        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_HEAD))
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Head      ", SLOT_HEAD, 1);

        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_SHOULDERS))
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Shoulders ", SLOT_SHOULDERS, 2);

        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_BACK))
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Back      ", SLOT_BACK, 3);

        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_CHEST))
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Chest     ", SLOT_CHEST, 4);

        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_WRISTS))
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Wrists    ", SLOT_WRISTS, 5);

        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_HANDS))
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Hands     ", SLOT_HANDS, 6);

        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_LEGS))
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Legs      ", SLOT_LEGS, 7);

        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_FEET))
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Feet      ", SLOT_FEET, 8);

        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_FINGER1))
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Finger one", SLOT_FINGER1, 9);

        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_FINGER2))
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Finger two", SLOT_FINGER2, 9);

        if (Item* item = pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_MAIN_HAND))
        {
            if (item->GetProto()->InventoryType == INVTYPE_2HWEAPON)
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Two hand  ", SLOT_MAIN_HAND, 10);
            else
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Main hand ", SLOT_MAIN_HAND, 11);
        }
        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_OFF_HAND))
        {
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Offhand   ", SLOT_OFF_HAND, 11);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Shield    ", SLOT_OFF_HAND, 14);
        }
        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_RANGED))
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Ranged    ", SLOT_RANGED, 13);

        if (!pPlayer->PlayerTalkClass->GetGossipMenu().Empty())
            pPlayer->PlayerTalkClass->SendGossipMenu("Don't you feel enchanted a day like this?", pCreature->GetObjectGuid());
        else
            pPlayer->PlayerTalkClass->SendGossipMenu("I've got nothing for you!", pCreature->GetObjectGuid());

        return true;
    }

    bool GossipSelect(Player *pPlayer, Creature *pCreature, uint32 sender, uint32 action, std::string code) override
    {
        CPlayer* pCPlayer = pPlayer->ToCPlayer();

        const char* cName = pCreature->GetName();
        pPlayer->PlayerTalkClass->CloseGossip();

        if (pPlayer->isInCombat())
        {
            pCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << " You are in combat!" << std::endl;
            return true;
        }
        else if (action == 0)
            GossipHello(pPlayer, pCreature);
        else if (action == 1)
        {
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "34 Attackpower 16 Hit          ", sender, 35452);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "17 Strength16 Intellect        ", sender, 37891);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "22 Spelldamage 14 Hit          ", sender, 35447);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "35 Healing 12 Spelldamage 7 mp5", sender, 35445);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "18 Stamina 20 Resilience       ", sender, 35453);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "16 Defense 17 Dodge            ", sender, 35443);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Fire Resistance             ", sender, 35456);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Arcane Resistance           ", sender, 35455);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Shadow Resistance           ", sender, 35458);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Nature Resistance           ", sender, 35454);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Frost Resistance            ", sender, 35457);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "8 Resist All                   ", sender, 37889);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back                        ", sender, 0);
        }
        else if (action == 2)
        {
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "30 Attackpower 10 Crit         ", sender, 35417);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "26 Attackpower 14 Crit         ", sender, 29483);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Attackpower 15 Crit         ", sender, 35439);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "18 Spelldamage 10 Crit         ", sender, 35406);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Spelldamage 14 Crit         ", sender, 29467);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Spelldamage 15 Crit         ", sender, 35437);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "33 Healing 11 Spelldamage 4 mp5", sender, 35404);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "31 Healing 11 Spelldamage 5 mp5", sender, 29475);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "22 Healing 6 mp5               ", sender, 35435);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "16 Stamina 100 Armor           ", sender, 29480);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "10 Dodge 15 Defense            ", sender, 35433);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Dodge 10 Defense            ", sender, 35402);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back                        ", sender, 0);
        }
        else if (action == 3)
        {
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Agility          ", sender, 34004);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Spell Penetration", sender, 34003);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Fire Resistance  ", sender, 25081);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Arcane Resistance", sender, 34005);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Shadow Resistance", sender, 34006);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Nature Resistance", sender, 25082);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "7 Resist All        ", sender, 27962);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Dodge            ", sender, 25086);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Defense          ", sender, 47051);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "120 Armor           ", sender, 27961);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Increase stealth    ", sender, 25083);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back             ", sender, 0);
        }
        else if (action == 4)
        {
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Resilience", sender, 33992);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "6 All Stats  ", sender, 27960);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Spirit    ", sender, 33990);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Defense   ", sender, 46594);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "150 Health   ", sender, 27957);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "150 Mana     ", sender, 27958);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back      ", sender, 0);
        }
        else if (action == 5)
        {
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "24 Attackpower           ", sender, 34002);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Spelldamage           ", sender, 27917);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "30 Healing 10 Spelldamage", sender, 27911);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Stamina               ", sender, 27914);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Defense               ", sender, 27906);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Strength              ", sender, 27899);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Intellect             ", sender, 34001);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "4 All Stats              ", sender, 27905);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "6 mp5                    ", sender, 27913);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "9 Spirit                 ", sender, 20009);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back                  ", sender, 0);
        }
        else if (action == 6)
        {
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "26 Attackpower           ", sender, 33996);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Strength              ", sender, 33995);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Agility               ", sender, 25080);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Spelldamage           ", sender, 33997);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "35 Healing 12 Spelldamage", sender, 33999);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Spell Hit             ", sender, 33994);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "10 Spell Crit            ", sender, 33993);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back                  ", sender, 0);
        }
        else if (action == 7)
        {
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "50 Attackpower 12 Crit              ", sender, 35490);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "40 Stamina 12 Agility               ", sender, 35495);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "35 Spelldamage 20 Stamina           ", sender, 31372);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "66 Healing 22 Spelldamage 20 Stamina", sender, 31370);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back                             ", sender, 0);
        }
        else if (action == 8)
        {
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Minor Speed 6 Agility      ", sender, 34007);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Minor Speed 9 Stamina      ", sender, 34008);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "5% Root/Snare Resist 10 Hit", sender, 27954);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Agility                 ", sender, 27951);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Stamina                 ", sender, 27950);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "4 hp/mp5                   ", sender, 27948);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back                    ", sender, 0);
        }
        else if (action == 9)
        {
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "4 All Stats             ", sender, 27927);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Spelldamage          ", sender, 27924);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Healing 7 Spelldamage", sender, 27926);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "2 Weapon Damage         ", sender, 27920);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back                 ", sender, 0);
        }
        else if (action == 10)
        {
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "70 Attackpower", sender, 27971);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "35 Agility    ", sender, 27977);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "9 Damage      ", sender, 20030);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back       ", sender, 0);
        }
        else if (action == 11)
        {
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Mongoose                  ", sender, 27984);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Executioner               ", sender, 42974);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Strength               ", sender, 27972);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Agility                ", sender, 42620);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "40 Spelldamage            ", sender, 27975);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "81 Healing 27 Spelldamage ", sender, 34010);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Spirit                 ", sender, 23803);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "7 Weapon Damage           ", sender, 27967);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "50 Arcane/Fire Spelldamage", sender, 27981);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back                   ", sender, 0);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Next ->                   ", sender, 13);
        }
        else if (action == 12)
        {
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "30 Intellect                              ", sender, 27968);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Battlemaster                              ", sender, 28004);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Lifestealing                              ", sender, 20032);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Crusader                                  ", sender, 20034);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Deathfrost                                ", sender, 46578);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Fieryweapon                               ", sender, 13898);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Icy Chill                                 ", sender, 20029);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "3% chance to restore 100 mana on spellcast", sender, 28003);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "-50% Disarm Duration 15 Parry             ", sender, 42687);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "54 Shadow/Frost Spelldamage               ", sender, 27982);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back                                   ", sender, 12);
        }
        else if (action == 13)
        {
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "28 Crit         ", sender, 30260);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "30 Hit          ", sender, 22779);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Ranged Damage", sender, 30252);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back         ", sender, 0);
        }
        else if (action == 14)
        {
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Resilience             ", sender, 44383);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "18 Stamina                ", sender, 34009);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Intellect              ", sender, 27945);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "5 Resist All              ", sender, 27947);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Shield Block           ", sender, 29454);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "9 Spirit                  ", sender, 20016);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "8 Frost Resistance        ", sender, 13933);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "30 Armor                  ", sender, 13464);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Deal 26-38 damage on block", sender, 23530);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back                   ", sender, 0);
        }
        else
        {
            pCPlayer->EnchantItem(action, sender, cName);
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