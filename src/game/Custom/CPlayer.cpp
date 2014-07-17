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
#include "Settings.h"
#include "CPlayer.h"
#include "CPlusMgr.h"
#include "NewPlayer.h"

CCPlayer::CCPlayer(Player* pPlayer)
{
    m_player = pPlayer;

    m_wChatOn = false;

    m_ScriptID = 0;
    m_SelectedGObject = 0;
}

CCPlayer::~CCPlayer()
{
    delete m_player;
}

void CCPlayer::CUpdate(uint32 diff)
{
    LearnGreenSpells();

    SendSavedChat(CHAT_BOX, BoxChat);
    SendSavedChat(CHAT_WIDE, WideChat);
    SendSavedChat(CHAT_BOTH, BothChat);

    m_player->ToCPlayer()->IncClientBasedServerTime(diff);
}

void CCPlayer::Sometimes()
{
    if (m_player->ToCPlayer()->GetRecache())
    {
        m_player->ToCPlayer()->RecachePlayersFromList();
        m_player->ToCPlayer()->RecachePlayersFromBG();
    }

    if (m_player->ToCPlayer()->GetFakeOnNextTick())
    {
        m_player->ToCPlayer()->SetFakeOnNextTick(false);

        m_player->SetByteValue(UNIT_FIELD_BYTES_0, 0, m_player->ToCPlayer()->getFRace());
        m_player->setFaction(m_player->ToCPlayer()->getFFaction());
        m_player->ToCPlayer()->FakeDisplayID();

        m_player->SetUInt32Value(PLAYER_BYTES, m_player->ToCPlayer()->getFPlayerBytes());
        m_player->SetUInt32Value(PLAYER_BYTES_2, m_player->ToCPlayer()->getFPlayerBytes2());
    }
}

void CCPlayer::LoadCountryData()
{
    std::ostringstream ss;

    ss << " SELECT"
       << " c.iso_code_2, c.iso_code_3, c.country"
       << " FROM"
       << " ip2nationcountries c,"
       << " ip2nation i"
       << " WHERE"
       << " i.ip < INET_ATON('" << m_player->GetSession()->GetRemoteAddress() << "')"
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

    if (m_player->GetSession()->GetRemoteAddress() == "127.0.0.1")
    {
        m_Country.ISO2 = "Local";
        m_Country.ISO3 = "Local";
        m_Country.FULL = "Local";
    }
}

void CCPlayer::OnLogin()
{
    m_player->GetSettings()->LoadSettings();
    //m_player->ToCPlayer()->SetFakeValues();

    LoadCountryData();

    SetWChat(m_player->GetSettings()->GetSetting(SETTING_UINT_WCHAT));

    if (!m_player->ToCPlayer()->NativeTeam())
        m_player->ToCPlayer()->SetFakeOnNextTick();

    if (!m_player->GetSettings()->GetSetting(SETTING_UINT_HIDETEMPLATEMENU))
        sCPlusMgr.OnGossipHello(m_player, 1);

    if (m_player->GetTotalPlayedTime() < 1)
    {
        m_player->m_Played_time[PLAYED_TIME_TOTAL] += 1;
        m_player->m_Played_time[PLAYED_TIME_LEVEL] += 1;

        OnFirstLogin();
    }
}

void CCPlayer::OnFirstLogin()
{
    FillGreenSpellList();
}

void CCPlayer::AddItemSet(uint32 setid)
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
                if (m_player->StoreNewItemInBestSlots(itemid, 1))
                    Items.push_back(itemid);
                else
                {
                    for (auto& itr : Items)
                        m_player->DestroyItemCount(itr, 1, true);

                    BoxChat << "Not enough space to store itemset" << std::endl;
                    break;
                }
            }
        }
    }
}

std::string CCPlayer::GetNameLink(bool applycolors)
{
    std::string name = m_player->GetName();
    std::string teamcolor = m_player->GetOTeam() == ALLIANCE ? MSG_COLOR_DARKBLUE : MSG_COLOR_RED;
    std::ostringstream ss;

    if (m_player->isGameMaster())
        teamcolor = MSG_COLOR_PURPLE;

    ss << "|Hplayer:" << name << "|h";

    if (applycolors)
        ss << teamcolor << "[" << sCustom.GetClassColor(m_player->getClass()) << name << teamcolor << "]|h";
    else
        ss << "[" << name << "]|h";

    return ss.str();
}

void CCPlayer::SendWorldChatMsg(std::string msg)
{
    std::ostringstream ss;
    ss << m_player->GetCCPlayer()->GetNameLink(true) << MSG_COLOR_WHITE << ": " << msg; // [Playername]: Message

    sCustom.SendWorldChat(m_player->GetObjectGuid(), sCustom.stringReplace(ss.str(), "|r", MSG_COLOR_WHITE));
}

void CCPlayer::SendSavedChat(MessageTypes type, std::stringstream &ss)
{
    if (!ss.str().empty())
    {
        std::string item;

        while (std::getline(ss, item))
        {
            const char* msg = item.c_str();

            if (type == CHAT_BOX || type == CHAT_BOTH)
                ChatHandler(m_player->GetSession()).SendSysMessage(msg);
            if (type == CHAT_WIDE || type == CHAT_BOTH)
            {
                WorldPacket data(SMSG_NOTIFICATION, (strlen(msg) + 1));
                data << msg;
                m_player->GetSession()->SendPacket(&data);
            }
        }

        ss.str(""); // Clear content
        ss.clear(); // Clear state flags.
    }
}

void CCPlayer::FillGreenSpellList()
{
    uint32 trainerid = 0;

    switch (m_player->getClass())
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

    Custom::SpellContainer* allSpellContainer = sCustom.GetCachedSpellContainer(m_player->getClass());

    if (!allSpellContainer)
    {
        allSpellContainer = new Custom::SpellContainer;

        Custom::SpellContainer classSpellContainer = sCustom.GetSpellContainerByCreatureEntry(trainerid);

        for (auto& itr : classSpellContainer)
            allSpellContainer->push_back(itr);

        sCustom.CacheSpellContainer(m_player->getClass(), allSpellContainer);
    }

    if (allSpellContainer->empty())
        return;

    m_DelayedSpellLearn.clear();


    for (auto itr = allSpellContainer->cbegin(); itr != allSpellContainer->cend(); ++itr)
    {
        TrainerSpell const* tSpell = &*itr;

        TrainerSpellState state = m_player->GetTrainerSpellState(tSpell, tSpell->reqLevel);

        if (state == TRAINER_SPELL_GREEN)
        {
            if (m_player->IsInWorld())
            {
                bool CastLearned = false;

                if (SpellEntry const* spellInfo = sSpellStore.LookupEntry(tSpell->spell))
                {
                    for (auto i = 0; i < MAX_EFFECT_INDEX; ++i)
                    {
                        if (spellInfo->Effect[i] == SPELL_EFFECT_LEARN_SPELL)
                        {
                            CastLearned = true;

                            if (!m_player->HasSpell(spellInfo->EffectTriggerSpell[i]))
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

void CCPlayer::LearnGreenSpells()
{
    if (m_DelayedSpellLearn.empty())
        return;

    uint32 spellid = m_DelayedSpellLearn.front();

    m_player->learnSpell(spellid, false);

    m_DelayedSpellLearn.erase(m_DelayedSpellLearn.begin());

    if (m_DelayedSpellLearn.empty())
        FillGreenSpellList();
}

void CCPlayer::CreatePet(uint32 entry, bool classcheck)
{
    if (classcheck && m_player->getClass() != CLASS_HUNTER)
        return;

    CreatureInfo const *cinfo = sObjectMgr.GetCreatureTemplate(entry);
    if (!cinfo)
    {
        BoxChat << MSG_COLOR_WHITE << " This pet doesn't exist in our database. Please report that creature " << entry << " is missing." << std::endl;
        return;
    }

    CreatureCreatePos pos(m_player->GetSession()->GetPlayer(), m_player->GetOrientation());

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

    if (m_player->GetPetGuid())
        m_player->UnsummonPetTemporaryIfAny();

    Pet* pet = new Pet(HUNTER_PET);

    if(!pet->CreateBaseAtCreature(pCreature))
    {
        delete pet;
        return;
    }

    pet->SetOwnerGuid(m_player->GetObjectGuid());
    pet->SetCreatorGuid(m_player->GetObjectGuid());
    pet->setFaction(m_player->getFaction());
    pet->SetUInt32Value(UNIT_CREATED_BY_SPELL, 13481);

    if (m_player->IsPvP())
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

    for (auto x = 0; x < 6; x++)
    {
        pet->SetPower(POWER_HAPPINESS,66600000);
        pet->ModifyLoyalty(150000);
        pet->TickLoyaltyChange();
        pet->SetTP(350);
    }

    // caster have pet now
    m_player->SetPet(pet);

    pet->SavePetToDB(PET_SAVE_AS_CURRENT);
    m_player->PetSpellInitialize();
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

void CCPlayer::EnchantItem(uint32 spellid, uint8 slot, std::string sendername)
{
    Item* pItem = m_player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
    if (!pItem)
    {
        if (sendername != "")
            BoxChat << sCustom.ChatNameWrapper(sendername) << " Your item could not be enchanted, there are no item equipped in the specified slot." << std::endl;

        return;
    }
    SpellEntry const* spellInfo = sSpellStore.LookupEntry(spellid);
    if (!spellInfo)
    {
        BoxChat << "Invalid spellid " << spellid << " report to devs" << std::endl;
        return;
    }
    uint32 enchantid = spellInfo->EffectMiscValue[0];
    if (!enchantid)
    {
        BoxChat << "Invalid enchantid " << enchantid << " report to devs" << std::endl;
        return;
    }

    if (!((1 << pItem->GetProto()->SubClass) & spellInfo->EquippedItemSubClassMask) &&
        !((1 << pItem->GetProto()->InventoryType) & spellInfo->EquippedItemInventoryTypeMask))
    {
        if (sendername != "")
            BoxChat << sCustom.ChatNameWrapper(sendername) << " Your item could not be enchanted, wrong item type equipped" << std::endl;

        return;
    }

    m_player->ApplyEnchantment(pItem, PERM_ENCHANTMENT_SLOT, false);
    pItem->SetEnchantment(PERM_ENCHANTMENT_SLOT, enchantid, 0, 0);
    m_player->ApplyEnchantment(pItem, PERM_ENCHANTMENT_SLOT, true);

    if (sendername != "")
        BoxChat << sCustom.ChatNameWrapper(sendername) << " Your item was enchanted successfully!" << std::endl;
}

void CCPlayer::SendMultiVendorInventory(uint32 cEntry, ObjectGuid guid)
{
    DEBUG_LOG("WORLD: Sent SMSG_LIST_INVENTORY");

    CreatureInfo const *cinfo = sObjectMgr.GetCreatureTemplate(cEntry);
    if (!cinfo)
    {
        BoxChat << MSG_COLOR_WHITE << "This vendor seems to be removed from the database. Please report that creature " << cEntry << " is missing." << std::endl;
        return;
    }

    Creature* pCreature = m_player->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_NONE);
    if (!pCreature)
    {
        DEBUG_LOG("WORLD: SendMultiVendorInventory - %s not found or you can't interact with him.", guid.GetString().c_str());
        m_player->SendSellError(SELL_ERR_CANT_FIND_VENDOR, NULL, guid, 0);
        return;
    }

    VendorItemData const* vItems = sObjectMgr.GetNpcVendorItemList(cEntry);
    VendorItemData const* tItems = cinfo->vendorId ? sObjectMgr.GetNpcVendorTemplateItemList(cinfo->vendorId) : NULL;


    if (!vItems && !tItems)
    {
        WorldPacket data(SMSG_LIST_INVENTORY, (8 + 1 + 1));
        data << m_player->GetObjectGuid();
        data << uint8(0);                                   // count==0, next will be error code
        data << uint8(0);                                   // "Vendor has no inventory"
        m_player->GetSession()->SendPacket(&data);
        return;
    }

    uint8 customitems = vItems ? vItems->GetItemCount() : 0;
    uint8 numitems = customitems + (tItems ? tItems->GetItemCount() : 0);

    uint8 count = 0;

    WorldPacket data(SMSG_LIST_INVENTORY, (8 + 1 + numitems * 8 * 4));
    data << m_player->GetObjectGuid();
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
                if (!m_player->isGameMaster())
                {
                    // class wrong item skip only for bindable case
                    if ((pProto->AllowableClass & m_player->getClassMask()) == 0 && pProto->Bonding == BIND_WHEN_PICKED_UP)
                        continue;

                    // race wrong item skip always
                    if ((pProto->AllowableRace & m_player->getRaceMask()) == 0)
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
        m_player->GetSession()->SendPacket(&data);
        return;
    }

    data.put<uint8>(count_pos, count);
    m_player->GetSession()->SendPacket(&data);
}

// Return true is the bought item has a max count to force refresh of window by caller
bool CCPlayer::BuyItemFromMultiVendor(uint32 item, uint8 count, uint8 bag, uint8 slot)
{
    DEBUG_LOG("WORLD: BuyItemFromMultiVendor");

    // cheating attempt
    if (count < 1) count = 1;

    if (!m_player->isAlive())
        return false;

    ItemPrototype const* pProto = ObjectMgr::GetItemPrototype(item);
    if (!pProto)
    {
        m_player->SendBuyError(BUY_ERR_CANT_FIND_ITEM, NULL, item, 0);
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

    Creature* pCreature = m_player->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_NONE);
    if (!pCreature)
    {
        DEBUG_LOG("WORLD: BuyItemFromMultiVendor - %s not found or you can't interact with him.", guid.GetString().c_str());
        m_player->SendBuyError(BUY_ERR_DISTANCE_TOO_FAR, NULL, item, 0);
        return false;
    }

    VendorItemData const* vItems = sObjectMgr.GetNpcVendorItemList(entry);
    VendorItemData const* tItems = cinfo->vendorId ? sObjectMgr.GetNpcVendorTemplateItemList(cinfo->vendorId) : NULL;

    if ((!vItems || vItems->Empty()) && (!tItems || tItems->Empty()))
    {
        m_player->SendBuyError(BUY_ERR_CANT_FIND_ITEM, pCreature, item, 0);
        return false;
    }

    uint32 vCount = vItems ? vItems->GetItemCount() : 0;
    uint32 tCount = tItems ? tItems->GetItemCount() : 0;

    size_t vendorslot = vItems ? vItems->FindItemSlot(item) : vCount;
    if (vendorslot >= vCount)
        vendorslot = vCount + (tItems ? tItems->FindItemSlot(item) : tCount);

    if (vendorslot >= vCount + tCount)
    {
        m_player->SendBuyError(BUY_ERR_CANT_FIND_ITEM, pCreature, item, 0);
        return false;
    }

    VendorItem const* crItem = vendorslot < vCount ? vItems->GetItem(vendorslot) : tItems->GetItem(vendorslot - vCount);
    if (!crItem || crItem->item != item) // store diff item (cheating)
    {
        m_player->SendBuyError(BUY_ERR_CANT_FIND_ITEM, pCreature, item, 0);
        return false;
    }

    uint32 totalCount = pProto->BuyCount * count;

    // check current item amount if it limited
    if (crItem->maxcount != 0)
    {
        if (pCreature->GetVendorItemCurrentCount(crItem) < totalCount)
        {
            m_player->SendBuyError(BUY_ERR_ITEM_ALREADY_SOLD, pCreature, item, 0);
            return false;
        }
    }

    if (uint32(m_player->GetReputationRank(pProto->RequiredReputationFaction)) < pProto->RequiredReputationRank)
    {
        m_player->SendBuyError(BUY_ERR_REPUTATION_REQUIRE, pCreature, item, 0);
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
        if (m_player->GetHonorPoints() < (iece->reqhonorpoints * count))
        {
            m_player->SendEquipError(EQUIP_ERR_NOT_ENOUGH_HONOR_POINTS, NULL, NULL);
            return false;
        }

        // arena points price
        if (m_player->GetArenaPoints() < (iece->reqarenapoints * count))
        {
            m_player->SendEquipError(EQUIP_ERR_NOT_ENOUGH_ARENA_POINTS, NULL, NULL);
            return false;
        }

        // item base price
        for (auto i = 0; i < MAX_EXTENDED_COST_ITEMS; ++i)
        {
            if (iece->reqitem[i] && !m_player->HasItemCount(iece->reqitem[i], iece->reqitemcount[i] * count))
            {
                m_player->SendEquipError(EQUIP_ERR_VENDOR_MISSING_TURNINS, NULL, NULL);
                return false;
            }
        }

        // check for personal arena rating requirement
        if (m_player->GetMaxPersonalArenaRatingRequirement() < iece->reqpersonalarenarating)
        {
            // probably not the proper equip err
            m_player->SendEquipError(EQUIP_ERR_CANT_EQUIP_RANK, NULL, NULL);
            return false;
        }
    }

    if (crItem->conditionId && !m_player->isGameMaster() && !sObjectMgr.IsPlayerMeetToCondition(crItem->conditionId, m_player, pCreature->GetMap(), pCreature, CONDITION_FROM_VENDOR))
    {
        m_player->SendBuyError(BUY_ERR_CANT_FIND_ITEM, pCreature, item, 0);
        return false;
    }

    uint32 price = pProto->BuyPrice * count;

    // reputation discount
    price = uint32(floor(price * m_player->GetReputationPriceDiscount(pCreature)));

    if (m_player->GetMoney() < price)
    {
        m_player->SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, pCreature, item, 0);
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

    if ((bag == NULL_BAG && slot == NULL_SLOT) || m_player->IsInventoryPos(bag, slot))
    {
        ItemPosCountVec dest;
        InventoryResult msg = m_player->CanStoreNewItem(bag, slot, dest, item, totalCount);
        if (msg != EQUIP_ERR_OK)
        {
            m_player->SendEquipError(msg, NULL, NULL, item);
            return false;
        }

        m_player->ModifyMoney(-int32(price));

        if (crItem->ExtendedCost)
            m_player->TakeExtendedCost(crItem->ExtendedCost, count);

        pItem = m_player->StoreNewItem(dest, item, true);
    }
    else if (m_player->IsEquipmentPos(bag, slot))
    {
        if (totalCount != 1)
        {
            m_player->SendEquipError(EQUIP_ERR_ITEM_CANT_BE_EQUIPPED, NULL, NULL);
            return false;
        }

        uint16 dest;
        InventoryResult msg = m_player->CanEquipNewItem(slot, dest, item, false);
        if (msg != EQUIP_ERR_OK)
        {
            m_player->SendEquipError(msg, NULL, NULL, item);
            return false;
        }

        m_player->ModifyMoney(-int32(price));

        if (crItem->ExtendedCost)
            m_player->TakeExtendedCost(crItem->ExtendedCost, count);

        pItem = m_player->EquipNewItem(dest, item, true);

        if (pItem)
            m_player->AutoUnequipOffhandIfNeed();
    }
    else
    {
        m_player->SendEquipError(EQUIP_ERR_ITEM_DOESNT_GO_TO_SLOT, NULL, NULL);
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
    m_player->GetSession()->SendPacket(&data);

    m_player->SendNewItem(pItem, totalCount, true, false, false);

    return crItem->maxcount != 0;
}

bool CCPlayer::SellItemToMultiVendor(ObjectGuid itemGuid, uint8 _count)
{
    DEBUG_LOG("WORLD: SellItemToMultiVendor");

    // prevent possible overflow, as mangos uses uint32 for item count
    uint32 count = _count;

    uint32 entry;
    ObjectGuid guid;
    GetMultiVendor(entry, guid);

    if (!itemGuid)
        return false;

    Creature* pCreature = m_player->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_NONE);
    if (!pCreature)
    {
        DEBUG_LOG("WORLD: SellItemToMultiVendor - %s not found or you can't interact with him.", guid.GetString().c_str());
        m_player->SendSellError(SELL_ERR_CANT_FIND_VENDOR, NULL, itemGuid, 0);
        return false;
    }

    // remove fake death
    if (m_player->hasUnitState(UNIT_STAT_DIED))
        m_player->RemoveSpellsCausingAura(SPELL_AURA_FEIGN_DEATH);

    Item* pItem = m_player->GetItemByGuid(itemGuid);
    if (pItem)
    {
        // prevent sell not owner item
        if (m_player->GetObjectGuid() != pItem->GetOwnerGuid())
        {
            m_player->SendSellError(SELL_ERR_CANT_SELL_ITEM, pCreature, itemGuid, 0);
            return false;
        }

        // prevent sell non empty bag by drag-and-drop at vendor's item list
        if (pItem->IsBag() && !((Bag*)pItem)->IsEmpty())
        {
            m_player->SendSellError(SELL_ERR_CANT_SELL_ITEM, pCreature, itemGuid, 0);
            return false;
        }

        // prevent sell currently looted item
        if (m_player->GetLootGuid() == pItem->GetObjectGuid())
        {
            m_player->SendSellError(SELL_ERR_CANT_SELL_ITEM, pCreature, itemGuid, 0);
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
                m_player->SendSellError(SELL_ERR_CANT_SELL_ITEM, pCreature, itemGuid, 0);
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
                    Item* pNewItem = pItem->CloneItem(count, m_player);
                    if (!pNewItem)
                    {
                        sLog.outError("WORLD: SellItemToMultiVendor - could not create clone of item %u; count = %u", pItem->GetEntry(), count);
                        m_player->SendSellError(SELL_ERR_CANT_SELL_ITEM, pCreature, itemGuid, 0);
                        return false;
                    }

                    pItem->SetCount(pItem->GetCount() - count);
                    m_player->ItemRemovedQuestCheck(pItem->GetEntry(), count);
                    if (m_player->IsInWorld())
                        pItem->SendCreateUpdateToPlayer(m_player);
                    pItem->SetState(ITEM_CHANGED, m_player);

                    m_player->AddItemToBuyBackSlot(pNewItem);
                    if (m_player->IsInWorld())
                        pNewItem->SendCreateUpdateToPlayer(m_player);
                }
                else
                {
                    m_player->ItemRemovedQuestCheck(pItem->GetEntry(), pItem->GetCount());
                    m_player->RemoveItem(pItem->GetBagSlot(), pItem->GetSlot(), true);
                    pItem->RemoveFromUpdateQueueOf(m_player);
                    m_player->AddItemToBuyBackSlot(pItem);
                }

                uint32 money = pProto->SellPrice * count;

                m_player->ModifyMoney(money);
            }
            else
                m_player->SendSellError(SELL_ERR_CANT_SELL_ITEM, pCreature, itemGuid, 0);

            return true;
        }
    }

    m_player->SendSellError(SELL_ERR_CANT_FIND_ITEM, pCreature, itemGuid, 0);
    return false;
}

bool CCPlayer::BuyBackItemFromMultiVendor(uint32 slot)
{
    DEBUG_LOG("WORLD: BuyBackItemFromMultiVendor");

    uint32 entry;
    ObjectGuid vendorGuid;
    GetMultiVendor(entry, vendorGuid);


    Creature* pCreature = m_player->GetNPCIfCanInteractWith(vendorGuid, UNIT_NPC_FLAG_NONE);
    if (!pCreature)
    {
        DEBUG_LOG("WORLD: BuyBackItemFromMultiVendor - %s not found or you can't interact with him.", vendorGuid.GetString().c_str());
        m_player->SendSellError(SELL_ERR_CANT_FIND_VENDOR, NULL, ObjectGuid(), 0);
        return false;
    }

    // remove fake death
    if (m_player->hasUnitState(UNIT_STAT_DIED))
        m_player->RemoveSpellsCausingAura(SPELL_AURA_FEIGN_DEATH);

    Item* pItem = m_player->GetItemFromBuyBackSlot(slot);
    if (pItem)
    {
        uint32 price = m_player->GetUInt32Value(PLAYER_FIELD_BUYBACK_PRICE_1 + slot - BUYBACK_SLOT_START);
        if (m_player->GetMoney() < price)
        {
            m_player->SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, pCreature, pItem->GetEntry(), 0);
            return false;
        }

        ItemPosCountVec dest;
        InventoryResult msg = m_player->CanStoreItem(NULL_BAG, NULL_SLOT, dest, pItem, false);
        if (msg == EQUIP_ERR_OK)
        {
            m_player->ModifyMoney(-(int32)price);
            m_player->RemoveItemFromBuyBackSlot(slot, false);
            m_player->ItemAddedQuestCheck(pItem->GetEntry(), pItem->GetCount());
            m_player->StoreItem(dest, pItem, true);
        }
        else
            m_player->SendEquipError(msg, pItem, NULL);

        return true;
    }
    else
        m_player->SendBuyError(BUY_ERR_CANT_FIND_ITEM, pCreature, 0, 0);

    return false;
}

void CCPlayer::LearnTalentTemplate(uint8 spec)
{
    m_player->resetTalents(true);

    for (auto& itr : sCustom.GetTalentContainer())
    if (itr->ClassId == m_player->getClass() && itr->SpecId == spec)
        m_player->LearnTalent(itr->TalentId, itr->TalentRank - 1);
}


void CCPlayer::ApplyEnchantTemplate(uint8 spec)
{
    for (auto& itr : sCustom.GetEnchantContainer())
    if (itr->ClassId == m_player->getClass() && itr->SpecId == spec)
        EnchantItem(itr->SpellId, itr->SlotId, "");
}

uint32 CCPlayer::GetAVGILevel(bool levelasmin)
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

        if (Item* pItem = m_player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
            TotLevel += pItem->GetProto()->ItemLevel;
    }

    uint32 avg = TotLevel / ItemCount;

    if (!levelasmin)
        return avg;

    return (avg > m_player->getLevel() ? avg : m_player->getLevel());
}
