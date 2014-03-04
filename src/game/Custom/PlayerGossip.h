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

#ifndef _PLAYERGOSSIP_H
#define _PLAYERGOSSIP_H

#define HEARTHSTONE 6948

class PlayerGossipScript;

class PlayerGossip
{
public:
    PlayerGossip() {}
    ~PlayerGossip();

    typedef std::map<uint32, PlayerGossipScript*> ScriptContainer;

    void RegisterScript(uint32 sender, PlayerGossipScript* pScript);

    bool GossipHello(Player* pPlayer, uint32 sender);
    bool GossipSelect(Player* pPlayer, uint32 sender, uint32 action, std::string code);

private:
    ScriptContainer m_ScriptContainer;
};

#define sPlayerGossip MaNGOS::Singleton<PlayerGossip>::Instance()

class PlayerGossipScript
{
public:
    PlayerGossipScript(uint32 sender) { sPlayerGossip.RegisterScript(sender, this); }
    ~PlayerGossipScript() {}

    virtual bool GossipHello(Player* pPlayer, uint32 sender) { return true; };
    virtual bool GossipSelect(Player* pPlayer, uint32 sender, uint32 action, std::string code) { return true; };

    ObjectGuid GetHearthStoneOrPlayerGuid(Player* pPlayer)
    {
        ObjectGuid guid = pPlayer->GetObjectGuid();

        for (uint8 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; ++i)
        if (Item* pItem = pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
        if (pItem->GetEntry() == HEARTHSTONE)
            guid = pItem->GetObjectGuid();

        return guid;
    }
};

#endif