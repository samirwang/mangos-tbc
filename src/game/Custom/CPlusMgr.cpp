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

#include "CPlusMgr.h"
#include "Log.h"


// This is the global static registry of scripts.
template<class TScript>
class ScriptRegistry
{
public:

    typedef std::map<uint32, TScript*> ScriptMap;
    typedef typename ScriptMap::iterator ScriptMapIterator;

    // The actual list of scripts. This will be accessed concurrently, so it must not be modified
    // after server startup.
    static ScriptMap ScriptPointerList;

    static void AddScript(TScript* const script)
    {
        MANGOS_ASSERT(script);

        // See if the script is using the same memory as another script. If this happens, it means that
        // someone forgot to allocate new memory for a script.
        for (ScriptMapIterator it = ScriptPointerList.begin(); it != ScriptPointerList.end(); ++it)
        {
            if (it->second == script)
            {
                sLog.outError("Script '%s' has same memory pointer as '%s'.",
                    script->GetName().c_str(), it->second->GetName().c_str());

                return;
            }
        }

        if (script->IsDatabaseBound())
        {
            // Get an ID for the script. An ID only exists if it's a script that is assigned in the database
            // through a script name (or similar).
            uint32 id = sScriptMgr.GetScriptId(script->GetName().c_str());
            if (id)
            {
                // Try to find an existing script.
                bool existing = false;
                for (ScriptMapIterator it = ScriptPointerList.begin(); it != ScriptPointerList.end(); ++it)
                {
                    // If the script names match...
                    if (it->second->GetName() == script->GetName())
                    {
                        // ... It exists.
                        existing = true;
                        break;
                    }
                }

                // If the script isn't assigned -> assign it!
                if (!existing)
                {
                    ScriptPointerList[id] = script;
                    sCPlusMgr.IncrementScriptCount();
                }
                else
                {
                    // If the script is already assigned -> delete it!
                    sLog.outError("Script '%s' already assigned with the same script name, so the script can't work.",
                        script->GetName().c_str());

                    MANGOS_ASSERT(false); // Error that should be fixed ASAP.
                }
            }
            else
            {
                // The script uses a script name from database, but isn't assigned to anything.
                sLog.outError("Script named '%s' does not have a script name assigned in database.", script->GetName().c_str());

                // Avoid calling "delete script;" because we are currently in the script constructor
                // In a valid scenario this will not happen because every script has a name assigned in the database
                // If that happens, it's acceptable to just leak a few bytes

                return;
            }
        }
        else
        {
            // We're dealing with a code-only script; just add it.
            ScriptPointerList[_scriptIdCounter++] = script;
            sCPlusMgr.IncrementScriptCount();
        }
    }

    // Gets a script by its ID (assigned by ObjectMgr).
    static TScript* GetScriptById(uint32 id)
    {
        ScriptMapIterator it = ScriptPointerList.find(id);
        if (it != ScriptPointerList.end())
            return it->second;

        return NULL;
    }

private:

    // Counter used for code-only scripts.
    static uint32 _scriptIdCounter;
};

// Utility macros to refer to the script registry.
#define SCR_REG_MAP(T) ScriptRegistry<T>::ScriptMap
#define SCR_REG_ITR(T) ScriptRegistry<T>::ScriptMapIterator
#define SCR_REG_LST(T) ScriptRegistry<T>::ScriptPointerList

// Utility macros for looping over scripts.
#define FOR_SCRIPTS(T, C, E) \
    if (SCR_REG_LST(T).empty()) \
        return; \
    for (SCR_REG_ITR(T) C = SCR_REG_LST(T).begin(); \
        C != SCR_REG_LST(T).end(); ++C)
#define FOR_SCRIPTS_RET(T, C, E, R) \
    if (SCR_REG_LST(T).empty()) \
        return R; \
    for (SCR_REG_ITR(T) C = SCR_REG_LST(T).begin(); \
        C != SCR_REG_LST(T).end(); ++C)
#define FOREACH_SCRIPT(T) \
    FOR_SCRIPTS(T, itr, end) \
    itr->second

// Utility macros for finding specific scripts.
#define GET_SCRIPT(T, I, V) \
    T* V = ScriptRegistry<T>::GetScriptById(I); \
    if (!V) \
        return;
#define GET_SCRIPT_RET(T, I, V, R) \
    T* V = ScriptRegistry<T>::GetScriptById(I); \
    if (!V) \
        return R;


CPlusMgr::CPlusMgr() : _scriptCount(0)
{
    _scheduledScripts = 0;
}

CPlusMgr::~CPlusMgr() { }

void AddScripts();

void CPlusMgr::Load()
{
    AddScripts();
}

void CPlusMgr::Unload()
{
#define SCR_CLEAR(T) \
        for (SCR_REG_ITR(T) itr = SCR_REG_LST(T).begin(); itr != SCR_REG_LST(T).end(); ++itr) \
            delete itr->second; \
        SCR_REG_LST(T).clear();

    // Clear scripts for every script type.
    SCR_CLEAR(UnitScript);
    SCR_CLEAR(CreatureScript);
    SCR_CLEAR(GameObjectScript);
    SCR_CLEAR(ItemScript);

#undef SCR_CLEAR
}

/* UnitScript */

UnitScript::UnitScript(const char* name, bool addToScripts)
    : ScriptObject(name)
{
    if (addToScripts)
        ScriptRegistry<UnitScript>::AddScript(this);
}

/* CreatureScript */

CreatureScript::CreatureScript(const char* name)
    : UnitScript(name, false)
{
    ScriptRegistry<CreatureScript>::AddScript(this);
}

CreatureAI* CPlusMgr::GetAI(Creature* creature)
{
    MANGOS_ASSERT(creature);

    GET_SCRIPT_RET(CreatureScript, creature->GetScriptId(), tmpscript, NULL);
    return tmpscript->GetAI(creature);
}

bool CPlusMgr::GossipHello(Player* player, Creature* creature)
{
    MANGOS_ASSERT(player);
    MANGOS_ASSERT(creature);

    GET_SCRIPT_RET(CreatureScript, creature->GetScriptId(), tmpscript, false);
    player->PlayerTalkClass->ClearMenus();
    return tmpscript->GossipHello(player, creature);
}


bool CPlusMgr::GossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action, std::string code)
{
    MANGOS_ASSERT(player);
    MANGOS_ASSERT(creature);

    GET_SCRIPT_RET(CreatureScript, creature->GetScriptId(), tmpscript, false);
    player->PlayerTalkClass->ClearMenus();
    return tmpscript->GossipSelect(player, creature, sender, action, code);
}

bool CPlusMgr::GossipSelect(Player* player, Creature* creature, GossipActionList actionlist, std::string code)
{
    MANGOS_ASSERT(player);
    MANGOS_ASSERT(creature);

    GET_SCRIPT_RET(CreatureScript, creature->GetScriptId(), tmpscript, false);
    player->PlayerTalkClass->ClearMenus();
    return tmpscript->GossipSelect(player, creature, actionlist, code);
}

bool CPlusMgr::QuestAccept(Player* player, Creature* creature, Quest const* quest)
{
    MANGOS_ASSERT(player);
    MANGOS_ASSERT(creature);

    GET_SCRIPT_RET(CreatureScript, creature->GetScriptId(), tmpscript, false);
    player->PlayerTalkClass->ClearMenus();
    return tmpscript->QuestAccept(player, creature, quest);
}

bool CPlusMgr::QuestRewarded(Player* player, Creature* creature, Quest const* quest)
{
    MANGOS_ASSERT(player);
    MANGOS_ASSERT(creature);

    GET_SCRIPT_RET(CreatureScript, creature->GetScriptId(), tmpscript, false);
    player->PlayerTalkClass->ClearMenus();
    return tmpscript->QuestRewarded(player, creature, quest);
}

uint32 CPlusMgr::GetDialogStatus(Player* player, Creature* creature)
{
    MANGOS_ASSERT(player);
    MANGOS_ASSERT(creature);

    GET_SCRIPT_RET(CreatureScript, creature->GetScriptId(), tmpscript, false);
    player->PlayerTalkClass->ClearMenus();
    return tmpscript->GetDialogStatus(player, creature);
}

/* GameObjectScript */

GameObjectScript::GameObjectScript(const char* name)
    : ScriptObject(name)
{
    ScriptRegistry<GameObjectScript>::AddScript(this);
}

bool CPlusMgr::GameObjectUse(Player* player, GameObject* gameobject)
{
    MANGOS_ASSERT(player);
    MANGOS_ASSERT(gameobject);

    GET_SCRIPT_RET(GameObjectScript, gameobject->GetScriptId(), tmpscript, false);
    player->PlayerTalkClass->ClearMenus();
    return tmpscript->GameObjectUse(player, gameobject);
}

bool CPlusMgr::GossipHello(Player* player, GameObject* gameobject)
{
    MANGOS_ASSERT(player);
    MANGOS_ASSERT(gameobject);

    GET_SCRIPT_RET(GameObjectScript, gameobject->GetScriptId(), tmpscript, false);
    player->PlayerTalkClass->ClearMenus();
    return tmpscript->GossipHello(player, gameobject);
}


bool CPlusMgr::GossipSelect(Player* player, GameObject* gameobject, uint32 sender, uint32 action, std::string code)
{
    MANGOS_ASSERT(player);
    MANGOS_ASSERT(gameobject);

    GET_SCRIPT_RET(GameObjectScript, gameobject->GetScriptId(), tmpscript, false);
    player->PlayerTalkClass->ClearMenus();
    return tmpscript->GossipSelect(player, gameobject, sender, action, code);
}

bool CPlusMgr::GossipSelect(Player* player, GameObject* gameobject, GossipActionList actionlist, std::string code)
{
    MANGOS_ASSERT(player);
    MANGOS_ASSERT(gameobject);

    GET_SCRIPT_RET(GameObjectScript, gameobject->GetScriptId(), tmpscript, false);
    player->PlayerTalkClass->ClearMenus();
    return tmpscript->GossipSelect(player, gameobject, actionlist, code);
}

bool CPlusMgr::QuestAccept(Player* player, GameObject* gameobject, Quest const* quest)
{
    MANGOS_ASSERT(player);
    MANGOS_ASSERT(gameobject);

    GET_SCRIPT_RET(GameObjectScript, gameobject->GetScriptId(), tmpscript, false);
    player->PlayerTalkClass->ClearMenus();
    return tmpscript->QuestAccept(player, gameobject, quest);
}

bool CPlusMgr::QuestRewarded(Player* player, GameObject* gameobject, Quest const* quest)
{
    MANGOS_ASSERT(player);
    MANGOS_ASSERT(gameobject);

    GET_SCRIPT_RET(GameObjectScript, gameobject->GetScriptId(), tmpscript, false);
    player->PlayerTalkClass->ClearMenus();
    return tmpscript->QuestRewarded(player, gameobject, quest);
}

uint32 CPlusMgr::GetDialogStatus(Player* player, GameObject* gameobject)
{
    MANGOS_ASSERT(player);
    MANGOS_ASSERT(gameobject);

    GET_SCRIPT_RET(GameObjectScript, gameobject->GetScriptId(), tmpscript, false);
    player->PlayerTalkClass->ClearMenus();
    return tmpscript->GetDialogStatus(player, gameobject);
}


/* ItemScript */

ItemScript::ItemScript(const char* name)
    : ScriptObject(name)
{
    ScriptRegistry<ItemScript>::AddScript(this);
}

bool CPlusMgr::ItemUse(Player* player, Item* item, SpellCastTargets const& targets)
{
    MANGOS_ASSERT(player);
    MANGOS_ASSERT(item);

    GET_SCRIPT_RET(ItemScript, item->GetScriptId(), tmpscript, false);
    player->PlayerTalkClass->ClearMenus();
    return tmpscript->ItemUse(player, item, targets);
}

bool CPlusMgr::GossipSelect(Player* player, Item* item, uint32 sender, uint32 action, std::string code)
{
    MANGOS_ASSERT(player);
    MANGOS_ASSERT(item);

    GET_SCRIPT_RET(ItemScript, item->GetScriptId(), tmpscript, false);
    player->PlayerTalkClass->ClearMenus();
    return tmpscript->GossipSelect(player, item, sender, action, code);
}

bool CPlusMgr::GossipSelect(Player* player, Item* item, GossipActionList actionlist, std::string code)
{
    MANGOS_ASSERT(player);
    MANGOS_ASSERT(item);

    GET_SCRIPT_RET(ItemScript, item->GetScriptId(), tmpscript, false);
    player->PlayerTalkClass->ClearMenus();
    return tmpscript->GossipSelect(player, item, actionlist, code);
}

bool CPlusMgr::QuestAccept(Player* player, Item* item, Quest const* quest)
{
    MANGOS_ASSERT(player);
    MANGOS_ASSERT(item);

    GET_SCRIPT_RET(ItemScript, item->GetScriptId(), tmpscript, false);
    player->PlayerTalkClass->ClearMenus();
    return tmpscript->QuestAccept(player, item, quest);
}


// Instantiate static members of ScriptRegistry.
template<class TScript> std::map<uint32, TScript*> ScriptRegistry<TScript>::ScriptPointerList;
template<class TScript> uint32 ScriptRegistry<TScript>::_scriptIdCounter = 0;


// Specialize for each script type class like so:
template class ScriptRegistry<UnitScript>;
template class ScriptRegistry<CreatureScript>;
template class ScriptRegistry<GameObjectScript>;
template class ScriptRegistry<ItemScript>;