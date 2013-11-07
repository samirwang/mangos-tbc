#ifndef __RFAG_H
#define __RFAG_H

namespace RFAGS
{
    enum
    {
        RFAG_NONE = 0,
        // Compability
        RFAG_PLAYER,        // 1
        RFAG_MODERATOR,     // 2
        RFAG_GAMEMASTER,    // 3
        RFAG_ADMINISTRATOR, // 4
        RFAG_LOG_TRADES,
        // !Compability
        RFAG_HIGH
    };
}

class RFAG
{
public:
    RFAG() { };
    ~RFAG() { };

    typedef std::multimap<uint32, uint32> PermissionMMap;

    void LoadRFAGGroupPerms();

    PermissionContainer GetAccountPerms(uint32 accid);

    bool HasPermissionDatabase(uint32 accid, uint32 permid);

private:
    PermissionMMap m_GroupPerms;
};

#define sRFAG MaNGOS::Singleton<RFAG>::Instance()

#endif