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

#include "Config.h"
#include "DBConfig.h"

INSTANTIATE_SINGLETON_1(DBConfig);

#define DBCONF_INSERT

bool DBConfig::SetSource(DatabaseType& DB, uint32 realmID)
{
    m_Database = &DB;
    m_RealmID = realmID;

    return Reload();
}

bool DBConfig::Reload()
{
    if (!m_Database)
        return false;

    auto result = m_Database->PQuery("SELECT name, value FROM config WHERE realm = %u", m_RealmID);

    if (!result)
        return false;

    m_ConfigEntries.clear();

    do
    {
        auto fields = result->Fetch();
        std::string name = fields[0].GetCppString();
        std::string val = fields[1].GetCppString();

        m_ConfigEntries.insert(std::make_pair(name, val));
    } while (result->NextRow());

    delete result;

    return true;
}

bool DBConfig::FindEntry(const std::string name, const std::string def)
{
    bool found = m_ConfigEntries.find(name) != m_ConfigEntries.end();

    if (!found)
    {
        sLog.outErrorDb("Could not find config entry by name %s, using default.", name.c_str());

#ifdef DBCONF_INSERT
        m_Database->PExecute("INSERT INTO config (realm, name, value) VALUES ('%u', '%s', '%s')", m_RealmID, name.c_str(), def.c_str());
#endif
    }

    return found;
}

std::string DBConfig::GetStringDefault(const std::string name, const std::string def)
{
    if (!FindEntry(name, def))
        return def;

    return m_ConfigEntries[name];
}

bool DBConfig::GetBoolDefault(const std::string name, const bool def /* = false */)
{
    if (!FindEntry(name, std::to_string(def)))
        return def;

    std::string val = m_ConfigEntries[name];

    for (auto& i : val)
        i = std::towlower(i);
        
    return
        (val.find("true") != std::string::npos) |
        (val.find("yes") != std::string::npos) | 
        (val.find("1") != std::string::npos);
}

int32 DBConfig::GetIntDefault(const std::string name, const int32 def)
{
    if (!FindEntry(name, std::to_string(def)))
        return def;

    int32 val = 0;

    try
    {
        val = std::stoi(m_ConfigEntries[name]);
    }
    catch (std::invalid_argument e)
    {
        sLog.outErrorDb("Config opion %s has invalid value. (Expecting integer) Using default!", name.c_str());
        return def;
    }

    return val;
}

float DBConfig::GetFloatDefault(const std::string name, const float def)
{
    if (!FindEntry(name, std::to_string(def)))
        return def;

    float val = 0;

    try
    {
        val = std::stof(m_ConfigEntries[name]);
    }
    catch (std::invalid_argument e)
    {
        sLog.outErrorDb("Config opion %s has invalid value. (Expecting float) Using default!", name.c_str());
        return def;
    }

    return val;
}