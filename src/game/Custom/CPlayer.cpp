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

#include "Player.h"
#include "Custom.h"
#include "Chat.h"
#include "WorldPacket.h"
#include "World.h"
#include "ObjectMgr.h"
#include "DBCStores.h"
#include "CPlusMgr.h"
#include "CPlayer.h"
#include "AntiCheat.h"

CPlayer::CPlayer(WorldSession* session) : Player(session)
{
    m_CheatDetectors.push_back(new AntiCheat_speed(this));
    m_CheatDetectors.push_back(new AntiCheat_jump(this));
    m_CheatDetectors.push_back(new AntiCheat_fly(this));
    m_CheatDetectors.push_back(new AntiCheat_climb(this));
    m_CheatDetectors.push_back(new AntiCheat_teleport(this));

    m_PendingReward = 0;

    m_GmFly = false;
    m_FirstMoveInfo = false;
    m_SkipAntiCheat = 1;

    m_fRace = 0;
    m_oRace = 0;
    m_fFaction = 0;
    m_oFaction = 0;
    m_oPlayerBytes = 0;
    m_oPlayerBytes2 = 0;
    m_fPlayerBytes = 0;
    m_fPlayerBytes2 = 0;
    m_Recache = 0;
    m_FakeOnNextTick = 0;

    m_wChatOn = false;
    m_Country = Country();
    m_LastGossipGuid = ObjectGuid();
    m_MultiVendor = MultiVendor();
    m_ScriptID = 0;
    m_SelectedGObject = 0;
}

CPlayer::~CPlayer()
{
    for (auto& i : m_CheatDetectors)
        delete i;
}

void CPlayer::CUpdate(uint32 diff)
{
    LearnGreenSpells();

    SendSteamMessages(MessageTypes(CHAT_BOX), BoxChat);
    SendSteamMessages(MessageTypes(CHAT_WIDE), WideChat);
    SendSteamMessages(MessageTypes(CHAT_BOX | CHAT_WIDE), BothChat);
}

void CPlayer::Sometimes()
{
    if (GetRecache())
    {
        RecachePlayersFromList();
        RecachePlayersFromBG();
    }

    if (GetFakeOnNextTick())
    {
        SetFakeOnNextTick(false);

        SetByteValue(UNIT_FIELD_BYTES_0, 0, getFRace());
        setFaction(getFFaction());
        FakeDisplayID();

        SetUInt32Value(PLAYER_BYTES, getFPlayerBytes());
        SetUInt32Value(PLAYER_BYTES_2, getFPlayerBytes2());
    }
}

void CPlayer::LoadCountryData()
{
    std::ostringstream ss;

    ss << " SELECT"
       << " c.iso_code_2, c.iso_code_3, c.country"
       << " FROM"
       << " ip2nationcountries c,"
       << " ip2nation i"
       << " WHERE"
       << " i.ip < INET_ATON('" << GetSession()->GetRemoteAddress() << "')"
       << " AND"
       << " c.code = i.country"
       << " ORDER BY"
       << " i.ip DESC"
       << " LIMIT 1";

    if (QueryResult* result = LoginDatabase.PQuery("%s", ss.str().c_str()))
    {
        Field* fields = result->Fetch();

        m_Country.ISO2 = fields[0].GetCppString();
        m_Country.ISO3 = fields[1].GetCppString();
        m_Country.FULL = fields[2].GetCppString();

        delete result;
    }

    if (GetSession()->GetRemoteAddress() == "127.0.0.1")
    {
        m_Country.ISO2 = "Local";
        m_Country.ISO3 = "Local";
        m_Country.FULL = "Local";
    }
}

void CPlayer::OnLogin()
{
    LoadSettings();
    SetFakeValues();

    LoadCountryData();

    SetWChat(GetSetting(Settings::SETTING_UINT_WCHAT));

    if (!NativeTeam())
        SetFakeOnNextTick();

    if (GetTotalPlayedTime() < 1)
    {
        m_Played_time[PLAYED_TIME_TOTAL] += 1;
        m_Played_time[PLAYED_TIME_LEVEL] += 1;

        OnFirstLogin();
    }
}

void CPlayer::OnFirstLogin()
{
    FillGreenSpellList();
}

void CPlayer::AddItemSet(uint32 setid)
{
    std::vector<uint32> Items;

    ItemSetEntry const* set = sItemSetStore.LookupEntry(setid);
    if (set)
    {
        for (auto i = 0; i < 17; ++i)
        {
            uint32 itemid = set->itemId[i];
            if (itemid)
            {
                if (StoreNewItemInBestSlots(itemid, 1))
                    Items.push_back(itemid);
                else
                {
                    for (auto& itr : Items)
                        DestroyItemCount(itr, 1, true);

                    BoxChat << "Not enough space to store itemset" << std::endl;
                    break;
                }
            }
        }
    }
}

std::string CPlayer::GetNameLink(bool applycolors)
{
    std::string name = GetName();
    std::string teamcolor = GetOTeam() == ALLIANCE ? MSG_COLOR_DARKBLUE : MSG_COLOR_RED;
    std::ostringstream ss;

    if (isGameMaster())
        teamcolor = MSG_COLOR_PURPLE;

    ss << "|Hplayer:" << name << "|h";

    if (applycolors)
        ss << teamcolor << "[" << sCustom.GetClassColor(getClass()) << name << teamcolor << "]|h";
    else
        ss << "[" << name << "]|h";

    return ss.str();
}

void CPlayer::SendWorldChatMsg(std::string msg)
{
    std::string chatstring = GetNameLink(true) + MSG_COLOR_WHITE + ": " + msg;

    sCustom.SendWorldChat(GetObjectGuid(), sCustom.stringReplace(chatstring, "|r", MSG_COLOR_WHITE));
}

void CPlayer::SendSteamMessages(MessageTypes type, std::stringstream &ss)
{
    if (!ss.str().empty())
    {
        std::string item;

        while (std::getline(ss, item))
        {
            item = MSG_COLOR_WHITE + item + "|r"; // Default chat to white.
            const char* msg = item.c_str();

            if ((type & CHAT_BOX) != 0)
                ChatHandler(GetSession()).SendSysMessage(msg);
            if ((type & CHAT_WIDE) != 0)
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

void CPlayer::FillGreenSpellList()
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

    Custom::SpellContainer* allSpellContainer = sCustom.GetCachedSpellContainer(getClass());

    if (!allSpellContainer)
    {
        allSpellContainer = new Custom::SpellContainer;

        Custom::SpellContainer classSpellContainer = sCustom.GetSpellContainerByCreatureEntry(trainerid);

        for (auto& itr : classSpellContainer)
            allSpellContainer->push_back(itr);

        sCustom.CacheSpellContainer(getClass(), allSpellContainer);
    }

    if (allSpellContainer->empty())
        return;

    m_DelayedSpellLearn.clear();

    for (auto itr = allSpellContainer->cbegin(); itr != allSpellContainer->cend(); ++itr)
    {
        TrainerSpell const* tSpell = &*itr;

        TrainerSpellState state = GetTrainerSpellState(tSpell, tSpell->reqLevel);

        if (state == TRAINER_SPELL_GREEN)
        {
            if (IsInWorld())
            {
                bool CastLearned = false;

                if (SpellEntry const* spellInfo = sSpellStore.LookupEntry(tSpell->spell))
                {
                    for (auto i = 0; i < MAX_EFFECT_INDEX; ++i)
                    {
                        if (spellInfo->Effect[i] == SPELL_EFFECT_LEARN_SPELL)
                        {
                            CastLearned = true;

                            if (!HasSpell(spellInfo->EffectTriggerSpell[i]))
                                m_DelayedSpellLearn.push_back(spellInfo->EffectTriggerSpell[i]);
                        }
                    }
                }

                if (!CastLearned)
                    m_DelayedSpellLearn.push_back(tSpell->spell);
            }
        }
    }
}

void CPlayer::LearnGreenSpells()
{
    if (m_DelayedSpellLearn.empty())
        return;

    uint32 spellid = m_DelayedSpellLearn.front();

    learnSpell(spellid, false);

    m_DelayedSpellLearn.erase(m_DelayedSpellLearn.begin());

    if (m_DelayedSpellLearn.empty())
        FillGreenSpellList();
}

void CPlayer::CreatePet(uint32 entry, bool classcheck)
{
    if (classcheck && getClass() != CLASS_HUNTER)
        return;

    CreatureInfo const *cinfo = sObjectMgr.GetCreatureTemplate(entry);
    if (!cinfo)
    {
        BoxChat << MSG_COLOR_WHITE << " This pet doesn't exist in our database. Please report that creature " << entry << " is missing." << std::endl;
        return;
    }

    CreatureCreatePos pos(GetSession()->GetPlayer(), GetOrientation());

    Creature* pCreature = new Creature;

    // used guids from specially reserved range (can be 0 if no free values)
    uint32 lowguid = sObjectMgr.GenerateStaticCreatureLowGuid();
    if (!lowguid)
        return;

    if (!pCreature->Create(lowguid, pos, cinfo))
    {
        delete pCreature;
        return;
    }

    //--------------------------------------------------

    if (GetPetGuid())
        UnsummonPetTemporaryIfAny();

    Pet* pet = new Pet(HUNTER_PET);

    if (!pet->CreateBaseAtCreature(pCreature))
    {
        delete pet;
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
    pet->SetUInt32Value(UNIT_FIELD_LEVEL, 70);

    for (auto x = 0; x < 6; x++)
    {
        pet->SetPower(POWER_HAPPINESS, 66600000);
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

bool CPlayer::EnchantItem(uint32 spellid, uint8 slot)
{
    Item* pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
    if (!pItem)
    {
        BoxChat << "Your item could not be enchanted, there are no item equipped in the specified slot." << std::endl;

        return false;
    }
    SpellEntry const* spellInfo = sSpellStore.LookupEntry(spellid);
    if (!spellInfo)
    {
        BoxChat << "Invalid spellid " << spellid << " report to devs" << std::endl;
        return false;
    }
    uint32 enchantid = spellInfo->EffectMiscValue[0];
    if (!enchantid)
    {
        BoxChat << "Invalid enchantid " << enchantid << " report to devs" << std::endl;
        return false;
    }

    if (!((1 << pItem->GetProto()->SubClass) & spellInfo->EquippedItemSubClassMask) &&
        !((1 << pItem->GetProto()->InventoryType) & spellInfo->EquippedItemInventoryTypeMask))
    {
        BoxChat << "Your item could not be enchanted, wrong item type equipped" << std::endl;

        return false;
    }

    ApplyEnchantment(pItem, PERM_ENCHANTMENT_SLOT, false);
    pItem->SetEnchantment(PERM_ENCHANTMENT_SLOT, enchantid, 0, 0);
    ApplyEnchantment(pItem, PERM_ENCHANTMENT_SLOT, true);

    return true;
}

void CPlayer::SendMultiVendorInventory(uint32 cEntry, ObjectGuid guid)
{
    DEBUG_LOG("WORLD: Sent SMSG_LIST_INVENTORY");

    CreatureInfo const *cinfo = sObjectMgr.GetCreatureTemplate(cEntry);
    if (!cinfo)
    {
        BoxChat << MSG_COLOR_WHITE << "This vendor seems to be removed from the database. Please report that creature " << cEntry << " is missing." << std::endl;
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
    VendorItemData const* tItems = cinfo->VendorTemplateId ? sObjectMgr.GetNpcVendorTemplateItemList(cinfo->VendorTemplateId) : NULL;

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

    for (auto i = 0; i < numitems; ++i)
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
                data << uint32(~0);
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
bool CPlayer::BuyItemFromMultiVendor(uint32 item, uint8 count, uint8 bag, uint8 slot)
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
        BoxChat << MSG_COLOR_WHITE << "This vendor seems to be removed from the database. Please report that creature " << entry << " is missing." << std::endl;
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
    VendorItemData const* tItems = cinfo->VendorTemplateId ? sObjectMgr.GetNpcVendorTemplateItemList(cinfo->VendorTemplateId) : NULL;

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
        for (auto i = 0; i < MAX_EXTENDED_COST_ITEMS; ++i)
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

    if (crItem->conditionId && !isGameMaster() && !sObjectMgr.IsPlayerMeetToCondition(crItem->conditionId, ToPlayer(), pCreature->GetMap(), pCreature, CONDITION_FROM_VENDOR))
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

bool CPlayer::SellItemToMultiVendor(ObjectGuid itemGuid, uint8 _count)
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
                    Item* pNewItem = pItem->CloneItem(count, ToPlayer());
                    if (!pNewItem)
                    {
                        sLog.outError("WORLD: SellItemToMultiVendor - could not create clone of item %u; count = %u", pItem->GetEntry(), count);
                        SendSellError(SELL_ERR_CANT_SELL_ITEM, pCreature, itemGuid, 0);
                        return false;
                    }

                    pItem->SetCount(pItem->GetCount() - count);
                    ItemRemovedQuestCheck(pItem->GetEntry(), count);
                    if (IsInWorld())
                        pItem->SendCreateUpdateToPlayer(ToPlayer());
                    pItem->SetState(ITEM_CHANGED, ToPlayer());

                    AddItemToBuyBackSlot(pNewItem);
                    if (IsInWorld())
                        pNewItem->SendCreateUpdateToPlayer(ToPlayer());
                }
                else
                {
                    ItemRemovedQuestCheck(pItem->GetEntry(), pItem->GetCount());
                    RemoveItem(pItem->GetBagSlot(), pItem->GetSlot(), true);
                    pItem->RemoveFromUpdateQueueOf(ToPlayer());
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

bool CPlayer::BuyBackItemFromMultiVendor(uint32 slot)
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

void CPlayer::LearnTalentTemplate(uint8 spec)
{
    resetTalents(true);

    for (auto& itr : sCustom.GetTalentContainer())
    if (itr->ClassId == getClass() && itr->SpecId == spec)
        LearnTalent(itr->TalentId, itr->TalentRank - 1);
}

void CPlayer::ApplyEnchantTemplate(uint8 spec)
{
    for (auto& itr : sCustom.GetEnchantContainer())
    if (itr->ClassId == getClass() && itr->SpecId == spec)
        EnchantItem(itr->SpellId, itr->SlotId);
}

uint32 CPlayer::GetAVGILevel(bool levelasmin)
{
    uint32 TotLevel = 0;
    uint8 ItemCount = 0;

    for (uint8 i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; ++i)
    {
        // Ignore these, because either they do not matter to the calculation
        // or they're allowed to be swapped during the game.
        if (i == EQUIPMENT_SLOT_MAINHAND || i == EQUIPMENT_SLOT_OFFHAND ||
            i == EQUIPMENT_SLOT_RANGED || i == EQUIPMENT_SLOT_TABARD ||
            i == EQUIPMENT_SLOT_BODY)
            continue;

        ++ItemCount;

        if (Item* pItem = GetItemByPos(INVENTORY_SLOT_BAG_0, i))
            TotLevel += pItem->GetProto()->ItemLevel;
    }

    uint32 avg = TotLevel / ItemCount;

    if (!levelasmin)
        return avg;

    return (avg > getLevel() ? avg : getLevel());
}
