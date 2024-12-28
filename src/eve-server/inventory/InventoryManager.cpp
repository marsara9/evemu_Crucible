/*
    ------------------------------------------------------------------------------------
    LICENSE:
    ------------------------------------------------------------------------------------
    This file is part of EVEmu: EVE Online Server Emulator
    Copyright 2006 - 2021 The EVEmu Team
    For the latest information visit https://evemu.dev
    ------------------------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any later
    version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License along with
    this program; if not, write to the Free Software Foundation, Inc., 59 Temple
    Place - Suite 330, Boston, MA 02111-1307, USA, or go to
    http://www.gnu.org/copyleft/lesser.txt.
    ------------------------------------------------------------------------------------
    Author:        MarSara9
*/

#include "eve-server.h"

#include "InventoryManager.h"

std::shared_ptr<InventoryBound> InventoryManager::Find(uint32 containerID) {
    _log(INV__INFO, "InventoryManager::Find(%u)", containerID);
    if(m_boundMap.count(containerID) == 0) {
        _log(INV__WARNING, "No InventoryBound for %u.", containerID);
        return nullptr;
    }
    auto ib = m_boundMap.at(containerID);
    _log(INV__BIND, "Found InventoryBound (%u) for %u.", ib->GetBoundID(), containerID);
    return ib;
}

void InventoryManager::Add(
    uint32 containerID,
    std::shared_ptr<InventoryBound> ib
) {
    _log(INV__BIND, "Adding InventoryBound for %u.", containerID);
    if(m_boundMap.count(containerID) != 0 && m_boundMap.at(containerID) != ib) {
        _log(INV__WARNING, "InventoryBound (%u) already exists for %u. Skipping add.", ib->GetBoundID(), containerID);
        return;
    }
    m_boundMap.insert(BoundEntry(containerID, ib));
}

void InventoryManager::Remove(uint32 containerID) {
    _log(INV__BIND, "Disposing of InventoryBound for %s.", containerID);
    m_boundMap.erase(containerID);
}
