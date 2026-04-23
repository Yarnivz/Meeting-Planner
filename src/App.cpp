//
// Created by Yarni on 2/26/2026.
//

#include "App.h"

#include <filesystem>
#include <queue>
#include <tinyxml.h>
#include "helper/DesignByContract.h"
#include "parser/Parser.h"


App::App(Parser* parser, std::ostream* output) : parser(parser), output(output)
{
    init_check_this_ptr = this;
}

bool App::isProperlyInitialized() const
{
    return init_check_this_ptr == this;
}


void App::parseFile(const std::string& filename, std::ostream& errStream)
{
    if (!parser)
    {
        errStream << "This App does not have a parser!" << std::endl;
        return;
    }


    parser->parse(filename);


    //> Add all elements in the correct order
    //  Final integrity checks

    for (const RoomElement& r : parser->parsedRooms())
    {
        if (getRoom(r.id) != nullptr)
        {
            errStream << "Room ids must be unique: " << r.id << std::endl;
            continue;
        }

        //TODO add buildings to app?
        addRoom(new Room(r.name, r.id, r.capacity));
    }

    for (const MeetingElement& m : parser->parsedMeetings())
    {
        if (getMeetingById(m.id) != nullptr)
        {
            errStream << "Meeting ids must be unique: " << m.id << std::endl;
            continue;
        }

        Room* mr = getRoom(m.room_id);
        if (mr == nullptr)
        {
            errStream << "Meeting " << m.id << " takes place in a room \'" << m.room_id <<
                "\' which doesnt exist." << std::endl;
            continue;
        }

        addMeeting(new Meeting(m.label, m.id, mr, m.date_time, m.externals_allowed));
    }

    for (const ParticipationElement& p : parser->parsedParticipations())
    {
        // Get meeting
        Meeting* m = getMeetingById(p.meeting);
        if (m == nullptr)
        {
            errStream << "User \'" << p.user << "\' participates in a meeting \'" << p.meeting <<
                "\' which doesnt exist." << std::endl;
            continue;
        }

        if ( p.external && !m->externalsAllowed())
        {
            errStream << "External user \'" << p.user << "\' can't participate in meeting \'" << p.meeting <<
                "\' which doesn't allow externals";
        }

        // Get user
        User* u = getUser(p.user);
        // And create user if it doesn't exist yet
        if (u == nullptr)
        {
            u = new User(p.user, p.external);
            addUser(u);
        }

        // Add user to meeting
        m->addParticipant(u);

        // Vice versa is fine too, but not both.
        // Either way each object will recieve a pointer to the other
        // u->addMeeting(m)


        ENSURE(getUser(p.user) == u, "Something went wrong. User does not exist and wasnt created");
        ENSURE(getMeetingById(p.meeting) == m, "Something went wrong. Meeting wasnt added.");
        ENSURE(m->getParticipant(p.user) == u, "Something went wrong. User was not added as participant of meeting");
        ENSURE(u->getMeetingById(p.meeting) == m, "Something went wrong. Meeting was not added to user.");
    }


    parser->clearRooms();
    parser->clearMeetings();
    parser->clearParticipations();
}

void App::writeToStream()
{
    REQUIRE(output, "App doesnt have an output attached.");

    std::list<const Meeting*> cancelled, processed, unprocessed;

    //Sort by state
    for (const std::pair<const std::string, Meeting*>& item : meetings.getRawIdMap())
    {
        const Meeting* m = item.second;
        if (m->isUnProcessed())
        {
            unprocessed.push_back(m);
        }
        else if (m->isProcessed())
        {
            processed.push_back(m);
        }
        else if (m->isCancelled())
        {
            cancelled.push_back(m);
        }
    }

    //Write all past meetings
    if (!processed.empty()) *output << std::endl << "Past meetings:" << std::endl;
    for (const Meeting* m : processed)
    {
        writeMeeting(*output, m);
    }

    //Write all future meetings
    if (!unprocessed.empty()) *output << std::endl << "Future meetings:" << std::endl;
    for (const Meeting* m : unprocessed)
    {
        writeMeeting(*output, m);
    }

    //Write all conflicts
    if (!cancelled.empty()) *output << std::endl << "Conflicts:" << std::endl;
    for (const Meeting* m : cancelled)
    {
        writeMeeting(*output, m);
        *output << "  Reason: " << m->getCancellationReason() << std::endl;
    }

    //Write all rooms
    if (!rooms.empty()) *output << std::endl << "Rooms:" << std::endl;
    for (const std::pair<std::string, Room*> item : rooms)
    {
        const Room* room = item.second;
        writeRoom(*output, room);
    }
}

void App::processSingleMeeting(const std::string& meetingId, const bool verbose)
{
    //REQUIRE(!meetingId.empty(), "Meeting id is empty");
    Meeting* meeting = getMeetingById(meetingId);
    REQUIRE(meeting, "This meeting doesn't exist.");
    REQUIRE(meeting->isProperlyInitialized(), "Meeting needs to be properly initialized.");

    Meeting* conflict;
    if ((conflict = findConflictingMeeting(meetingId)))
    {
        meeting->cancel("conflict with meeting " + conflict->getId());
        if (verbose) std::cout << meeting->getId() << " has been cancelled due to '" + meeting->getCancellationReason() << "'" << std::endl;
    }
    else
    {
        meeting->process();
        if (verbose) std::cout << meeting->getId() << " has taken place" << std::endl;
    }
    participantsToRoomsSize.push_back({meeting->getParticipantCount(), meeting->getRoom()->getCapacity()});

    ENSURE(meeting->isCancelled() || meeting->isProcessed(), "Meeting hasn't been processed");
}

void App::processAllMeetings(const bool verbose)
{
    std::vector<Meeting*> sortedMeetings;
    for (std::pair<std::string, Meeting*> m : meetings.getRawIdMap())
    {
        if (m.second->getDateTime() <= DateTime()) sortedMeetings.push_back(m.second);
    }
    std::sort(sortedMeetings.begin(), sortedMeetings.end(),
              [](const Meeting* comparedMeeting1, const Meeting* comparedMeeting2)
              {
                  if (comparedMeeting1->getDateTime() != comparedMeeting2->getDateTime())
                  {
                      return comparedMeeting1->getDateTime() < comparedMeeting2->getDateTime();
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

    //TODO: fix this
    ENSURE(true, "Not all meetings have been processed");
}

void App::addRoom(Room* room)
{
    REQUIRE(room, "The provided room cannot be null.");
    REQUIRE(room->isProperlyInitialized(), "Room needs to be properly initialized by the constructor.");
    REQUIRE(!rooms.contains(room->getId()), "Room id has to be unique.");

    rooms.insert({room->getId(), room}); // Add room

    ENSURE(getRoom(room->getId()) == room, "The room was not added to the App");
}

Room* App::getRoom(const std::string& roomId)
{
    const Rooms::iterator it = rooms.find(roomId);

    if (it == rooms.end()) return nullptr;

    ENSURE(it->second->getId() == roomId, "Something went wrong. The room which was found did not have the right id.");
    return it->second;
}

const Rooms& App::getAllRooms() const
{
    return rooms;
}


bool App::isRoomOccupied(const std::string& roomId, const DateTime& date_time)
{
    const Room* r = getRoom(roomId);
    REQUIRE(r, "This room does not exist.");

    std::list<Meeting*>& possible_occupations = getMeetingsByDateTime(date_time);

    for (Meeting* possible_occupation : possible_occupations)
    {
        if (possible_occupation != nullptr && possible_occupation->isProcessed() && possible_occupation->getRoom() == r) return true;
    }

    return false;
}

Meeting* App::findConflictingMeeting(const std::string& meetingId)
{
    Meeting* m = getMeetingById(meetingId);
    REQUIRE(m, "This meeting doesn't exist.");

    std::list<Meeting*>& possible_conflicts = meetings.getByDateTime(m->getDateTime());

    for (Meeting* possible_conflict : possible_conflicts)
    {
        ENSURE(possible_conflict->getDateTime() == m->getDateTime(), "Something went wrong. Looking meetings up by date failed.");
        if (possible_conflict != m &&
            possible_conflict->isProcessed() &&
            possible_conflict->getRoom() == m->getRoom()
        )
            return possible_conflict;
    }

    return nullptr;
}


void App::addMeeting(Meeting* meeting)
{
    if (meeting->getOrder() == 0)
    {
        int order = meetings.getRawIdMap().size() + 1;
        meeting->setOrder(order);
        //std::cout << "Set order of meeting " << meeting->getId() << "  to " << order << std::endl;
    }
    meetings.add(meeting);
}


Meeting* App::getMeetingById(const std::string& meetingId)
{
    return meetings.getById(meetingId);
}

std::list<Meeting*>& App::getMeetingsByDateTime(const DateTime& meetingDateTime)
{
    return meetings.getByDateTime(meetingDateTime);
}

const MeetingRegistry& App::getMeetingRegistry() const
{
    return meetings;
}


void App::addUser(User* user)
{
    REQUIRE(user != nullptr, "User can not be null");
    REQUIRE(user->isProperlyInitialized(), "User needs to be properly initialized.");

    users.insert({user->getId(), user});

    REQUIRE(getUser(user->getId()) == user, "Something went wrong. User was not added.");
}

User* App::getUser(const std::string& userId)
{
    const Users::iterator it = users.find(userId);

    if (it == users.end()) return nullptr;

    ENSURE(it->second->getId() == userId, "Something went wrong, The user which was found did not have the correct id.");
    return it->second;
}

const Users& App::getAllUsers() const
{
    return users;
}

void App::addUserToMeeting(const std::string& userId, const std::string& meetingId)
{
    Meeting* m = getMeetingById(meetingId);
    User* u = getUser(userId);
    REQUIRE(m, "This meeting doesn't exist: '%s'", meetingId.c_str());
    REQUIRE(u, "This user doesn't exist: '%s'", userId.c_str());

    m->addParticipant(u);

    ENSURE(m->getParticipant(userId) == u, "Something went wrong. The participant '%s' was not added to meeting '%s'.", userId.c_str(), meetingId.c_str());
    ENSURE(u->getMeetingById(meetingId) == m, "Something went wrong. The meeting '%s' was not added to user '%s'.", meetingId.c_str(), userId.c_str());
}


bool App::isUserOccupied(const std::string& userId, const DateTime& date_time)
{
    const User* u = getUser(userId);
    REQUIRE(u, "This user does not exist.");

    std::list<Meeting*>& possible_occupations = getMeetingsByDateTime(date_time);

    for (Meeting* possible_occupation : possible_occupations)
    {
        if (possible_occupation == nullptr || !possible_occupation->isProcessed()) continue;

        const User* has_participant = possible_occupation->getParticipant(userId);
        REQUIRE(has_participant == nullptr || has_participant == u, "Something went wrong. The user which was found was not correct.");

        if (has_participant == nullptr) continue;

        return true;
    }

    return false;
}


App::~App()
{
    for (const std::pair<const std::string, Room*>& r : rooms) delete r.second;
    for (const std::pair<const std::string, Meeting*>& m : meetings.getRawIdMap()) delete m.second;
    for (const std::pair<const std::string, User*>& u : users) delete u.second;
}


void App::writeMeeting(std::ostream& onStream, const Meeting* meeting)
{
    const DateTime& date_time = meeting->getDateTime();

    onStream << "- " << *(meeting->getRoom()) << ", " << date_time.getWeekDay() << " " << date_time << std::endl;
    onStream << "  " << *meeting << std::endl;

    onStream << "  ";
    for (Users::const_iterator it = meeting->getParticipants().begin(); it != meeting->getParticipants().end(); ++it)
    {
        const User* participant = it->second;
        onStream << participant->getId();
        if (std::next(it) != meeting->getParticipants().end()) onStream << ", ";
    }
    onStream << std::endl;
    onStream << "  Meeting ID: " << meeting->getId() << std::endl;
}

void App::writeRoom(std::ostream& onStream, const Room* room)
{
    onStream << "- " << *room << std::endl;
    onStream << "  Capacity: " << room->getCapacity() << " people" << std::endl;
}

// Meetings* App::_getMutMeetingsByRoom(const std::string& roomId)
// {
//     const MeetingsByRoomMap::iterator it = meetings_by_room.find(roomId);
//
//     if (it == meetings_by_room.end()) return nullptr;
//
//     return &it->second;
// }
