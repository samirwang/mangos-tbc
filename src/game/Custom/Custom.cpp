#include "Custom.h"
#include "Log.h"
#include "ObjectMgr.h"

Custom::~Custom()
{
    for (CachedSpellContainer::const_iterator itr = m_CachedSpellContainer.begin(); itr != m_CachedSpellContainer.end(); ++itr)
        delete itr->second;
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

const std::string Custom::StaffText[] =
{
    "Player",
    MSG_COLOR_ANN_GREEN,
    "Moderator",
    MSG_COLOR_BLUE,
    "Game Master",
    MSG_COLOR_PURPLE,
    "Administrator",
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

void Custom::InitializeFakePlayerBytes()
{
    FakePlayerBytes bytes;
    
    // ALLIANCE

    bytes.PlayerBytes[GENDER_MALE] = 151061248;
    bytes.PlayerBytes2[GENDER_MALE] = 33554435;
    bytes.PlayerBytes[GENDER_FEMALE] = 67242757;
    bytes.PlayerBytes2[GENDER_FEMALE] = 33554437;
    m_FakePlayerBytesContainer.insert(std::make_pair(RACE_HUMAN, bytes));

    bytes.PlayerBytes[GENDER_MALE] = 67306498;
    bytes.PlayerBytes2[GENDER_MALE] = 33554440;
    bytes.PlayerBytes[GENDER_FEMALE] = 16909320;
    bytes.PlayerBytes2[GENDER_FEMALE] = 33554432;
    m_FakePlayerBytesContainer.insert(std::make_pair(RACE_DWARF, bytes));

    bytes.PlayerBytes[GENDER_MALE] = 50464008;
    bytes.PlayerBytes2[GENDER_MALE] = 33554435;
    bytes.PlayerBytes[GENDER_FEMALE] = 50724871;
    bytes.PlayerBytes2[GENDER_FEMALE] = 33554439;
    m_FakePlayerBytesContainer.insert(std::make_pair(RACE_NIGHTELF, bytes));

    bytes.PlayerBytes[GENDER_MALE] = 134546689;
    bytes.PlayerBytes2[GENDER_MALE] = 33554436;
    bytes.PlayerBytes[GENDER_FEMALE] = 100663811;
    bytes.PlayerBytes2[GENDER_FEMALE] = 33554434;
    m_FakePlayerBytesContainer.insert(std::make_pair(RACE_GNOME, bytes));

    bytes.PlayerBytes[GENDER_MALE] = 395523;
    bytes.PlayerBytes2[GENDER_MALE] = 33554436;
    bytes.PlayerBytes[GENDER_FEMALE] = 50660358;
    bytes.PlayerBytes2[GENDER_FEMALE] = 33554438;
    m_FakePlayerBytesContainer.insert(std::make_pair(RACE_DRAENEI, bytes));

    // HORDE

    bytes.PlayerBytes[GENDER_MALE] = 67110400;
    bytes.PlayerBytes2[GENDER_MALE] = 33554433;
    bytes.PlayerBytes[GENDER_FEMALE] = 34014978;
    bytes.PlayerBytes2[GENDER_FEMALE] = 33554434;
    m_FakePlayerBytesContainer.insert(std::make_pair(RACE_ORC, bytes));

    bytes.PlayerBytes[GENDER_MALE] = 67306497;
    bytes.PlayerBytes2[GENDER_MALE] = 33554438;
    bytes.PlayerBytes[GENDER_FEMALE] = 100860930;
    bytes.PlayerBytes2[GENDER_FEMALE] = 33554438;
    m_FakePlayerBytesContainer.insert(std::make_pair(RACE_UNDEAD, bytes));

    bytes.PlayerBytes[GENDER_MALE] = 132111;
    bytes.PlayerBytes2[GENDER_MALE] = 33554438;
    bytes.PlayerBytes[GENDER_FEMALE] = 33816584;
    bytes.PlayerBytes2[GENDER_FEMALE] = 33554433;
    m_FakePlayerBytesContainer.insert(std::make_pair(RACE_TAUREN, bytes));

    bytes.PlayerBytes[GENDER_MALE] = 67175170;
    bytes.PlayerBytes2[GENDER_MALE] = 33554438;
    bytes.PlayerBytes[GENDER_FEMALE] = 67371778;
    bytes.PlayerBytes2[GENDER_FEMALE] = 33554436;
    m_FakePlayerBytesContainer.insert(std::make_pair(RACE_TROLL, bytes));

    bytes.PlayerBytes[GENDER_MALE] = 84215302;
    bytes.PlayerBytes2[GENDER_MALE] = 33554439;
    bytes.PlayerBytes[GENDER_FEMALE] = 460547;
    bytes.PlayerBytes2[GENDER_FEMALE] = 33554438;
    m_FakePlayerBytesContainer.insert(std::make_pair(RACE_BLOODELF, bytes));
}

uint8 Custom::PickFakeRace(uint8 pclass, Team team)
{
    uint8 m_fRace = 0;
    if (team == ALLIANCE)
    {
        switch (pclass)
        {
        case CLASS_ROGUE:
        case CLASS_WARRIOR:
            {
                switch (urand(0,3))
                {
                case 0: m_fRace = RACE_ORC;    break;
                case 1: m_fRace = RACE_UNDEAD; break;
                case 2: m_fRace = RACE_TAUREN; break;
                case 3: m_fRace = RACE_TROLL;  break;
                default:
                    break;
                }
                break;
            }
        case CLASS_PALADIN: m_fRace = RACE_BLOODELF; break;
        case CLASS_HUNTER:
            {
                switch (urand(0,3))
                {
                case 0: m_fRace = RACE_ORC;     break;
                case 1: m_fRace = RACE_TAUREN;  break;
                case 2: m_fRace = RACE_TROLL;   break;
                case 3: m_fRace = RACE_BLOODELF;break;
                default:
                    break;
                }
                break;
            }
        case CLASS_PRIEST:
            {
                switch (urand(0,2))
                {
                case 0: m_fRace = RACE_UNDEAD;   break;
                case 1: m_fRace = RACE_TROLL;    break;
                case 2: m_fRace = RACE_BLOODELF; break;
                default:
                    break;
                }
                break;
            }
        case CLASS_SHAMAN:
            {
                switch (urand(0,2))
                {
                case 0: m_fRace = RACE_ORC;    break;
                case 1: m_fRace = RACE_TAUREN; break;
                case 2: m_fRace = RACE_TROLL;  break;
                default:
                    break;
                }
                break;
            }
        case CLASS_MAGE:
            {
                switch (urand(0,2))
                {
                case 0: m_fRace = RACE_UNDEAD;   break;
                case 1: m_fRace = RACE_TROLL;    break;
                case 2: m_fRace = RACE_BLOODELF; break;
                default:
                    break;
                }
                break;
            }
        case CLASS_WARLOCK:
            {
                switch (urand(0,2))
                {
                case 0: m_fRace = RACE_ORC;      break;
                case 1: m_fRace = RACE_UNDEAD;   break;
                case 2: m_fRace = RACE_BLOODELF; break;
                default:
                    break;
                }
                break;
            }
        case CLASS_DRUID: m_fRace = RACE_TAUREN; break;
        default:
            break;
        }
    }
    else
    {
        switch (pclass)
        {
        case CLASS_WARRIOR:
            {
                switch (urand(0,4))
                {
                case 0: m_fRace = RACE_HUMAN;    break;
                case 1: m_fRace = RACE_DWARF;    break;
                case 2: m_fRace = RACE_NIGHTELF; break;
                case 3: m_fRace = RACE_GNOME;    break;
                case 4: m_fRace = RACE_DRAENEI;  break;
                default:
                    break;
                }
                break;
            }
        case CLASS_PALADIN:
            {
                switch (urand(0,2))
                {
                case 0: m_fRace = RACE_HUMAN;    break;
                case 1: m_fRace = RACE_DWARF;    break;
                case 2: m_fRace = RACE_DRAENEI;  break;
                default:
                    break;
                }
                break;
            }
        case CLASS_HUNTER:
            {
                switch (urand(0,2))
                {
                case 0: m_fRace = RACE_DWARF;    break;
                case 1: m_fRace = RACE_NIGHTELF; break;
                case 2: m_fRace = RACE_DRAENEI;  break;
                default:
                    break;
                }
                break;
            }
        case CLASS_ROGUE:
            {
                switch (urand(0,3))
                {
                case 0: m_fRace = RACE_HUMAN;    break;
                case 1: m_fRace = RACE_DWARF;    break;
                case 2: m_fRace = RACE_NIGHTELF; break;
                case 3: m_fRace = RACE_GNOME;    break;
                default:
                    break;
                }
                break;
            }
        case CLASS_PRIEST:
            {
                switch (urand(0,3))
                {
                case 0: m_fRace = RACE_HUMAN;    break;
                case 1: m_fRace = RACE_DWARF;    break;
                case 2: m_fRace = RACE_NIGHTELF; break;
                case 3: m_fRace = RACE_DRAENEI;  break;
                default:
                    break;
                }
                break;
            }
        case CLASS_SHAMAN: m_fRace = RACE_DRAENEI; break;
        case CLASS_MAGE:
            {
                switch (urand(0,2))
                {
                case 0: m_fRace = RACE_HUMAN;   break;
                case 1: m_fRace = RACE_GNOME;   break;
                case 2: m_fRace = RACE_DRAENEI; break;
                default:
                    break;
                }
                break;
            }
        case CLASS_WARLOCK:
            {
                switch (urand(0,1))
                {
                case 0: m_fRace = RACE_HUMAN;      break;
                case 1: m_fRace = RACE_GNOME;   break;
                default:
                    break;
                }
                break;
            }
        case CLASS_DRUID: m_fRace = RACE_NIGHTELF; break;
        default:
            break;
        }
    }

    return m_fRace;
}
