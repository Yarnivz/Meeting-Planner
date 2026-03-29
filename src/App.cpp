//
// Created by Yarni on 2/26/2026.
//

#include "App.h"

#include <filesystem>
#include <queue>
#include <tinyxml.h>
#include "DesignByContract.h"
#include "Parser.h"
#include "XmlParser.h"


App::App(Parser* parser, std::ostream* output) : parser(parser), output(output) {
    init_check_this_ptr = this;
}

bool App::isProperlyInitialized() const {
    return init_check_this_ptr == this;
}


void App::parseFile(const std::string& filename, std::ostream& errStream)
{

    if (!parser) {errStream << "This App does not have a parser!" << std::endl; return;}



    parser->parse(filename, errStream);


    //> Add all elements in the correct order
    //  Final integrity checks

    for (Room* r : parser->parsedRooms()) {
        if (getRoom(r->getId()) != nullptr) {
            errStream << "Room ids must be unique: " << r->getId() << std::endl;
            continue;
        }

        addRoom(r);
    }

    for (Meeting* m : parser->parsedMeetings()) {
        if (getRoom(m->getRoomId()) == nullptr) {
            errStream << "Meeting " << m->getId() << " takes place in a room \'" << m->getRoomId() << "\' which doesnt exist." << std::endl;
            continue;
        }

        addMeeting(m);
    }

    for (Participation* p : parser->parsedParticipations()) {
        if (getMeeting(p->getMeetingId()) == nullptr) {
            errStream << "User \'" << p->getUser() << "\' participates in a meeting \'" << p->getMeetingId() << "\' which doesnt exist." << std::endl;
            continue;
        }

        addParticipation(p);
    }



    parser->clearRooms();
    parser->clearMeetings();
    parser->clearParticipations();
}

void App::writeToStream() {
    REQUIRE(output, "App doesnt have an output attached.");

    //Write all past meetings
    if (!ongoing_meetings.empty()) *output << std::endl << "Past meetings:" << std::endl;
    for (const std::pair<std::string,Meeting*> item : ongoing_meetings) {
        Meeting* meeting = item.second;
        writeMeeting(*output, meeting);
    }

    //Write all future meetings
    if (!future_meetings.empty()) *output << std::endl << "Future meetings:" << std::endl;
    for (const std::pair<std::string, Meeting*> item : future_meetings) {
        Meeting* meeting = item.second;
        writeMeeting(*output, meeting);
    }

    //Write all conflicts
    if (!cancelling_meetings.empty()) *output << std::endl << "Conflicts:" << std::endl;
    for (const std::pair<const std::string, Meeting *>& item : cancelling_meetings) {
        const Meeting* meeting = item.second;
        writeMeeting(*output, meeting);
        *output << "  Reason: " << getCancellationReason(meeting->getId()) << std::endl;
    }

    //Write all rooms
    if (!rooms.empty()) *output << std::endl << "Rooms:" << std::endl;
    for (const std::pair<std::string, Room*> item : rooms) {
        const Room* room = item.second;
        writeRoom(*output, room);
    }
}

void App::processSingleMeeting(const std::string &meetingId, const bool verbose)
{
    //REQUIRE(!meetingId.empty(), "Meeting id is empty");
    Meeting* meeting = getMeeting(meetingId);
    REQUIRE(meeting, "This meeting doesn't exist.");
    REQUIRE(meeting->isProperlyInitialized(), "Meeting needs to be properly initialized.");

    bool meetingProcessed = false;

    Meeting* conflict;
    if ((conflict = findConflictingMeeting(meetingId)))
    {
        cancelMeeting(meetingId, "conflict with meeting " + conflict->getId());
        if (verbose) std::cout << meeting->getId() << " has been cancelled due to '" + getCancellationReason(meeting->getId()) << "'" << std::endl;
        meetingProcessed = true;
    } else
    {
        doMeeting(meetingId);
        if (verbose) std::cout << meeting->getId() << " has taken place" << std::endl;
        meetingProcessed = true;
    }

    ENSURE(meetingProcessed, "Meeting hasn't been processed");
}

void App::processAllMeetings(const bool verbose)
{
    std::vector<Meeting*> sortedMeetings;
    for (std::pair<std::string, Meeting*> m : all_meetings)
    {
        if (m.second->getDate() <= Date()) sortedMeetings.push_back(m.second);
    }
    std::sort(sortedMeetings.begin(), sortedMeetings.end(), [](const Meeting* comparedMeeting1, const Meeting* comparedMeeting2)
    {
        if (comparedMeeting1->getDate() != comparedMeeting2->getDate())
        {
            return comparedMeeting1->getDate() < comparedMeeting2->getDate();
        }
        return comparedMeeting1->getOrder() < comparedMeeting2->getOrder();
    });

    for (size_t i = 0; i < sortedMeetings.size(); ++i)
    {
        const Meeting* currentMeeting = sortedMeetings[i];
        REQUIRE(currentMeeting, "Meeting can not be null.");
        REQUIRE(currentMeeting->isProperlyInitialized(), "Meeting needs to be properly initialized.");
        processSingleMeeting(currentMeeting->getId(), verbose);
    }

    size_t totalMeetingsProcessed = cancelling_meetings.size() + ongoing_meetings.size();
    size_t allPastMeetingsSize = all_meetings.size() - future_meetings.size();
    ENSURE(allPastMeetingsSize == totalMeetingsProcessed, "Not all meetings have been processed");
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


const Rooms & App::getAllRooms() const {
    ENSURE(!rooms.empty(), "rooms contains no room");
    return rooms;
}




bool App::isRoomOccupied(const std::string &roomId, const Date& date) {
    Meetings *mt_list = _getMutMeetingsByRoom(roomId);
    REQUIRE(mt_list, "This room doesnt exist.");

    for (Meetings::iterator it = mt_list->begin(); it != mt_list->end(); ++it) {
        const Meeting* m = it->second;

        if (getDoneMeeting(m->getId()) && m->getDate() == date)
            return true;
    }

    return false;
}

Meeting* App::findConflictingMeeting(const std::string &meetingId) {
    Meeting* m = getMeeting(meetingId);
    REQUIRE(m, "This meeting doesn't exist.");
    Meetings *mt_list = _getMutMeetingsByRoom(m->getRoomId());
    ENSURE(mt_list, "Encountered a meeting with a roomId which doesnt exist.");

    for (Meetings::iterator it = mt_list->begin(); it != mt_list->end(); ++it) {
        Meeting* other_m = it->second;

        if (getDoneMeeting(other_m->getId()) && other_m->getDate() == m->getDate())
            return other_m;
    }

    return nullptr;
}













void App::addMeeting(Meeting *meeting) {
    REQUIRE(meeting, "Meeting can not be null.");
    REQUIRE(meeting->isProperlyInitialized(), "Meeting needs to be properly initialized.");
    REQUIRE(!all_meetings.contains(meeting->getId()), "Meetings Id needs to be unique!");

    Room* rm = getRoom(meeting->getRoomId());

    REQUIRE(rm, "The meeting has to take place in a room which was already registered.");

    // REQUIRE(
    //     !isRoomOccupied(rm->getId(), meeting->getDate()),
    //     "There is already a meeting planned in this room at that date."
    //     );


    Meetings *mt_list = _getMutMeetingsByRoom(meeting->getRoomId());
    ENSURE(mt_list, "Something went wrong, the meeting list was not found.");

    // If meeting order was not assigned, automatically assign the order using the count of the meetings already saved in the system.
    if (meeting->getOrder() == 0) {
        meeting->setOrder(static_cast<int>(all_meetings.size() + 1));
    }

    all_meetings.insert({meeting->getId(), meeting});       // Insert into flat map by meetingId
    future_meetings.insert({meeting->getId(), meeting});    // Also into future meetings
    mt_list->insert({meeting->getId(), meeting});           // Also into nested map by roomId => meetingId

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

Meeting* App::getCanceledMeeting(const std::string& meetingId) {
    const Meetings::iterator it = cancelling_meetings.find(meetingId);

    if (it == cancelling_meetings.end()) return nullptr;

    ENSURE(it->second->getId() == meetingId, "Something went wrong, The meeting which was found did not have the correct id.");
    return it->second;
}

const std::string& App::getCancellationReason(const std::string& meetingId) {
    REQUIRE(getCanceledMeeting(meetingId), "That meeting does not exist or it isn't cancelled");

    const std::unordered_map<std::string, std::string>::iterator it = canceled_meeting_reasons.find(meetingId);

    ENSURE(it != canceled_meeting_reasons.end(), "Something went wrong, The meeting was found but the cancellation reason wasn't.");
    ENSURE(it->first == meetingId, "Something went wrong, The wrong cancellation reason was retrieved.");

    return it->second;
}



Meeting* App::getDoneMeeting(const std::string& meetingId) {
    const Meetings::iterator it = ongoing_meetings.find(meetingId);

    if (it == ongoing_meetings.end()) return nullptr;

    ENSURE(it->second->getId() == meetingId, "Something went wrong, The meeting which was found did not have the correct id.");
    return it->second;
}

Meeting* App::getFutureMeeting(const std::string& meetingId) {
    const Meetings::iterator it = future_meetings.find(meetingId);

    if (it == future_meetings.end()) return nullptr;

    ENSURE(it->second->getId() == meetingId, "Something went wrong, The meeting which was found did not have the correct id.");
    return it->second;
}


void App::cancelMeeting(const std::string& meetingId, const std::string& reason) {
    Meeting* m;
    REQUIRE((m = getMeeting(meetingId)), "This meeting does not exist.");
    REQUIRE(getCanceledMeeting(meetingId) == nullptr, "This meeting was already canceled.");
    REQUIRE(getDoneMeeting(meetingId) == nullptr, "This meeting was already done.");
    REQUIRE(getFutureMeeting(meetingId), "This meeting is not in the list of unprocessed meetings.");

    future_meetings.erase(meetingId);
    cancelling_meetings.insert({meetingId, m});
    canceled_meeting_reasons.insert({meetingId, reason});

    ENSURE(getDoneMeeting(meetingId) == nullptr, "Something went wrong. The meeting was not canceled.");
}

void App::uncancelMeeting(const std::string& meetingId) {
    Meeting* m;
    REQUIRE((m = getMeeting(meetingId)), "This meeting does not exist.");
    REQUIRE(getCanceledMeeting(meetingId), "This meeting was not canceled.");
    REQUIRE(getDoneMeeting(meetingId) == nullptr, "This meeting was already done.");
    REQUIRE(getFutureMeeting(meetingId), "This meeting is not in the list of unprocessed meetings.");

    future_meetings.insert({meetingId, m});
    cancelling_meetings.erase(meetingId);
    canceled_meeting_reasons.erase(meetingId);

    ENSURE(getDoneMeeting(meetingId) == nullptr, "Something went wrong. The meeting was not uncanceled.");
}

void App::doMeeting(const std::string& meetingId) {
    Meeting* m;
    REQUIRE((m = getMeeting(meetingId)), "This meeting does not exist.");
    REQUIRE(getCanceledMeeting(meetingId) == nullptr, "This meeting was canceled.");
    REQUIRE(getDoneMeeting(meetingId) == nullptr, "This meeting was already done.");
    REQUIRE(getFutureMeeting(meetingId), "This meeting is not in the list of unprocessed meetings.");


    future_meetings.erase(meetingId);
    ongoing_meetings.insert({meetingId, m});

    ENSURE(getDoneMeeting(meetingId) == m, "Something went wrong. The meeting was not done.");
    ENSURE(getFutureMeeting(meetingId) == nullptr, "Something went wrong. The meeting was not deleted from future (unprocessed) meetings");
}

void App::undoMeeting(const std::string& meetingId) {
    Meeting* m;
    REQUIRE((m = getMeeting(meetingId)), "This meeting does not exist.");
    REQUIRE(getCanceledMeeting(meetingId) == nullptr, "This meetings was canceled.");
    REQUIRE(getDoneMeeting(meetingId), "This meeting was not done.");

    ongoing_meetings.erase(meetingId);

    ENSURE(getDoneMeeting(meetingId) == nullptr, "Something went wrong. The meeting was not undone.");

}



const Meetings & App::getAllMeetings() const {
    ENSURE(!all_meetings.empty(), "all_meetings contains no meetings");
    return all_meetings;
}

const Meetings* App::getMeetingsByRoom(const std::string &roomId) {
    ENSURE(_getMutMeetingsByRoom(roomId),  "Something went wong while going through the meetings.");
    return _getMutMeetingsByRoom(roomId);
}


















void App::addParticipation(Participation *participation) {
    REQUIRE(participation != nullptr, "Participation can not be null.");
    REQUIRE(participation->isProperlyInitialized(), "Participation needs to be properly initialized.");


    const Meeting* mt = getMeeting(participation->getMeetingId());
    REQUIRE(mt, "Could not add participation: The requested meeting does not exist.");
    REQUIRE(!isUserOccupied(participation->getUser(), mt->getDate()), "Could not add participation: This user already participates in another meeting.");


    const Room* rm = getRoom(mt->getRoomId());
    ENSURE(rm, "Something went wrong. Encountered a meeting with invalid room id.");

    const Room* mt_room = getRoom(mt->getRoomId());
    ENSURE(mt_room, "Something went wrong. Found meeting in App but it did not have an assigned room.");

    Participations* ps_by_meeting = _getMutParticipationsByMeeting(participation->getMeetingId());
    ENSURE(ps_by_meeting, "Something went wrong. The list of participations by meeting went out of sync.");
    REQUIRE(ps_by_meeting->size() <= rm->getCapacity(), "Could not add participation: The room the meeting takes place in was full.");

    //REQUIRE(!isRoomOccupied(mt_room->getId(), mt->getDate()), "This rooms is already occupied.");
    REQUIRE(!isUserOccupied(participation->getUser(), mt->getDate()), "This user already participates in another meeting.");



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





const Participations & App::getAllParticipations() const {
    return all_participations;
}

Participations* App::getParticipationsByUser(const std::string &userId) {
    return _getMutParticipationsByUser(userId);
}

Participations* App::getParticipationsByMeeting(const std::string &meetindId) {
    return _getMutParticipationsByMeeting(meetindId);
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



void App::writeMeeting(std::ostream &onStream, const Meeting *meeting) {
    const Date date{meeting->getDate()};
    onStream << "- " << *getRoom(meeting->getRoomId()) << ", " << date.getWeekDay() << " " << date << std::endl;
    onStream << "  " << *meeting << std::endl;
    const Participations* participations = getParticipationsByMeeting(meeting->getId());
    onStream << "  ";
    for (Participation* participation : *participations) {
        onStream << participation->getUser();
        if (participation != participations->back()) onStream << ", ";
    }
    onStream << std::endl;
    onStream << "  Meeting ID: " << meeting->getId() << std::endl;
}

void App::writeRoom(std::ostream &onStream, const Room *room) {
    onStream << "- " << *room << std::endl;
    onStream << "  Capacity: " << room->getCapacity() << " people" << std::endl;
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


