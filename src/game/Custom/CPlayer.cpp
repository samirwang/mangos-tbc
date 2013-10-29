#include "Player.h"
#include "Custom.h"
#include "Chat.h"

void Player::CUpdate(uint32 diff)
{
    if (!m_DelayedSpellLearn.empty())
    {
        learnSpell(m_DelayedSpellLearn.front(), false);
        m_DelayedSpellLearn.erase(m_DelayedSpellLearn.begin());

        if (m_DelayedSpellLearn.empty())
            LearnGreenSpells();
    }

//     BoxChat << "Faction: " << getFaction() << " OFaction: " << getOFaction() << " FFaction: " << getFFaction() << std::endl;
//     BoxChat << "Race: " << uint32(getRace()) << " ORace: " << uint32(getORace()) << " FRace: " << uint32(getFRace()) <<  std::endl;

    SendSavedChat(CHAT_BOX, BoxChat);
    SendSavedChat(CHAT_WIDE, WideChat);
    SendSavedChat(CHAT_BOTH, BothChat);
}

void Player::OnLogin()
{
    if (GetTotalPlayedTime() < 1)
    {
        m_Played_time[PLAYED_TIME_TOTAL] += 1;
        m_Played_time[PLAYED_TIME_LEVEL] += 1;

        OnFirstLogin();
    }

    SetFakeValues();
}

void Player::OnFirstLogin()
{
    LearnGreenSpells();
}

void Player::SetFakeValues()
{
    m_fRace = 0;

    switch (getORace())
    {
    case RACE_HUMAN   : m_fRace = RACE_ORC;         break;
    case RACE_ORC     : m_fRace = RACE_HUMAN;       break;
    case RACE_DWARF   : m_fRace = RACE_TROLL;       break;
    case RACE_NIGHTELF: m_fRace = RACE_UNDEAD;      break;
    case RACE_UNDEAD  : m_fRace = RACE_NIGHTELF;    break;
    case RACE_TAUREN  : m_fRace = RACE_GNOME;       break;
    case RACE_GNOME   : m_fRace = RACE_TAUREN;      break;
    case RACE_TROLL   : m_fRace = RACE_DWARF;       break;
    case RACE_BLOODELF: m_fRace = RACE_DRAENEI;     break;
    case RACE_DRAENEI : m_fRace = RACE_BLOODELF;    break;
    default:
        sLog.outError("CFBG: Player %s has invalid race %u, cannot set fake race!", GetObjectGuid().GetString().c_str(), getORace());
        break;
    }

    m_fFaction = getFactionForRace(m_fRace);
}

void Player::SendSavedChat(MessageTypes type, std::stringstream &ss)
{
    if (!ss.str().empty())
    {
        std::string item;

        while (std::getline(ss, item))
        {
            const char* msg = item.c_str();

            if (type == CHAT_BOX || type == CHAT_BOTH)
                ChatHandler(GetSession()).SendSysMessage(msg);
            if (type == CHAT_WIDE || type == CHAT_BOTH)
            {
                WorldPacket data(SMSG_NOTIFICATION, (strlen(msg) + 1));
                data << msg;
                GetSession()->SendPacket(&data);
            }
        }

        ss.str(""); // Clear content
        ss.clear(); // Clear state flags.
    }
}

void Player::LearnGreenSpells()
{
    uint32 trainerid = 0;

    switch (getClass())
    {
    case CLASS_WARRIOR: trainerid = 26332;  break;
    case CLASS_PALADIN: trainerid = 26327;  break;
    case CLASS_HUNTER:  trainerid = 26325;  break;
    case CLASS_ROGUE:   trainerid = 26329;  break;
    case CLASS_PRIEST:  trainerid = 26328;  break;
    case CLASS_SHAMAN:  trainerid = 26330;  break;
    case CLASS_MAGE:    trainerid = 26326;  break;
    case CLASS_WARLOCK: trainerid = 26331;  break;
    case CLASS_DRUID:   trainerid = 26324;  break;
    default:
        break;
    }

    if (!trainerid)
        return;

    uint32 CRFlag = sCustom.GetCRFlag(getClass(), getRace());

    Custom::SpellContainer* allSpellContainer = sCustom.GetSpellContainerByCR(CRFlag);

    if (!allSpellContainer)
    {
        allSpellContainer = new Custom::SpellContainer;

        Custom::SpellContainer classSpellContainer = sCustom.GetSpellContainerByCreatureEntry(trainerid);

        for (Custom::SpellContainer::const_iterator itr = classSpellContainer.begin(); itr != classSpellContainer.end(); ++itr)
            allSpellContainer->push_back(*itr);

        sCustom.AddCachedSpellContainerByCR(CRFlag, allSpellContainer);
    }

    if (allSpellContainer->empty())
        return;

    m_DelayedSpellLearn.clear();

    bool taughtspell = true;

    while(taughtspell)
    {
        taughtspell = false;
        for (Custom::SpellContainer::const_iterator itr = allSpellContainer->begin(); itr != allSpellContainer->end(); ++itr)
        {
            TrainerSpell const* tSpell = &*itr;

            TrainerSpellState state = GetTrainerSpellState(tSpell, tSpell->reqLevel);

            if (state == TRAINER_SPELL_GREEN)
            {
                if (IsInWorld())
                    m_DelayedSpellLearn.push_back(tSpell->spell);
                else
                {
                    learnSpell(tSpell->spell, false);
                    taughtspell = true;
                }
            }
        }
    }
}