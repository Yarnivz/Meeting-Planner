//
// Created by lucas on 4/5/26.
//

#include "User.h"

#include "helper/DesignByContract.h"

User::User(const std::string& name, bool external)
    : name(name), external(external)
{
    init_check_this_ptr = this;

    ENSURE(isProperlyInitialized(), "User creation failed. Object was not properly initialized.");
    ENSURE(getId() == name, "User creation failed. Name/Id was not correctly set.");
}

bool User::isProperlyInitialized() const
{
    return init_check_this_ptr == this;
}

const std::string& User::getId() const
{
    REQUIRE(!name.empty(), "name/Id cannot be empty");
    return name;
}

bool User::isExternal() const
{
    return external;
}

void User::addMeeting(Meeting* meeting)
{
    this->_addMeeting(meeting);
    meeting->_addParticipant(this);
}

Meeting* User::getMeetingById(const std::string& meetingId)
{
    return meetings.getById(meetingId);
}

std::list<Meeting*>& User::getMeetingByDateTime(const DateTime& meetingDateTime)
{
    return meetings.getByDateTime(meetingDateTime);
}


void User::_addMeeting(Meeting* meeting)
{
    REQUIRE(meeting != nullptr, "Meeting can not be null");
    REQUIRE(meeting->isProperlyInitialized(), "Meeting needs to be properly initialized.");
    REQUIRE(!this->isExternal() || meeting->externalsAllowed(), "Can't add external user %s to meeting %s which doesn't allow external users.", this->getId().c_str(), meeting->getId().c_str());

    meetings.add(meeting);

    REQUIRE(getMeetingById(meeting->getId()) == meeting, "User %s must be added to meeting %s.", this->getId().c_str(), meeting->getId().c_str());
}
