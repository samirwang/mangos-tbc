#include "WorldSession.h"
#include "Chat.h"
#include "WardenDataStorage.h"
#include "WardenWin.h"
#include "WardenMac.h"
#include "Log.h"
#include "World.h"
#include "Unit.h"
#include "BattleGround/BattleGroundMgr.h"
#include "Util.h"

void WorldSession::InitWarden(BigNumber *K, std::string os)
{
    if (!sWorld.getConfig(CONFIG_BOOL_WARDEN_ENABLED))
        return;

    if (os == "Win")                                        // Windows
        m_Warden = (WardenBase*)new WardenWin();
    else                                                    // MacOS
        m_Warden = (WardenBase*)new WardenMac();

    m_Warden->Init(this, K);
}

bool ChatHandler::HandleReloadWarden(char* /*args*/)
{
    sLog.outString( "Re-Loading Warden..." );
    WardenDataStorage.Init();
    SendGlobalSysMessage("Warden DB reloaded.");
    return true;
}

bool BattleGroundQueue::CheckMixedMatch(BattleGround* bg_template, BattleGroundBracketId bracket_id, uint32 minPlayers, uint32 maxPlayers)
{
    if (!sWorld.getConfig(CONFIG_BOOL_CFBG_ENABLED) || !bg_template->isBattleGround())
        return false;

    // Empty selection pool, we do not want old data.
    m_SelectionPools[BG_TEAM_ALLIANCE].Init();
    m_SelectionPools[BG_TEAM_HORDE].Init();

    uint32 addedally = 0;
    uint32 addedhorde = 0;

    for (GroupsQueueType::const_iterator itr = m_QueuedGroups[bracket_id][BG_QUEUE_NORMAL_ALLIANCE].begin();
        itr != m_QueuedGroups[bracket_id][BG_QUEUE_NORMAL_ALLIANCE].end(); ++itr)
    {
        GroupQueueInfo* ginfo = *itr;
        if (!ginfo->IsInvitedToBGInstanceGUID)
        {
            bool makeally = addedally < addedhorde;

            if (addedally == addedhorde)
                makeally = urand(0,1);

            ginfo->GroupTeam = makeally ? ALLIANCE : HORDE;

            if (m_SelectionPools[makeally ? BG_TEAM_ALLIANCE : BG_TEAM_HORDE].AddGroup(*itr, maxPlayers))
                makeally ? addedally += ginfo->Players.size() : addedhorde += ginfo->Players.size();
            else
                break;

            if (m_SelectionPools[BG_TEAM_ALLIANCE].GetPlayerCount() >= minPlayers &&
                m_SelectionPools[BG_TEAM_HORDE].GetPlayerCount() >= minPlayers)
                return true;
        }
    }

    if (sBattleGroundMgr.isTesting() && (m_SelectionPools[BG_TEAM_ALLIANCE].GetPlayerCount()
        || m_SelectionPools[BG_TEAM_HORDE].GetPlayerCount()))
        return true;
    
    return false;
}

bool BattleGroundQueue::MixPlayersToBG(BattleGround* bg, BattleGroundBracketId bracket_id)
{
    if (!sWorld.getConfig(CONFIG_BOOL_CFBG_ENABLED) || bg->isArena())
        return false;

    int32 allyFree   = bg->GetFreeSlotsForTeam(ALLIANCE);
    int32 hordeFree = bg->GetFreeSlotsForTeam(HORDE);

    uint32 addedally = bg->GetMaxPlayersPerTeam() - bg->GetFreeSlotsForTeam(ALLIANCE);
    uint32 addedhorde = bg->GetMaxPlayersPerTeam() - bg->GetFreeSlotsForTeam(HORDE);

    for (GroupsQueueType::const_iterator itr = m_QueuedGroups[bracket_id][BG_QUEUE_NORMAL_ALLIANCE].begin();
        itr != m_QueuedGroups[bracket_id][BG_QUEUE_NORMAL_ALLIANCE].end(); ++itr)
    {
        GroupQueueInfo* ginfo = *itr;
        if (!ginfo->IsInvitedToBGInstanceGUID)
        {
            bool makeally = addedally < addedhorde;

            if (addedally == addedhorde)
                makeally = urand(0,1);

            makeally ? ++addedally : ++addedhorde;

            ginfo->GroupTeam = makeally ? ALLIANCE : HORDE;

            if (m_SelectionPools[makeally ? BG_TEAM_ALLIANCE : BG_TEAM_HORDE].AddGroup(ginfo, makeally ? allyFree : hordeFree))
                makeally ? addedally += ginfo->Players.size() : addedhorde += ginfo->Players.size();
            else
                break;
        }
    }

    return true;
}

uint32 World::GetSaveInterval()
{
    if (sWorld.getConfig(CONFIG_UINT32_INTERVAL_SAVE))
        return sWorld.getConfig(CONFIG_UINT32_INTERVAL_SAVE);

    uint32 hardtime = ceil(float((1.f/sWorld.getConfig(CONFIG_UINT32_INTERVAL_SAVEPERSEC))*sWorld.GetActiveSessionCount()*IN_MILLISECONDS)+2*IN_MILLISECONDS);
    return urand(hardtime / 2, hardtime * 3 / 2);
}

void BattleGround::RewardReputationToXBGTeam(uint32 faction_ally, uint32 faction_horde, uint32 gain, Team teamId)
{
    FactionEntry const* a_factionEntry = sFactionStore.LookupEntry(faction_ally);
    FactionEntry const* h_factionEntry = sFactionStore.LookupEntry(faction_horde);

    if (!a_factionEntry || !h_factionEntry)
        return;

    for (BattleGroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
    {
        if (itr->second.OfflineRemoveTime)
            continue;

        Player* plr = sObjectMgr.GetPlayer(itr->first);

        if (!plr)
        {
            sLog.outError("BattleGround:RewardReputationToTeam: %s not found!", itr->first.GetString().c_str());
            continue;
        }

        if (plr->GetTeam() == teamId) // Check if player is playing in the team that capped and then reward by original team.
            plr->GetReputationMgr().ModifyReputation(plr->GetOTeam() == ALLIANCE ? a_factionEntry : h_factionEntry, gain);
    }
}
