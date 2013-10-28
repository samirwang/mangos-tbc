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

void Player::OnLogin()
{
    if (GetTotalPlayedTime() < 1)
    {
        m_Played_time[PLAYED_TIME_TOTAL] += 1;
        m_Played_time[PLAYED_TIME_LEVEL] += 1;

        OnFirstLogin();
    }
}

void Player::OnFirstLogin()
{
    LearnGreenSpells();
}