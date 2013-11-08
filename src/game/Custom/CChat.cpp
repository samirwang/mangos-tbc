#include "Chat.h"
#include "WorldSession.h"
#include "Player.h"
#include "BattleGround/BattleGround.h"
#include "RFAG.h"

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
    Player* player = m_session->GetPlayer();

    char* areaargs = args;
    uint32 areaid = 0;
    if (ExtractUint32KeyFromLink(&areaargs, "Harea", areaid))
        player->BoxChat << "areaid: " << areaid << std::endl;

    char* areatriggerargs = args;
    uint32 areatriggerid = 0;
    if (ExtractUint32KeyFromLink(&areatriggerargs, "Hareatrigger", areatriggerid))
        player->BoxChat << "areatriggerid: " << areatriggerid << std::endl;

    char* areatrigger_targetargs = args;
    uint32 areatrigger_targetid = 0;
    if (ExtractUint32KeyFromLink(&areatrigger_targetargs, "Hareatrigger_target", areatrigger_targetid))
        player->BoxChat << "areatrigger_targetid: " << areatrigger_targetid << std::endl;

    char* creatureargs = args;
    uint32 creatureid = 0;
    if (ExtractUint32KeyFromLink(&creatureargs, "Hcreature", creatureid))
        player->BoxChat << "creatureid: " << creatureid << std::endl;

    char* creature_entryargs = args;
    uint32 creature_entryid = 0;
    if (ExtractUint32KeyFromLink(&creature_entryargs, "Hcreature_entry", creature_entryid))
        player->BoxChat << "creature_entryid: " << creature_entryid << std::endl;

    char* enchantargs = args;
    uint32 enchantid = 0;
    if (ExtractUint32KeyFromLink(&enchantargs, "Henchant", enchantid))
        player->BoxChat << "enchantid: " << enchantid << std::endl;

    char* gameeventargs = args;
    uint32 gameeventid = 0;
    if (ExtractUint32KeyFromLink(&gameeventargs, "Hgameevent", gameeventid))
        player->BoxChat << "gameeventid: " << gameeventid << std::endl;

    char* gameobjectargs = args;
    uint32 gameobjectid = 0;
    if (ExtractUint32KeyFromLink(&gameobjectargs, "Hgameobject", gameobjectid))
        player->BoxChat << "gameobjectid: " << gameobjectid << std::endl;

    char* gameobject_entryargs = args;
    uint32 gameobject_entryid = 0;
    if (ExtractUint32KeyFromLink(&gameobject_entryargs, "Hgameobject_entry", gameobject_entryid))
        player->BoxChat << "gameobject_entryid: " << gameobject_entryid << std::endl;

    char* itemargs = args;
    uint32 itemid = 0;
    if (ExtractUint32KeyFromLink(&itemargs, "Hitem", itemid))
        player->BoxChat << "itemid: " << itemid << std::endl;

    char* itemsetargs = args;
    uint32 itemsetid = 0;
    if (ExtractUint32KeyFromLink(&itemsetargs, "Hitemset", itemsetid))
        player->BoxChat << "itemsetid: " << itemsetid << std::endl;

    char* playerargs = args;
    uint32 playerid = 0;
    if (ExtractUint32KeyFromLink(&playerargs, "Hplayer", playerid))
        player->BoxChat << "playerid: " << playerid << std::endl;

    char* poolargs = args;
    uint32 poolid = 0;
    if (ExtractUint32KeyFromLink(&poolargs, "Hpool", poolid))
        player->BoxChat << "poolid: " << poolid << std::endl;

    char* questargs = args;
    uint32 questid = 0;
    if (ExtractUint32KeyFromLink(&questargs, "Hquest", questid))
        player->BoxChat << "questid: " << questid << std::endl;

    char* skillargs = args;
    uint32 skillid = 0;
    if (ExtractUint32KeyFromLink(&skillargs, "Hskill", skillid))
        player->BoxChat << "skillid: " << skillid << std::endl;

    char* spellargs = args;
    uint32 spellid = 0;
    if (ExtractUint32KeyFromLink(&spellargs, "Hspell", spellid))
        player->BoxChat << "spellid: " << spellid << std::endl;

    char* talentargs = args;
    uint32 talentid = 0;
    if (ExtractUint32KeyFromLink(&talentargs, "Htalent", talentid))
        player->BoxChat << "talentid: " << talentid << std::endl;

    char* taxinodeargs = args;
    uint32 taxinodeid = 0;
    if (ExtractUint32KeyFromLink(&taxinodeargs, "Htaxinode", taxinodeid))
        player->BoxChat << "taxinodeid: " << taxinodeid << std::endl;

    char* teleargs = args;
    uint32 teleid = 0;
    if (ExtractUint32KeyFromLink(&teleargs, "Htele", teleid))
        player->BoxChat << "teleid: " << teleid << std::endl;

    char* titleargs = args;
    uint32 titleid = 0;
    if (ExtractUint32KeyFromLink(&titleargs, "Htitle", titleid))
        player->BoxChat << "titleid: " << titleid << std::endl;

    return true;
}

bool ChatHandler::HandleGetPermByName(char* args)
{
    char* name = ExtractQuotedOrLiteralArg(&args);

    if (!name)
        return false;

    if (uint32 permid = sRFAG.GetPermByName(std::string(name)))
        PSendSysMessage("Permission %s has id %u", name, permid);

    return true;
}

bool ChatHandler::HandleGetNameByPerm(char* args)
{
    uint32 id;
    if (!ExtractUInt32(&args, id))
        return false;

    std::string name = sRFAG.GetNameByPerm(id);

    if (name != "UNKNOWN")
        PSendSysMessage("Permission %u has name %s", id, name);

    return true;
}