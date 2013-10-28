#ifndef _CUSTOM_H
#define _CUSTOM_H

#include "Policies/Singleton.h"
#include "Player.h"

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

enum ClassRaceFlags
{
    CRFLAG_WARRIOR  = 0x00000000,
    CRFLAG_PALADIN  = 0x00000001,
    CRFLAG_HUNTER   = 0x00000002,
    CRFLAG_ROGUE    = 0x00000004,
    CRFLAG_PRIEST   = 0x00000008,
    CRFLAG_SHAMAN   = 0x00000010,
    CRFLAG_MAGE     = 0x00000020,
    CRFLAG_WARLOCK  = 0x00000040,
    CRFLAG_DRUID    = 0x00000080,

    CRFLAG_HUMAN    = 0x00000100,
    CRFLAG_ORC      = 0x00000200,
    CRFLAG_DWARF    = 0x00000400,
    CRFLAG_NIGHTELF = 0x00000800,
    CRFLAG_UNDEAD   = 0x00001000,
    CRFLAG_TAUREN   = 0x00002000,
    CRFLAG_GNOME    = 0x00004000,
    CRFLAG_TROLL    = 0x00008000,
    CRFLAG_BLOODELF = 0x00010000,
    CRFLAG_DRAENEI  = 0x00020000,
};

class Custom
{
public:
    Custom() { };
    ~Custom();

    typedef std::vector<TrainerSpell> SpellContainer;
    typedef std::map<uint32, SpellContainer*> CachedSpellContainer;

    // Flip all the bits to get highest possible value!
    static const uint32 maxuint32 = ~0;

    // Bitwise functions simplified!
    bool HasFlag(uint32& var, int32 flag);
    void SetFlag(uint32& var, int32 flag, std::string description = "");
    void DelFlag(uint32& var, int32 flag, std::string description = "");

    uint32 GetCRFlag(Player* player);
    uint32 GetCRFlag(uint8 pclass, uint8 prace);

    SpellContainer GetSpellContainerByCreatureEntry(uint32 entry);
    SpellContainer* GetSpellContainerByCR(uint32 crval);

    void AddCachedSpellContainerByCR(uint32 crval, SpellContainer* container)
    {
        m_CachedSpellContainer.insert(std::make_pair(crval, container));
    }

    void ClearCachedSpellContainer()
    {
        m_CachedSpellContainer.clear();
    }

    template <class T>
    std::string stringReplace(T istr, T ioldStr, T inewStr)
    {
        std::string str = std::to_string(istr);
        std::string oldStr = std::to_string(ioldStr);
        std::string newStr = std::to_string(inewStr);

        size_t pos = 0;
        while((pos = str.find(oldStr, pos)) != std::string::npos)
        {
            str.replace(pos, oldStr.length(), newStr);
            pos += newStr.length();
        }
        return str;
    }

private:
    static const std::string m_ClassColor[];
    static const std::string m_ItemColor[];
    static const std::string StaffText[];
    static const std::string m_SlotNames[];

    CachedSpellContainer m_CachedSpellContainer;

};

#define sCustom MaNGOS::Singleton<Custom>::Instance()

#endif