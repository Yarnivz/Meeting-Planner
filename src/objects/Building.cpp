//
// Created by Yarni on 3/29/2026.
//

#include "./Building.h"
#include "../helper/DesignByContract.h"

Building::Building(const std::string& name, const std::string& id, const std::string& campus) :
    name(name), id(id), campus(campus)
{
    REQUIRE(!name.empty(), "Name cannot be empty");
    REQUIRE(!id.empty(), "Id cannot be empty");
    REQUIRE(!campus.empty(), "CampusId cannot be empty");

    init_check_this_ptr = this;

    ENSURE(this->name == name, "Name was not added to this Building object");
    ENSURE(this->id == id, "Id was not added to this Building object");
    ENSURE(this->id == id, "CampusId was not added to this Building object");
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

const std::string& Building::getCampusId() const
{
    REQUIRE(!name.empty(), "Name cannot be empty");
    return campus;
}

const std::string& Building::toString() const
{
    REQUIRE(!campus.empty(), "CampusId cannot be empty");
    return name;
}

std::ostream& operator<<(std::ostream& os, const Building& building)
{
    REQUIRE(building.isProperlyInitialized(), "Building must be properly initialized");
    os << building.toString();
    return os;
}

Building::~Building() = default;
