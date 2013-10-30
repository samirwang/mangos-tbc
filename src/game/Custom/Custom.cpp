#include "Custom.h"
#include "Log.h"
#include "ObjectMgr.h"

Custom::~Custom()
{
    for (CachedSpellContainer::const_iterator itr = m_CachedSpellContainer.begin(); itr != m_CachedSpellContainer.end(); ++itr)
        delete itr->second;
}

uint32 Custom::GetCRFlag(Player* player)
{
    return GetCRFlag(player->getClass(), player->getRace());
}

uint32 Custom::GetCRFlag(uint8 pclass, uint8 prace)
{
    uint32 flag = 0;

    switch (pclass)
    {
    case RACE_HUMAN:    SetFlag(flag, CRFLAG_HUMAN);    break;
    case RACE_ORC:      SetFlag(flag, CRFLAG_ORC);      break;
    case RACE_DWARF:    SetFlag(flag, CRFLAG_DWARF);    break;
    case RACE_NIGHTELF: SetFlag(flag, CRFLAG_NIGHTELF); break;
    case RACE_UNDEAD:   SetFlag(flag, CRFLAG_UNDEAD);   break;
    case RACE_TAUREN:   SetFlag(flag, CRFLAG_TAUREN);   break;
    case RACE_GNOME:    SetFlag(flag, CRFLAG_GNOME);    break;
    case RACE_TROLL:    SetFlag(flag, CRFLAG_TROLL);    break;
    case RACE_BLOODELF: SetFlag(flag, CRFLAG_BLOODELF); break;
    case RACE_DRAENEI:  SetFlag(flag, CRFLAG_DRAENEI);  break;
    default:
        return 0;
    }

    switch (prace)
    {
    case CLASS_WARRIOR: SetFlag(flag, CRFLAG_WARRIOR); break;
    case CLASS_PALADIN: SetFlag(flag, CRFLAG_PALADIN); break;
    case CLASS_HUNTER:  SetFlag(flag, CRFLAG_HUNTER);  break;
    case CLASS_ROGUE:   SetFlag(flag, CRFLAG_ROGUE);   break;
    case CLASS_PRIEST:  SetFlag(flag, CRFLAG_PRIEST);  break;
    case CLASS_SHAMAN:  SetFlag(flag, CRFLAG_SHAMAN);  break;
    case CLASS_MAGE:    SetFlag(flag, CRFLAG_MAGE);    break;
    case CLASS_WARLOCK: SetFlag(flag, CRFLAG_WARLOCK); break;
    case CLASS_DRUID:   SetFlag(flag, CRFLAG_DRUID);   break;
    default:
        return 0;
    }

    return flag;
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

SpellContainer* Custom::GetSpellContainerByCR(uint32 crval)
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
