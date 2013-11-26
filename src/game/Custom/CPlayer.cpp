#include "Player.h"
#include "Custom.h"
#include "Chat.h"
#include "WorldPacket.h"
#include "World.h"
#include "ObjectMgr.h"
#include "DBCStores.h"

void Player::CUpdate(uint32 diff)
{
    if (!m_DelayedSpellLearn.empty())
    {
        uint32 spellid = m_DelayedSpellLearn.front();
        bool castable = false;
        bool learn = false;

        if (SpellEntry const* spellInfo = sSpellStore.LookupEntry(spellid))
        {
            for (uint8 i = 0; i < 2; i++)
            {
                if (spellInfo->Effect[i] == SPELL_EFFECT_LEARN_SPELL)
                {
                    castable = true;
                    if (!HasSpell(spellInfo->EffectTriggerSpell[i]))
                       learn = true; 
                }
            }
        }

        if (!learn && !castable && !HasSpell(spellid))
            learn = true;

        if (learn)
        {
            if (castable)
                CastSpell(this, spellid, true);
            else
                learnSpell(spellid, false);
        }

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


    if (m_FakeOnNextTick)
    {
        m_FakeOnNextTick = false;

        SetByteValue(UNIT_FIELD_BYTES_0, 0, getFRace());
        setFaction(getFFaction());
        FakeDisplayID();

        SetUInt32Value(PLAYER_BYTES, getFPlayerBytes());
        SetUInt32Value(PLAYER_BYTES_2, getFPlayerBytes2());
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
        FakeOnNextTick();
}

void Player::OnFirstLogin()
{
    LearnGreenSpells();
}

void Player::SetFakeValues()
{
    m_fRace = sCustom.PickFakeRace(getClass(), GetOTeam());

    m_fFaction = getFactionForRace(m_fRace);

    m_oPlayerBytes = GetUInt32Value(PLAYER_BYTES);
    m_oPlayerBytes2 = GetUInt32Value(PLAYER_BYTES_2);
    m_fPlayerBytes = sCustom.GetFakePlayerBytes(m_fRace, getGender());
    m_fPlayerBytes2 = sCustom.GetFakePlayerBytes2(m_fRace, getGender());

    if (!m_fPlayerBytes)
        m_fPlayerBytes = m_oPlayerBytes;

    if (!m_fPlayerBytes2)
        m_fPlayerBytes2 = m_oPlayerBytes2;
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

    SpellContainer* allSpellContainer = sCustom.GetCachedSpellContainer(getClass());

    if (!allSpellContainer)
    {
        allSpellContainer = new SpellContainer;

        SpellContainer classSpellContainer = sCustom.GetSpellContainerByCreatureEntry(trainerid);

        for (SpellContainer::const_iterator itr = classSpellContainer.begin(); itr != classSpellContainer.end(); ++itr)
            allSpellContainer->push_back(*itr);

        sCustom.CacheSpellContainer(getClass(), allSpellContainer);
    }

    if (allSpellContainer->empty())
        return;

    m_DelayedSpellLearn.clear();


    for (SpellContainer::const_iterator itr = allSpellContainer->begin(); itr != allSpellContainer->end(); ++itr)
    {
        TrainerSpell const* tSpell = &*itr;

        TrainerSpellState state = GetTrainerSpellState(tSpell, tSpell->reqLevel);

        if (state == TRAINER_SPELL_GREEN)
            if (IsInWorld())
                m_DelayedSpellLearn.push_back(tSpell->spell);
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

        SetUInt32Value(PLAYER_BYTES, getFPlayerBytes());
        SetUInt32Value(PLAYER_BYTES_2, getFPlayerBytes2());
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

    SetUInt32Value(PLAYER_BYTES, getOPlayerBytes());
    SetUInt32Value(PLAYER_BYTES_2, getOPlayerBytes2());
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

void Player::SendMultiVendorInventory(uint32 cEntry, ObjectGuid guid)
{
    DEBUG_LOG("WORLD: Sent SMSG_LIST_INVENTORY");

    CreatureInfo const *cinfo = sObjectMgr.GetCreatureTemplate(cEntry);
    if (!cinfo)
    {
        BoxChat << MSG_COLOR_WHITE << "This vendor seems to be removed from the database. Please report that creature " << cEntry << " is missing.\n";
        return;
    }

    Creature* pCreature = GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_NONE);
    if (!pCreature)
    {
        DEBUG_LOG("WORLD: SendMultiVendorInventory - %s not found or you can't interact with him.", guid.GetString().c_str());
        SendSellError(SELL_ERR_CANT_FIND_VENDOR, NULL, guid, 0);
        return;
    }

    VendorItemData const* vItems = sObjectMgr.GetNpcVendorItemList(cEntry);
    VendorItemData const* tItems = cinfo->vendorId ? sObjectMgr.GetNpcVendorTemplateItemList(cinfo->vendorId) : NULL;


    if (!vItems && !tItems)
    {
        WorldPacket data(SMSG_LIST_INVENTORY, (8 + 1 + 1));
        data << GetObjectGuid();
        data << uint8(0);                                   // count==0, next will be error code
        data << uint8(0);                                   // "Vendor has no inventory"
        GetSession()->SendPacket(&data);
        return;
    }

    uint8 customitems = vItems ? vItems->GetItemCount() : 0;
    uint8 numitems = customitems + (tItems ? tItems->GetItemCount() : 0);

    uint8 count = 0;

    WorldPacket data(SMSG_LIST_INVENTORY, (8 + 1 + numitems * 8 * 4));
    data << GetObjectGuid();
    SetMultiVendor(cEntry, guid);

    size_t count_pos = data.wpos();
    data << uint8(count);

    for (int i = 0; i < numitems; ++i)
    {
        VendorItem const* crItem = i < customitems ? vItems->GetItem(i) : tItems->GetItem(i - customitems);

        if (crItem)
        {
            uint32 itemId = crItem->item;
            ItemPrototype const* pProto = ObjectMgr::GetItemPrototype(itemId);
            if (pProto)
            {
                if (!isGameMaster())
                {
                    // class wrong item skip only for bindable case
                    if ((pProto->AllowableClass & getClassMask()) == 0 && pProto->Bonding == BIND_WHEN_PICKED_UP)
                        continue;

                    // race wrong item skip always
                    if ((pProto->AllowableRace & getRaceMask()) == 0)
                        continue;
                }

                ++count;

                data << uint32(count);
                data << uint32(itemId);
                data << uint32(pProto->DisplayInfoID);
                data << uint32(sCustom.maxuint32);
                data << uint32(pProto->BuyPrice);
                data << uint32(pProto->MaxDurability);
                data << uint32(pProto->BuyCount);
                data << uint32(crItem->ExtendedCost);
            }
        }
    }

    if (count == 0)
    {
        data << uint8(0);                                   // "Vendor has no inventory"
        GetSession()->SendPacket(&data);
        return;
    }

    data.put<uint8>(count_pos, count);
    GetSession()->SendPacket(&data);
}

// Return true is the bought item has a max count to force refresh of window by caller
bool Player::BuyItemFromMultiVendor(uint32 item, uint8 count, uint8 bag, uint8 slot)
{
    DEBUG_LOG("WORLD: BuyItemFromMultiVendor");

    // cheating attempt
    if (count < 1) count = 1;

    if (!isAlive())
        return false;

    ItemPrototype const* pProto = ObjectMgr::GetItemPrototype(item);
    if (!pProto)
    {
        SendBuyError(BUY_ERR_CANT_FIND_ITEM, NULL, item, 0);
        return false;
    }

    uint32 entry;
    ObjectGuid guid;
    GetMultiVendor(entry, guid);


    CreatureInfo const *cinfo = sObjectMgr.GetCreatureTemplate(entry);
    if (!cinfo)
    {
        BoxChat << MSG_COLOR_WHITE << "This vendor seems to be removed from the database. Please report that creature " << entry << " is missing.\n";
        return false;
    }

    Creature* pCreature = GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_NONE);
    if (!pCreature)
    {
        DEBUG_LOG("WORLD: BuyItemFromMultiVendor - %s not found or you can't interact with him.", guid.GetString().c_str());
        SendBuyError(BUY_ERR_DISTANCE_TOO_FAR, NULL, item, 0);
        return false;
    }

    VendorItemData const* vItems = sObjectMgr.GetNpcVendorItemList(entry);
    VendorItemData const* tItems = cinfo->vendorId ? sObjectMgr.GetNpcVendorTemplateItemList(cinfo->vendorId) : NULL;

    if ((!vItems || vItems->Empty()) && (!tItems || tItems->Empty()))
    {
        SendBuyError(BUY_ERR_CANT_FIND_ITEM, pCreature, item, 0);
        return false;
    }

    uint32 vCount = vItems ? vItems->GetItemCount() : 0;
    uint32 tCount = tItems ? tItems->GetItemCount() : 0;

    size_t vendorslot = vItems ? vItems->FindItemSlot(item) : vCount;
    if (vendorslot >= vCount)
        vendorslot = vCount + (tItems ? tItems->FindItemSlot(item) : tCount);

    if (vendorslot >= vCount + tCount)
    {
        SendBuyError(BUY_ERR_CANT_FIND_ITEM, pCreature, item, 0);
        return false;
    }

    VendorItem const* crItem = vendorslot < vCount ? vItems->GetItem(vendorslot) : tItems->GetItem(vendorslot - vCount);
    if (!crItem || crItem->item != item) // store diff item (cheating)
    {
        SendBuyError(BUY_ERR_CANT_FIND_ITEM, pCreature, item, 0);
        return false;
    }

    uint32 totalCount = pProto->BuyCount * count;

    // check current item amount if it limited
    if (crItem->maxcount != 0)
    {
        if (pCreature->GetVendorItemCurrentCount(crItem) < totalCount)
        {
            SendBuyError(BUY_ERR_ITEM_ALREADY_SOLD, pCreature, item, 0);
            return false;
        }
    }

    if (uint32(GetReputationRank(pProto->RequiredReputationFaction)) < pProto->RequiredReputationRank)
    {
        SendBuyError(BUY_ERR_REPUTATION_REQUIRE, pCreature, item, 0);
        return false;
    }

    if (uint32 extendedCostId = crItem->ExtendedCost)
    {
        ItemExtendedCostEntry const* iece = sItemExtendedCostStore.LookupEntry(extendedCostId);
        if (!iece)
        {
            sLog.outError("Item %u have wrong ExtendedCost field value %u", pProto->ItemId, extendedCostId);
            return false;
        }

        // honor points price
        if (GetHonorPoints() < (iece->reqhonorpoints * count))
        {
            SendEquipError(EQUIP_ERR_NOT_ENOUGH_HONOR_POINTS, NULL, NULL);
            return false;
        }

        // arena points price
        if (GetArenaPoints() < (iece->reqarenapoints * count))
        {
            SendEquipError(EQUIP_ERR_NOT_ENOUGH_ARENA_POINTS, NULL, NULL);
            return false;
        }

        // item base price
        for (uint8 i = 0; i < MAX_EXTENDED_COST_ITEMS; ++i)
        {
            if (iece->reqitem[i] && !HasItemCount(iece->reqitem[i], iece->reqitemcount[i] * count))
            {
                SendEquipError(EQUIP_ERR_VENDOR_MISSING_TURNINS, NULL, NULL);
                return false;
            }
        }

        // check for personal arena rating requirement
        if (GetMaxPersonalArenaRatingRequirement() < iece->reqpersonalarenarating)
        {
            // probably not the proper equip err
            SendEquipError(EQUIP_ERR_CANT_EQUIP_RANK, NULL, NULL);
            return false;
        }
    }

    if (crItem->conditionId && !isGameMaster() && !sObjectMgr.IsPlayerMeetToCondition(crItem->conditionId, this, pCreature->GetMap(), pCreature, CONDITION_FROM_VENDOR))
    {
        SendBuyError(BUY_ERR_CANT_FIND_ITEM, pCreature, item, 0);
        return false;
    }

    uint32 price = pProto->BuyPrice * count;

    // reputation discount
    price = uint32(floor(price * GetReputationPriceDiscount(pCreature)));

    if (GetMoney() < price)
    {
        SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, pCreature, item, 0);
        return false;
    }

//     if (GetAverageItemLevel() < crItem->itemlevel && !isGameMaster())
//     {
//         // probably not the proper equip err
//         SendEquipError(EQUIP_ERR_CANT_EQUIP_RANK, NULL, NULL);
//         SendChatMessage("You need at least %u averange itemlevel.", crItem->itemlevel);
//         SendChatMessage("You currently have a averange itemlevel %u.", uint32(floor(GetAverageItemLevel())));
//         return false;
//     }

    Item* pItem = NULL;

    if ((bag == NULL_BAG && slot == NULL_SLOT) || IsInventoryPos(bag, slot))
    {
        ItemPosCountVec dest;
        InventoryResult msg = CanStoreNewItem(bag, slot, dest, item, totalCount);
        if (msg != EQUIP_ERR_OK)
        {
            SendEquipError(msg, NULL, NULL, item);
            return false;
        }

        ModifyMoney(-int32(price));

        if (crItem->ExtendedCost)
            TakeExtendedCost(crItem->ExtendedCost, count);

        pItem = StoreNewItem(dest, item, true);
    }
    else if (IsEquipmentPos(bag, slot))
    {
        if (totalCount != 1)
        {
            SendEquipError(EQUIP_ERR_ITEM_CANT_BE_EQUIPPED, NULL, NULL);
            return false;
        }

        uint16 dest;
        InventoryResult msg = CanEquipNewItem(slot, dest, item, false);
        if (msg != EQUIP_ERR_OK)
        {
            SendEquipError(msg, NULL, NULL, item);
            return false;
        }

        ModifyMoney(-int32(price));

        if (crItem->ExtendedCost)
            TakeExtendedCost(crItem->ExtendedCost, count);

        pItem = EquipNewItem(dest, item, true);

        if (pItem)
            AutoUnequipOffhandIfNeed();
    }
    else
    {
        SendEquipError(EQUIP_ERR_ITEM_DOESNT_GO_TO_SLOT, NULL, NULL);
        return false;
    }

    if (!pItem)
        return false;

    uint32 new_count = pCreature->UpdateVendorItemCurrentCount(crItem, totalCount);

    WorldPacket data(SMSG_BUY_ITEM, 8 + 4 + 4 + 4);
    data << pCreature->GetObjectGuid();
    data << uint32(vendorslot + 1); // numbered from 1 at client
    data << uint32(crItem->maxcount > 0 ? new_count : 0xFFFFFFFF);
    data << uint32(count);
    GetSession()->SendPacket(&data);

    SendNewItem(pItem, totalCount, true, false, false);

    return crItem->maxcount != 0;
}

bool Player::SellItemToMultiVendor(ObjectGuid itemGuid, uint8 _count)
{
    DEBUG_LOG("WORLD: SellItemToMultiVendor");

    // prevent possible overflow, as mangos uses uint32 for item count
    uint32 count = _count;

    uint32 entry;
    ObjectGuid guid;
    GetMultiVendor(entry, guid);

    if (!itemGuid)
        return false;

    Creature* pCreature = GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_NONE);
    if (!pCreature)
    {
        DEBUG_LOG("WORLD: SellItemToMultiVendor - %s not found or you can't interact with him.", guid.GetString().c_str());
        SendSellError(SELL_ERR_CANT_FIND_VENDOR, NULL, itemGuid, 0);
        return false;
    }

    // remove fake death
    if (hasUnitState(UNIT_STAT_DIED))
        RemoveSpellsCausingAura(SPELL_AURA_FEIGN_DEATH);

    Item* pItem = GetItemByGuid(itemGuid);
    if (pItem)
    {
        // prevent sell not owner item
        if (GetObjectGuid() != pItem->GetOwnerGuid())
        {
            SendSellError(SELL_ERR_CANT_SELL_ITEM, pCreature, itemGuid, 0);
            return false;
        }

        // prevent sell non empty bag by drag-and-drop at vendor's item list
        if (pItem->IsBag() && !((Bag*)pItem)->IsEmpty())
        {
            SendSellError(SELL_ERR_CANT_SELL_ITEM, pCreature, itemGuid, 0);
            return false;
        }

        // prevent sell currently looted item
        if (GetLootGuid() == pItem->GetObjectGuid())
        {
            SendSellError(SELL_ERR_CANT_SELL_ITEM, pCreature, itemGuid, 0);
            return false;
        }

        // special case at auto sell (sell all)
        if (count == 0)
        {
            count = pItem->GetCount();
        }
        else
        {
            // prevent sell more items that exist in stack (possible only not from client)
            if (count > pItem->GetCount())
            {
                SendSellError(SELL_ERR_CANT_SELL_ITEM, pCreature, itemGuid, 0);
                return false;
            }
        }

        ItemPrototype const* pProto = pItem->GetProto();
        if (pProto)
        {
            if (pProto->SellPrice > 0)
            {
                if (count < pItem->GetCount())              // need split items
                {
                    Item* pNewItem = pItem->CloneItem(count, this);
                    if (!pNewItem)
                    {
                        sLog.outError("WORLD: SellItemToMultiVendor - could not create clone of item %u; count = %u", pItem->GetEntry(), count);
                        SendSellError(SELL_ERR_CANT_SELL_ITEM, pCreature, itemGuid, 0);
                        return false;
                    }

                    pItem->SetCount(pItem->GetCount() - count);
                    ItemRemovedQuestCheck(pItem->GetEntry(), count);
                    if (IsInWorld())
                        pItem->SendCreateUpdateToPlayer(this);
                    pItem->SetState(ITEM_CHANGED, this);

                    AddItemToBuyBackSlot(pNewItem);
                    if (IsInWorld())
                        pNewItem->SendCreateUpdateToPlayer(this);
                }
                else
                {
                    ItemRemovedQuestCheck(pItem->GetEntry(), pItem->GetCount());
                    RemoveItem(pItem->GetBagSlot(), pItem->GetSlot(), true);
                    pItem->RemoveFromUpdateQueueOf(this);
                    AddItemToBuyBackSlot(pItem);
                }

                uint32 money = pProto->SellPrice * count;

                ModifyMoney(money);
            }
            else
                SendSellError(SELL_ERR_CANT_SELL_ITEM, pCreature, itemGuid, 0);

            return true;
        }
    }

    SendSellError(SELL_ERR_CANT_FIND_ITEM, pCreature, itemGuid, 0);
    return false;
}

bool Player::BuyBackItemFromMultiVendor(uint32 slot)
{
    DEBUG_LOG("WORLD: BuyBackItemFromMultiVendor");

    uint32 entry;
    ObjectGuid vendorGuid;
    GetMultiVendor(entry, vendorGuid);


    Creature* pCreature = GetNPCIfCanInteractWith(vendorGuid, UNIT_NPC_FLAG_NONE);
    if (!pCreature)
    {
        DEBUG_LOG("WORLD: BuyBackItemFromMultiVendor - %s not found or you can't interact with him.", vendorGuid.GetString().c_str());
        SendSellError(SELL_ERR_CANT_FIND_VENDOR, NULL, ObjectGuid(), 0);
        return false;
    }

    // remove fake death
    if (hasUnitState(UNIT_STAT_DIED))
        RemoveSpellsCausingAura(SPELL_AURA_FEIGN_DEATH);

    Item* pItem = GetItemFromBuyBackSlot(slot);
    if (pItem)
    {
        uint32 price = GetUInt32Value(PLAYER_FIELD_BUYBACK_PRICE_1 + slot - BUYBACK_SLOT_START);
        if (GetMoney() < price)
        {
            SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, pCreature, pItem->GetEntry(), 0);
            return false;
        }

        ItemPosCountVec dest;
        InventoryResult msg = CanStoreItem(NULL_BAG, NULL_SLOT, dest, pItem, false);
        if (msg == EQUIP_ERR_OK)
        {
            ModifyMoney(-(int32)price);
            RemoveItemFromBuyBackSlot(slot, false);
            ItemAddedQuestCheck(pItem->GetEntry(), pItem->GetCount());
            StoreItem(dest, pItem, true);
        }
        else
            SendEquipError(msg, pItem, NULL);

        return true;
    }
    else
        SendBuyError(BUY_ERR_CANT_FIND_ITEM, pCreature, 0, 0);

    return false;
}
