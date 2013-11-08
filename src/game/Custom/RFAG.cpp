#include "RFAG.h"
#include "Log.h"

INSTANTIATE_SINGLETON_1(RFAG);

const PermNameAssign RFAG::m_PermNameAssign[] =
{
    {RFAGS::PLAYER,                "PLAYER"},
    {RFAGS::MODERATOR,             "MODERATOR"},
    {RFAGS::GAMEMASTER,            "GAMEMASTER"},
    {RFAGS::ADMINISTRATOR,         "ADMINISTRATOR"},
    {RFAGS::LOG_TRADES,            "LOG_TRADES"},
    {RFAGS::SHOW_GMLIST,           "SHOW_GMLIST"},
};

uint32 RFAG::GetPermByName(std::string name)
{
    uint32 end = sizeof(m_PermNameAssign)/sizeof(m_PermNameAssign[0]);
    for (uint32 i = 0; i < end; ++i)
        if (m_PermNameAssign[i].name == name)
            return m_PermNameAssign[i].permid;

    sLog.outError("PERMISSION WITH NAME %s NOT FOUND", name);

    return RFAGS::NONE;
}

std::string RFAG::GetNameByPerm(uint32 permid)
{
    uint32 end = sizeof(m_PermNameAssign)/sizeof(m_PermNameAssign[0]);
    for (uint32 i = 0; i < end; ++i)
        if (m_PermNameAssign[i].permid == permid)
            return m_PermNameAssign[i].name;

    sLog.outError("PERMISSION WITH PERMID %u NOT FOUND", uint32(permid));

    return "UNKNOWN";
}

void RFAG::LoadRFAGGroupPerms()
{
    m_GroupPerms.clear();

    QueryResult* result = LoginDatabase.PQuery("SELECT groupid, permname FROM rfag_group_permissions");
    if (result)
    {
        do 
        {
            Field* fields  = result->Fetch();

            uint32 groupid  = fields[0].GetUInt32();
            std::string permname = fields[1].GetCppString();
            uint32 permid = sRFAG.GetPermByName(permname);

            sLog.outDebug("rfag_group_permissions: groupid: %u permname: %s permid: %u", groupid, permname.c_str(), permid);

            if (permid != RFAGS::NONE)
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

    result = LoginDatabase.PQuery("SELECT groupid FROM rfag_account_groups WHERE accid = %u", accid);
    if (result)
    {
        do 
        {
            Field* fields  = result->Fetch();

            uint32 groupid = fields[0].GetUInt32();

            for (PermissionMMap::const_iterator itr1 = m_GroupPerms.begin(); itr1 != m_GroupPerms.end(); ++itr1)
            {
                if (itr1->first == groupid)
                {
                    sLog.outDebug("rfag_account_groups: accid: %u groupid: %u", accid, groupid);
                    permissions.push_back(itr1->second);
                }
            }
        }
        while (result->NextRow());
    }

    delete result;
    result = NULL;

    result = LoginDatabase.PQuery("SELECT permname, remove FROM rfag_account_permissions WHERE accid = %u", accid);
    if (result)
    {
        do 
        {
            Field* fields  = result->Fetch();
            std::string permname = fields[0].GetCppString();
            uint32 permid = sRFAG.GetPermByName(permname);
            bool remove = fields[1].GetUInt8();

            sLog.outDebug("rfag_account_permissions: accid: %u permname: %s permid: %u", accid, permname.c_str(), permid);

            if (permid != RFAGS::NONE)
            {
                if (remove)
                    permissions.erase(std::remove(permissions.begin(), permissions.end(), permid), permissions.end());
                else
                    permissions.push_back(permid);
            }
        }
        while (result->NextRow());
    }

    delete result;

    // Copypasta from interwebz, we want no double permissions.
    std::sort(permissions.begin(), permissions.end());
    permissions.erase(std::unique(permissions.begin(), permissions.end()), permissions.end());

    return permissions;
}

bool RFAG::HasPermissionDatabase(uint32 accid, uint32 permid)
{
    PermissionContainer permissions = GetAccountPerms(accid);

    return (std::find(permissions.begin(), permissions.end(), permid) != permissions.end());
}
