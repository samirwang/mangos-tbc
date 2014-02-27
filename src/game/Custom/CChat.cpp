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

#include "Chat.h"
#include "WorldSession.h"
#include "Player.h"
#include "BattleGround/BattleGround.h"
#include "CPlayer.h"

bool ChatHandler::HandleBGStartCommand(char*)
{
    if(m_session->GetPlayer() && m_session->GetPlayer()->GetBattleGround())
        m_session->GetPlayer()->GetBattleGround()->SetStartDelayTime(100);

    PSendSysMessage("Battleground was started instantly.");

    return true;
}

bool ChatHandler::HandleBGStopCommand(char*)
{
    Player* pPlayer = m_session->GetPlayer();
    BattleGround* pBattleGround = pPlayer->GetBattleGround();

    if(pPlayer && pBattleGround)
        pBattleGround->EndBattleGround(pPlayer->GetTeam());

    PSendSysMessage("Battleground was stopped instantly.");

    return true;
}

bool ChatHandler::HandleLookupIDCommand(char* args)
{
    Player* pPlayer = m_session->GetPlayer();
    CPlayer* pCPlayer = pPlayer->GetCPlayer();

    char* areaargs = args;
    uint32 areaid = 0;
    if (ExtractUint32KeyFromLink(&areaargs, "Harea", areaid))
        pCPlayer->BoxChat << "areaid: " << areaid << std::endl;

    char* areatriggerargs = args;
    uint32 areatriggerid = 0;
    if (ExtractUint32KeyFromLink(&areatriggerargs, "Hareatrigger", areatriggerid))
        pCPlayer->BoxChat << "areatriggerid: " << areatriggerid << std::endl;

    char* areatrigger_targetargs = args;
    uint32 areatrigger_targetid = 0;
    if (ExtractUint32KeyFromLink(&areatrigger_targetargs, "Hareatrigger_target", areatrigger_targetid))
        pCPlayer->BoxChat << "areatrigger_targetid: " << areatrigger_targetid << std::endl;

    char* creatureargs = args;
    uint32 creatureid = 0;
    if (ExtractUint32KeyFromLink(&creatureargs, "Hcreature", creatureid))
        pCPlayer->BoxChat << "creatureid: " << creatureid << std::endl;

    char* creature_entryargs = args;
    uint32 creature_entryid = 0;
    if (ExtractUint32KeyFromLink(&creature_entryargs, "Hcreature_entry", creature_entryid))
        pCPlayer->BoxChat << "creature_entryid: " << creature_entryid << std::endl;

    char* enchantargs = args;
    uint32 enchantid = 0;
    if (ExtractUint32KeyFromLink(&enchantargs, "Henchant", enchantid))
        pCPlayer->BoxChat << "enchantid: " << enchantid << std::endl;

    char* gameeventargs = args;
    uint32 gameeventid = 0;
    if (ExtractUint32KeyFromLink(&gameeventargs, "Hgameevent", gameeventid))
        pCPlayer->BoxChat << "gameeventid: " << gameeventid << std::endl;

    char* gameobjectargs = args;
    uint32 gameobjectid = 0;
    if (ExtractUint32KeyFromLink(&gameobjectargs, "Hgameobject", gameobjectid))
        pCPlayer->BoxChat << "gameobjectid: " << gameobjectid << std::endl;

    char* gameobject_entryargs = args;
    uint32 gameobject_entryid = 0;
    if (ExtractUint32KeyFromLink(&gameobject_entryargs, "Hgameobject_entry", gameobject_entryid))
        pCPlayer->BoxChat << "gameobject_entryid: " << gameobject_entryid << std::endl;

    char* itemargs = args;
    uint32 itemid = 0;
    if (ExtractUint32KeyFromLink(&itemargs, "Hitem", itemid))
        pCPlayer->BoxChat << "itemid: " << itemid << std::endl;

    char* itemsetargs = args;
    uint32 itemsetid = 0;
    if (ExtractUint32KeyFromLink(&itemsetargs, "Hitemset", itemsetid))
        pCPlayer->BoxChat << "itemsetid: " << itemsetid << std::endl;

    char* playerargs = args;
    uint32 playerid = 0;
    if (ExtractUint32KeyFromLink(&playerargs, "Hplayer", playerid))
        pCPlayer->BoxChat << "playerid: " << playerid << std::endl;

    char* poolargs = args;
    uint32 poolid = 0;
    if (ExtractUint32KeyFromLink(&poolargs, "Hpool", poolid))
        pCPlayer->BoxChat << "poolid: " << poolid << std::endl;

    char* questargs = args;
    uint32 questid = 0;
    if (ExtractUint32KeyFromLink(&questargs, "Hquest", questid))
        pCPlayer->BoxChat << "questid: " << questid << std::endl;

    char* skillargs = args;
    uint32 skillid = 0;
    if (ExtractUint32KeyFromLink(&skillargs, "Hskill", skillid))
        pCPlayer->BoxChat << "skillid: " << skillid << std::endl;

    char* spellargs = args;
    uint32 spellid = 0;
    if (ExtractUint32KeyFromLink(&spellargs, "Hspell", spellid))
        pCPlayer->BoxChat << "spellid: " << spellid << std::endl;

    char* talentargs = args;
    uint32 talentid = 0;
    if (ExtractUint32KeyFromLink(&talentargs, "Htalent", talentid))
        pCPlayer->BoxChat << "talentid: " << talentid << std::endl;

    char* taxinodeargs = args;
    uint32 taxinodeid = 0;
    if (ExtractUint32KeyFromLink(&taxinodeargs, "Htaxinode", taxinodeid))
        pCPlayer->BoxChat << "taxinodeid: " << taxinodeid << std::endl;

    char* teleargs = args;
    uint32 teleid = 0;
    if (ExtractUint32KeyFromLink(&teleargs, "Htele", teleid))
        pCPlayer->BoxChat << "teleid: " << teleid << std::endl;

    char* titleargs = args;
    uint32 titleid = 0;
    if (ExtractUint32KeyFromLink(&titleargs, "Htitle", titleid))
        pCPlayer->BoxChat << "titleid: " << titleid << std::endl;

    return true;
}