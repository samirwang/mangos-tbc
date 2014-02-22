#include "PlayerSettings.h"
#include "Log.h"

Settings::Settings(Player* pPlayer)
{
    m_player = pPlayer;
}

void Settings::LoadSettings()
{
    QueryResult* result = WorldDatabase.PQuery("SELECT DataTypeId, FloatSetting, IntSetting, UintSetting, StringSetting, SettingNumber FROM player_settings WHERE guid = %u", m_player->GetGUIDLow());
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();

            DataTypeId rDataTypeId = DataTypeId(fields[0].GetUInt32());
            uint32 SettingNumber = fields[6].GetUInt32();

            switch (rDataTypeId)
            {
            case SETTING_FLOAT:
                m_FloatContainer.insert(std::make_pair(FloatSettings(SettingNumber), fields[rDataTypeId].GetFloat()));
                break;
            case SETTING_INT:
                m_IntContainer.insert(std::make_pair(IntSettings(SettingNumber), fields[rDataTypeId].GetInt32()));
                break;
            case SETTING_UINT:
                m_UintContainer.insert(std::make_pair(UintSettings(SettingNumber), fields[rDataTypeId].GetUInt32()));
                break;
            case SETTING_STRING:
                m_StringContainer.insert(std::make_pair(StringSettings(SettingNumber), fields[rDataTypeId].GetCppString()));
                break;
            default:
                sLog.outError("Invalid DataTypeId %u for player %u", uint32(rDataTypeId), m_player->GetGUIDLow());
                break;
            }
        }
        while (result->NextRow());
    }
}

void Settings::SaveSettings()
{
    WorldDatabase.BeginTransaction();
    WorldDatabase.PExecute("DELETE FROM player_settings WHERE guid = %u", m_player->GetGUIDLow());

    for (FloatContainer::const_iterator itr = m_FloatContainer.begin(); itr != m_FloatContainer.end(); ++itr)
    {
        WorldDatabase.PExecute("INSERT INTO player_settings (guid, SettingNumber, DataTypeID, BoolSetting) VALUES (%u, %u, %u, %f)", m_player->GetGUIDLow(), itr->first, SETTING_FLOAT, itr->second);
    }

    for (IntContainer::const_iterator itr = m_IntContainer.begin(); itr != m_IntContainer.end(); ++itr)
    {
        WorldDatabase.PExecute("INSERT INTO player_settings (guid, SettingNumber, DataTypeID, BoolSetting) VALUES (%u, %u, %u, %i)", m_player->GetGUIDLow(), itr->first, SETTING_INT, itr->second);
    }

    for (UintContainer::const_iterator itr = m_UintContainer.begin(); itr != m_UintContainer.end(); ++itr)
    {
        WorldDatabase.PExecute("INSERT INTO player_settings (guid, SettingNumber, DataTypeID, BoolSetting) VALUES (%u, %u, %u, %u)", m_player->GetGUIDLow(), itr->first, SETTING_UINT, itr->second);
    }

    for (StringContainer::const_iterator itr = m_StringContainer.begin(); itr != m_StringContainer.end(); ++itr)
    {
        WorldDatabase.PExecute("INSERT INTO player_settings (guid, SettingNumber, DataTypeID, BoolSetting) VALUES (%u, %u, %u, %s)", m_player->GetGUIDLow(), itr->first, SETTING_STRING, itr->second.c_str());
    }

    WorldDatabase.CommitTransaction();
}