//
// Created by Yarni on 3/29/2026.
//

#include "Campus.h"

#include "Catering.h"
#include "helper/DesignByContract.h"

Campus::Campus(const std::string& name, const std::string& id) :
    name(name), id(id)
{
    REQUIRE(!name.empty(), "Name cannot be empty");
    REQUIRE(!id.empty(), "Id cannot be empty");

    init_check_this_ptr = this;

    ENSURE(isProperlyInitialized(), "Campus must be properly initialized.");
    ENSURE(this->name == name, "Name must be added to this Campus object");
    ENSURE(getId() == id, "Id must be added to this Campus object");
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

const std::list<Catering*>& Campus::getCaterings() const
{
    return caterings;
}

Campus::~Campus() = default;


void Campus::_addCatering(Catering* c)
{
    REQUIRE(c != nullptr, "Catering cannot be null");
    REQUIRE(c->isProperlyInitialized(), "Catering must be properly initialized");
    REQUIRE(c->getCampus() == this, "Caterings campus does not match this.");
    caterings.push_back(c);
}
