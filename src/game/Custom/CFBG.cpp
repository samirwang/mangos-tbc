#include "CFBG.h"
#include "World.h"
#include "Custom.h"

CFBG::CFBG(Player* pPlayer)
{
    m_player = pPlayer;

    m_fRace = 0;
    m_oRace = 0;
    m_fFaction = 0;
    m_oFaction = 0;
    m_oPlayerBytes = 0;
    m_oPlayerBytes2 = 0;
    m_fPlayerBytes = 0;
    m_fPlayerBytes2 = 0;
    m_FakeOnNextTick = 0;
}

bool CFBG::NativeTeam() const
{
    return m_player->GetTeam() == m_player->GetOTeam();
}

void CFBG::SetFakeValues()
{
    m_oRace = m_player->GetByteValue(UNIT_FIELD_BYTES_0, 0);
    m_oFaction = m_player->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE);

    m_fRace = sCustom.PickFakeRace(m_player->getClass(), m_player->GetOTeam());
    m_fFaction = Player::getFactionForRace(m_fRace);

    m_oPlayerBytes = m_player->GetUInt32Value(PLAYER_BYTES);
    m_oPlayerBytes2 = m_player->GetUInt32Value(PLAYER_BYTES_2);
    m_fPlayerBytes = sCustom.GetFakePlayerBytes(m_fRace, m_player->getGender());
    m_fPlayerBytes2 = sCustom.GetFakePlayerBytes2(m_fRace, m_player->getGender());

    if (!m_fPlayerBytes)
        m_fPlayerBytes = m_oPlayerBytes;

    if (!m_fPlayerBytes2)
        m_fPlayerBytes2 = m_oPlayerBytes2;
}


void CFBG::RecachePlayersFromBG()
{
    if (BattleGround* bg = m_player->GetBattleGround())
    {
        for (BattleGround::BattleGroundPlayerMap::const_iterator itr = bg->GetPlayers().begin();
            itr != bg->GetPlayers().end(); ++itr)
        {
            if (Player* player = sObjectMgr.GetPlayer(itr->first))
            {
                if (!player->GetCFBG()->NativeTeam())
                {
                    WorldPacket data(SMSG_INVALIDATE_PLAYER, 8);
                    data << player->GetObjectGuid();
                    m_player->GetSession()->SendPacket(&data);

                    data = player->GetCFBG()->BuildNameQuery();
                    m_player->GetSession()->SendPacket(&data);
                }

                if (!NativeTeam())
                {
                    WorldPacket data(SMSG_INVALIDATE_PLAYER, 8);
                    data << m_player->GetObjectGuid();
                    player->GetSession()->SendPacket(&data);

                    data = BuildNameQuery();
                    player->GetSession()->SendPacket(&data);
                }
            }
            else
            {
                WorldPacket data(SMSG_INVALIDATE_PLAYER, 8);
                data << itr->first;
                m_player->GetSession()->SendPacket(&data);
            }
        }
    }
}

void CFBG::RecachePlayersFromList()
{
    for (FakedPlayers::const_iterator itr = m_FakedPlayers.begin(); itr != m_FakedPlayers.end(); ++itr)
    {
        WorldPacket data(SMSG_INVALIDATE_PLAYER, 8);
        data << *itr;
        m_player->GetSession()->SendPacket(&data);

        if (Player* player = sObjectMgr.GetPlayer(*itr))
        {
            WorldPacket data = player->GetCFBG()->BuildNameQuery();
            m_player->GetSession()->SendPacket(&data);
        }
    }

    m_FakedPlayers.clear();
}

WorldPacket CFBG::BuildNameQuery()
{
    WorldPacket data(SMSG_NAME_QUERY_RESPONSE, (8 + 1 + 4 + 4 + 4 + 10));
    data << m_player->GetObjectGuid();                             // player guid
    data << m_player->GetName();                                   // played name
    data << uint8(0);                                       // realm name for cross realm BG usage
    data << uint32(m_player->getRace());
    data << uint32(m_player->getGender());
    data << uint32(m_player->getClass());
    if (DeclinedName const* names = m_player->GetDeclinedNames())
    {
        data << uint8(1);                                   // is declined
        for (int i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
            data << names->name[i];
    }
    else
        data << uint8(0);                                   // is not declined

    return data;
}

void CFBG::FakeDisplayID()
{
    if (!NativeTeam())
    {
        PlayerInfo const* info = sObjectMgr.GetPlayerInfo(m_player->getRace(), m_player->getClass());
        if (!info)
        {
            for (uint8 i = 1; i <= CLASS_DRUID; i++)
            {
                info = sObjectMgr.GetPlayerInfo(m_player->getRace(), i);
                if (info)
                    break;
            }
        }

        if (!info)
        {
            sLog.outError("Player %u has incorrect race/class pair. Can't init display ids.", m_player->GetGUIDLow());
            return;
        }

        // reset scale before reapply auras
        m_player->SetObjectScale(DEFAULT_OBJECT_SCALE);

        uint8 gender = m_player->getGender();
        switch (gender)
        {
        case GENDER_FEMALE:
            m_player->SetDisplayId(info->displayId_f);
            m_player->SetNativeDisplayId(info->displayId_f);
            break;
        case GENDER_MALE:
            m_player->SetDisplayId(info->displayId_m);
            m_player->SetNativeDisplayId(info->displayId_m);
            break;
        default:
            sLog.outError("Invalid gender %u for player", gender);
            return;
        }

        m_player->SetUInt32Value(PLAYER_BYTES, getFPlayerBytes());
        m_player->SetUInt32Value(PLAYER_BYTES_2, getFPlayerBytes2());
    }
}

void CFBG::CJoinBattleGround(BattleGround* bg)
{
    if (bg->isArena())
        return;

    if (!NativeTeam())
    {
        m_FakedPlayers.push_back(m_player->GetObjectGuid());
        m_player->SetByteValue(UNIT_FIELD_BYTES_0, 0, getFRace());
        m_player->setFaction(getFFaction());
    }

    SetRecache();
    FakeDisplayID();
}

void CFBG::CLeaveBattleGround(BattleGround* bg)
{
    if (bg->isArena())
        return;

    m_player->SetByteValue(UNIT_FIELD_BYTES_0, 0, getORace());
    m_player->setFaction(getOFaction());
    m_player->InitDisplayIds();

    SetFakedPlayers(m_FakedPlayers);
    SetRecache();

    m_player->SetUInt32Value(PLAYER_BYTES, getOPlayerBytes());
    m_player->SetUInt32Value(PLAYER_BYTES_2, getOPlayerBytes2());
}

bool CFBG::SendBattleGroundChat(ChatMsg msgtype, std::string message)
{
    // Select distance to broadcast to.
    float distance = sWorld.getConfig(CONFIG_FLOAT_LISTEN_RANGE_SAY);

    if (msgtype == CHAT_MSG_YELL)
        sWorld.getConfig(CONFIG_FLOAT_LISTEN_RANGE_YELL);
    else if (msgtype == CHAT_MSG_EMOTE)
        sWorld.getConfig(CONFIG_FLOAT_LISTEN_RANGE_TEXTEMOTE);

    if (BattleGround* pBattleGround = m_player->GetBattleGround())
    {
        if (pBattleGround->isArena()) // Only fake chat in BG's. CFBG should not interfere with arenas.
            return false;

        for (BattleGround::BattleGroundPlayerMap::const_iterator itr = pBattleGround->GetPlayers().begin(); itr != pBattleGround->GetPlayers().end(); ++itr)
        {
            if (Player* pPlayer = sObjectMgr.GetPlayer(itr->first))
            {
                if (m_player->GetDistance2d(pPlayer->GetPositionX(), pPlayer->GetPositionY()) <= distance)
                {
                    WorldPacket data(SMSG_MESSAGECHAT, 200);


                    if (m_player->GetTeam() == pPlayer->GetTeam())
                        ChatHandler::BuildChatPacket(data, msgtype, message.c_str(), LANG_UNIVERSAL, m_player->GetChatTag(), m_player->GetObjectGuid(), m_player->GetName());
                    else if (msgtype != CHAT_MSG_EMOTE)
                        ChatHandler::BuildChatPacket(data, msgtype, message.c_str(), pPlayer->GetOTeam() == ALLIANCE ? LANG_ORCISH : LANG_COMMON, m_player->GetChatTag(), m_player->GetObjectGuid(), m_player->GetName());

                    pPlayer->GetSession()->SendPacket(&data);
                }
            }
        }
        return true;
    }
    else
        return false;
}
