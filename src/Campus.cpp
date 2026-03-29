//
// Created by User on 3/29/2026.
//

#include "Campus.h"

Campus::Campus(const std::string &name, const std::string &id):
name(name), id(id) {
    init_check_this_ptr = this;
}

bool Campus::isProperlyInitialized() const {
    return init_check_this_ptr == this;
}

const std::string & Campus::getId() const {
    return id;
}

const std::string & Campus::toString() const {
    return name;
}

std::ostream & operator<<(std::ostream &os, const Campus &campus) {
    os << campus.toString();
    return os;
}

Campus::~Campus() = default;
