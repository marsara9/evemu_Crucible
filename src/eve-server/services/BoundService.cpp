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
    Author:        Almamu
*/

#include "BoundService.h"

void EVEBoundObject<Bound>::NewReference (Client* client) {
    // ensure the client is not there yet
    auto it = this->mClients.find (client);

    if (it != this->mClients.end ())
        return;

    // the client didn't hold a reference to this service
    // so add it to the list and increase the RefCount
    this->mClients.insert_or_assign (client, true);
    // also add it to the bind list of the client
    client->AddBindID (this->GetBoundID ());
}
/**
 * @brief Signals this EVEBoundObject that one of the clients that held a reference, released it
 * @returns Whether the bound object was destroyed or someone still has a reference to it
 */
bool EVEBoundObject<Bound>::Release(Client* client) {
    auto it = this->mClients.find (client);

    // the client doesn't have access to this bound service, so nothing has to be done
    if (it == this->mClients.end ())
        return false;

    // remove the client for the list, and if that's the last one, free the service
    this->mClients.erase (it);

    if (this->mClients.size () == 0) {
        this->GetParent ().BoundReleased (reinterpret_cast <Bound*> (this));
        delete this; // we hate this
        return true;
    }

    return false;
}