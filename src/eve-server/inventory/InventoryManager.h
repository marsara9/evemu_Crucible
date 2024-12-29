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

#ifndef __INVENTORY_MANAGER__H__INCL__
#define __INVENTORY_MANAGER__H__INCL__

#include <shared_mutex>

#include "InventoryBound.h"
#include "InvBrokerService.h"
#include "ItemRef.h"

class InventoryManager : public Singleton<InventoryManager> {
public:
    /**
     * Searches for an existing InventoryBound for the provided containerID.
     * If one is found a reference to its InventoryBound will be returned,
     * otherwise `nullptr` is returned instead.
     * 
     * @param containerID The itemID of the container in which to find the associated InventoryBound.
     * @returns The associated InventoryBound or nullptr if no match exists.
     */
    std::shared_ptr<InventoryBound> Find(
        uint32 containerID,
        uint32 ownerID
    );

    std::shared_ptr<InventoryBound> FindOrCreate(
        uint32 containerID,
        uint32 ownerID
        std::function<std::shared_ptr<InventoryBound>()> create
    );

    /**
     * Adds the provided InventoryBound to be cached.
     * 
     * @param containerID The containerID that should be used for future lookups.  This should 
     *   be the itemID of the container in which the InventoryBound is for.
     * @param bound The InventoryBound to cache.
     */
    void Add(
        uint32 containerID,
        uint32 ownerID,
        std::shared_ptr<InventoryBound> ib
    );

    /**
     * Removes any InventoryBound associated with the given containerID.
     */
    void Remove(
        uint32 containerID,
        uint32 ownerID
    );
private:
    typedef std::pair<uint32, uint32> Key;
    typedef std::map<Key, std::shared_ptr<InventoryBound>> BoundMap;
    typedef std::pair<Key, std::shared_ptr<InventoryBound>> BoundEntry;

    typedef std::shared_mutex Lock;
    typedef std::unique_lock<Lock> WriteLock;
    typedef std::shared_lock<Lock> ReadLock;

    Lock m_lock;

    BoundMap m_boundMap;
};

// Should this be a singleton? 
// Can we use dependency injection somehow instead?
#define sInventoryManager \
 ( InventoryManager::get() )

#endif
