#ifndef _CUSTOM_H
#define _CUSTOM_H

#include "Policies/Singleton.h"
#include "Player.h"
#include "Creature.h"

#define MSG_COLOR_LIGHTRED     "|cffff6060"
#define MSG_COLOR_LIGHTBLUE    "|cff00ccff"
#define MSG_COLOR_ANN_GREEN    "|c1f40af20"
#define MSG_COLOR_RED          "|cffff0000"
#define MSG_COLOR_GOLD         "|cffffcc00"
#define MSG_COLOR_SUBWHITE     "|cffbbbbbb"
#define MSG_COLOR_MAGENTA      "|cffff00ff"
#define MSG_COLOR_YELLOW       "|cffffff00"
#define MSG_COLOR_CYAN         "|cff00ffff"
#define MSG_COLOR_DARKBLUE     "|cff0000ff"

#define MSG_COLOR_GREY         "|cff9d9d9d"
#define MSG_COLOR_WHITE        "|cffffffff"
#define MSG_COLOR_GREEN        "|cff1eff00"
#define MSG_COLOR_BLUE         "|cff0080ff"
#define MSG_COLOR_PURPLE       "|cffb048f8"
#define MSG_COLOR_ORANGE       "|cffff8000"

#define MSG_COLOR_DRUID        "|cffff7d0a"
#define MSG_COLOR_HUNTER       "|cffabd473"
#define MSG_COLOR_MAGE         "|cff69ccf0"
#define MSG_COLOR_PALADIN      "|cfff58cba"
#define MSG_COLOR_PRIEST       "|cffffffff"
#define MSG_COLOR_ROGUE        "|cfffff569"
#define MSG_COLOR_SHAMAN       "|cff0070de"
#define MSG_COLOR_WARLOCK      "|cff9482c9"
#define MSG_COLOR_WARRIOR      "|cffc79c6e"

enum
{
    // Gossip senders
    GOSSIP_SENDER_MAIN             = 1,
    GOSSIP_SENDER_INN_INFO         = 2,
    GOSSIP_SENDER_INFO             = 3,
    GOSSIP_SENDER_SEC_PROFTRAIN    = 4,
    GOSSIP_SENDER_SEC_CLASSTRAIN   = 5,
    GOSSIP_SENDER_SEC_BATTLEINFO   = 6,
    GOSSIP_SENDER_SEC_BANK         = 7,
    GOSSIP_SENDER_SEC_INN          = 8,
    GOSSIP_SENDER_SEC_MAILBOX      = 9,
    GOSSIP_SENDER_SEC_STABLEMASTER = 10,
    GOSSIP_SENDER_MULTIVENDOR      = 11,
    GOSSIP_SENDER_FIRSTLOGIN       = 12,

    // Gossip icons
    ICON_CHAT_BUBBLE               = 0,
    ICON_BAG                       = 1,
    ICON_FLY                       = 2,
    ICON_BOOK                      = 3,
    ICON_GEAR1                     = 4,
    ICON_GEAR2                     = 5,
    ICON_BAG_COIN                  = 6,
    ICON_CHAT_BUBBLE_DOTS          = 7,
    ICON_TABARD                    = 8,
    ICON_CROSSING_SWORDS           = 9,
    ICON_YELLOW_DOT                = 10
};

struct FakePlayerBytes
{
    uint32 PlayerBytes[2];
    uint32 PlayerBytes2[2];
};

typedef std::vector<TrainerSpell> SpellContainer;
typedef std::map<uint32, SpellContainer*> CachedSpellContainer;
typedef std::map<uint8, FakePlayerBytes> FakePlayerBytesContainer;

class Custom
{
public:
    Custom() { };
    ~Custom();

    // Flip all the bits to get highest possible value!
    static const uint32 maxuint32 = ~0;

    // Bitwise functions simplified!
    template <class T1, class T2>
    bool HasFlag(T1& var, T2 flag)
    {
        return (var & flag) != 0;
    }

    template <class T1, class T2>
    void SetFlag(T1& var, T2 flag)
    {
        var |= flag;
    }

    template <class T1, class T2>
    void DelFlag(T1& var, T2 flag)
    {
        var &= ~flag;
    }

    SpellContainer GetSpellContainerByCreatureEntry(uint32 entry);
    SpellContainer* GetCachedSpellContainer(uint32 crval);

    std::string ChatNameWrapper(std::string cName);

    void CacheSpellContainer(uint32 crval, SpellContainer* container)
    {
        m_CachedSpellContainer.insert(std::make_pair(crval, container));
    }

    void ClearCachedSpellContainer()
    {
        m_CachedSpellContainer.clear();
    }

    template <class T1, class T2, class T3>
    std::string stringReplace(const T1 istr, const T2 ioldStr, const T3 inewStr)
    {
        size_t pos = 0;

        std::string str = std::string(istr);
        std::string oldStr = std::string(ioldStr);
        std::string newStr = std::string(inewStr);

        while((pos = str.find(oldStr, pos)) != std::string::npos)
        {
            str.replace(pos, oldStr.length(), newStr);
            pos += newStr.length();
        }
        return str;
    }

    void LoadFakePlayerBytes();

    uint8 PickFakeRace(uint8 pclass, Team team);

    uint32 GetFakePlayerBytes(uint8 race, uint8 gender)
    {
        if (m_FakePlayerBytesContainer.find(race) != m_FakePlayerBytesContainer.end())
            return m_FakePlayerBytesContainer[race].PlayerBytes[gender];

        return 0;
    }

    uint32 GetFakePlayerBytes2(uint8 race, uint8 gender)
    {
        if (m_FakePlayerBytesContainer.find(race) != m_FakePlayerBytesContainer.end())
            return m_FakePlayerBytesContainer[race].PlayerBytes2[gender];

        return 0;
    }

    std::string GetClassColor(uint8 classid) { return m_ClassColor[classid]; }
    std::string GetItemColor(uint8 quality) { return m_ItemColor[quality]; }
    std::string GetSlotName(uint8 slotid) { return m_SlotNames[slotid]; }

private:
    static const std::string m_ClassColor[];
    static const std::string m_ItemColor[];
    static const std::string m_SlotNames[];

    CachedSpellContainer m_CachedSpellContainer;

    FakePlayerBytesContainer m_FakePlayerBytesContainer;
};

#define sCustom MaNGOS::Singleton<Custom>::Instance()

#endif