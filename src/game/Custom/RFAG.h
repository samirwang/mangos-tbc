#ifndef __RFAG_H
#define __RFAG_H

#include "Common.h"
#include "WorldSession.h"

namespace RFAGS
{
    enum
    {
        NONE,
        // Compability
        PLAYER,
        MODERATOR,
        GAMEMASTER,
        ADMINISTRATOR,
        LOG_TRADES,
        SHOW_GMLIST,
        // !Compability
        INVALID
    };
};

struct PermNameAssign
{
    uint32 permid;
    std::string  name;
};

class RFAG
{
public:
    RFAG() { };
    ~RFAG() { };

    typedef std::multimap<uint32, uint32> PermissionMMap;

    void LoadRFAGGroupPerms();

    PermissionContainer GetAccountPerms(uint32 accid);

    bool HasPermissionDatabase(uint32 accid, uint32 permid);

    uint32 GetPermByName(std::string name);
    std::string  GetNameByPerm(uint32 perm);

private:
    PermissionMMap m_GroupPerms;
    static const PermNameAssign m_PermNameAssign[];
};

#define sRFAG MaNGOS::Singleton<RFAG>::Instance()

#endif