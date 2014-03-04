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

#include "Transmog.h"
#include "Log.h"

void Transmog::SetTransmog(ObjectGuid guid, uint32 entry)
{
    m_TransmogMap[guid] = entry;
    m_NewTransmogs[guid] = entry;
}

void Transmog::DelTransmog(ObjectGuid guid)
{
    m_TransmogMap.erase(guid);
    m_DelTransmogs[guid] = 1;
}

uint32 Transmog::GetTransmog(ObjectGuid guid, uint32 entry)
{
    auto itr = m_TransmogMap.find(guid);

    if (itr != m_TransmogMap.end())
    if (itr->second)
        return itr->second;
    
    return entry;
}

void Transmog::LoadTransmogs()
{
    if (QueryResult* result = CharacterDatabase.PQuery("SELECT guid, transmog FROM item_transmog"))
    {
        do 
        {
            Field* fields = result->Fetch();

            uint32 uguid    = fields[0].GetUInt32();
            uint32 transmog = fields[1].GetUInt32();

            ObjectGuid guid(HIGHGUID_ITEM, uguid);

            m_TransmogMap.insert(std::make_pair(guid, transmog));
        }
        while (result->NextRow());

        delete result;
    }
}

void Transmog::SaveTransmogs()
{
    WorldDatabase.BeginTransaction();

    for (auto& itr : m_DelTransmogs)
        WorldDatabase.PExecute("DELETE FROM item_transmog WHERE guid = %u", itr.first.GetCounter());

    m_DelTransmogs.clear();

    for (auto& itr : m_NewTransmogs)
        WorldDatabase.PExecute("INSERT INTO item_transmog (guid, transmog) VALUES (%u, %u)", itr.first.GetCounter(), itr.second);

    m_NewTransmogs.clear();

    WorldDatabase.CommitTransaction();
}
