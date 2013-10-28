#include "WorldSession.h"
#include "Chat.h"
#include "WardenDataStorage.h"
#include "WardenWin.h"
#include "WardenMac.h"
#include "Log.h"
#include "World.h"

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
