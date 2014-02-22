#include "PlayerGossip.h"
#include "Player.h"
#include "Settings.h"
#include "Custom.h"

PlayerGossip::PlayerGossip(Player* pPlayer)
{
    m_player = pPlayer;
}

void PlayerGossip::PlayerGossipHello(uint32 sender)
{
    if (sender == GOSSIP_SENDER_FIRSTLOGIN)
    {
        if (!m_player->GetSettings()->GetSetting(SETTING_UINT_HIDETEMPLATEMENU))
        {
            if (m_player->getClass() == CLASS_WARRIOR)
            {
                m_player->AddGossipMenuItem(Icon::BAG, "I want to play Arms", sender, 1);
                m_player->AddGossipMenuItem(Icon::BAG, "I want to play Fury", sender, 2);
                m_player->AddGossipMenuItem(Icon::BAG, "I want to play Protection", sender, 3);
            }
            else if (m_player->getClass() == CLASS_PALADIN)
            {
                m_player->AddGossipMenuItem(Icon::BAG, "I want to play Holy", sender, 4);
                m_player->AddGossipMenuItem(Icon::BAG, "I want to play Protection", sender, 5);
                m_player->AddGossipMenuItem(Icon::BAG, "I want to play Retribution", sender, 6);
            }
            else if (m_player->getClass() == CLASS_HUNTER)
            {
                m_player->AddGossipMenuItem(Icon::BAG, "I want to play Beast Mastery", sender, 7);
                m_player->AddGossipMenuItem(Icon::BAG, "I want to play Marksmanship", sender, 8);
                m_player->AddGossipMenuItem(Icon::BAG, "I want to play Survival", sender, 9);
            }
            else if (m_player->getClass() == CLASS_ROGUE)
            {
                m_player->AddGossipMenuItem(Icon::BAG, "I want to play Assassination", sender, 10);
                m_player->AddGossipMenuItem(Icon::BAG, "I want to play Combat", sender, 11);
                m_player->AddGossipMenuItem(Icon::BAG, "I want to play Subtlety", sender, 12);
            }
            else if (m_player->getClass() == CLASS_PRIEST)
            {
                m_player->AddGossipMenuItem(Icon::BAG, "I want to play Discipline", sender, 13);
                m_player->AddGossipMenuItem(Icon::BAG, "I want to play Holy", sender, 14);
                m_player->AddGossipMenuItem(Icon::BAG, "I want to play Shadow", sender, 15);
            }
            else if (m_player->getClass() == CLASS_SHAMAN)
            {
                m_player->AddGossipMenuItem(Icon::BAG, "I want to play Elemental", sender, 16);
                m_player->AddGossipMenuItem(Icon::BAG, "I want to play Enhancement", sender, 17);
                m_player->AddGossipMenuItem(Icon::BAG, "I want to play Restoration", sender, 18);
            }
            else if (m_player->getClass() == CLASS_MAGE)
            {
                m_player->AddGossipMenuItem(Icon::BAG, "I want to play Arcane", sender, 19);
                m_player->AddGossipMenuItem(Icon::BAG, "I want to play Fire", sender, 20);
                m_player->AddGossipMenuItem(Icon::BAG, "I want to play Frost", sender, 21);
            }
            else if (m_player->getClass() == CLASS_WARLOCK)
            {
                m_player->AddGossipMenuItem(Icon::BAG, "I want to play Affliction", sender, 22);
                m_player->AddGossipMenuItem(Icon::BAG, "I want to play Demonology", sender, 23);
                m_player->AddGossipMenuItem(Icon::BAG, "I want to play Destruction", sender, 24);
            }
            else if (m_player->getClass() == CLASS_DRUID)
            {
                m_player->AddGossipMenuItem(Icon::BAG, "I want to play Balance", sender, 25);
                m_player->AddGossipMenuItem(Icon::BAG, "I want to play Feral", sender, 26);
                m_player->AddGossipMenuItem(Icon::BAG, "I want to play Restoration", sender, 27);
            }

            m_player->AddGossipMenuItem(Icon::GEAR, "Do not show this again", sender, 30);

            ObjectGuid guid = m_player->GetObjectGuid();

            for (uint8 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; ++i)
            if (Item* pItem = m_player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
            if (pItem->GetEntry() == 6948) // Hearthstone
                guid = pItem->GetObjectGuid();

            if (!m_player->PlayerTalkClass->GetGossipMenu().Empty())
                m_player->PlayerTalkClass->SendGossipMenu("Please select your spec!", guid);
        }
    }
}

void PlayerGossip::PlayerGossipSelect(uint32 sender, uint32 action, std::string code)
{
    m_player->PlayerTalkClass->CloseGossip();

    if (sender == GOSSIP_SENDER_FIRSTLOGIN)
    {
        // Warrior
        if (action == 1)
        { // Warrior - Arms
            m_player->AddItemSet(750);
            m_player->LearnTalentTemplate(1);
        }
        else if (action == 2)
        { // Warrior - Fury
            m_player->AddItemSet(750);
            m_player->LearnTalentTemplate(2);
        }
        else if (action == 3)
        { // Warrior - Protection
            m_player->AddItemSet(750);
            m_player->LearnTalentTemplate(3);
        }
        // Paladin
        else if (action == 4)
        { // Paladin - Holy
            m_player->AddItemSet(751);
            m_player->LearnTalentTemplate(1);
        }
        else if (action == 5)
        { // Paladin - Protection
            m_player->AddItemSet(752);
            m_player->LearnTalentTemplate(2);
        }
        else if (action == 6)
        { // Paladin - Retribution
            m_player->AddItemSet(752);
            m_player->LearnTalentTemplate(3);
        }
        // Hunter
        else if (action == 7)
        { // Hunter - Beast Mastery
            m_player->AddItemSet(749);
            m_player->LearnTalentTemplate(1);
        }
        else if (action == 8)
        { // Hunter - Marksmanship
            m_player->AddItemSet(749);
            m_player->LearnTalentTemplate(2);
        }
        else if (action == 9)
        { // Hunter - Survival
            m_player->AddItemSet(749);
            m_player->LearnTalentTemplate(3);
        }
        // Rogue
        else if (action == 10)
        { // Rogue - Assassination
            m_player->AddItemSet(745);
            m_player->LearnTalentTemplate(1);
        }
        else if (action == 11)
        { // Rogue - Combat
            m_player->AddItemSet(745);
            m_player->LearnTalentTemplate(2);
        }
        else if (action == 12)
        { // Rogue - Subtlety
            m_player->AddItemSet(745);
            m_player->LearnTalentTemplate(3);
        }
        // Priest
        else if (action == 13)
        { // Priest - Discipline
            m_player->AddItemSet(739);
            m_player->LearnTalentTemplate(1);
        }
        else if (action == 14)
        { // Priest - Holy
            m_player->AddItemSet(739);
            m_player->LearnTalentTemplate(2);
        }
        else if (action == 15)
        { // Priest - Shadow
            m_player->AddItemSet(740);
            m_player->LearnTalentTemplate(3);
        }
        // Shaman
        else if (action == 16)
        { // Shaman - Elemental
            m_player->AddItemSet(739);
            m_player->LearnTalentTemplate(1);
        }
        else if (action == 17)
        { // Shaman - Enhancement
            m_player->AddItemSet(746);
            m_player->LearnTalentTemplate(2);
        }
        else if (action == 18)
        { // Shaman - Restoration
            m_player->AddItemSet(747);
            m_player->LearnTalentTemplate(3);
        }
        // Mage
        else if (action == 19)
        { // Mage - Arcane
            m_player->AddItemSet(741);
            m_player->LearnTalentTemplate(1);
        }
        else if (action == 20)
        { // Mage - Fire
            m_player->AddItemSet(741);
            m_player->LearnTalentTemplate(2);
        }
        else if (action == 21)
        { // Mage - Frost
            m_player->AddItemSet(741);
            m_player->LearnTalentTemplate(3);
        }
        // Warlock
        else if (action == 22)
        { // Warlock - Affliction
            m_player->AddItemSet(738);
            m_player->LearnTalentTemplate(1);
        }
        else if (action == 23)
        { // Warlock - Demonology
            m_player->AddItemSet(738);
            m_player->LearnTalentTemplate(2);
        }
        else if (action == 24)
        { // Warlock - Destruction
            m_player->AddItemSet(738);
            m_player->LearnTalentTemplate(3);
        }
        // Druid
        else if (action == 25)
        { // Druid - Balance
            m_player->AddItemSet(743);
            m_player->LearnTalentTemplate(1);
        }
        else if (action == 26)
        { // Druid - Feral
            m_player->AddItemSet(742);
            m_player->LearnTalentTemplate(2);
        }
        else if (action == 27)
        { // Druid - Restoration
            m_player->AddItemSet(744);
            m_player->LearnTalentTemplate(3);
            m_player->ApplyEnchantTemplate(3);
        }
        else if (action == 30)
            m_player->GetSettings()->SetSetting(SETTING_UINT_HIDETEMPLATEMENU, 1);
    }
}
