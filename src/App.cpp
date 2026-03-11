//
// Created by User on 2/26/2026.
//

#include "App.h"
#include "DesignByContract.h"

App::App() : rooms(), meetings_by_room() {
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

    rooms.insert({room->getId(), room});          // Add room
    meetings_by_room.insert({room->getId(), {}}); // Add empty meetings map

    ENSURE(getRoom(room->getId()) == room, "The room was not added to the App");
}

Room* App::getRoom(const std::string& id) {
    const Rooms::iterator it = rooms.find(id);

    if (it == rooms.end()) return nullptr;
    return it->second;
}






void App::addMeeting(Meeting *meeting) {
    REQUIRE(meeting, "Meeting can not be null.");
    REQUIRE(meeting->isProperlyInitialized(), "Meeting needs to be properly initialized.");

    Room* rm = getRoom(meeting->getRoomId());

    REQUIRE(
        rm,
        "The meeting has to take place in a room which was already registered."
        );

    REQUIRE(
        !isRoomOccupied(rm->getId(), meeting->getDate()),
        "There is already a meeting planned in this room at that date."
        );


    Meetings *mt_list = _getMutMeetingsByRoom(meeting->getRoomId());
    ENSURE(mt_list, "Something went wrong, the meeting list was not found.");

    mt_list->insert({meeting->getId(), meeting});

    ENSURE(getMeetingInRoom(meeting->getId(), meeting->getRoomId()) == meeting, "The meeting was not added to the App");
}



Meeting * App::getMeetingInRoom(const std::string &meetingId, const std::string &roomId) {
    Meetings* ms = _getMutMeetingsByRoom(roomId);
    if (!ms) return nullptr;

    const Meetings::iterator ms_it = ms->find(meetingId);

    if (ms_it == ms->end()) return nullptr;
    return ms_it->second;
}


Meeting* App::getMeeting(const std::string& meetingId) {

    for (MeetingsByRoomMap::iterator it = meetings_by_room.begin(); it != meetings_by_room.end(); ++it) {
        Meetings::iterator ms_it = it->second.find(meetingId);

        if (ms_it != it->second.end()) return ms_it->second;
    }

    return nullptr;
}









void App::addParticipation(Participation *participation) {
    REQUIRE(participation != nullptr, "Participation can not be null.");
    REQUIRE(participation->isProperlyInitialized(), "Participation needs to be properly initialized.");


    const Meeting* mt = getMeeting(participation->getMeetingId());
    REQUIRE(mt, "The requested meeting does not exist.");


    const Room* mt_room = getRoom(mt->getRoomId());
    ENSURE(mt_room, "Something went wrong. Found meeting in App but it did not have an assigned room.");


    //REQUIRE(!isRoomOccupied(mt_room->getId(), mt->getDate()), "This rooms is already occupied.");
    REQUIRE(!isUserOccupied(participation->getUser(), mt->getDate()), "This user already participates in another meeting.");

    Participations* prts =  _getMutParticipationsByUser(participation->getUser());
    if (prts) {
        prts->push_back(participation);
    } else {
        participations_by_user.insert({participation->getUser(), {participation}});
    }

    ENSURE(_getMutParticipationsByUser(participation->getUser())->back() == participation, "The participation was not added to the App");
}







const Meetings* App::getMeetingsByRoom(const std::string &roomId) {
    return _getMutMeetingsByRoom(roomId);
}

const Participations* App::getParticipationsByUser(const std::string &userId) {
    return _getMutParticipationsByUser(userId);

}






bool App::isRoomOccupied(const std::string &roomId, const Date& date) {
    Meetings *mt_list = _getMutMeetingsByRoom(roomId);
    REQUIRE(mt_list, "This room doesnt exist.");

    for (Meetings::iterator it = mt_list->begin(); it != mt_list->end(); ++it) {
        Meeting* m = it->second;
        if (m->getDate() == date) return true;
    }

    return false;
}

bool App::isUserOccupied(const std::string &userId, const Date &date) {
    Participations *prt_list = _getMutParticipationsByUser(userId);

    if (prt_list == nullptr) return false;

    for (Participations::iterator it = prt_list->begin(); it != prt_list->end(); ++it) {

        Participation* p = *it;
        const Meeting* m = getMeeting(p->getMeetingId());
        ENSURE(m, "Something went wrong. Encountered a participation without a valid meeting Id.");

        if (m->getDate() == date) return true;
    }

    return false;
}









App::~App() {
    for (const std::pair<const std::string, Room*>& r : rooms) delete r.second;
    for (const std::pair<const std::string, Meetings>& ls : meetings_by_room) {
        for (const std::pair<const std::string, Meeting*>& m : ls.second) delete m.second;
    }
    for (const std::pair<const std::string, Participations>& ls : participations_by_user) {
        for (const Participation* p : ls.second) delete p;
    }
}

Meetings * App::_getMutMeetingsByRoom(const std::string &roomId) {
    const MeetingsByRoomMap::iterator it = meetings_by_room.find(roomId);

    if (it == meetings_by_room.end()) return nullptr;

    return &it->second;
}

Participations *App::_getMutParticipationsByUser(const std::string &userId) {
    const ParticipationsByUserMap::iterator it = participations_by_user.find(userId);

    if (it == participations_by_user.end()) return nullptr;

    return &it->second;
}


