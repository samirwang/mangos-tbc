/*
* See AUTHORS file for Copyright information
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

#include "Log.h"
#include "NewPlayer.h"

void CPlayer::LoadSettings()
{
    QueryResult* result = CharacterDatabase.PQuery("SELECT DataTypeId, FloatSetting, IntSetting, UintSetting, StringSetting, SettingNumber FROM player_settings WHERE guid = %u", GetGUIDLow());
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();

            Settings::DataTypeId rDataTypeId = Settings::DataTypeId(fields[0].GetUInt32());
            uint32 SettingNumber = fields[5].GetUInt32();

            switch (rDataTypeId)
            {
            case Settings::SETTING_FLOAT:
                m_FloatContainer.insert(std::make_pair(Settings::FloatSettings(SettingNumber), fields[rDataTypeId].GetFloat()));
                break;
            case Settings::SETTING_INT:
                m_IntContainer.insert(std::make_pair(Settings::IntSettings(SettingNumber), fields[rDataTypeId].GetInt32()));
                break;
            case Settings::SETTING_UINT:
                m_UintContainer.insert(std::make_pair(Settings::UintSettings(SettingNumber), fields[rDataTypeId].GetUInt32()));
                break;
            case Settings::SETTING_STRING:
                m_StringContainer.insert(std::make_pair(Settings::StringSettings(SettingNumber), fields[rDataTypeId].GetCppString()));
                break;
            default:
                sLog.outError("Invalid DataTypeId %u for player %u", uint32(rDataTypeId), GetGUIDLow());
                break;
            }
        } while (result->NextRow());

        delete result;
    }
}

void CPlayer::SaveSettings()
{
    CharacterDatabase.BeginTransaction();
    CharacterDatabase.PExecute("DELETE FROM player_settings WHERE guid = %u", GetGUIDLow());

    for (auto& itr : m_FloatContainer)
        CharacterDatabase.PExecute("INSERT INTO player_settings (guid, SettingNumber, DataTypeID, FloatSetting) VALUES (%u, %u, %u, %f)", GetGUIDLow(), itr.first, Settings::SETTING_FLOAT, itr.second);

    for (auto& itr : m_IntContainer)
        CharacterDatabase.PExecute("INSERT INTO player_settings (guid, SettingNumber, DataTypeID, IntSetting) VALUES (%u, %u, %u, %i)", GetGUIDLow(), itr.first, Settings::SETTING_INT, itr.second);

    for (auto& itr : m_UintContainer)
        CharacterDatabase.PExecute("INSERT INTO player_settings (guid, SettingNumber, DataTypeID, UintSetting) VALUES (%u, %u, %u, %u)", GetGUIDLow(), itr.first, Settings::SETTING_UINT, itr.second);

    for (auto& itr : m_StringContainer)
        CharacterDatabase.PExecute("INSERT INTO player_settings (guid, SettingNumber, DataTypeID, StringSetting) VALUES (%u, %u, %u, '%s')", GetGUIDLow(), itr.first, Settings::SETTING_STRING, itr.second.c_str());

    CharacterDatabase.CommitTransaction();
}