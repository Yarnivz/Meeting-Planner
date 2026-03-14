//
// Created by User on 2/26/2026.
//

#include "Meeting.h"

#include "DesignByContract.h"

Meeting::Meeting(const std::string &label, const std::string &id, const std::string &roomId, const Date &date)
: label(label), id(id), room(roomId), date(date)
{
    REQUIRE(!id.empty(), "Failed to construct meeting. 'id' can not be empty.");
    REQUIRE(!roomId.empty(), "Failed to construct meeting. 'room' can not be empty.");
    REQUIRE(date.isProperlyInitialized(), "Failed to construct meeting. 'date' has to be properly initialized with the constructor.");

    init_check_this_ptr = this;
}

bool Meeting::isProperlyInitialized() const {
    return init_check_this_ptr == this;
}


const std::string& Meeting::getId() const {
    REQUIRE(isProperlyInitialized(), "Failed to get id. Meeting has to be properly initialized with the constructor.");
    return id;
}

const std::string & Meeting::getRoomId() const {
    REQUIRE(isProperlyInitialized(), "Failed to get room id. Meeting has to be properly initialized with the constructor.");
    return room;
}

const Date & Meeting::getDate() const {
    REQUIRE(isProperlyInitialized(), "Failed to get date. Meeting has to be properly initialized with the constructor.");
    return date;
}


const std::string& Meeting::toString() const {
    REQUIRE(isProperlyInitialized(), "Failed to convert to string. Meeting has to be properly initialized with the constructor.");
    return label;
}


int Meeting::getOrder() const { return order; }

void Meeting::setOrder(const int orderAdded) { order = orderAdded; }


std::ostream & operator<<(std::ostream &os, const Meeting &meeting) {
    REQUIRE(meeting.isProperlyInitialized(), "meeting must be properly initialized");
    os << meeting.toString();
    ENSURE(os, "ostream variable is not usable");
    return os;
}

void Meeting::process() {}

Meeting::~Meeting() = default;



