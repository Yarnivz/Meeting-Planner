//
// Created by Yarni on 3/29/2026.
//

#include "Campus.h"
#include "helper/DesignByContract.h"

Campus::Campus(const std::string& name, const std::string& id) :
    name(name), id(id)
{
    REQUIRE(!name.empty(), "Name cannot be empty");
    REQUIRE(!id.empty(), "Id cannot be empty");

    init_check_this_ptr = this;

    ENSURE(isProperlyInitialized(), "Campus was not properly initialized.");
    ENSURE(this->name == name, "Name was not added to this Campus object");
    ENSURE(getId() == id, "Id was not added to this Campus object");
}

bool Campus::isProperlyInitialized() const
{
    return init_check_this_ptr == this;
}

const std::string& Campus::getId() const
{
    //REQUIRE(!id.empty(), "Id cannot be empty");
    return id;
}

const std::string& Campus::toString() const
{
    //REQUIRE(!id.empty(), "Name cannot be empty");
    return name;
}

std::ostream& operator<<(std::ostream& os, const Campus& campus)
{
    REQUIRE(campus.isProperlyInitialized(), "Campus must be properly initialized");
    os << campus.toString();
    return os;
}

Campus::~Campus() = default;
