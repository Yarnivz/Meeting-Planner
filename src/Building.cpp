//
// Created by User on 3/29/2026.
//

#include "Building.h"

Building::Building(const std::string &name, const std::string &id, const std::string &campus):
name(name), id(id), campus(campus) {
    init_check_this_ptr = this;
}

bool Building::isProperlyInitialized() const {
    return init_check_this_ptr == this;
}

const std::string & Building::getId() const {
    return id;
}

const std::string & Building::getCampusId() const {
    return campus;
}

const std::string & Building::toString() const {
    return name;
}

std::ostream & operator<<(std::ostream &os, const Building &building) {
    os << building.toString();
    return os;
}

Building::~Building() = default;
