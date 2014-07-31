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

#include "Precompiled.h"
#include "NewPlayer.h"

class pgossip_templatesystem : public PlayerScript
{
public:
    pgossip_templatesystem() : PlayerScript("playerscript_templates") {}

    bool GossipHello(Player* pPlayer) override
    {
        CPlayer* pCPlayer = pPlayer->ToCPlayer();

        if (pPlayer->getClass() == CLASS_WARRIOR)
        {
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Arms", 0, 1);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Fury", 0, 2);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Protection", 0, 3);
        }
        else if (pPlayer->getClass() == CLASS_PALADIN)
        {
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Holy", 0, 4);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Protection", 0, 5);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Retribution", 0, 6);
        }
        else if (pPlayer->getClass() == CLASS_HUNTER)
        {
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Beast Mastery", 0, 7);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Marksmanship", 0, 8);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Survival", 0, 9);
        }
        else if (pPlayer->getClass() == CLASS_ROGUE)
        {
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Assassination", 0, 10);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Combat", 0, 11);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Subtlety", 0, 12);
        }
        else if (pPlayer->getClass() == CLASS_PRIEST)
        {
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Discipline", 0, 13);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Holy", 0, 14);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Shadow", 0, 15);
        }
        else if (pPlayer->getClass() == CLASS_SHAMAN)
        {
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Elemental", 0, 16);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Enhancement", 0, 17);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Restoration", 0, 18);
        }
        else if (pPlayer->getClass() == CLASS_MAGE)
        {
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Arcane", 0, 19);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Fire", 0, 20);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Frost", 0, 21);
        }
        else if (pPlayer->getClass() == CLASS_WARLOCK)
        {
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Affliction", 0, 22);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Demonology", 0, 23);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Destruction", 0, 24);
        }
        else if (pPlayer->getClass() == CLASS_DRUID)
        {
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Balance", 0, 25);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Feral", 0, 26);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Restoration", 0, 27);
        }

        pCPlayer->AddGossipMenuItem(Icon::GEAR, "Do not show this again", 0, 30);

        if (!pPlayer->PlayerTalkClass->GetGossipMenu().Empty())
            pPlayer->PlayerTalkClass->SendGossipMenu("Please select your spec!", GetHearthStoneOrPlayerGuid(pPlayer), 1);

        return true;
    }

    bool GossipSelect(Player* pPlayer, uint32 sender, uint32 action, std::string code) override
    {
        CPlayer* pCPlayer = pPlayer->ToCPlayer();

        pPlayer->PlayerTalkClass->CloseGossip();

        // Warrior
        if (action == 1)
        { // Warrior - Arms
            pCPlayer->AddItemSet(750);
            pCPlayer->LearnTalentTemplate(1);
        }
        else if (action == 2)
        { // Warrior - Fury
            pCPlayer->AddItemSet(750);
            pCPlayer->LearnTalentTemplate(2);
        }
        else if (action == 3)
        { // Warrior - Protection
            pCPlayer->AddItemSet(750);
            pCPlayer->LearnTalentTemplate(3);
        }
        // Paladin
        else if (action == 4)
        { // Paladin - Holy
            pCPlayer->AddItemSet(751);
            pCPlayer->LearnTalentTemplate(1);
        }
        else if (action == 5)
        { // Paladin - Protection
            pCPlayer->AddItemSet(752);
            pCPlayer->LearnTalentTemplate(2);
        }
        else if (action == 6)
        { // Paladin - Retribution
            pCPlayer->AddItemSet(752);
            pCPlayer->LearnTalentTemplate(3);
        }
        // Hunter
        else if (action == 7)
        { // Hunter - Beast Mastery
            pCPlayer->AddItemSet(749);
            pCPlayer->LearnTalentTemplate(1);
        }
        else if (action == 8)
        { // Hunter - Marksmanship
            pCPlayer->AddItemSet(749);
            pCPlayer->LearnTalentTemplate(2);
        }
        else if (action == 9)
        { // Hunter - Survival
            pCPlayer->AddItemSet(749);
            pCPlayer->LearnTalentTemplate(3);
        }
        // Rogue
        else if (action == 10)
        { // Rogue - Assassination
            pCPlayer->AddItemSet(745);
            pCPlayer->LearnTalentTemplate(1);
        }
        else if (action == 11)
        { // Rogue - Combat
            pCPlayer->AddItemSet(745);
            pCPlayer->LearnTalentTemplate(2);
        }
        else if (action == 12)
        { // Rogue - Subtlety
            pCPlayer->AddItemSet(745);
            pCPlayer->LearnTalentTemplate(3);
        }
        // Priest
        else if (action == 13)
        { // Priest - Discipline
            pCPlayer->AddItemSet(739);
            pCPlayer->LearnTalentTemplate(1);
        }
        else if (action == 14)
        { // Priest - Holy
            pCPlayer->AddItemSet(739);
            pCPlayer->LearnTalentTemplate(2);
        }
        else if (action == 15)
        { // Priest - Shadow
            pCPlayer->AddItemSet(740);
            pCPlayer->LearnTalentTemplate(3);
        }
        // Shaman
        else if (action == 16)
        { // Shaman - Elemental
            pCPlayer->AddItemSet(739);
            pCPlayer->LearnTalentTemplate(1);
        }
        else if (action == 17)
        { // Shaman - Enhancement
            pCPlayer->AddItemSet(746);
            pCPlayer->LearnTalentTemplate(2);
        }
        else if (action == 18)
        { // Shaman - Restoration
            pCPlayer->AddItemSet(747);
            pCPlayer->LearnTalentTemplate(3);
        }
        // Mage
        else if (action == 19)
        { // Mage - Arcane
            pCPlayer->AddItemSet(741);
            pCPlayer->LearnTalentTemplate(1);
        }
        else if (action == 20)
        { // Mage - Fire
            pCPlayer->AddItemSet(741);
            pCPlayer->LearnTalentTemplate(2);
        }
        else if (action == 21)
        { // Mage - Frost
            pCPlayer->AddItemSet(741);
            pCPlayer->LearnTalentTemplate(3);
        }
        // Warlock
        else if (action == 22)
        { // Warlock - Affliction
            pCPlayer->AddItemSet(738);
            pCPlayer->LearnTalentTemplate(1);
        }
        else if (action == 23)
        { // Warlock - Demonology
            pCPlayer->AddItemSet(738);
            pCPlayer->LearnTalentTemplate(2);
        }
        else if (action == 24)
        { // Warlock - Destruction
            pCPlayer->AddItemSet(738);
            pCPlayer->LearnTalentTemplate(3);
        }
        // Druid
        else if (action == 25)
        { // Druid - Balance
            pCPlayer->AddItemSet(743);
            pCPlayer->LearnTalentTemplate(1);
        }
        else if (action == 26)
        { // Druid - Feral
            pCPlayer->AddItemSet(742);
            pCPlayer->LearnTalentTemplate(2);
        }
        else if (action == 27)
        { // Druid - Restoration
            pCPlayer->AddItemSet(744);
            pCPlayer->LearnTalentTemplate(3);
            pCPlayer->ApplyEnchantTemplate(3);
        }

        pPlayer->ToCPlayer()->SetSetting(Settings::SETTING_UINT_HIDETEMPLATEMENU, 1);
        pCPlayer->BoxChat << sCustom.ChatNameWrapper("Template System") << " If you want to show this menu again you can enable it by the teleporter npc" << std::endl;

        return true;
    }
};

void AddSC_pgossip_template()
{
    new pgossip_templatesystem;
}