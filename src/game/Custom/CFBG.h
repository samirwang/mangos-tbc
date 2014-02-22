#ifndef _CFBG_H
#define _CFBG_H

#include "Player.h"

class CFBG
{
public:
    CFBG(Player* pPlayer);
    ~CFBG() { };

    typedef std::vector<ObjectGuid> FakedPlayers;

    bool NativeTeam() const;
    uint8 getFRace() const { return m_fRace; }
    uint8 getORace() const { return m_oRace; }
    uint32 getOFaction() const { return m_oFaction; }
    uint32 getFFaction() const { return m_fFaction; }
    uint32 getOPlayerBytes() const { return m_oPlayerBytes; }
    uint32 getFPlayerBytes() const { return m_fPlayerBytes; }
    uint32 getOPlayerBytes2() const { return m_oPlayerBytes2; }
    uint32 getFPlayerBytes2() const { return m_fPlayerBytes2; }

    void SetFakeValues();
    void RecachePlayersFromList();
    void RecachePlayersFromBG();
    WorldPacket BuildNameQuery();
    bool GetRecache() { return m_Recache; }
    void SetRecache() { m_Recache = true; }
    void SetFakedPlayers(FakedPlayers guidlist) { m_FakedPlayers = guidlist; }

    void CJoinBattleGround(BattleGround* bg);
    void CLeaveBattleGround(BattleGround* bg);

    void FakeDisplayID();

    void SetFakeOnNextTick(bool value = true) { m_FakeOnNextTick = value; }
    bool GetFakeOnNextTick() { return m_FakeOnNextTick; }

    bool SendBattleGroundChat(ChatMsg msgtype, std::string message);

private:
    Player* m_player;

    uint8 m_fRace;
    uint8 m_oRace;
    uint32 m_fFaction;
    uint32 m_oFaction;
    uint32 m_oPlayerBytes;
    uint32 m_oPlayerBytes2;
    uint32 m_fPlayerBytes;
    uint32 m_fPlayerBytes2;

    FakedPlayers m_FakedPlayers;

    bool m_Recache;
    bool m_FakeOnNextTick;

};

#endif
