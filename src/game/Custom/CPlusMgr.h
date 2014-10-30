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

#pragma once

#include <atomic>

class ScriptObject
{
    friend class CPlusMgr;

public:

    // Do not override this in scripts; it should be overridden by the various script type classes. It indicates
    // whether or not this script type must be assigned in the database.
    virtual bool IsDatabaseBound() const { return false; }

    const std::string& GetName() const { return _name; }

protected:

    ScriptObject(const char* name)
        : _name(name)
    {
    }

    virtual ~ScriptObject()
    {
    }

private:

    const std::string _name;
};

template<class TObject> class UpdatableScript
{
protected:

    UpdatableScript()
    {
    }

    virtual ~UpdatableScript() { }

public:

    virtual void OnUpdate(TObject* /*obj*/, uint32 /*diff*/) { }
};

class UnitScript : public ScriptObject
{
protected:
    UnitScript(const char* name, bool addToScripts = true);

public:
};

class CreatureScript : public UnitScript, public UpdatableScript < Creature >
{
protected:
    CreatureScript(const char* name);

public:
    bool IsDatabaseBound() const final override { return true; }

public:
    virtual CreatureAI* GetAI(Creature* /*creature*/) { return nullptr; }
    virtual bool GossipHello(Player* /*player*/, Creature* /*creature*/) { return false; }
    virtual bool GossipSelect(Player* /*player*/, Creature* /*creature*/, uint32 /*sender*/, uint32 /*action*/, std::string /*code*/) { return false; }
    virtual bool GossipSelect(Player* /*player*/, Creature* /*creature*/, GossipActionList /*actionlist*/, std::string /*code*/) { return false; }
    virtual bool QuestAccept(Player* /*player*/, Creature* /*creature*/, Quest const* /*quest*/) { return false; }
    virtual bool QuestRewarded(Player* /*player*/, Creature* /*creature*/, Quest const* /*quest*/) { return false; }
    virtual uint32 GetDialogStatus(Player* /*player*/, Creature* /*creature*/) { return 0; }
};

class GameObjectScript : public ScriptObject, public UpdatableScript < GameObject >
{
protected:
    GameObjectScript(const char* name);

public:
    bool IsDatabaseBound() const final override { return true; }

public:
    virtual bool GameObjectUse(Player* /*player*/, GameObject* /*gameobject*/) { return false; }
    virtual bool GossipHello(Player* /*player*/, GameObject* /*gameobject*/) { return false; }
    virtual bool GossipSelect(Player* /*player*/, GameObject* /*gameobject*/, uint32 /*sender*/, uint32 /*action*/, std::string /*code*/) { return false; }
    virtual bool GossipSelect(Player* /*player*/, GameObject* /*gameobject*/, GossipActionList /*actionlist*/, std::string /*code*/) { return false; }
    virtual bool QuestAccept(Player* /*player*/, GameObject* /*gameobject*/, Quest const* /*quest*/) { return false; }
    virtual bool QuestRewarded(Player* /*player*/, GameObject* /*gameobject*/, Quest const* /*qeust*/) { return false; }
    virtual uint32 GetDialogStatus(Player* /*player*/, GameObject* /*gameobject*/) { return 0; }
};

class ItemScript : public ScriptObject
{
protected:
    ItemScript(const char* name);

public:
    bool IsDatabaseBound() const final override { return true; }

public:
    virtual bool ItemUse(Player* /*pPlayer*/, Item* /*pItem*/, SpellCastTargets const& /*targets*/) { return false; }
    virtual bool GossipSelect(Player* /*pPlayer*/, Item* /*pItem*/, uint32 /*sender*/, uint32 /*action*/, std::string /*code*/) { return false; }
    virtual bool GossipSelect(Player* /*player*/, Item* /*item*/, GossipActionList /*actionlist*/, std::string /*code*/) { return false; }
    virtual bool QuestAccept(Player* /*pPlayer*/, Item* /*pItem*/, Quest const* /*pQuest*/) { return false; }
};

// Placed here due to ScriptRegistry::AddScript dependency.
#define sCPlusMgr CPlusMgr::instance()

// Manages registration, loading, and execution of scripts.
class CPlusMgr
{
    friend class ScriptObject;

private:
    CPlusMgr();
    virtual ~CPlusMgr();

public: /* Initialization */
    static CPlusMgr& instance()
    {
        static CPlusMgr instance;
        return instance;
    }

    const char* ScriptsVersion() const { return "Integrated MaNGOS Scripts"; }

    void IncrementScriptCount() { ++_scriptCount; }
    uint32 GetScriptCount() const { return _scriptCount; }

public: /* Loading/Unloading */

    void Load();
    void Unload();

public: /* CreatureScript */

    CreatureAI* GetAI(Creature* creature);
    bool GossipHello(Player* player, Creature* creature);
    bool GossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action, std::string code);
    bool GossipSelect(Player* player, Creature* creature, GossipActionList actionlist, std::string code);
    bool QuestAccept(Player* player, Creature* creature, Quest const* quest);
    bool QuestRewarded(Player* player, Creature* creature, Quest const* quest);
    uint32 GetDialogStatus(Player* player, Creature* creature);

public: /* GameObjectScript */

    bool GameObjectUse(Player* player, GameObject* gameobject);
    bool GossipHello(Player* player, GameObject* gameobject);
    bool GossipSelect(Player* player, GameObject* gameobject, uint32 sender, uint32 action, std::string code);
    bool GossipSelect(Player* player, GameObject* gameobject, GossipActionList actionlist, std::string code);
    bool QuestAccept(Player* player, GameObject* gameobject, Quest const* quest);
    bool QuestRewarded(Player* player, GameObject* gameobject, Quest const* quest);
    uint32 GetDialogStatus(Player* player, GameObject* gameobject);

public: /* ItemScript */

    bool ItemUse(Player* player, Item* item, SpellCastTargets const& targets);
    bool GossipSelect(Player* player, Item* item, uint32 sender, uint32 action, std::string code);
    bool GossipSelect(Player* player, Item* item, GossipActionList actionlist, std::string code);
    bool QuestAccept(Player* player, Item* item, Quest const* quest);

private:

    uint32 _scriptCount;

    //atomic op counter for active scripts amount
    std::atomic_long _scheduledScripts;
};