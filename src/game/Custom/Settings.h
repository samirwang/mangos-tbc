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

#ifndef _SETTINGS_H
#define _SETTINGS_H

#include "Player.h"
#include "Database/DatabaseEnv.h"

enum DataTypeId
{
    SETTING_FLOAT = 1,
    SETTING_INT,
    SETTING_UINT,
    SETTING_STRING
};

enum FloatSettings
{
    SETTING_FLOAT_BEGIN = 0,
    SETTING_FLOAT_END
};

enum IntSettings
{
    SETTING_INT_BEGIN = 0,
    SETTING_INT_END
};

enum UintSettings
{
    SETTING_UINT_BEGIN = 0,
    SETTING_UINT_WCHAT,
    SETTING_UINT_HIDETEMPLATEMENU,
    SETTING_UINT_END
};

enum StringSettings
{
    SETTING_STRING_BEGIN = 0,
    SETTING_STRING_END
};

class Settings
{
public:
    explicit Settings(Player* pPlayer);
    ~Settings() { };

    typedef std::map<FloatSettings, float>      FloatContainer;
    typedef std::map<IntSettings,   int32>      IntContainer;
    typedef std::map<UintSettings,  uint32>     UintContainer;
    typedef std::map<StringSettings,std::string>StringContainer;

    void LoadSettings();
    void SaveSettings();

    void SetSetting(FloatSettings setting, float value) { m_FloatContainer[setting] = value; }
    float GetSetting(FloatSettings setting) { return m_FloatContainer[setting]; }

    void SetSetting(IntSettings setting, int32 value) { m_IntContainer[setting] = value; }
    int32 GetSetting(IntSettings setting) { return m_IntContainer[setting]; }

    void SetSetting(UintSettings setting, uint32 value) { m_UintContainer[setting] = value; }
    uint32 GetSetting(UintSettings setting) { return m_UintContainer[setting]; }

    void SetSetting(StringSettings setting, std::string value) { m_StringContainer[setting] = value; }
    std::string GetSetting(StringSettings setting) { return m_StringContainer[setting]; }

private:
    Player* m_player;

    FloatContainer  m_FloatContainer;
    IntContainer    m_IntContainer;
    UintContainer   m_UintContainer;
    StringContainer m_StringContainer;
};

#endif