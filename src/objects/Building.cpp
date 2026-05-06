//
// Created by Yarni on 3/29/2026.
//

#include "Building.h"
#include "helper/DesignByContract.h"

Building::Building(const std::string& name, const std::string& id, Campus* campus) :
    name(name), id(id), campus(campus)
{
    REQUIRE(!name.empty(), "Name cannot be empty");
    REQUIRE(!id.empty(), "Id cannot be empty");
    REQUIRE(campus != nullptr, "Campus cannot be a null pointer");
    REQUIRE(campus->isProperlyInitialized(), "Campus must be properly initialized");

    init_check_this_ptr = this;

    ENSURE(this->name == name, "Name must be added to this Building object");
    ENSURE(getId() == id, "Id must be added to this Building object");
    ENSURE(getCampus() == campus, "Campus must be added to this Building object");
    ENSURE(isProperlyInitialized(), "Building must be properly initialized");
}

bool Building::isProperlyInitialized() const
{
    return init_check_this_ptr == this;
}

const std::string& Building::getId() const
{
    REQUIRE(!id.empty(), "Id cannot be empty");
    return id;
}

Campus* Building::getCampus() const
{
    REQUIRE(campus != nullptr, "Campus cannot be a null pointer");
    REQUIRE(campus->isProperlyInitialized(), "Campus must be properly initialized");
    return campus;
}

const std::string& Building::toString() const
{
    REQUIRE(!name.empty(), "Name cannot be empty");
    return name;
}

std::ostream& operator<<(std::ostream& os, const Building& building)
{
    REQUIRE(building.isProperlyInitialized(), "Building must be properly initialized");
    os << building.toString();
    return os;
}

Building::~Building() = default;
