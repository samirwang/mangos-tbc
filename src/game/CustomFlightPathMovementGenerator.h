/*
 * This file is part of the CMaNGOS Project. See AUTHORS file for Copyright information
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

#ifndef MANGOS_CUSTOMFLIGHTPATHMOVEMENTGENERATOR_H
#define MANGOS_CUSTOMFLIGHTPATHMOVEMENTGENERATOR_H

#include "MovementGenerator.h"
#include "WaypointManager.h"
#include "DBCStructure.h"
#include "G3D/Vector3.h"

class FlightNode
{
public:
    FlightNode(uint32 mapid, float x, float y, float z)
    {
        m_MapId = mapid;
        m_Pos = G3D::Vector3(x, y, z);
    }

    uint32 GetNodeId() { return m_NodeId; }

    bool CanReachNode(uint32 nodeid)
    {
        for (FlightNode* i : m_ConnectedNodes)
            if (i->GetNodeId() == nodeid || i->CanReachNode(nodeid))
                return true;

        return false;
    }

    void GetDistanceToNode(uint32 nodeid, float& distance)
    {
        std::map<float, FlightNode*> NodeList;

        for (FlightNode* i : m_ConnectedNodes)
            if (CanReachNode(nodeid))

    }

private:
    uint32 m_NodeId;
    uint32 m_MapId;
    G3D::Vector3 m_Pos;
    std::string m_Name;
    std::deque<FlightNode*> m_ConnectedNodes;
};

class MANGOS_DLL_SPEC CustomFlightPathMovementGenerator
    : public MovementGeneratorMedium< Player, CustomFlightPathMovementGenerator >
{
    public:
        explicit CustomFlightPathMovementGenerator()
        {
            m_Nodes.push_back(FlightNode(530, -248.124f, 934.789f, 84.3782f));
            m_Nodes.push_back(FlightNode(530, -243.227f, 945.386f, 84.3782f));
            m_Nodes.push_back(FlightNode(530, -253.062f, 956.439f, 84.3736f));
            m_Nodes.push_back(FlightNode(530, -267.511f, 964.357f, 84.3672f));
            m_Nodes.push_back(FlightNode(530, -280.361f, 958.683f, 84.3786f));
            m_Nodes.push_back(FlightNode(530, -274.788f, 947.613f, 84.3786f));
            m_Nodes.push_back(FlightNode(530, -263.252f, 957.896f, 84.3786f));
            m_Nodes.push_back(FlightNode(530, -248.796f, 955.347f, 84.3693f));
            m_Nodes.push_back(FlightNode(530, -248.606f, 934.299f, 84.3798f));
            m_Nodes.push_back(FlightNode(530, -248.124f, 934.789f, 84.3783f));
        }

        void Initialize(Player&);
        void Finalize(Player&);
        void Interrupt(Player&);
        void Reset(Player&);
        bool Update(Player&, const uint32&);
        MovementGeneratorType GetMovementGeneratorType() const override { return FLIGHT_MOTION_TYPE; }
private:
    std::vector<Node> m_Nodes;
};

#endif
