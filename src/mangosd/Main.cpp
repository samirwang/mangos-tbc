/*
 * This file is part of the CMaNGOS Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/// \addtogroup mangosd Mangos Daemon
/// @{
/// \file

#include "Common.h"
#include "Database/DatabaseEnv.h"
#include "Config/Config.h"
#include "Log.h"
#include "Master.h"
#include "SystemConfig.h"
#include "revision.h"
#include "revision_nr.h"
#include <openssl/opensslv.h>
#include <openssl/crypto.h>
#include <ace/Version.h>
#include <ace/Get_Opt.h>

DatabaseType WorldDatabase;                                 ///< Accessor to the world database
DatabaseType CharacterDatabase;                             ///< Accessor to the character database
DatabaseType LoginDatabase;                                 ///< Accessor to the realm/login database

uint32 realmID;                                             ///< Id of the realm

/// Print out the usage string for this program on the console.
void usage(const char* prog)
{
    sLog.outString("Usage: \n%s [<options>]\n"
                   "    -v, --version            print version and exist\n\r"
                   "    -c config_file           use config_file as configuration file\n\r"
                   "    -g                       generate config descriptions\n\r"
                   , prog);
}

/// Launch the mangos server
extern int main(int argc, char** argv)
{
    ///- Command line parsing
    char const* cfg_file = _MANGOSD_CONFIG;

    char const* options = ":a:c:s:";

    ACE_Get_Opt cmd_opts(argc, argv, options);
    cmd_opts.long_option("version", 'v', ACE_Get_Opt::NO_ARG);

    int option;
    while ((option = cmd_opts()) != EOF)
    {
        switch (option)
        {
            case 'c':
                cfg_file = cmd_opts.opt_arg();
                break;
            case 'v':
                printf("%s\n", _FULLVERSION(REVISION_DATE, REVISION_TIME, REVISION_NR, REVISION_ID));
                return 0;
            case ':':
                sLog.outError("Runtime-Error: -%c option requires an input argument", cmd_opts.opt_opt());
                usage(argv[0]);
                Log::WaitBeforeContinueIfNeed();
                return 1;
            default:
                sLog.outError("Runtime-Error: bad format of commandline arguments");
                usage(argv[0]);
                Log::WaitBeforeContinueIfNeed();
                return 1;
        }
    }

    if (!sFileConfig.SetSource(cfg_file))
    {
        sLog.outError("Could not find configuration file %s.", cfg_file);
        Log::WaitBeforeContinueIfNeed();
        return 1;
    }

    sLog.outString("%s [world-daemon]", _FULLVERSION(REVISION_DATE, REVISION_TIME, REVISION_NR, REVISION_ID));
    sLog.outString("<Ctrl-C> to stop.");
    sLog.outString("\n\n"
        "       _____     __  __       _   _  _____  ____   _____ \n"
        "      / ____|   |  \\/  |     | \\ | |/ ____|/ __ \\ / ____|\n"
        "     | |        | \\  / |     |  \\| | |  __  |  | | (___  \n"
        "     | |ontinued| |\\/| | __ _| . ` | | |_ | |  | |\\___ \\ \n"
        "     | |____    | |  | |/ _` | |\\  | |__| | |__| |____) |\n"
        "      \\_____|   |_|  |_| (_| |_| \\_|\\_____|\\____/ \\____/ \n"
        "      http://cmangos.net\\__,_|     Doing things right!\n\n");

    sLog.outString("Using configuration file %s.", cfg_file);

    DETAIL_LOG("%s (Library: %s)", OPENSSL_VERSION_TEXT, SSLeay_version(SSLEAY_VERSION));
    if (SSLeay() < 0x009080bfL)
    {
        DETAIL_LOG("WARNING: Outdated version of OpenSSL lib. Logins to server may not work!");
        DETAIL_LOG("WARNING: Minimal required version [OpenSSL 0.9.8k]");
    }

    DETAIL_LOG("Using ACE: %s", ACE_VERSION);

    ///- and run the 'Master'
    /// \todo Why do we need this 'Master'? Can't all of this be in the Main as for Realmd?
    return sMaster.Run();

    // at sMaster return function exist with codes
    // 0 - normal shutdown
    // 1 - shutdown at error
    // 2 - restart command used, this code can be used by restarter for restart mangosd
}

/// @}
