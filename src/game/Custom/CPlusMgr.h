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

#ifndef _CPLUSMGR_H
#define _CPLUSMGR_H

class Player;
class Creature;
class GameObject;
class Quest;
class GameObject;
class Item;

void AddScripts();

template<class T1>
class ScriptObjects
{
public:
    void DeleteScripts()
    {
        for (auto& itr : m_ScriptObjects)
            delete itr.second;

        for (auto& itr : m_ScriptIDs)
            delete itr.second;
    }

    void AddScript(std::string name, T1* script)
    {
        auto itr = m_ScriptObjects.find(name);
        if (itr == m_ScriptObjects.end())
            m_ScriptObjects.insert(std::make_pair(name, script));
        else
            sLog.outError("CPlusMgr: Trying to register scriptname %s which was already registered!", name.c_str());
    }

    T1* GetScript(std::string name)
    {
        auto& itr = m_ScriptObjects.find(name);
        if (itr != m_ScriptObjects.end())
            return itr->second;

        return nullptr;
    }

    T1* GetScript(uint32 entry)
    {
        auto& itr = m_ScriptIDs.find(entry);
        if (itr != m_ScriptIDs.end())
            return itr->second;

        return nullptr;
    }

    void LoadScriptNames(std::string tablename, std::string columnname)
    {
        if (QueryResult* result = WorldDatabase.PQuery("SELECT %s, ScriptName FROM %s WHERE ScriptName <> ''", columnname.c_str(), tablename.c_str()))
        {
            do 
            {
                Field* field = result->Fetch();

                uint32 entry = field[0].GetUInt32();
                std::string name = field[1].GetCppString();

                if (T1* script = GetScript(name))
                    m_ScriptIDs.insert(std::make_pair(entry, script));
                else
                    sLog.outError("CPlusMgr: %s.%s %u has scriptname %s which doesn't belong to any script!", tablename.c_str(), columnname.c_str(), entry, name.c_str());
            }
            while (result->NextRow());

            delete result;
        }
    }

private:
    std::map<std::string, T1*> m_ScriptObjects;
    std::multimap<uint32, T1*> m_ScriptIDs;
};

class CreatureScript;
class GameObjectScript;
class ItemScript;

class CPlusMgr
{
    friend class Script;

public:
    ~CPlusMgr();
    void LoadScripts();

    /* Creature Script */

    CreatureAI* GetCreatureAI(Creature* pCreature);

    bool OnGossipHello(Player* pPlayer, Creature* pCreature);
    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action, std::string code);

    virtual bool OnQuestAccept(Player* pPlayer, Creature* pCreature, Quest const* pQuest);
    virtual bool OnQuestRewarded(Player* pPlayer, Creature* pCreature, Quest const* pQuest);
    virtual uint32 GetDialogStatus(Player* pPlayer, Creature* pCreature);

    /* GameObject Script */

    bool OnGossipHello(Player* pPlayer, GameObject* pGameObject);
    bool OnGossipSelect(Player* pPlayer, GameObject* pGameObject, uint32 sender, uint32 action, std::string code);

    bool OnQuestAccept(Player* pPlayer, GameObject* pGameObject, Quest const* pQuest);
    bool OnQuestRewarded(Player* pPlayer, GameObject* pGameObject, Quest const* pQuest);
    uint32 GetDialogStatus(Player* pPlayer, GameObject* pGameObject);

    bool OnGameObjectUse(Player* pPlayer, GameObject* pGameObject);

    /* Item Script */

    bool OnGossipSelect(Player* pPlayer, Item* pItem, uint32 sender, uint32 action, std::string code);

    bool OnQuestAccept(Player* pPlayer, Item* pItem, Quest const* pQuest);
    bool OnItemUse(Player* pPlayer, Item* pItem, SpellCastTargets const& targets);

    /* Script Storages */

    void AddScript(std::string name, CreatureScript* script) { m_CreatureScripts.AddScript(name, script); }
    void AddScript(std::string name, GameObjectScript* script) { m_GameObjectScripts.AddScript(name, script); }
    void AddScript(std::string name, ItemScript* script) { m_ItemScripts.AddScript(name, script); }

private:
    ScriptObjects<CreatureScript> m_CreatureScripts;
    ScriptObjects<GameObjectScript> m_GameObjectScripts;
    ScriptObjects<ItemScript> m_ItemScripts;
};

#define sCPlusMgr MaNGOS::Singleton<CPlusMgr>::Instance()

class CreatureScript
{
public:
    CreatureScript(std::string name) { sCPlusMgr.AddScript(name, this); }

    virtual CreatureAI* GetCreatureAI(Creature* pCreature) { return nullptr; }

    virtual bool OnGossipHello(Player* pPlayer, Creature* pCreature) { return false; }
    virtual bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action, std::string code) { return false; }

    virtual bool OnQuestAccept(Player* pPlayer, Creature* pCreature, Quest const* pQuest) { return false; }
    virtual bool OnQuestRewarded(Player* pPlayer, Creature* pCreature, Quest const* pQuest) { return false; }
    virtual uint32 GetDialogStatus(Player* pPlayer, Creature* pCreature) { return 0; }
};

class GameObjectScript
{
public:
    GameObjectScript(std::string name) { sCPlusMgr.AddScript(name, this); }

    virtual bool OnGossipHello(Player* pPlayer, GameObject* pGameObject) { return false; }
    virtual bool OnGossipSelect(Player* pPlayer, GameObject* pGameObject, uint32 sender, uint32 action, std::string code) { return false; }

    virtual bool OnQuestAccept(Player* pPlayer, GameObject* pGameObject, Quest const* pQuest) { return false; }
    virtual bool OnQuestRewarded(Player* pPlayer, GameObject* pGameObject, Quest const* pQuest) { return false; }
    virtual uint32 GetDialogStatus(Player* pPlayer, GameObject* pGameObject) { return 0; }

    virtual bool OnGameObjectUse(Player* pPlayer, GameObject* pGameObject) { return false; }
};

class ItemScript
{
public:
    ItemScript(std::string name) { sCPlusMgr.AddScript(name, this); }

    virtual bool OnGossipSelect(Player* pPlayer, Item* pItem, uint32 sender, uint32 action, std::string code) { return false; }

    virtual bool OnQuestAccept(Player* pPlayer, Item* pItem, Quest const* pQuest) { return false; }
    virtual bool OnItemUse(Player* pPlayer, Item* pItem, SpellCastTargets const& targets) { return false; }
};

#endif
