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

void App::processMeetings()
{
    for (auto it = all_meetings.begin(); it != all_meetings.end(); ++it)
    {
        REQUIRE(it->second->isProperlyInitialized(), "this meeting has not been properly loaded");
        std::string thisMeetingId = it->second->getId();
        std::cout << thisMeetingId << " has taken place" << std::endl;
        for (auto it2 = all_meetings.begin(); it2 != all_meetings.end(); ++it2)
        {
            REQUIRE(it2->second->isProperlyInitialized(), "other meeting has not been properly loaded");
            std::string otherMeetingId = it2->second->getId();
            if (it->second->getRoomId() == it2->second->getRoomId())
            {
                std::cerr<< otherMeetingId <<  " shares the same room with " <<  thisMeetingId << ", cancelling " << otherMeetingId << std::endl;
                delete it->second;
                all_meetings.erase(it2);
            }
        }
        std::cout << it->second << std::endl;
    }
}






void App::addRoom(Room *room) {
    REQUIRE(room, "The provided room cannot be null.");
    REQUIRE(room->isProperlyInitialized(), "Room needs to be properly initialized by the constructor.");
    REQUIRE(!rooms.contains(room->getId()), "Room id has to be unique.");


    rooms.insert({room->getId(), room});          // Add room
    meetings_by_room.insert({room->getId(), {}}); // Add empty meetings map

    ENSURE(getRoom(room->getId()) == room, "The room was not added to the App");
}

Room* App::getRoom(const std::string& roomId) {
    const Rooms::iterator it = rooms.find(roomId);

    if (it == rooms.end()) return nullptr;

    ENSURE(it->second->getId() == roomId, "Something went wrong. The room which was found did not have the right id.");
    return it->second;
}






void App::addMeeting(Meeting *meeting) {
    REQUIRE(meeting, "Meeting can not be null.");
    REQUIRE(meeting->isProperlyInitialized(), "Meeting needs to be properly initialized.");
    REQUIRE(!all_meetings.contains(meeting->getId()), "Meetings Id needs to be unique!");

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


    all_meetings.insert({meeting->getId(), meeting}); // Insert into flat map by meetingId
    mt_list->insert({meeting->getId(), meeting});     // Also into nested map by roomId => meetingId

    participations_by_meeting.insert({meeting->getId(), {}}); // Insert empty participations list


    ENSURE(getMeetingInRoom(meeting->getId(), meeting->getRoomId()) == meeting, "Something went wrong, The meeting was not added to the App");
    ENSURE(getMeeting(meeting->getId()) == meeting, "Something went wrong, the meeting was not added to the App");
}



Meeting * App::getMeetingInRoom(const std::string &meetingId, const std::string &roomId) {
    Meetings* ms = _getMutMeetingsByRoom(roomId);
    if (!ms) return nullptr;

    const Meetings::iterator ms_it = ms->find(meetingId);

    if (ms_it == ms->end()) return nullptr;

    ENSURE(ms_it->second->getId() == meetingId, "Something went wrong, The meeting which was found did not have the correct id.");
    return ms_it->second;
}


Meeting* App::getMeeting(const std::string& meetingId) {

    const Meetings::iterator it = all_meetings.find(meetingId);

    if (it == all_meetings.end()) return nullptr;

    ENSURE(it->second->getId() == meetingId, "Something went wrong, The meeting which was found did not have the correct id.");
    return it->second;
}








void App::addParticipation(Participation *participation) {
    REQUIRE(participation != nullptr, "Participation can not be null.");
    REQUIRE(participation->isProperlyInitialized(), "Participation needs to be properly initialized.");


    const Meeting* mt = getMeeting(participation->getMeetingId());
    REQUIRE(mt, "Could not add participation: The requested meeting does not exist.");
    REQUIRE(!isUserOccupied(participation->getUser(), mt->getDate()), "Could not add participation: This user already participates in another meeting.");


    const Room* rm = getRoom(mt->getRoomId());
    ENSURE(rm, "Something went wrong. Encountered a meeting with invalid room id.");


    Participations* ps_by_meeting = _getMutParticipationsByMeeting(participation->getMeetingId());
    ENSURE(ps_by_meeting, "Something went wrong. The list of participations by meeting went out of sync.");
    REQUIRE(ps_by_meeting->size() <= rm->getCapacity(), "Could not add participation: The room the meeting takes place in was full.");




    all_participations.push_back(participation); //Add it to a flat list of all participations


    //Also insert into map by userId
    Participations* ps_by_user =  _getMutParticipationsByUser(participation->getUser());
    if (ps_by_user) {
        ps_by_user->push_back(participation);
    } else {
        participations_by_user.insert({participation->getUser(), {participation}});
    }



    ps_by_meeting->push_back(participation);

    ENSURE(all_participations.back() == participation, "Something went wrong. The participation was not added to the global list.");
    ENSURE(_getMutParticipationsByUser(participation->getUser())->back() == participation, "Something went wrong. The participation was not added to the list by user.");
    ENSURE(_getMutParticipationsByMeeting(participation->getMeetingId())->back() == participation, "Something went wrong. The participation was not added to the list by app.");
}





const Participations* App::getParticipationsByUser(const std::string &userId) {
    return _getMutParticipationsByUser(userId);
}

const Participations* App::getParticipationsByMeeting(const std::string &meetindId) {
    return _getMutParticipationsByMeeting(meetindId);
}




const Meetings & App::getAllMeetings() const {
    return all_meetings;
}

const Participations & App::getAllParticipations() const {
    return all_participations;
}



const Rooms & App::getAllRooms() const {
    return rooms;
}

const Meetings* App::getMeetingsByRoom(const std::string &roomId) {
    return _getMutMeetingsByRoom(roomId);
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
    for (const std::pair<const std::string, Meeting*>& m : all_meetings) delete m.second;
    for (Participation* p : all_participations) delete p;
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

Participations *App::_getMutParticipationsByMeeting(const std::string &meetingId) {
    const ParticipationsByMeetingMap::iterator it = participations_by_meeting.find(meetingId);

    if (it == participations_by_meeting.end()) return nullptr;

    return &it->second;
}


