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


#ifndef _TRANSMOG_H
#define _TRANSMOG_H

class Transmog
{
public:
    Transmog() {}
    ~Transmog() {};

    typedef std::map<ObjectGuid, uint32> TransmogMap;

    void SetTransmog(ObjectGuid guid, uint32 entry);
    void DelTransmog(ObjectGuid guid);
    uint32 GetTransmog(ObjectGuid guid, uint32 entry);

    void LoadTransmogs();
    void SaveTransmogs();

private:
    TransmogMap m_TransmogMap;
    TransmogMap m_NewTransmogs;
    TransmogMap m_DelTransmogs;
};

#define sTransmog MaNGOS::Singleton<Transmog>::Instance()

#endif
