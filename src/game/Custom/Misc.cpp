#include "WorldSession.h"
#include "Chat.h"
#include "WardenDataStorage.h"
#include "WardenWin.h"
#include "WardenMac.h"
#include "Log.h"
#include "World.h"
#include "Unit.h"

void WorldSession::InitWarden(BigNumber *K, std::string os)
{
    if (!sWorld.getConfig(CONFIG_BOOL_WARDEN_ENABLED))
        return;

    if (os == "Win")                                        // Windows
        m_Warden = (WardenBase*)new WardenWin();
    else                                                    // MacOS
        m_Warden = (WardenBase*)new WardenMac();

    m_Warden->Init(this, K);
}

bool ChatHandler::HandleReloadWarden(char* /*args*/)
{
    sLog.outString( "Re-Loading Warden..." );
    WardenDataStorage.Init();
    SendGlobalSysMessage("Warden DB reloaded.");
    return true;
}

uint8 Unit::getRace() const
{
    if (GetTypeId() == TYPEID_PLAYER)
        return ((Player*)this)->NativeTeam() ? GetByteValue(UNIT_FIELD_BYTES_0, 0) : ((Player*)this)->getFRace();

    return GetByteValue(UNIT_FIELD_BYTES_0, 0);
}

uint32 Unit::getFaction() const
{
    if (GetTypeId() == TYPEID_PLAYER)
        return ((Player*)this)->NativeTeam() ? GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE) : ((Player*)this)->getFFaction();

    return GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE);
}