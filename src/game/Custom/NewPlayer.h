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

#ifndef _NEWPLAYER_H
#define _NEWPLAYER_H

#include "Player.h"

namespace Cheats
{
    enum NewOld
    {
        NEW = 0,
        OLD,
    };

    enum Detector
    {
        COM = 0,
        SPEED,
    };

    enum DetectBitmasks
    {
        MSPEED = 1,
        MTIME = 2,
        MJUMP = 4,
        MFLY = 8,
    };

    const uint32 ALL_DETECTORS = MSPEED | MTIME | MJUMP | MFLY;
}

namespace Settings
{
    enum DataTypeId
    {
        SETTING_FLOAT = 1,
        SETTING_INT,
        SETTING_UINT,
        SETTING_STRING
    };

    enum FloatSettings
    {
        SETTING_FLOAT_BEGIN = 0,
        SETTING_FLOAT_END
    };

    enum IntSettings
    {
        SETTING_INT_BEGIN = 0,
        SETTING_INT_END
    };

    enum UintSettings
    {
        SETTING_UINT_BEGIN = 0,
        SETTING_UINT_WCHAT,
        SETTING_UINT_HIDETEMPLATEMENU,
        SETTING_UINT_END
    };

    enum StringSettings
    {
        SETTING_STRING_BEGIN = 0,
        SETTING_STRING_END
    };
}

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

class CPlayer : public Player
{
public:
    explicit CPlayer(WorldSession* session);

    ~CPlayer() {}

    Player* ToPlayer() { return static_cast<Player*>(this); }

    /************************************************************************/
    /**********************************ANTICHEAT*****************************/
    /************************************************************************/
public:
    void DetectHacks(MovementInfo& MoveInfo, Opcodes Opcode);
    void DetectSpeed();
    void DetectTime();
    void DetectJump();
    void DetectFly();

    void ReportCheat(std::string cheat, std::string info);

    float GetCurSpeed();
    float GetSpeedRate();
    float GetClientDiff();
    float GetServerDiff();
    float GetDistance(bool threeD);
    float GetDistance2D();
    float GetDistance3D();
    float GetDistanceZ();
    float GetMoveAngle();

    bool IsFlying();
    bool IsFalling();
    bool IsSwimming();
    bool IsRooted();

    bool CanFly();

    void SetGMFly(bool value) { m_GmFly = value; }
    void SkipAntiCheat() { ++m_SkipAntiCheat; }

    bool Skipping() { return m_SkipAntiCheat; }
    bool IsGMFly() { return m_GmFly; }

    void SendFly(bool value);

    void IncClientBasedServerTime(uint32 diff) { if (!m_FirstMoveInfo) m_ClientBasedServerTime += diff; }

    uint32 GetLastServerTime() { return m_ServerTime[Cheats::COM][Cheats::NEW]; }
    uint32 GetClientBasedServerTime() { return m_ClientBasedServerTime; }
    bool HadFirstMovementSent() { return !m_FirstMoveInfo; }

private:
    MovementInfo m_MoveInfo[2][2];
    Opcodes m_Opcode[2][2];
    uint32 m_ServerTime[2][2];

    uint32 m_SkipAntiCheat;
    bool m_GmFly;
    bool m_FirstMoveInfo;

    uint32 m_LastSpeedCheck;
    uint32 m_ClientBasedServerTime;

    /************************************************************************/
    /*************************************CFBG*******************************/
    /************************************************************************/
public:
    typedef std::vector<ObjectGuid> FakedPlayers;

    bool NativeTeam() const;
    uint8 getFRace() const { return m_fRace; }
    uint8 getORace() const { return m_oRace; }
    uint32 getOFaction() const { return m_oFaction; }
    uint32 getFFaction() const { return m_fFaction; }
    uint32 getOPlayerBytes() const { return m_oPlayerBytes; }
    uint32 getFPlayerBytes() const { return m_fPlayerBytes; }
    uint32 getOPlayerBytes2() const { return m_oPlayerBytes2; }
    uint32 getFPlayerBytes2() const { return m_fPlayerBytes2; }

    void SetFakeValues();
    void RecachePlayersFromList();
    void RecachePlayersFromBG();
    WorldPacket BuildNameQuery();
    bool GetRecache() { return m_Recache; }
    void SetRecache() { m_Recache = true; }
    void SetFakedPlayers(FakedPlayers guidlist) { m_FakedPlayers = guidlist; }

    void JoinBattleGround(BattleGround* bg);
    void LeaveBattleGround(BattleGround* bg);

    void FakeDisplayID();

    void SetFakeOnNextTick(bool value = true) { m_FakeOnNextTick = value; }
    bool GetFakeOnNextTick() { return m_FakeOnNextTick; }

    bool SendBattleGroundChat(ChatMsg msgtype, std::string message);

private:
    uint8 m_fRace;
    uint8 m_oRace;
    uint32 m_fFaction;
    uint32 m_oFaction;
    uint32 m_oPlayerBytes;
    uint32 m_oPlayerBytes2;
    uint32 m_fPlayerBytes;
    uint32 m_fPlayerBytes2;

    FakedPlayers m_FakedPlayers;

    bool m_Recache;
    bool m_FakeOnNextTick;

    /************************************************************************/
    /*************************************CFBG*******************************/
    /************************************************************************/
public:
    typedef std::map<Settings::FloatSettings, float>      FloatContainer;
    typedef std::map<Settings::IntSettings, int32>      IntContainer;
    typedef std::map<Settings::UintSettings, uint32>     UintContainer;
    typedef std::map<Settings::StringSettings, std::string>StringContainer;

    void LoadSettings();
    void SaveSettings();

    void SetSetting(Settings::FloatSettings setting, float value) { m_FloatContainer[setting] = value; }
    float GetSetting(Settings::FloatSettings setting) { return m_FloatContainer[setting]; }

    void SetSetting(Settings::IntSettings setting, int32 value) { m_IntContainer[setting] = value; }
    int32 GetSetting(Settings::IntSettings setting) { return m_IntContainer[setting]; }

    void SetSetting(Settings::UintSettings setting, uint32 value) { m_UintContainer[setting] = value; }
    uint32 GetSetting(Settings::UintSettings setting) { return m_UintContainer[setting]; }

    void SetSetting(Settings::StringSettings setting, std::string value) { m_StringContainer[setting] = value; }
    std::string GetSetting(Settings::StringSettings setting) { return m_StringContainer[setting]; }

private:
    FloatContainer  m_FloatContainer;
    IntContainer    m_IntContainer;
    UintContainer   m_UintContainer;
    StringContainer m_StringContainer;

    /************************************************************************/
    /********************************UNCATEGORIZED***************************/
    /************************************************************************/
public:
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
        PlayerTalkClass->GetGossipMenu().AddMenuItem(icon, message.c_str(), sender, action, "", 0);
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
    DelayedSpellLearn m_DelayedSpellLearn;

    bool m_wChatOn;
    Country m_Country;

    ObjectGuid m_LastGossipGuid;

    MultiVendor m_MultiVendor;

    uint32 m_ScriptID;

    uint32 m_SelectedGObject;
};

#endif