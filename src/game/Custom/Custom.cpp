#include "Custom.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "Database/DatabaseEnv.h"
#include "World.h"
#include "Log.h"
#include "SocialMgr.h"
#include "GossipDef.h"

Custom::~Custom()
{
    for (CachedSpellContainer::const_iterator itr = m_CachedSpellContainer.begin(); itr != m_CachedSpellContainer.end(); ++itr)
        delete itr->second;

    for (TalentContainer::const_iterator itr = m_TalentContainer.begin(); itr != m_TalentContainer.end(); ++itr)
        delete *itr;
}

void World::SendWorldChat(ObjectGuid guid, std::string msg)
{
    for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
    if (itr->second)
    if (Player* pPlayer = itr->second->GetPlayer())
    if (pPlayer->IsInWorld() && !pPlayer->GetSocial()->HasIgnore(guid) && pPlayer->WCHatOn())
        pPlayer->BoxChat << msg << "\n";
}

void World::SendGMMessage(std::string msg)
{
    for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
    if (itr->second)
    if (Player* pPlayer = itr->second->GetPlayer())
    if (pPlayer->IsInWorld() && pPlayer->GetSession()->GetSecurity() > SEC_PLAYER)
        pPlayer->BothChat << msg << "\n";
}

SpellContainer Custom::GetSpellContainerByCreatureEntry(uint32 entry)
{
    SpellContainer spellContainer;

    if (TrainerSpellData const* spelldata = sObjectMgr.GetNpcTrainerSpells(entry))
        for (TrainerSpellMap::const_iterator itr = spelldata->spellList.begin(); itr != spelldata->spellList.end(); ++itr)
            spellContainer.push_back(itr->second);

    const CreatureInfo* creature = sObjectMgr.GetCreatureTemplate(entry);

    if (!creature)
        return spellContainer;

    uint32 trainertemplate = creature->trainerId;

    if (trainertemplate)
        if (TrainerSpellData const* spelldata2 = sObjectMgr.GetNpcTrainerTemplateSpells(trainertemplate))
            for (TrainerSpellMap::const_iterator itr = spelldata2->spellList.begin(); itr != spelldata2->spellList.end(); ++itr)
                spellContainer.push_back(itr->second);

    return spellContainer;
}

SpellContainer* Custom::GetCachedSpellContainer(uint32 crval)
{
    if (m_CachedSpellContainer.find(crval) != m_CachedSpellContainer.end())
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
        }
        while (result->NextRow());
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

    for (uint8 i = RACE_HUMAN; i <= RACE_DRAENEI; i++)
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
    m_session->GetPlayer()->ToggleWChat();
    PSendSysMessage("%s World chat is now %sabled", sCustom.ChatNameWrapper("World Chat").c_str(), m_session->GetPlayer()->WCHatOn() ? "en" : "dis");

    return true;
}

bool ChatHandler::HandleWChatCommand(char* args)
{
    if (!*args)
        return false;

    if (m_session->GetPlayer()->WCHatOn())
        m_session->GetPlayer()->SendWorldChatMsg(args);
    else
        PSendSysMessage("%s You have disabled the worldchat, please enable it to speak in it.", sCustom.ChatNameWrapper("World Chat").c_str());

    return true;
}

void PlayerMenu::SendGossipMenu(std::string text, ObjectGuid objectGuid, uint32 textid)
{
    WorldPacket data(SMSG_NPC_TEXT_UPDATE, 100);            // guess size
    data << textid;                                         // can be < 0

    for (uint32 i = 0; i < 8; ++i)
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

    GetMenuSession()->SendPacket(&data);

    SendGossipMenu(textid, objectGuid);
}

void Custom::LoadTalentContainer()
{
    for (TalentContainer::const_iterator itr = m_TalentContainer.begin(); itr != m_TalentContainer.end(); ++itr)
        delete *itr;

    m_TalentContainer.clear();

    uint32 count = 0;

    QueryResult* result = WorldDatabase.PQuery("SELECT class, spec, id, rank FROM playertemplate_talents");
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();

            TalentTemplate* pTalent = new TalentTemplate;

            pTalent->ClassId    = fields[0].GetUInt8();
            pTalent->SpecId     = fields[1].GetUInt8();
            pTalent->TalentId   = fields[2].GetUInt32();
            pTalent->TalentRank = fields[3].GetUInt8();

            m_TalentContainer.push_back(pTalent);
            ++count;
        }
        while (result->NextRow());

        delete result;
    }
}

void Custom::LoadEnchantContainer()
{
    for (EnchantContainer::const_iterator itr = m_EnchantContainer.begin(); itr != m_EnchantContainer.end(); ++itr)
        delete *itr;

    m_EnchantContainer.clear();

    uint32 count = 0;

    QueryResult* result = WorldDatabase.PQuery("SELECT class, spec, spellid, slotid FROM playertemplate_enchants");
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();

            EnchantTemplate* pEnchant = new EnchantTemplate;

            pEnchant->ClassId   = fields[0].GetUInt8();
            pEnchant->SpecId    = fields[1].GetUInt8();
            pEnchant->SpellId   = fields[2].GetUInt32();
            pEnchant->SlotId    = fields[3].GetUInt8();

            m_EnchantContainer.push_back(pEnchant);
            ++count;
        }
        while (result->NextRow());

        delete result;
    }
}
