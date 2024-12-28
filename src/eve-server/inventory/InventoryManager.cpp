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

#include "InventoryManager.h"

/**
 * Searches for an existing InventoryBound for the provided containerID.
 * If one is found a reference to its InventoryBound will be returned,
 * otherwise `nullptr` is returned instead.
 */
InventoryBound* InventoryManager::Find(
    uint32 containerID
) {
    auto bound = m_inventoryBoundMap.find(containerID);
    if(bound == m_inventoryBoundMap.end()) {
        return nullptr;
    } else {
        return bound->second;
    }
}

void InventoryManager::Add(
    uint32 containerID,
    InventoryBound* bound
) {
    _log(INV__ERROR, "Creating InventoryBound for %u.", containerID);
    m_inventoryBoundMap[containerID] = bound;
}

void InventoryManager::Remove(uint32 containerID) {
    _log(INV__ERROR, "Disposing of InventoryBound for %s.", containerID);
    m_inventoryBoundMap.erase(containerID);
}
