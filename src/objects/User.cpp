//
// Created by lucas on 4/5/26.
//

#include "User.h"

#include "helper/DesignByContract.h"

User::User(const std::string& name, bool external)
: name(name), external(external)
{
    init_check_this_ptr = this;
}

bool User::isProperlyInitialized() const
{
    return init_check_this_ptr == this;
}

const std::string& User::getId() const
{
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
    meetings.add(meeting);
}
