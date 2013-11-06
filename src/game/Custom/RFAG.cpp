#include "RFAG.h"
#include "Log.h"

INSTANTIATE_SINGLETON_1(RFAG);

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

Player::PermissionContainer RFAG::GetAccountPerms(uint32 accid)
{
    Player::PermissionContainer permissions;
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