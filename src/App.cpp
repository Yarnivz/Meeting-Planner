//
// Created by User on 2/26/2026.
//

#include "App.h"

App::App() : rooms(), meetings(), participations(){}

void App::parseFile(std::string filename) {
}

void App::writeToStream(std::ostream stream) {
}


void App::processMeetings() {
}


void App::addRoom(Room *room) {
    if (!room) return;

    rooms.insert({room->getId(), room});
}

Room* App::getRoom(const std::string& id) {
    auto it = rooms.find(id);

    if (it == rooms.end()) return nullptr;

    return it->second;
}

void App::addMeeting(Meeting *meeting) {
    if (!meeting) return;

    meetings.insert({meeting->getId(), meeting});
}

Meeting* App::getMeeting(const std::string& id) {
    auto it = meetings.find(id);

    if (it == meetings.end()) return nullptr;

    return it->second;
}

void App::addParticipation(Participation *participation) {
    if (!participation) return;

    //! participations.insert({std::string(), participation});
}

Participation* App::getParticipation(const std::string& id) {
    auto it = participations.find(id);

    if (it == participations.end()) return nullptr;

    return it->second;
}



App::~App() {
    for (auto& r : rooms) delete r.second;
    for (auto& m : meetings) delete m.second;
    for (auto& p : participations) delete p.second;
}