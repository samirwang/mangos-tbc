#include "RFAG.h"
#include "Log.h"

INSTANTIATE_SINGLETON_1(RFAG);

const PermNameAssign RFAG::m_PermNameAssign[] =
{
    {RFAGS::RFAG_NONE,                  "RFAG_NONE"},
    {RFAGS::RFAG_PLAYER,                "RFAG_PLAYER"},
    {RFAGS::RFAG_MODERATOR,             "RFAG_MODERATOR"},
    {RFAGS::RFAG_GAMEMASTER,            "RFAG_GAMEMASTER"},
    {RFAGS::RFAG_ADMINISTRATOR,         "RFAG_ADMINISTRATOR"},
    {RFAGS::RFAG_LOG_TRADES,            "RFAG_LOG_TRADES"},
    {RFAGS::RFAG_SHOW_GMLIST,           "RFAG_SHOW_GMLIST"},
};

uint32 RFAG::GetPermByName(std::string name)
{
    for (uint32 i = 0; m_PermNameAssign[i].permid != NULL; ++i)
        if (m_PermNameAssign[i].name == name)
            return m_PermNameAssign[i].permid;

    sLog.outError("PERMISSION WITH NAME %s NOT FOUND", name);

    return RFAGS::RFAG_NONE;
}

std::string RFAG::GetNameByPerm(uint32 permid)
{
    for (uint32 i = 0; m_PermNameAssign[i].permid != NULL; ++i)
        if (m_PermNameAssign[i].permid == permid)
            return m_PermNameAssign[i].name;

    sLog.outError("PERMISSION WITH PERMID %u NOT FOUND", uint32(permid));

    return "UNKNOWN";
}

void RFAG::LoadRFAGGroupPerms()
{
    m_GroupPerms.clear();

    QueryResult* result = LoginDatabase.PQuery("SELECT groupid, permid FROM rfag_group_permissions");
    if (result)
    {
        do 
        {
            Field* fields  = result->Fetch();

            uint32 groupid = fields[0].GetUInt32();
            uint32 permid  = fields[1].GetUInt32();

            m_GroupPerms.insert(std::make_pair(groupid, permid));
        }
        while (result->NextRow());
    }

    delete result;
}

PermissionContainer RFAG::GetAccountPerms(uint32 accid)
{
    PermissionContainer permissions;
    QueryResult* result = NULL;

    result = LoginDatabase.PQuery("SELECT permid FROM rfag_account_permissions WHERE accid = %u", accid);
    if (result)
    {
        do 
        {
            Field* fields  = result->Fetch();

            uint32 permid  = fields[0].GetUInt32();

            permissions.push_back(permid);
        }
        while (result->NextRow());
    }

    delete result;
    result = NULL;

    result = LoginDatabase.PQuery("SELECT groupid FROM rfag_account_groups WHERE accid = %u", accid);
    if (result)
    {
        do 
        {
            Field* fields  = result->Fetch();

            uint32 groupid = fields[0].GetUInt32();

            for (PermissionMMap::const_iterator itr1 = m_GroupPerms.begin(); itr1 != m_GroupPerms.end(); ++itr1)
                if (itr1->first == groupid)
                    permissions.push_back(itr1->second);
        }
        while (result->NextRow());
    }

    delete result;

    return permissions;
}

bool RFAG::HasPermissionDatabase(uint32 accid, uint32 permid)
{
    PermissionContainer permissions = GetAccountPerms(accid);

    return (std::find(permissions.begin(), permissions.end(), permid) != permissions.end());
}
