//
// Created by Yarni on 2/26/2026.
//

#include "./Room.h"

#include <stdexcept>

#include "helper/DesignByContract.h"

Room::Room(const std::string& name, const std::string& id, unsigned capacity, Building* building) :
    name(name), id(id), capacity(capacity), building(building)
{
    REQUIRE(!name.empty(), "name must not be empty");
    REQUIRE(!id.empty(), "id must not be empty");
    REQUIRE(capacity > 0, "capacity must be greater than 0");
    //TestRoom contractviolation works if this is enabled
    //REQUIRE(building != nullptr, "The given building must exist");
    //REQUIRE(building->isProperlyInitialized(), "The given building must be properly initialized");
    if (capacity == 0)
    {
        throw std::invalid_argument("Capacity must be strict greater than 0");
    }
    if (building != nullptr)
    {
        campus = building->getCampus();
    }
    else
    {
        campus = nullptr;
    }

    init_check_this_ptr = this;

    ENSURE(isProperlyInitialized(), "Room creation failed. Object was not properly initialized.");
    ENSURE(getCapacity() == capacity, "Room creation failed. Capacity was not set correctly.");
    ENSURE(getId() == id, "Room creation failed. Id was not set correctly.");
    //ENSURE(getBuilding() == building, "Room creation failed. Building was not set correctly")
}

Room::Room(const Room& r)
{
    REQUIRE(r.isProperlyInitialized(), "Tried to copy a room which was not properly initialized by the constructor.");

    name = r.name;
    id = r.id;
    capacity = r.capacity;
    occupancy = r.occupancy;
    campus = r.campus;
    building = r.building;

    init_check_this_ptr = this;

    ENSURE(isProperlyInitialized(), "Room creation failed. Object was not properly initialized.");
    ENSURE(getCapacity() == r.capacity, "Room creation failed. Capacity was not set correctly.");
    ENSURE(getId() == r.id, "Room creation failed. Id was not set correctly.");
    //ENSURE(getBuilding() == building, "Room creation failed. Building was not set correctly")
}

bool Room::isProperlyInitialized() const
{
    return init_check_this_ptr == this;
}

Building* Room::getBuilding() const
{
    ENSURE(building != nullptr, "Building cannot be null.");
    return building;
}

Campus* Room::getCampus() const
{
    ENSURE(campus != nullptr, "Campus cannot be null.");
    return campus;
}


const std::string& Room::getId() const
{
    ENSURE(!id.empty(), "id must not be empty");
    return id;
}

const std::string& Room::toString() const
{
    ENSURE(!name.empty(), "name must not be empty");
    return name;
}

unsigned Room::getCapacity() const
{
    ENSURE(capacity > 0, "Capacity must be greater than 0");
    return capacity;
}


std::ostream& operator<<(std::ostream& os, const Room& room)
{
    REQUIRE(room.isProperlyInitialized(), "room must be properly initialized");
    os << room.toString();
    ENSURE(os, "ostream variable is not usable");
    return os;
}

Room::~Room() = default;
