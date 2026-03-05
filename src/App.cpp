//
// Created by User on 2/26/2026.
//

#include "App.h"

#include "DesignByContract.h"

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
    REQUIRE(room != nullptr, "The provided room cannot be a nullptr. Room needs to have a value.");
    REQUIRE(room->isProperlyInitialized(), "There needs to be a properly initialized room object to be inserted into the App");

    rooms.insert({room->getId(), room});
    ENSURE(getRoom(room->getId()) == room, "The room was not added to the App");
}

Room* App::getRoom(const std::string& id) {
    REQUIRE(!id.empty() ,"The provided id cannot be empty or null.");
    const std::unordered_map<std::string, Room*>::iterator it = rooms.find(id);

    if (it == rooms.end()) return nullptr;
    return it->second;
}

void App::addMeeting(Meeting *meeting) {
    REQUIRE(meeting != nullptr, "The provided meeting cannot be a nullptr. Room needs to have a value.");
    REQUIRE(meeting->isProperlyInitialized(), "There needs to be a properly initialized meeting object to be inserted into the App");

    meetings.insert({meeting->getId(), meeting});
    ENSURE(getMeeting(meeting->getId()) == meeting, "The meeting was not added to the App");
}

Meeting* App::getMeeting(const std::string& id) {
    REQUIRE(!id.empty() ,"The provided id cannot be empty or null.");
    const std::unordered_map<std::string, Meeting*>::iterator it = meetings.find(id);

    if (it == meetings.end()) return nullptr;

    return it->second;

}

void App::addParticipation(Participation *participation) {
    REQUIRE(participation != nullptr, "The provided participation cannot be a nullptr. Participation needs to have a value.");
    REQUIRE(participation->isProperlyInitialized(), "There needs to be a properly initialized participation object to be inserted into the App");

    const std::string u = participation->getUser();

    const std::unordered_map<std::string, std::list<Participation*>>::iterator it = participation_lists.find(u);

    if (it == participation_lists.end()) {
        participation_lists.insert({participation->getUser(), {participation}});
        return;
    }

    it->second.push_back(participation);
    ENSURE(getParticipationList(participation->getUser())->back() == participation, "The participation was not added to the App");

}

const std::list<Participation *> *App::getParticipationList(const std::string &username) {
    REQUIRE(!username.empty() ,"The provided user cannot be empty or null.");
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


