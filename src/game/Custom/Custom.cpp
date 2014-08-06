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

#include "Custom.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "Database/DatabaseEnv.h"
#include "World.h"
#include "Log.h"
#include "SocialMgr.h"
#include "GossipDef.h"
#include "CPlayer.h"

#include <fstream>

Custom::~Custom()
{
    for (auto& itr : m_CachedSpellContainer)
        delete itr.second;

    for (auto& itr : m_TalentContainer)
        delete itr;
}

void Custom::SendWorldChat(ObjectGuid guid, std::string msg)
{
    for (auto& itr : sWorld.GetSessionMap())
    if (itr.second)
    if (Player* pPlayer = itr.second->GetPlayer())
    if (pPlayer->IsInWorld() && !pPlayer->GetSocial()->HasIgnore(guid) && pPlayer->ToCPlayer()->WChatOn())
        pPlayer->ToCPlayer()->BoxChat << msg << std::endl;
}

void Custom::SendGMMessage(std::string msg)
{
    for (auto& itr : sWorld.GetSessionMap())
    if (itr.second)
    if (Player* pPlayer = itr.second->GetPlayer())
    if (pPlayer->IsInWorld() && pPlayer->GetSession()->GetSecurity() > SEC_PLAYER)
        pPlayer->ToCPlayer()->BothChat << msg << std::endl;
}

Custom::SpellContainer Custom::GetSpellContainerByCreatureEntry(uint32 entry)
{
    SpellContainer spellContainer;

    if (TrainerSpellData const* spelldata = sObjectMgr.GetNpcTrainerSpells(entry))
    for (auto& itr : spelldata->spellList)
        spellContainer.push_back(itr.second);

    const CreatureInfo* creature = sObjectMgr.GetCreatureTemplate(entry);

    if (!creature)
        return spellContainer;

    uint32 trainertemplate = creature->TrainerTemplateId;

    if (trainertemplate)
    if (TrainerSpellData const* spelldata2 = sObjectMgr.GetNpcTrainerTemplateSpells(trainertemplate))
    for (auto& itr : spelldata2->spellList)
        spellContainer.push_back(itr.second);

    return spellContainer;
}

Custom::SpellContainer* Custom::GetCachedSpellContainer(uint32 crval)
{
    if (m_CachedSpellContainer.find(crval) != m_CachedSpellContainer.cend())
        return m_CachedSpellContainer[crval];

    return NULL;
}

const std::string Custom::m_ClassColor[] =
{
    "",
    MSG_COLOR_WARRIOR,
    MSG_COLOR_PALADIN,
    MSG_COLOR_HUNTER,
    MSG_COLOR_ROGUE,
    MSG_COLOR_PRIEST,
    "",
    MSG_COLOR_SHAMAN,
    MSG_COLOR_MAGE,
    MSG_COLOR_WARLOCK,
    "",
    MSG_COLOR_DRUID
};

const std::string Custom::m_ItemColor[] =
{
    MSG_COLOR_GREY,
    MSG_COLOR_WHITE,
    MSG_COLOR_GREEN,
    MSG_COLOR_BLUE,
    MSG_COLOR_PURPLE,
    MSG_COLOR_ORANGE
};

const std::string Custom::m_SlotNames[] =
{
    "Head",
    "Neck",
    "Shoulders",
    "Shirt",
    "Chest",
    "Waist",
    "Legs",
    "Feet",
    "Wrists",
    "Hands",
    "Ring 1",
    "Ring 2",
    "Trinket 1",
    "Trinket 2",
    "Back",
    "Mainhand",
    "Offhand",
    "Ranged",
    "Tabard"
};

std::string Custom::ChatNameWrapper(std::string cName)
{
    std::ostringstream ss;
    ss << MSG_COLOR_WHITE << "[" << MSG_COLOR_MAGENTA << cName << MSG_COLOR_WHITE << "]:";
    return ss.str().c_str();
}

void Custom::LoadFakePlayerBytes()
{
    uint32 count = 0;

    QueryResult* result = WorldDatabase.PQuery("SELECT race, maleBytes, maleBytes2, femaleBytes, femaleBytes2 FROM fakeplayerbytes");
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();

            FakePlayerBytes bytes;

            uint8 race = fields[0].GetUInt8();
            bytes.PlayerBytes[GENDER_MALE] = fields[1].GetUInt32();
            bytes.PlayerBytes2[GENDER_MALE] = fields[2].GetUInt32();
            bytes.PlayerBytes[GENDER_FEMALE] = fields[3].GetUInt32();
            bytes.PlayerBytes2[GENDER_FEMALE] = fields[4].GetUInt32();

            m_FakePlayerBytesContainer.insert(std::make_pair(race, bytes));

            if (race && bytes.PlayerBytes[GENDER_MALE] && bytes.PlayerBytes2[GENDER_MALE] &&
                bytes.PlayerBytes[GENDER_FEMALE] && bytes.PlayerBytes[GENDER_FEMALE])
                ++count;
        } while (result->NextRow());
    }

    if (sWorld.getConfig(CONFIG_BOOL_CFBG_ENABLED) && (count < 10 || !result))
    {
        const char* message = "There was something wrong with loading fakeplayerbytes for crossfaction battlegrounds!";
        sLog.outError(message);
        sLog.outErrorDb(message);
        std::exit(EXIT_FAILURE);
    }
}

uint8 Custom::PickFakeRace(uint8 pclass, Team team)
{
    std::vector<uint8> playableRaces;

    for (uint8 i = RACE_HUMAN; i <= RACE_DRAENEI; ++i)
    {
        if (i == RACE_GOBLIN)
            continue;

        PlayerInfo const* info = sObjectMgr.GetPlayerInfo(i, pclass);
        if (!info)
            continue;

        if (Player::TeamForRace(i) == team)
            continue;

        playableRaces.push_back(i);
    }

    return playableRaces[urand(0, playableRaces.size() - 1)];
}

bool ChatHandler::HandleWToggleCommand(char* /*args*/)
{
    m_session->GetPlayer()->ToCPlayer()->ToggleWChat();
    PSendSysMessage("%s World chat is now %sabled", sCustom.ChatNameWrapper("World Chat").c_str(), m_session->GetPlayer()->ToCPlayer()->WChatOn() ? "en" : "dis");

    return true;
}

bool ChatHandler::HandleWChatCommand(char* args)
{
    if (!*args)
        return false;

    if (m_session->GetPlayer()->ToCPlayer()->WChatOn())
        m_session->GetPlayer()->ToCPlayer()->SendWorldChatMsg(args);
    else
        PSendSysMessage("%s You have disabled the worldchat, please enable it to speak in it.", sCustom.ChatNameWrapper("World Chat").c_str());

    return true;
}

bool ChatHandler::HandleSpellCreatureFindCommand(char*)
{
    ofstream myfile;
    myfile.open("npcdbc.log");

    uint16 count = 0;
    for (uint32 i = 1; i < 53086; ++i)
    {
        SpellEntry const* spellInfo = sSpellStore.LookupEntry(i);
        if (spellInfo)
        {
            for (uint32 idx = 0; idx < 3; ++idx)
            {
                if (spellInfo->EffectApplyAuraName[idx] == SPELL_AURA_MOUNTED)
                {
                    myfile << spellInfo->EffectMiscValue[idx] << "\n";
                    ++count;
                }
                if (spellInfo->EffectApplyAuraName[idx] == SPELL_AURA_TRANSFORM)
                {
                    myfile << spellInfo->EffectMiscValue[idx] << "\n";
                    ++count;
                }
                else if (spellInfo->Effect[idx] == SPELL_EFFECT_SUMMON)
                {
                    myfile << spellInfo->EffectMiscValue[idx] << "\n";
                    ++count;
                }
            }
        }
    }
    PSendSysMessage("Found %u npc's in spell.dbc", count);
    myfile.close();

    return true;
}

void PlayerMenu::SendGossipMenu(std::string text, ObjectGuid objectGuid, uint32 scriptid, uint32 textid)
{
    WorldPacket data(SMSG_NPC_TEXT_UPDATE, 100);            // guess size
    data << textid;                                         // can be < 0

    for (auto i = 0; i < 8; ++i)
    {
        data << float(0);
        data << text;
        data << text;
        data << uint32(0);
        data << uint32(0);
        data << uint32(0);
        data << uint32(0);
        data << uint32(0);
        data << uint32(0);
        data << uint32(0);
    }

    GetMenuSession()->GetPlayer()->ToCPlayer()->SetScriptID(scriptid);

    GetMenuSession()->SendPacket(&data);

    SendGossipMenu(textid, objectGuid);
}

void Custom::LoadTalentContainer()
{
    for (auto& itr : m_TalentContainer)
        delete itr;

    m_TalentContainer.clear();

    uint32 count = 0;

    QueryResult* result = WorldDatabase.PQuery("SELECT class, spec, id, rank FROM playertemplate_talents");
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();

            TalentTemplate* pTalent = new TalentTemplate;

            pTalent->ClassId = fields[0].GetUInt8();
            pTalent->SpecId = fields[1].GetUInt8();
            pTalent->TalentId = fields[2].GetUInt32();
            pTalent->TalentRank = fields[3].GetUInt8();

            m_TalentContainer.push_back(pTalent);
            ++count;
        } while (result->NextRow());

        delete result;
    }
}

void Custom::LoadEnchantContainer()
{
    for (auto& itr : m_EnchantContainer)
        delete itr;

    m_EnchantContainer.clear();

    uint32 count = 0;

    QueryResult* result = WorldDatabase.PQuery("SELECT class, spec, spellid, slotid FROM playertemplate_enchants");
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();

            EnchantTemplate* pEnchant = new EnchantTemplate;

            pEnchant->ClassId = fields[0].GetUInt8();
            pEnchant->SpecId = fields[1].GetUInt8();
            pEnchant->SpellId = fields[2].GetUInt32();
            pEnchant->SlotId = fields[3].GetUInt8();

            m_EnchantContainer.push_back(pEnchant);
            ++count;
        } while (result->NextRow());

        delete result;
    }
}

void BattleGround::RewardReputationToXBGTeam(uint32 faction_ally, uint32 faction_horde, uint32 gain, Team teamId)
{
    FactionEntry const* a_factionEntry = sFactionStore.LookupEntry(faction_ally);
    FactionEntry const* h_factionEntry = sFactionStore.LookupEntry(faction_horde);

    if (!a_factionEntry || !h_factionEntry)
        return;

    for (auto& itr : GetPlayers())
    {
        if (itr.second.OfflineRemoveTime)
            continue;

        Player* plr = sObjectMgr.GetPlayer(itr.first);

        if (!plr)
        {
            sLog.outError("BattleGround:RewardReputationToTeam: %s not found!", itr.first.GetString().c_str());
            continue;
        }

        if (plr->GetTeam() == teamId) // Check if player is playing in the team that capped and then reward by original team.
            plr->GetReputationMgr().ModifyReputation(plr->GetOTeam() == ALLIANCE ? a_factionEntry : h_factionEntry, gain);
    }
}

CPlayer* Custom::GetCPlayer(const char* name)
{
    Player* plr = ObjectAccessor::FindPlayerByName(name);
    if (!plr)
        return NULL;

    return static_cast<CPlayer*>(plr);
}
CPlayer* Custom::GetCPlayer(ObjectGuid guid, bool inWorld /*=true*/)
{
    Player* plr = ObjectAccessor::FindPlayer(guid, inWorld);
    if (!plr)
        return NULL;

    return static_cast<CPlayer*>(plr);
}