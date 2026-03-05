//
// Created by User on 2/26/2026.
//

#include "App.h"

App::App() : rooms(), meetings(), participation_lists() {
    init_check_this_ptr = this;
}

bool App::isProperlyInitialized() const {
    return init_check_this_ptr == this;
}

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
    const std::unordered_map<std::string, Room*>::iterator it = rooms.find(id);

    if (it == rooms.end()) return nullptr;

    return it->second;
}

void App::addMeeting(Meeting *meeting) {
    if (!meeting) return;

    meetings.insert({meeting->getId(), meeting});
}

Meeting* App::getMeeting(const std::string& id) {
    const std::unordered_map<std::string, Meeting*>::iterator it = meetings.find(id);

    if (it == meetings.end()) return nullptr;

    return it->second;
}

void App::addParticipation(Participation *participation) {
    if (!participation) return;

    const std::string u = participation->getUser();

    const std::unordered_map<std::string, std::list<Participation*>>::iterator it = participation_lists.find(u);

    if (it == participation_lists.end()) {
        participation_lists.insert({participation->getUser(), {participation}});
        return;
    }

    it->second.push_back(participation);
}

const std::list<Participation *> *App::getParticipationList(const std::string &username) {
    const std::unordered_map<std::string, std::list<Participation*>>::iterator it = participation_lists.find(username);

    if (it == participation_lists.end()) return nullptr;

    return &it->second;
}



App::~App() {
    for (auto& r : rooms) delete r.second;
    for (auto& m : meetings) delete m.second;
    for (const auto& pl : participation_lists) {
        for (const auto& p : pl.second ) {
            delete p;
        }
    }
}


