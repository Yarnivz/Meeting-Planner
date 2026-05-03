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
    ENSURE(isExternal() == external, "User creation failed. External flag was not correctly set.");
}

User::User(const User& o) : name(o.name), external(o.external)
{
    REQUIRE(o.isProperlyInitialized(), "Attempted to copy a User which was not properly initialized.");

    // Copy constructor with proper init_check_this_ptr reset
    init_check_this_ptr = this;

    ENSURE(isProperlyInitialized(), "User copy failed. Object was not properly initialized.");
    ENSURE(getId() == name, "User creation failed. Name/Id was not correctly set.");
    ENSURE(isExternal() == external, "User creation failed. External flag was not correctly set.");
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
    REQUIRE(meeting != nullptr, "Meeting cannot be null.");
    this->_addMeeting(meeting);
    meeting->_addParticipant(this);
    ENSURE(meetings.getRawIdMap().contains(meeting->getId()), "Meeting  must be in MeetingRegistery");
    ENSURE(meeting->getParticipants().contains(this->getId()), "User must be in meeting participants");
}

Meeting* User::getMeetingById(const std::string& meetingId)
{
    REQUIRE(!meetingId.empty(), "Meeting id cannot be empty.");
    ENSURE(meetings.getRawIdMap().contains(meetingId), "MeetingId must be in MeetingRegistery");
    return meetings.getById(meetingId);
}

std::list<Meeting*>& User::getMeetingByDateTime(const DateTime& meetingDateTime)
{
    ENSURE(meetings.getRawDateMap().contains(meetingDateTime), "DateTime must be in MeetingRegistry");
    return meetings.getByDateTime(meetingDateTime);
}


void User::_addMeeting(Meeting* meeting)
{
    REQUIRE(meeting != nullptr, "Meeting can not be null");
    REQUIRE(meeting->isProperlyInitialized(), "Meeting must be properly initialized.");
    REQUIRE(!this->isExternal() || meeting->externalsAllowed(), "Can't add external user %s to meeting %s which doesn't allow external users.", this->getId().c_str(), meeting->getId().c_str());

    meetings.add(meeting);

    ENSURE(getMeetingById(meeting->getId()) == meeting, "User %s must be added to meeting %s.", this->getId().c_str(), meeting->getId().c_str());
}
