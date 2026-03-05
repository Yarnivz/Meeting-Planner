//
// Created by User on 2/26/2026.
//

#include "Meeting.h"

Meeting::Meeting(const std::string &label, const std::string &id, const std::string &room, const Date &date):
label(label), id(id), room(room), date(date) {
}

const std::string& Meeting::getId() { return id; }

const std::string& Meeting::toString() { return label; }

std::ostream & operator<<(std::ostream &os, Meeting &meeting) {
    os << meeting.toString();
    return os;
}

void Meeting::process() {}

Meeting::~Meeting() = default;


