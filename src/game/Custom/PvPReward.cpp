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

#include "CPlayer.h"
#include "Custom.h"

void CPlayer::HandlePvPKill(CPlayer* pVictim)
{
    // Damage
    float TotalDMG = 0;
    for (auto& i : pVictim->GetDamagers())
        TotalDMG += i.second;

    for (auto& i : pVictim->GetDamagers())
    {
        CPlayer* pAttacker = sCustom.GetCPlayer(i.first);
        if (!pAttacker)
            continue;

        float AReward = i.second / TotalDMG;
        pAttacker->AddReward(pVictim->GetNameLink(true), AReward);

        // Healing
        float TotalHeal = 0;
        for (auto& j : pAttacker->GetHealers())
            TotalHeal += j.second;

        for (auto& j : pAttacker->GetHealers())
        {
            CPlayer* pHealer = sCustom.GetCPlayer(j.first);
            if (!pHealer)
                continue;

            float HReward = AReward * (j.second / TotalDMG);
            pHealer->AddReward(pVictim->GetNameLink(true), HReward);
        }
    }
}

void CPlayer::AddReward(std::string name, float amount)
{
    m_Rewards.push_back(name);
    m_PendingReward += amount;

    if (m_PendingReward >= 1)
    {
        uint32 Reward = m_PendingReward * 10000.f;
        m_PendingReward = 0;

        SetMoney(GetMoney() + Reward);
        BoxChat << "You were rewarded with " << GetGoldString(Reward) << " for the kills of " << GetRewardNames() << std::endl;
    }
}

std::string CPlayer::GetRewardNames(bool duplicates)
{
    auto Names = m_Rewards;

    if (!duplicates)
        Names.erase(std::unique(Names.begin(), Names.end()), Names.end());

    std::ostringstream ss;
    for (auto itr = Names.begin(); itr != Names.end(); ++itr)
    {
        auto jtr = itr;
        std::advance(jtr, 1);

        if (jtr == Names.end())
            ss << " and ";
        else if (itr != Names.begin())
            ss << ", ";

        ss << *itr;
    }

    return ss.str();
}

std::string CPlayer::GetGoldString(uint32 copper)
{
    std::ostringstream ss;

    int gold = copper / 10000;
    copper -= gold * 10000;
    int silver = copper / 100;
    copper -= silver * 100;

    if (gold)
        ss << gold << " Gold";

    if (gold && silver && !copper)
        ss << " and ";
    else if (gold && silver)
        ss << ", ";

    if (silver)
        ss << silver << " Silver";
    if (silver && copper)
        ss << " and ";
    if (copper)
        ss << copper << " Copper";

    return ss.str();
}