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

std::shared_ptr<InventoryBound> InventoryManager::Find(
    uint32 containerID,
    uint32 ownerID
) {
    ReadLock r_lock(m_lock);

    Key key(containerID, ownerID);

    _log(INV__INFO, "InventoryManager::Find(%u, %u)", key.first, key.second);
    if(m_boundMap.count(key) == 0) {
        _log(INV__WARNING, "No InventoryBound for %u-%u.", key.first, key.second);
        return nullptr;
    }
    auto ib = m_boundMap.at(key);
    _log(INV__BIND, "Found InventoryBound (%u) for %u-%u.", ib->GetBoundID(), key.first, key.second);
    return ib;
}

std::shared_ptr<InventoryBound> InventoryManager::FindOrCreate(
    uint32 containerID,
    uint32 ownerID,
    std::function<std::shared_ptr<InventoryBound>()> create
) {
    WriteLock w_lock(m_lock);

    Key key(containerID, ownerID);

    _log(INV__INFO, "InventoryManager::FindOrCreate(%u, %u)", key.first, key.second);
    auto f = m_boundMap.find(key);
    std::shared_ptr<InventoryBound> ib;
    if(f == m_boundMap.end()) {
        _log(INV__WARNING, "No InventoryBound for %u-%u. Creating.", key.first, key.second);
        ib = create();
        m_boundMap.insert(key, ib);
    } else {
        ib = f->first;
        _log(INV__BIND, "Found InventoryBound (%u) for %u-%u.", ib->GetBoundID(), key.first, key.second);
    }
    return ib;
}

void InventoryManager::Add(
    uint32 containerID,
    uint32 ownerID,
    std::shared_ptr<InventoryBound> ib
) {
    WriteLock w_lock(m_lock);

    Key key(containerID, ownerID);

    _log(INV__BIND, "Adding InventoryBound for %u.", key.first, key.second);
    if(m_boundMap.count(key) != 0 && m_boundMap.at(key) != ib) {
        auto existing = m_boundMap.at(key);
        _log(INV__WARNING, "InventoryBound (%u) already exists for %u-%u. Skipping adding (%u).", existing->GetBoundID(), key.first, key.second, ib->GetBoundID());
        return;
    }
    m_boundMap.insert(BoundEntry(key, ib));
}

void InventoryManager::Remove(
    uint32 containerID,
    uint32 ownerID
) {
    WriteLock w_lock(m_lock);

    Key key(containerID, ownerID);

    _log(INV__BIND, "Disposing of InventoryBound for %u-%u.", key.first, key.second);
    m_boundMap.erase(key);
}
