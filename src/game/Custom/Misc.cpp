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

#include "WorldSession.h"
#include "Chat.h"
#include "WardenDataStorage.h"
#include "WardenWin.h"
#include "WardenMac.h"
#include "Log.h"
#include "World.h"
#include "Item.h"
#include "Player.h"
#include "ObjectMgr.h"

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
    sLog.outString("Re-Loading Warden...");
    WardenDataStorage.Init();
    SendGlobalSysMessage("Warden DB reloaded.");
    return true;
}

Team Player::GetTeam() const
{
    if (GetBattleGround())
        return m_bgData.bgTeam ? m_bgData.bgTeam : GetOTeam();

    return GetOTeam();
}

uint32 World::GetSaveInterval()
{
    uint32 hardtime = sWorld.getConfig(CONFIG_UINT32_INTERVAL_SAVE);

    if (!hardtime)
        hardtime = ceil((float)(1.f / sWorld.getConfig(CONFIG_UINT32_INTERVAL_SAVEPERSEC) * sWorld.GetActiveSessionCount()*IN_MILLISECONDS));

    if (hardtime < 2000) // Milliseconds
        hardtime = 2000;

    return urand(hardtime / 2, hardtime * 3 / 2);
}

void Item::SetTransmog(uint32 entry)
{
    if (!ObjectMgr::GetItemPrototype(entry) && entry != 0)
        return;

    m_TransmogEntry = entry;

    GetOwner()->SetVisibleItemSlot(GetSlot(), this);

    SetState(ITEM_CHANGED);
}