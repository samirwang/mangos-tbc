#include "Precompiled.h"
#include "PlayerGossip.h"
#include "CPlayer.h"
#include "Settings.h"

class pgossip_templatesystem : public PlayerGossipScript
{
public:
    pgossip_templatesystem() : PlayerGossipScript(GOSSIP_SENDER_FIRSTLOGIN) {}

    bool GossipHello(Player* pPlayer, uint32 sender) override
    {
        CPlayer* pCPlayer = pPlayer->GetCPlayer();

        if (pPlayer->getClass() == CLASS_WARRIOR)
        {
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Arms", sender, 1);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Fury", sender, 2);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Protection", sender, 3);
        }
        else if (pPlayer->getClass() == CLASS_PALADIN)
        {
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Holy", sender, 4);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Protection", sender, 5);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Retribution", sender, 6);
        }
        else if (pPlayer->getClass() == CLASS_HUNTER)
        {
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Beast Mastery", sender, 7);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Marksmanship", sender, 8);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Survival", sender, 9);
        }
        else if (pPlayer->getClass() == CLASS_ROGUE)
        {
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Assassination", sender, 10);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Combat", sender, 11);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Subtlety", sender, 12);
        }
        else if (pPlayer->getClass() == CLASS_PRIEST)
        {
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Discipline", sender, 13);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Holy", sender, 14);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Shadow", sender, 15);
        }
        else if (pPlayer->getClass() == CLASS_SHAMAN)
        {
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Elemental", sender, 16);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Enhancement", sender, 17);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Restoration", sender, 18);
        }
        else if (pPlayer->getClass() == CLASS_MAGE)
        {
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Arcane", sender, 19);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Fire", sender, 20);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Frost", sender, 21);
        }
        else if (pPlayer->getClass() == CLASS_WARLOCK)
        {
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Affliction", sender, 22);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Demonology", sender, 23);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Destruction", sender, 24);
        }
        else if (pPlayer->getClass() == CLASS_DRUID)
        {
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Balance", sender, 25);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Feral", sender, 26);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "I want to play Restoration", sender, 27);
        }

        pCPlayer->AddGossipMenuItem(Icon::GEAR, "Do not show this again", sender, 30);

        if (!pPlayer->PlayerTalkClass->GetGossipMenu().Empty())
            pPlayer->PlayerTalkClass->SendGossipMenu("Please select your spec!", GetHearthStoneOrPlayerGuid(pPlayer));

        return true;
    }

    bool GossipSelect(Player* pPlayer, uint32 sender, uint32 action, std::string code) override
    {
        CPlayer* pCPlayer = pPlayer->GetCPlayer();

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

        pPlayer->GetSettings()->SetSetting(SETTING_UINT_HIDETEMPLATEMENU, 1);
        pCPlayer->BoxChat << sCustom.ChatNameWrapper("Template System") << " If you want to show this menu again you can enable it by the teleporter npc" << std::endl;

        return true;
    }
};

void AddSC_pgossip_template()
{
    new pgossip_templatesystem;
}
