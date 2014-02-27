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

#include "PlayerGossip.h"
#include "Player.h"
#include "Settings.h"
#include "Custom.h"
#include "CPlayer.h"

PlayerGossip::PlayerGossip(Player* pPlayer)
{
    m_player = pPlayer;
}

void PlayerGossip::PlayerGossipHello(uint32 sender)
{
    CPlayer* pCplayer = m_player->GetCPlayer();

    if (sender == GOSSIP_SENDER_FIRSTLOGIN)
    {
        if (!m_player->GetSettings()->GetSetting(SETTING_UINT_HIDETEMPLATEMENU))
        {
            if (m_player->getClass() == CLASS_WARRIOR)
            {
                pCplayer->AddGossipMenuItem(Icon::BAG, "I want to play Arms", sender, 1);
                pCplayer->AddGossipMenuItem(Icon::BAG, "I want to play Fury", sender, 2);
                pCplayer->AddGossipMenuItem(Icon::BAG, "I want to play Protection", sender, 3);
            }
            else if (m_player->getClass() == CLASS_PALADIN)
            {
                pCplayer->AddGossipMenuItem(Icon::BAG, "I want to play Holy", sender, 4);
                pCplayer->AddGossipMenuItem(Icon::BAG, "I want to play Protection", sender, 5);
                pCplayer->AddGossipMenuItem(Icon::BAG, "I want to play Retribution", sender, 6);
            }
            else if (m_player->getClass() == CLASS_HUNTER)
            {
                pCplayer->AddGossipMenuItem(Icon::BAG, "I want to play Beast Mastery", sender, 7);
                pCplayer->AddGossipMenuItem(Icon::BAG, "I want to play Marksmanship", sender, 8);
                pCplayer->AddGossipMenuItem(Icon::BAG, "I want to play Survival", sender, 9);
            }
            else if (m_player->getClass() == CLASS_ROGUE)
            {
                pCplayer->AddGossipMenuItem(Icon::BAG, "I want to play Assassination", sender, 10);
                pCplayer->AddGossipMenuItem(Icon::BAG, "I want to play Combat", sender, 11);
                pCplayer->AddGossipMenuItem(Icon::BAG, "I want to play Subtlety", sender, 12);
            }
            else if (m_player->getClass() == CLASS_PRIEST)
            {
                pCplayer->AddGossipMenuItem(Icon::BAG, "I want to play Discipline", sender, 13);
                pCplayer->AddGossipMenuItem(Icon::BAG, "I want to play Holy", sender, 14);
                pCplayer->AddGossipMenuItem(Icon::BAG, "I want to play Shadow", sender, 15);
            }
            else if (m_player->getClass() == CLASS_SHAMAN)
            {
                pCplayer->AddGossipMenuItem(Icon::BAG, "I want to play Elemental", sender, 16);
                pCplayer->AddGossipMenuItem(Icon::BAG, "I want to play Enhancement", sender, 17);
                pCplayer->AddGossipMenuItem(Icon::BAG, "I want to play Restoration", sender, 18);
            }
            else if (m_player->getClass() == CLASS_MAGE)
            {
                pCplayer->AddGossipMenuItem(Icon::BAG, "I want to play Arcane", sender, 19);
                pCplayer->AddGossipMenuItem(Icon::BAG, "I want to play Fire", sender, 20);
                pCplayer->AddGossipMenuItem(Icon::BAG, "I want to play Frost", sender, 21);
            }
            else if (m_player->getClass() == CLASS_WARLOCK)
            {
                pCplayer->AddGossipMenuItem(Icon::BAG, "I want to play Affliction", sender, 22);
                pCplayer->AddGossipMenuItem(Icon::BAG, "I want to play Demonology", sender, 23);
                pCplayer->AddGossipMenuItem(Icon::BAG, "I want to play Destruction", sender, 24);
            }
            else if (m_player->getClass() == CLASS_DRUID)
            {
                pCplayer->AddGossipMenuItem(Icon::BAG, "I want to play Balance", sender, 25);
                pCplayer->AddGossipMenuItem(Icon::BAG, "I want to play Feral", sender, 26);
                pCplayer->AddGossipMenuItem(Icon::BAG, "I want to play Restoration", sender, 27);
            }

            pCplayer->AddGossipMenuItem(Icon::GEAR, "Do not show this again", sender, 30);

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
    CPlayer* pCplayer = m_player->GetCPlayer();

    m_player->PlayerTalkClass->CloseGossip();

    if (sender == GOSSIP_SENDER_FIRSTLOGIN)
    {
        // Warrior
        if (action == 1)
        { // Warrior - Arms
            pCplayer->AddItemSet(750);
            pCplayer->LearnTalentTemplate(1);
        }
        else if (action == 2)
        { // Warrior - Fury
            pCplayer->AddItemSet(750);
            pCplayer->LearnTalentTemplate(2);
        }
        else if (action == 3)
        { // Warrior - Protection
            pCplayer->AddItemSet(750);
            pCplayer->LearnTalentTemplate(3);
        }
        // Paladin
        else if (action == 4)
        { // Paladin - Holy
            pCplayer->AddItemSet(751);
            pCplayer->LearnTalentTemplate(1);
        }
        else if (action == 5)
        { // Paladin - Protection
            pCplayer->AddItemSet(752);
            pCplayer->LearnTalentTemplate(2);
        }
        else if (action == 6)
        { // Paladin - Retribution
            pCplayer->AddItemSet(752);
            pCplayer->LearnTalentTemplate(3);
        }
        // Hunter
        else if (action == 7)
        { // Hunter - Beast Mastery
            pCplayer->AddItemSet(749);
            pCplayer->LearnTalentTemplate(1);
        }
        else if (action == 8)
        { // Hunter - Marksmanship
            pCplayer->AddItemSet(749);
            pCplayer->LearnTalentTemplate(2);
        }
        else if (action == 9)
        { // Hunter - Survival
            pCplayer->AddItemSet(749);
            pCplayer->LearnTalentTemplate(3);
        }
        // Rogue
        else if (action == 10)
        { // Rogue - Assassination
            pCplayer->AddItemSet(745);
            pCplayer->LearnTalentTemplate(1);
        }
        else if (action == 11)
        { // Rogue - Combat
            pCplayer->AddItemSet(745);
            pCplayer->LearnTalentTemplate(2);
        }
        else if (action == 12)
        { // Rogue - Subtlety
            pCplayer->AddItemSet(745);
            pCplayer->LearnTalentTemplate(3);
        }
        // Priest
        else if (action == 13)
        { // Priest - Discipline
            pCplayer->AddItemSet(739);
            pCplayer->LearnTalentTemplate(1);
        }
        else if (action == 14)
        { // Priest - Holy
            pCplayer->AddItemSet(739);
            pCplayer->LearnTalentTemplate(2);
        }
        else if (action == 15)
        { // Priest - Shadow
            pCplayer->AddItemSet(740);
            pCplayer->LearnTalentTemplate(3);
        }
        // Shaman
        else if (action == 16)
        { // Shaman - Elemental
            pCplayer->AddItemSet(739);
            pCplayer->LearnTalentTemplate(1);
        }
        else if (action == 17)
        { // Shaman - Enhancement
            pCplayer->AddItemSet(746);
            pCplayer->LearnTalentTemplate(2);
        }
        else if (action == 18)
        { // Shaman - Restoration
            pCplayer->AddItemSet(747);
            pCplayer->LearnTalentTemplate(3);
        }
        // Mage
        else if (action == 19)
        { // Mage - Arcane
            pCplayer->AddItemSet(741);
            pCplayer->LearnTalentTemplate(1);
        }
        else if (action == 20)
        { // Mage - Fire
            pCplayer->AddItemSet(741);
            pCplayer->LearnTalentTemplate(2);
        }
        else if (action == 21)
        { // Mage - Frost
            pCplayer->AddItemSet(741);
            pCplayer->LearnTalentTemplate(3);
        }
        // Warlock
        else if (action == 22)
        { // Warlock - Affliction
            pCplayer->AddItemSet(738);
            pCplayer->LearnTalentTemplate(1);
        }
        else if (action == 23)
        { // Warlock - Demonology
            pCplayer->AddItemSet(738);
            pCplayer->LearnTalentTemplate(2);
        }
        else if (action == 24)
        { // Warlock - Destruction
            pCplayer->AddItemSet(738);
            pCplayer->LearnTalentTemplate(3);
        }
        // Druid
        else if (action == 25)
        { // Druid - Balance
            pCplayer->AddItemSet(743);
            pCplayer->LearnTalentTemplate(1);
        }
        else if (action == 26)
        { // Druid - Feral
            pCplayer->AddItemSet(742);
            pCplayer->LearnTalentTemplate(2);
        }
        else if (action == 27)
        { // Druid - Restoration
            pCplayer->AddItemSet(744);
            pCplayer->LearnTalentTemplate(3);
            pCplayer->ApplyEnchantTemplate(3);
        }

        m_player->GetSettings()->SetSetting(SETTING_UINT_HIDETEMPLATEMENU, 1);
        pCplayer->BoxChat << sCustom.ChatNameWrapper("Template System") << " If you want to show this menu again you can enable it by the teleporter npc" << std::endl;
    }
}
