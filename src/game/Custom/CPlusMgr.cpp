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
#include "GossipDef.h"

CPlusMgr::~CPlusMgr()
{
    m_CreatureScripts.DeleteScripts();
}

void CPlusMgr::LoadScripts()
{
    AddScripts();

    m_CreatureScripts.LoadScriptNames("creature_template", "entry");
}

CreatureAI* CPlusMgr::GetCreatureAI(Creature* pCreature)
{
    if (auto script = m_CreatureScripts.GetScript(pCreature->GetEntry()))
    if (auto AI = script->GetCreatureAI(pCreature))
        return AI;

    return nullptr;
}

bool CPlusMgr::OnGossipHello(Player* pPlayer, Creature* pCreature)
{
    pPlayer->PlayerTalkClass->ClearMenus();

    if (auto script = m_CreatureScripts.GetScript(pCreature->GetEntry()))
    if (script->OnGossipHello(pPlayer, pCreature))
        return true;

    return false;
}

bool CPlusMgr::OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action, std::string code)
{
    pPlayer->PlayerTalkClass->ClearMenus();

    if (auto script = m_CreatureScripts.GetScript(pCreature->GetEntry()))
    if (script->OnGossipSelect(pPlayer, pCreature, sender, action, code))
        return true;

    return false;
}

bool CPlusMgr::OnQuestAccept(Player* pPlayer, Creature* pCreature, Quest const* pQuest)
{
    if (auto script = m_CreatureScripts.GetScript(pCreature->GetEntry()))
    if (script->OnQuestAccept(pPlayer, pCreature, pQuest))
        return true;

    return false;
}

bool CPlusMgr::OnQuestRewarded(Player* pPlayer, Creature* pCreature, Quest const* pQuest)
{
    if (auto script = m_CreatureScripts.GetScript(pCreature->GetEntry()))
    if (script->OnQuestRewarded(pPlayer, pCreature, pQuest))
        return true;

    return false;
}

uint32 CPlusMgr::GetDialogStatus(Player* pPlayer, Creature* pCreature)
{
    if (auto script = m_CreatureScripts.GetScript(pCreature->GetEntry()))
    if (uint32 status = script->GetDialogStatus(pPlayer, pCreature))
        return status;

    return false;
}

bool CPlusMgr::OnGossipHello(Player* pPlayer, GameObject* pGameObject)
{
    if (auto script = m_GameObjectScripts.GetScript(pGameObject->GetEntry()))
    if (script->OnGossipHello(pPlayer, pGameObject))
        return true;

    return false;
}

bool CPlusMgr::OnGossipSelect(Player* pPlayer, GameObject* pGameObject, uint32 sender, uint32 action, std::string code)
{
    if (auto script = m_GameObjectScripts.GetScript(pGameObject->GetEntry()))
    if (script->OnGossipSelect(pPlayer, pGameObject, sender, action, code))
        return true;

    return false;
}

bool CPlusMgr::OnQuestAccept(Player* pPlayer, GameObject* pGameObject, Quest const* pQuest)
{
    if (auto script = m_GameObjectScripts.GetScript(pGameObject->GetEntry()))
    if (script->OnQuestAccept(pPlayer, pGameObject, pQuest))
        return true;

    return false;
}

bool CPlusMgr::OnQuestRewarded(Player* pPlayer, GameObject* pGameObject, Quest const* pQuest)
{
    if (auto script = m_GameObjectScripts.GetScript(pGameObject->GetEntry()))
    if (script->OnGossipHello(pPlayer, pGameObject))
        return true;

    return false;
}

uint32 CPlusMgr::GetDialogStatus(Player* pPlayer, GameObject* pGameObject)
{
    if (auto script = m_GameObjectScripts.GetScript(pGameObject->GetEntry()))
    if (uint32 status = script->GetDialogStatus(pPlayer, pGameObject))
        return status;

    return 0;
}

bool CPlusMgr::OnGameObjectUse(Player* pPlayer, GameObject* pGameObject)
{
    if (auto script = m_GameObjectScripts.GetScript(pGameObject->GetEntry()))
    if (script->OnGameObjectUse(pPlayer, pGameObject))
        return true;

    return false;
}

bool CPlusMgr::OnGossipSelect(Player* pPlayer, Item* pItem, uint32 sender, uint32 action, std::string code)
{
    if (auto script = m_ItemScripts.GetScript(pItem->GetEntry()))
    if (script->OnGossipSelect(pPlayer, pItem, sender, action, code))
        return true;

    return false;
}

bool CPlusMgr::OnQuestAccept(Player* pPlayer, Item* pItem, Quest const* pQuest)
{
    if (auto script = m_ItemScripts.GetScript(pItem->GetEntry()))
    if (script->OnQuestAccept(pPlayer, pItem, pQuest))
        return true;

    return false;
}

bool CPlusMgr::OnItemUse(Player* pPlayer, Item* pItem, SpellCastTargets const& targets)
{
    if (auto script = m_ItemScripts.GetScript(pItem->GetEntry()))
    if (script->OnItemUse(pPlayer, pItem, targets))
        return true;

    return false;
}
