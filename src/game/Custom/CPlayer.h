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

#ifndef _CCPlayer_H
#define _CCPlayer_H

#include "Player.h"
#include "GossipDef.h"

enum MessageTypes
{
    CHAT_BOX,
    CHAT_WIDE,
    CHAT_BOTH
};

struct MultiVendor
{
    MultiVendor()
    {
        entry = 0;
        guid = ObjectGuid();
    }

    uint32 entry;
    ObjectGuid guid;
};

struct Country
{
    Country()
    {
        ISO2 = "";
        ISO3 = "";
        FULL = "";
    }

    std::string ISO2;
    std::string ISO3;
    std::string FULL;
};

class CCPlayer
{
public:
    CCPlayer(Player* pPlayer);
    ~CCPlayer();

    typedef std::vector<uint32> DelayedSpellLearn;

    std::stringstream BoxChat;
    std::stringstream WideChat;
    std::stringstream BothChat;

    void CUpdate(uint32 diff);
    void Sometimes();
    void OnLogin();
    void OnFirstLogin();

    void AddItemSet(uint32 setid);

    std::string GetNameLink(bool applycolors = false);
    void SendWorldChatMsg(std::string msg);
    bool WChatOn() { return m_wChatOn; }
    void SetWChat(bool value) { m_wChatOn = value; }
    void ToggleWChat() { m_wChatOn = !m_wChatOn; }

    void SendSavedChat(MessageTypes type, std::stringstream &ss);
    void FillGreenSpellList();
    void LearnGreenSpells();

    void CreatePet(uint32 entry, bool classcheck = true);
    void EnchantItem(uint32 spellid, uint8 slot, std::string sendername = "Enchanting");

    void AddGossipMenuItem(Icon::Icon icon, std::string message, uint32 sender, uint32 action)
    {
        m_player->PlayerTalkClass->GetGossipMenu().AddMenuItem(icon, message.c_str(), sender, action, "", 0);
    }

    void SetLastGossipGuid(ObjectGuid guid) { m_LastGossipGuid = guid; }
    ObjectGuid GetLastGossipGuid() { return m_LastGossipGuid; }

    void SendMultiVendorInventory(uint32 cEntry, ObjectGuid guid);
    bool BuyItemFromMultiVendor(uint32 item, uint8 count, uint8 bag, uint8 slot);
    bool SellItemToMultiVendor(ObjectGuid itemGuid, uint8 _count);
    bool BuyBackItemFromMultiVendor(uint32 slot);

    void SetMultiVendor(uint32 entry, ObjectGuid guid)
    {
        m_MultiVendor.entry = entry;
        m_MultiVendor.guid = guid;
    }
    void GetMultiVendor(uint32& entry, ObjectGuid& guid)
    {
        entry = m_MultiVendor.entry;
        guid = m_MultiVendor.guid;
    }

    void LearnTalentTemplate(uint8 spec);
    void ApplyEnchantTemplate(uint8 spec);

    void LoadCountryData();

    void SetScriptID(uint32 id) { m_ScriptID = id; }
    uint32 GetScriptID() { return m_ScriptID; }

    void SelectGObject(uint32 guidlow) { m_SelectedGObject = guidlow; }
    uint32 GetSelectedGObject() { return m_SelectedGObject; }

    uint32 GetAVGILevel(bool levelasmin = false);

private:
    Player* m_player;

    DelayedSpellLearn m_DelayedSpellLearn;

    bool m_wChatOn;
    Country m_Country;

    ObjectGuid m_LastGossipGuid;

    MultiVendor m_MultiVendor;

    uint32 m_ScriptID;

    uint32 m_SelectedGObject;
};

#endif
