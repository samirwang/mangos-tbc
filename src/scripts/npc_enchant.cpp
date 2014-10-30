#include "Precompiled.h"
#include "CPlayer.h"

enum
{
    CAT_MENU,
    CAT_ENCH,
};

class npc_enchant : public CreatureScript
{
public:
    npc_enchant() : CreatureScript("npc_enchant") {}

    /*
    Position 0 = category

    If category = CAT_MENU
    Position 1 = menu ID
    Position 2 = item POS
    If category = CAT_ENCH
    Position 1 = item POS
    Position 2 = spell ID
    */

    bool GossipHello(Player* pPlayer, Creature* pCreature) override
    {
        CPlayer* pCPlayer = pPlayer->ToCPlayer();

        if (pPlayer->isInCombat())
        {
            pCPlayer->BothChat << sCustom.ChatNameWrapper(pCreature->GetName()) << " You are in combat!" << std::endl;
            return true;
        }

        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_HEAD))
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Head", {CAT_MENU, 2, SLOT_HEAD});

        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_SHOULDERS))
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Shoulders", { CAT_MENU, 3, SLOT_SHOULDERS });

        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_BACK))
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Back", { CAT_MENU, 4, SLOT_BACK });

        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_CHEST))
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Chest", { CAT_MENU, 5, SLOT_CHEST });

        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_WRISTS))
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Wrists", { CAT_MENU, 6, SLOT_WRISTS });

        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_HANDS))
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Hands", { CAT_MENU, 7, SLOT_HANDS });

        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_LEGS))
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Legs", { CAT_MENU, 8, SLOT_LEGS });

        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_FEET))
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Feet", { CAT_MENU, 9, SLOT_FEET });

        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_FINGER1))
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Finger one", { CAT_MENU, 10, SLOT_FINGER1 });

        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_FINGER2))
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Finger two", { CAT_MENU, 10, SLOT_FINGER2 });

        if (Item* item = pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_MAIN_HAND))
        {
            if (item->GetProto()->InventoryType == INVTYPE_2HWEAPON)
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Two hand", { CAT_MENU, 11, SLOT_MAIN_HAND });
            else
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Main hand", { CAT_MENU, 12, SLOT_MAIN_HAND });
        }
        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_OFF_HAND))
        {
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Offhand", { CAT_MENU, 13, SLOT_OFF_HAND });
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Shield", { CAT_MENU, 14, SLOT_OFF_HAND });
        }
        if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, SLOT_RANGED))
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Enchant Ranged", { CAT_MENU, 15, SLOT_RANGED });

        if (!pPlayer->PlayerTalkClass->GetGossipMenu().Empty())
            pPlayer->PlayerTalkClass->SendGossipMenu("Don't you feel enchanted a day like this?", pCreature->GetObjectGuid());
        else
            pPlayer->PlayerTalkClass->SendGossipMenu("I've got nothing for you!", pCreature->GetObjectGuid());

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
            uint32 itemPOS = actionlist[2];

            if (menuID == 1)
                GossipHello(pPlayer, pCreature);
            else if (menuID == 2)
            {
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "34 Attackpower 16 Hit", { CAT_ENCH, itemPOS, 35452 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "17 Strength16 Intellect", { CAT_ENCH, itemPOS, 37891 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "22 Spelldamage 14 Hit", { CAT_ENCH, itemPOS, 35447 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "35 Healing 12 Spelldamage 7 mp5", { CAT_ENCH, itemPOS, 35445 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "18 Stamina 20 Resilience", { CAT_ENCH, itemPOS, 35453 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "16 Defense 17 Dodge", { CAT_ENCH, itemPOS, 35443 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Fire Resistance", { CAT_ENCH, itemPOS, 35456 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Arcane Resistance", { CAT_ENCH, itemPOS, 35455 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Shadow Resistance", { CAT_ENCH, itemPOS, 35458 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Nature Resistance", { CAT_ENCH, itemPOS, 35454 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Frost Resistance", { CAT_ENCH, itemPOS, 35457 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "8 Resist All", { CAT_ENCH, itemPOS, 37889 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back", { CAT_MENU, 1, itemPOS });
            }
            else if (menuID == 3)
            {
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "30 Attackpower 10 Crit", { CAT_ENCH, itemPOS, 35417 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "26 Attackpower 14 Crit", { CAT_ENCH, itemPOS, 29483 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Attackpower 15 Crit", { CAT_ENCH, itemPOS, 35439 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "18 Spelldamage 10 Crit", { CAT_ENCH, itemPOS, 35406 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Spelldamage 14 Crit", { CAT_ENCH, itemPOS, 29467 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Spelldamage 15 Crit", { CAT_ENCH, itemPOS, 35437 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "33 Healing 11 Spelldamage 4 mp5", { CAT_ENCH, itemPOS, 35404 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "31 Healing 11 Spelldamage 5 mp5", { CAT_ENCH, itemPOS, 29475 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "22 Healing 6 mp5", { CAT_ENCH, itemPOS, 35435 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "16 Stamina 100 Armor", { CAT_ENCH, itemPOS, 29480 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "10 Dodge 15 Defense", { CAT_ENCH, itemPOS, 35433 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Dodge 10 Defense", { CAT_ENCH, itemPOS, 35402 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back", { CAT_MENU, 1, itemPOS });
            }
            else if (menuID == 4)
            {
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Agility", { CAT_ENCH, itemPOS, 34004 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Spell Penetration", { CAT_ENCH, itemPOS, 34003 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Fire Resistance  ", { CAT_ENCH, itemPOS, 25081 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Arcane Resistance", { CAT_ENCH, itemPOS, 34005 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Shadow Resistance", { CAT_ENCH, itemPOS, 34006 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Nature Resistance", { CAT_ENCH, itemPOS, 25082 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "7 Resist All", { CAT_ENCH, itemPOS, 27962 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Dodge", { CAT_ENCH, itemPOS, 25086 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Defense", { CAT_ENCH, itemPOS, 47051 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "120 Armor", { CAT_ENCH, itemPOS, 27961 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Increase stealth", { CAT_ENCH, itemPOS, 25083 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back", { CAT_MENU, 1, itemPOS });
            }
            else if (menuID == 5)
            {
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Resilience", { CAT_ENCH, itemPOS, 33992 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "6 All Stats", { CAT_ENCH, itemPOS, 27960 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Spirit", { CAT_ENCH, itemPOS, 33990 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Defense", { CAT_ENCH, itemPOS, 46594 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "150 Health", { CAT_ENCH, itemPOS, 27957 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "150 Mana", { CAT_ENCH, itemPOS, 27958 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back", { CAT_MENU, 1, itemPOS });
            }
            else if (menuID == 6)
            {
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "24 Attackpower", { CAT_ENCH, itemPOS, 34002 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Spelldamage", { CAT_ENCH, itemPOS, 27917 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "30 Healing 10 Spelldamage", { CAT_ENCH, itemPOS, 27911 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Stamina", { CAT_ENCH, itemPOS, 27914 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Defense", { CAT_ENCH, itemPOS, 27906 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Strength", { CAT_ENCH, itemPOS, 27899 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Intellect", { CAT_ENCH, itemPOS, 34001 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "4 All Stats", { CAT_ENCH, itemPOS, 27905 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "6 mp5", { CAT_ENCH, itemPOS, 27913 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "9 Spirit", { CAT_ENCH, itemPOS, 20009 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back", { CAT_MENU, 1, itemPOS });
            }
            else if (menuID == 7)
            {
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "26 Attackpower", { CAT_ENCH, itemPOS, 33996 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Strength", { CAT_ENCH, itemPOS, 33995 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Agility", { CAT_ENCH, itemPOS, 25080 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Spelldamage", { CAT_ENCH, itemPOS, 33997 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "35 Healing 12 Spelldamage", { CAT_ENCH, itemPOS, 33999 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Spell Hit", { CAT_ENCH, itemPOS, 33994 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "10 Spell Crit", { CAT_ENCH, itemPOS, 33993 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back", { CAT_MENU, 1, itemPOS });
            }
            else if (menuID == 8)
            {
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "50 Attackpower 12 Crit", { CAT_ENCH, itemPOS, 35490 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "40 Stamina 12 Agility", { CAT_ENCH, itemPOS, 35495 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "35 Spelldamage 20 Stamina", { CAT_ENCH, itemPOS, 31372 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "66 Healing 22 Spelldamage 20 Stamina", { CAT_ENCH, itemPOS, 31370 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back", { CAT_MENU, 1, itemPOS });
            }
            else if (menuID == 9)
            {
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Minor Speed 6 Agility", { CAT_ENCH, itemPOS, 34007 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Minor Speed 9 Stamina", { CAT_ENCH, itemPOS, 34008 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "5% Root/Snare Resist 10 Hit", { CAT_ENCH, itemPOS, 27954 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Agility", { CAT_ENCH, itemPOS, 27951 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Stamina", { CAT_ENCH, itemPOS, 27950 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "4 hp/mp5", { CAT_ENCH, itemPOS, 27948 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back", { CAT_MENU, 1, itemPOS });
            }
            else if (menuID == 10)
            {
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "4 All Stats", { CAT_ENCH, itemPOS, 27927 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Spelldamage", { CAT_ENCH, itemPOS, 27924 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Healing 7 Spelldamage", { CAT_ENCH, itemPOS, 27926 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "2 Weapon Damage", { CAT_ENCH, itemPOS, 27920 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back", { CAT_MENU, 1, itemPOS });
            }
            else if (menuID == 11)
            {
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "70 Attackpower", { CAT_ENCH, itemPOS, 27971 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "35 Agility", { CAT_ENCH, itemPOS, 27977 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "9 Damage", { CAT_ENCH, itemPOS, 20030 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back", { CAT_MENU, 1, itemPOS });
            }
            else if (menuID == 12)
            {
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Mongoose", { CAT_ENCH, itemPOS, 27984 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Executioner", { CAT_ENCH, itemPOS, 42974 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Strength", { CAT_ENCH, itemPOS, 27972 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Agility", { CAT_ENCH, itemPOS, 42620 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "40 Spelldamage", { CAT_ENCH, itemPOS, 27975 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "81 Healing 27 Spelldamage ", { CAT_ENCH, itemPOS, 34010 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "20 Spirit", { CAT_ENCH, itemPOS, 23803 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "7 Weapon Damage", { CAT_ENCH, itemPOS, 27967 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "50 Arcane/Fire Spelldamage", { CAT_ENCH, itemPOS, 27981 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back", { CAT_MENU, 1, itemPOS });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Next ->", { CAT_MENU, 13, itemPOS });
            }
            else if (menuID == 13)
            {
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "30 Intellect", { CAT_ENCH, itemPOS, 27968 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Battlemaster", { CAT_ENCH, itemPOS, 28004 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Lifestealing", { CAT_ENCH, itemPOS, 20032 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Crusader", { CAT_ENCH, itemPOS, 20034 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Deathfrost", { CAT_ENCH, itemPOS, 46578 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Fieryweapon", { CAT_ENCH, itemPOS, 13898 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Icy Chill", { CAT_ENCH, itemPOS, 20029 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "3% chance to restore 100 mana on spellcast", { CAT_ENCH, itemPOS, 28003 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "-50% Disarm Duration 15 Parry", { CAT_ENCH, itemPOS, 42687 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "54 Shadow/Frost Spelldamage", { CAT_ENCH, itemPOS, 27982 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back", { CAT_MENU, 12, itemPOS });
            }
            else if (menuID == 14)
            {
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "28 Crit", { CAT_ENCH, itemPOS, 30260 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "30 Hit", { CAT_ENCH, itemPOS, 22779 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Ranged Damage", { CAT_ENCH, itemPOS, 30252 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back", { CAT_MENU, 1, itemPOS });
            }
            else if (menuID == 15)
            {
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Resilience", { CAT_ENCH, itemPOS, 44383 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "18 Stamina", { CAT_ENCH, itemPOS, 34009 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "12 Intellect", { CAT_ENCH, itemPOS, 27945 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "5 Resist All", { CAT_ENCH, itemPOS, 27947 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "15 Shield Block", { CAT_ENCH, itemPOS, 29454 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "9 Spirit", { CAT_ENCH, itemPOS, 20016 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "8 Frost Resistance", { CAT_ENCH, itemPOS, 13933 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "30 Armor", { CAT_ENCH, itemPOS, 13464 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Deal 26-38 damage on block", { CAT_ENCH, itemPOS, 23530 });
                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "<- Back", { CAT_MENU, 1, itemPOS });
            }
        }
        else if (category == CAT_ENCH)
        {
            uint32 itemPOS = actionlist[1];
            uint32 spellID = actionlist[2];

            if (pCPlayer->EnchantItem(spellID, itemPOS))
                pCPlayer->BoxChat << "Your item was successfully enchanted" << std::endl;

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