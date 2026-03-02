//
// Created by User on 2/26/2026.
//

#include "Meeting.h"

Meeting::Meeting(const std::string &label, const std::string &id, const std::string &room, const Date &date = Date()):
label(label), id(id), room(room), date(date) {
}

const std::string& Meeting::getId() { return id; }

const std::string & Meeting::toString() { return label; }

void Meeting::process() {}

Meeting::~Meeting() = default;
