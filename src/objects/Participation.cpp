//
// Created by Yarni on 2/26/2026.
//

#include "./Participation.h"

#include "../helper/DesignByContract.h"

Participation::Participation(const std::string& user, const std::string& meeting)
    : user(user), meeting(meeting)
{
    REQUIRE(!user.empty(), "'user' cannot be empty.");
    REQUIRE(!meeting.empty(), "'meeting' cannot be empty.");

    init_check_this_ptr = this;
}

Participation::Participation(const Participation& p)
{
    REQUIRE(p.isProperlyInitialized(),
            "Tried to copy a participation which was not properly initialized by the constructor.");

    user = p.user;
    meeting = p.meeting;

    init_check_this_ptr = this;
}

bool Participation::isProperlyInitialized() const
{
    return init_check_this_ptr == this;
}


std::string Participation::getUser()
{
    REQUIRE(isProperlyInitialized(), "User must be properly initialized.");
    return user;
}

std::string Participation::getMeetingId()
{
    REQUIRE(isProperlyInitialized(), "User must be properly initialized.");
    return meeting;
}

Participation::~Participation() = default;
