//
// Created by User on 2/26/2026.
//

#include "Room.h"
#include "../DesignByContract.h"

Room::Room(const std::string &name, const std::string &id, unsigned capacity):
name(name), id(id), capacity(capacity) {
    REQUIRE(capacity > 0, "Capacity must be strict greater than 0");
}

const std::string& Room::getId() { return id; }

const std::string & Room::toString() { return name; }

Room::~Room() = default;
