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
    Settings(Player* pPlayer);
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