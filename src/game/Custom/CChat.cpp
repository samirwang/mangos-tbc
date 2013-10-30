#include "Chat.h"
#include "WorldSession.h"
#include "Player.h"
#include "BattleGround/BattleGround.h"

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