//
// Created by Yarni on 2/26/2026.
//

#include "Room.h"

#include <stdexcept>

#include "DesignByContract.h"

Room::Room(const std::string &name, const std::string &id, unsigned capacity):
name(name), id(id), capacity(capacity) {

    REQUIRE(!name.empty(), "name must not be empty");
    REQUIRE(!id.empty(), "id must not be empty");
    REQUIRE(capacity > 0, "capacity must be greater than 0");
    if (capacity == 0) {
        throw std::invalid_argument("Capacity must be strict greater than 0");
    }

    init_check_this_ptr = this;
}

Room::Room(const Room& r) {
    REQUIRE(r.isProperlyInitialized(), "Tried to copy a room which was not properly initialized by the constructor.");

    name = r.name;
    id = r.id;
    capacity = r.capacity;

    init_check_this_ptr = this;
}

bool Room::isProperlyInitialized() const {
    return init_check_this_ptr == this;
}


const std::string& Room::getId() const
{
    REQUIRE(!id.empty(), "id must not be empty");
    return id;

}

const std::string& Room::toString() const
{
    REQUIRE(!name.empty(), "name must not be empty");
    return name;
}

unsigned Room::getCapacity() const {
    REQUIRE(capacity > 0, "Capacity must be greater than 0");
    return capacity;
}


std::ostream & operator<<(std::ostream& os, const Room &room) {
    REQUIRE(room.isProperlyInitialized(), "room must be properly initialized");
    os << room.toString();
    ENSURE(os, "ostream variable is not usable");
    return os;
}

Room::~Room() = default;
