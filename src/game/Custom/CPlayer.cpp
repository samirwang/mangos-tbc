#include "Player.h"
#include "Custom.h"
#include "Chat.h"
#include "WorldPacket.h"
#include "World.h"
#include "ObjectMgr.h"

void Player::CUpdate(uint32 diff)
{
    if (!m_DelayedSpellLearn.empty())
    {
        learnSpell(m_DelayedSpellLearn.front(), false);
        m_DelayedSpellLearn.erase(m_DelayedSpellLearn.begin());

        if (m_DelayedSpellLearn.empty())
            LearnGreenSpells();
    }

    SendSavedChat(CHAT_BOX, BoxChat);
    SendSavedChat(CHAT_WIDE, WideChat);
    SendSavedChat(CHAT_BOTH, BothChat);
}

void Player::Sometimes()
{
    if (GetRecache())
    {
        RecachePlayersFromList();
        RecachePlayersFromBG();
    }

}

void Player::OnLogin()
{
    SetFakeValues();

    if (GetTotalPlayedTime() < 1)
    {
        m_Played_time[PLAYED_TIME_TOTAL] += 1;
        m_Played_time[PLAYED_TIME_LEVEL] += 1;

        OnFirstLogin();
    }

    if (!NativeTeam())
    {
        SetByteValue(UNIT_FIELD_BYTES_0, 0, getFRace());
        setFaction(getFFaction());
        FakeDisplayID();
    }
}

void Player::OnFirstLogin()
{
    LearnGreenSpells();
}

void Player::SetFakeValues()
{
    m_fRace = m_oRace;

    switch (getORace())
    {
    case RACE_HUMAN   : m_fRace = RACE_ORC;         break;
    case RACE_ORC     : m_fRace = RACE_HUMAN;       break;
    case RACE_DWARF   : m_fRace = RACE_TROLL;       break;
    case RACE_NIGHTELF: m_fRace = RACE_UNDEAD;      break;
    case RACE_UNDEAD  : m_fRace = RACE_NIGHTELF;    break;
    case RACE_TAUREN  : m_fRace = RACE_GNOME;       break;
    case RACE_GNOME   : m_fRace = RACE_TAUREN;      break;
    case RACE_TROLL   : m_fRace = RACE_DWARF;       break;
    case RACE_BLOODELF: m_fRace = RACE_DRAENEI;     break;
    case RACE_DRAENEI : m_fRace = RACE_BLOODELF;    break;
    default:
        sLog.outError("CFBG: Player %s has invalid race %u, cannot set fake race!", GetObjectGuid().GetString().c_str(), getORace());
        break;
    }

    m_fFaction = getFactionForRace(m_fRace);
}

void Player::SendSavedChat(MessageTypes type, std::stringstream &ss)
{
    if (!ss.str().empty())
    {
        std::string item;

        while (std::getline(ss, item))
        {
            const char* msg = item.c_str();

            if (type == CHAT_BOX || type == CHAT_BOTH)
                ChatHandler(GetSession()).SendSysMessage(msg);
            if (type == CHAT_WIDE || type == CHAT_BOTH)
            {
                WorldPacket data(SMSG_NOTIFICATION, (strlen(msg) + 1));
                data << msg;
                GetSession()->SendPacket(&data);
            }
        }

        ss.str(""); // Clear content
        ss.clear(); // Clear state flags.
    }
}

void Player::LearnGreenSpells()
{
    uint32 trainerid = 0;

    switch (getClass())
    {
    case CLASS_WARRIOR: trainerid = 26332;  break;
    case CLASS_PALADIN: trainerid = 26327;  break;
    case CLASS_HUNTER:  trainerid = 26325;  break;
    case CLASS_ROGUE:   trainerid = 26329;  break;
    case CLASS_PRIEST:  trainerid = 26328;  break;
    case CLASS_SHAMAN:  trainerid = 26330;  break;
    case CLASS_MAGE:    trainerid = 26326;  break;
    case CLASS_WARLOCK: trainerid = 26331;  break;
    case CLASS_DRUID:   trainerid = 26324;  break;
    default:
        break;
    }

    if (!trainerid)
        return;

    uint32 CRFlag = sCustom.GetCRFlag(getClass(), getRace());

    SpellContainer* allSpellContainer = sCustom.GetSpellContainerByCR(CRFlag);

    if (!allSpellContainer)
    {
        allSpellContainer = new SpellContainer;

        SpellContainer classSpellContainer = sCustom.GetSpellContainerByCreatureEntry(trainerid);

        for (SpellContainer::const_iterator itr = classSpellContainer.begin(); itr != classSpellContainer.end(); ++itr)
            allSpellContainer->push_back(*itr);

        sCustom.AddCachedSpellContainerByCR(CRFlag, allSpellContainer);
    }

    if (allSpellContainer->empty())
        return;

    m_DelayedSpellLearn.clear();

    bool taughtspell = true;

    while(taughtspell)
    {
        taughtspell = false;
        for (SpellContainer::const_iterator itr = allSpellContainer->begin(); itr != allSpellContainer->end(); ++itr)
        {
            TrainerSpell const* tSpell = &*itr;

            TrainerSpellState state = GetTrainerSpellState(tSpell, tSpell->reqLevel);

            if (state == TRAINER_SPELL_GREEN)
            {
                if (IsInWorld())
                    m_DelayedSpellLearn.push_back(tSpell->spell);
                else
                {
                    learnSpell(tSpell->spell, false);
                    taughtspell = true;
                }
            }
        }
    }
}

void Player::RecachePlayersFromList()
{
    for (FakedPlayers::const_iterator itr = m_FakedPlayers.begin(); itr != m_FakedPlayers.end(); ++itr)
    {
        WorldPacket data(SMSG_INVALIDATE_PLAYER, 8);
        data << *itr;
        GetSession()->SendPacket(&data);

        if (Player* player = sObjectMgr.GetPlayer(*itr))
        {
            WorldPacket data = player->BuildNameQuery();
            GetSession()->SendPacket(&data);
        }
    }

    m_FakedPlayers.clear();
}

void Player::RecachePlayersFromBG()
{
    if (BattleGround* bg = GetBattleGround())
    {
        for (BattleGround::BattleGroundPlayerMap::const_iterator itr = bg->GetPlayers().begin();
            itr != bg->GetPlayers().end(); ++itr)
        {
            if (Player* player = sObjectMgr.GetPlayer(itr->first))
            {
                if (!player->NativeTeam())
                {
                    WorldPacket data(SMSG_INVALIDATE_PLAYER, 8);
                    data << player->GetObjectGuid();
                    GetSession()->SendPacket(&data);

                    data = player->BuildNameQuery();
                    GetSession()->SendPacket(&data);
                }

                if (!NativeTeam())
                {
                    WorldPacket data(SMSG_INVALIDATE_PLAYER, 8);
                    data << GetObjectGuid();
                    player->GetSession()->SendPacket(&data);

                    data = BuildNameQuery();
                    player->GetSession()->SendPacket(&data);
                }
            }
            else
            {
                WorldPacket data(SMSG_INVALIDATE_PLAYER, 8);
                data << itr->first;
                GetSession()->SendPacket(&data);
            }
        }
    }
}

WorldPacket Player::BuildNameQuery()
{
    WorldPacket data(SMSG_NAME_QUERY_RESPONSE, (8 + 1 + 4 + 4 + 4 + 10));
    data << GetObjectGuid();                             // player guid
    data << GetName();                                   // played name
    data << uint8(0);                                       // realm name for cross realm BG usage
    data << uint32(getRace());
    data << uint32(getGender());
    data << uint32(getClass());
    if (DeclinedName const* names = GetDeclinedNames())
    {
        data << uint8(1);                                   // is declined
        for (int i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
            data << names->name[i];
    }
    else
        data << uint8(0);                                   // is not declined

    return data;
}

void Player::FakeDisplayID()
{
    if (!NativeTeam())
    {
        PlayerInfo const* info = sObjectMgr.GetPlayerInfo(getRace(), getClass());
        if (!info)
        {
            for (int i = 1; i <= CLASS_DRUID; i++)
            {
                info = sObjectMgr.GetPlayerInfo(getRace(), i);
                if (info)
                    break;
            }
        }

        if (!info)
        {
            sLog.outError("Player %u has incorrect race/class pair. Can't init display ids.", GetGUIDLow());
            return;
        }

        // reset scale before reapply auras
        SetObjectScale(DEFAULT_OBJECT_SCALE);

        uint8 gender = getGender();
        switch (gender)
        {
        case GENDER_FEMALE:
            SetDisplayId(info->displayId_f);
            SetNativeDisplayId(info->displayId_f);
            break;
        case GENDER_MALE:
            SetDisplayId(info->displayId_m);
            SetNativeDisplayId(info->displayId_m);
            break;
        default:
            sLog.outError("Invalid gender %u for player", gender);
            return;
        }
    }
}

Team Player::GetTeam() const
{
    if (GetBattleGround() && GetBattleGround()->isBattleGround())
        return m_bgData.bgTeam ? m_bgData.bgTeam : GetOTeam();

    return GetOTeam();
}

void Player::CJoinBattleGround(BattleGround* bg)
{
    if (!NativeTeam())
        m_FakedPlayers.push_back(GetObjectGuid());

    if (!NativeTeam())
    {
        SetByteValue(UNIT_FIELD_BYTES_0, 0, getFRace());
        setFaction(getFFaction());
    }

    SetRecache();
    FakeDisplayID();
}

void Player::CLeaveBattleGround(BattleGround* /*bg*/)
{
    SetByteValue(UNIT_FIELD_BYTES_0, 0, getORace());
    setFaction(getOFaction());
    InitDisplayIds();

    SetFakedPlayers(m_FakedPlayers);
    SetRecache();
}

bool Player::SendBattleGroundChat(uint32 msgtype, std::string message)
{
    // Select distance to broadcast to.
    float distance = msgtype == CHAT_MSG_SAY ? sWorld.getConfig(CONFIG_FLOAT_LISTEN_RANGE_SAY) : sWorld.getConfig(CONFIG_FLOAT_LISTEN_RANGE_YELL);

    if (BattleGround* pBattleGround = GetBattleGround())
    {
        if (pBattleGround->isArena()) // Only fake chat in BG's. CFBG should not interfere with arenas.
            return false;

        for (BattleGround::BattleGroundPlayerMap::const_iterator itr = pBattleGround->GetPlayers().begin(); itr != pBattleGround->GetPlayers().end(); ++itr)
        {
            if (Player* pPlayer = sObjectMgr.GetPlayer(itr->first))
            {
                if (GetDistance2d(pPlayer->GetPositionX(), pPlayer->GetPositionY()) <= distance)
                {
                    WorldPacket data(SMSG_MESSAGECHAT, 200);

                    if (GetTeam() == pPlayer->GetTeam())
                        BuildPlayerChat(&data, msgtype, message, LANG_UNIVERSAL);
                    else if (msgtype != CHAT_MSG_EMOTE)
                        BuildPlayerChat(&data, msgtype, message, pPlayer->GetOTeam() == ALLIANCE ? LANG_ORCISH : LANG_COMMON);

                    pPlayer->GetSession()->SendPacket(&data);
                }
            }
        }
        return true;
    }
    else
        return false;
}

void Player::CreatePet(uint32 entry, bool classcheck)
{
    if (classcheck && getClass() != CLASS_HUNTER)
        return;

    CreatureInfo const *cinfo = sObjectMgr.GetCreatureTemplate(entry);
    if (!cinfo)
    {
        BoxChat << MSG_COLOR_WHITE << " This pet seems to be removed from the database. Please report that creature " << entry << " is missing.\n";
        return;
    }

    CreatureCreatePos pos(GetSession()->GetPlayer(), GetOrientation());

    Creature* pCreature = new Creature;

    // used guids from specially reserved range (can be 0 if no free values)
    uint32 lowguid = sObjectMgr.GenerateStaticCreatureLowGuid();
    if (!lowguid)
    {
        return;
    }

    if (!pCreature->Create(lowguid, pos, cinfo))
    {
        delete pCreature;
        return;
    }

    //--------------------------------------------------

    if (GetPetGuid())
        UnsummonPetTemporaryIfAny();

    Pet* pet = new Pet(HUNTER_PET);

    if(!pet->CreateBaseAtCreature(pCreature))
    {
        delete pet;
        //PlayerTalkClass->CloseGossip();
        return;
    }

    pet->SetOwnerGuid(GetObjectGuid());
    pet->SetCreatorGuid(GetObjectGuid());
    pet->setFaction(getFaction());
    pet->SetUInt32Value(UNIT_CREATED_BY_SPELL, 13481);

    if (IsPvP())
        pet->SetPvP(true);

    if (!pet->InitStatsForLevel(pCreature->getLevel()))
    {
        sLog.outError("Pet::InitStatsForLevel() failed for creature (Entry: %u)!", pCreature->GetEntry());
        delete pet;
        return;
    }

    pet->GetCharmInfo()->SetPetNumber(sObjectMgr.GeneratePetNumber(), true);
    // this enables pet details window (Shift+P)
    pet->AIM_Initialize();
    pet->InitPetCreateSpells();
    pet->SetHealth(pet->GetMaxHealth());

    // add to world
    pet->GetMap()->Add((Creature*)pet);

    // visual effect for levelup
    pet->SetUInt32Value(UNIT_FIELD_LEVEL,70);

    for (int x = 0; x < 6; x++)
    {
        pet->SetPower(POWER_HAPPINESS,66600000);
        pet->ModifyLoyalty(150000);
        pet->TickLoyaltyChange();
        pet->SetTP(350);
    }

    // caster have pet now
    SetPet(pet);

    pet->SavePetToDB(PET_SAVE_AS_CURRENT);
    PetSpellInitialize();
    pet->learnSpell(27052);
    pet->learnSpell(35698);
    pet->learnSpell(25076);
    pet->learnSpell(27048);
    pet->learnSpell(27053);
    pet->learnSpell(27054);
    pet->learnSpell(27062);
    pet->learnSpell(27047);
    pet->learnSpell(24551);
    delete pCreature;
}

void Player::EnchantItem(uint32 spellid, uint8 slot, const char* sendername)
{
    Item* pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
    if (!pItem)
    {
        BoxChat << sCustom.ChatNameWrapper(sendername) << " Your item could not be enchanted, there are no item equipped in the specified slot.\n";
        return;
    }
    SpellEntry const* spellInfo = sSpellStore.LookupEntry(spellid);
    if (!spellInfo)
    {
        BoxChat << "Invalid spellid " << spellid << " report to devs\n";
        return;
    }
    uint32 enchantid = spellInfo->EffectMiscValue[0];
    if (!enchantid)
    {
        BoxChat << "Invalid enchantid " << enchantid << " report to devs\n";
        return;
    }

    if (!((1 << pItem->GetProto()->SubClass) & spellInfo->EquippedItemSubClassMask) &&
        !((1 << pItem->GetProto()->InventoryType) & spellInfo->EquippedItemInventoryTypeMask))
    {
        BoxChat << sCustom.ChatNameWrapper(sendername) << " Your item could not be enchanted, wrong item type equipped\n";
        return;
    }

    ApplyEnchantment(pItem, PERM_ENCHANTMENT_SLOT, false);
    pItem->SetEnchantment(PERM_ENCHANTMENT_SLOT, enchantid, 0, 0);
    ApplyEnchantment(pItem, PERM_ENCHANTMENT_SLOT, true);
    BoxChat << sCustom.ChatNameWrapper(sendername) << " Your item was enchanted successfully!\n";
}

bool Player::IsAllowedGossipAction(uint32 sender, uint32 action)
{
    bool allowed = false;

    for (AvailableGossipOptions::const_iterator itr = m_AvailableGossipOptions.begin(); itr != m_AvailableGossipOptions.end(); ++itr)
        if (itr->first == sender && itr->second == action)
            allowed = true;

    return allowed;
}