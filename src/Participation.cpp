//
// Created by User on 2/26/2026.
//

#include "Participation.h"

Participation::Participation(const std::string &user, const std::string &meeting)
: user(user), meeting(meeting)
{
    init_check_this_ptr = this;
}

bool Participation::isProperlyInitialized() const {
    return init_check_this_ptr == this;
}


std::string Participation::getUser() { return user; }

std::string Participation::getMeetingId() { return meeting; }

Participation::~Participation() = default;
