#ifndef __RFAG_H
#define __RFAG_H

class RFAG
{
public:
    RFAG() { };
    ~RFAG() { };

    typedef std::multimap<uint32, uint32> PermissionMMap;

    void LoadRFAGGroupPerms();

    Player::PermissionContainer GetAccountPerms(uint32 accid);

private:
    PermissionMMap m_GroupPerms;
};

#define sRFAG MaNGOS::Singleton<RFAG>::Instance()

#endif