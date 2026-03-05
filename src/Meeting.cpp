//
// Created by User on 2/26/2026.
//

#include "Meeting.h"

#include "DesignByContract.h"

Meeting::Meeting(const std::string &label, const std::string &id, const std::string &room, const Date &date)
: label(label), id(id), room(room), date(date)
{
    REQUIRE(!id.empty(), "Failed to construct meeting. 'id' can not be empty.");
    REQUIRE(!id.empty(), "Failed to construct meeting. 'room' can not be empty.");
    REQUIRE(date.isProperlyInitialized(), "Failed to construct meeting. 'date' has to be properly initialized with the constructor.");

    init_check_this_ptr = this;
}

bool Meeting::isProperlyInitialized() const {
    return init_check_this_ptr == this;
}


const std::string& Meeting::getId() {
    REQUIRE(isProperlyInitialized(), "Failed to get id. Room has to be properly initialized with the constructor.");
    return id;
}

const std::string& Meeting::toString() {
    REQUIRE(isProperlyInitialized(), "Failed to convert to string. Room has to be properly initialized with the constructor.");
    return label;
}

std::ostream & operator<<(std::ostream &os, Meeting &meeting) {
    os << meeting.toString();
    return os;
}

void Meeting::process() {}

Meeting::~Meeting() = default;


