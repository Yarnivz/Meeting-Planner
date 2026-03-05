//
// Created by User on 2/26/2026.
//

#include "Room.h"

#include <stdexcept>

#include "DesignByContract.h"

Room::Room(const std::string &name, const std::string &id, unsigned capacity):
name(name), id(id), capacity(capacity) {
    if (capacity == 0) {
        throw std::invalid_argument("Capacity must be strict greater than 0");
    }

    init_check_this_ptr = this;
}

bool Room::isProperlyInitialized() const {
    return init_check_this_ptr == this;
}


const std::string& Room::getId() { return id; }

const std::string& Room::toString() { return name; }

std::ostream & operator<<(std::ostream& os, Room &room) {
    os << room.toString();
    return os;
}

Room::~Room() = default;
