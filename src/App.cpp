//
// Created by Yarni on 2/26/2026.
//

#include "App.h"

#include <filesystem>
#include <queue>
#include <tinyxml.h>
#include "helper/DesignByContract.h"


App::App(Parser* parser, Output* output) : parser(parser), output(output), emission(0.0)
{
    init_check_this_ptr = this;

    ENSURE(isProperlyInitialized(), "object must be property initialized in order to create App.");
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

    for (const CampusElement& c : parser->parsedCampuses())
    {
        if (getCampus(c.id) != nullptr)
        {
            errStream << "Campus ids must be unique: " << c.id << std::endl;
            continue;
        }

        addCampus(new Campus(c.name, c.id));
    }

    for (const BuildingElement& b : parser->parsedBuildings())
    {
        if (getBuilding(b.id) != nullptr)
        {
            errStream << "Building ids must be unique: " << b.id << std::endl;
            continue;
        }

        Campus* c = getCampus(b.campus_id);
        if (c == nullptr)
        {
            errStream << "Building \'" << b.id << "\' is in a campus \'" << b.campus_id << "\', which doesn't exist." << std::endl;
            continue;
        }
        addBuilding(new Building(b.name, b.id, c));
    }

    //> Add all elements in the correct order
    //  Final integrity checks

    for (const RoomElement& r : parser->parsedRooms())
    {
        if (getRoom(r.id) != nullptr)
        {
            errStream << "Room ids must be unique: " << r.id << std::endl;
            continue;
        }

        Building* building = getBuilding(r.building_id);
        if (building == nullptr)
        {
            errStream << "Room \'" << r.id << "\' is in a building \'" << r.building_id << "\', which doesn't exist." << std::endl;
            continue;
        }

        Campus* campus = getCampus(r.campus_id);
        if (campus == nullptr)
        {
            errStream << "Room \'" << r.id << "\' is in a campus \'" << r.campus_id << "\', which doesn't exist." << std::endl;
            continue;
        }

        if (campus != building->getCampus())
        {
            errStream << "Room \'" << r.id << "\' is in a campus \'" << r.campus_id << "\', but it's building \'" << r.building_id
            << "\' is in a different campus \'" << building->getCampus()->getId() << "\'." << std::endl;
            continue;
        }

        addRoom(new Room(r.name, r.id, r.capacity, building));
    }

    for (const CateringElement& c : parser->parsedCaterings())
    {
        Campus* campus = getCampus(c.campus_id);
        if (campus == nullptr)
        {
            errStream << "Catering is for a campus \'" << c.campus_id << "\', which doesn't exist" << std::endl;
            continue;
        }

        // This will automatically add the catering to the correct campus
        caterings.push_back(new Catering(campus, c.co2_count));
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
        //add temporary false status to meeting online status for now as workaround
        addMeeting(new Meeting(m.label, m.id, mr, m.date_time, false, m.externals_allowed, m.catering_needed));
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

        if (p.external && !m->externalsAllowed())
        {
            errStream << "External user \'" << p.user << "\' can't participate in meeting \'" << p.meeting <<
                "\' which doesn't allow externals" << std::endl;
            continue;
        }

        // Get user
        User* u = getUser(p.user);
        // And create user if it doesn't exist yet
        if (u == nullptr)
        {
            u = new User(p.user, p.external);
            addUser(u);
        }

        if (m->getParticipant(p.user) != nullptr)
        {
            errStream << "User with id \'" << p.user << "\' was added twice to the same meeting \'" << p.meeting << "\'." << std::endl;
            continue;
        }


        if (!u->isExternal() && p.external)
        {
            errStream << "User \'" << p.user << "\' was marked as external for meeting \'" << p.meeting
            << "\' but was registered as not-external earlier" << std::endl;
            continue;
        } else if (u->isExternal() && !p.external)
        {
            errStream << "User \'" << p.user << "\' was marked as not-external for meeting \'" << p.meeting
            << "\' but was registered as external earlier" << std::endl;
            continue;
        }



        // Add user to meeting
        m->addParticipant(u);

        // Vice versa is fine too, but not both.
        // Either way each object will recieve a pointer to the other
        // u->addMeeting(m)
    }

    for (const RenovationElement& r : parser->parsedRenovations())
    {
        //Get room
        Room* room = getRoom(r.room);

        if (room == nullptr)
        {
            errStream << "Renovation that starts on " << r.start_date << " and ends on " << r.end_date << " was added to Room \'"
            << r.room << "\' which does not exist." << std::endl;
            continue;
        }

        //TODO: Check if renovations are overlapping

        //Add renovation to the room
        room->addRenovation(r.start_date, r.end_date);

    }

    parser->clearRooms();
    parser->clearMeetings();
    parser->clearParticipations();
    parser->clearRenovations();
}

void App::writeToStream()
{
    REQUIRE(output, "App doesnt have an output attached.");

    output->printMeetings(meetings);
    output->printRooms(rooms);
    output->printBuildings(buildings);
    output->printCampuses(campuses);
    output->printUsers(users);
    output->printMeetingsCO2(meetings);
}

void App::processSingleMeeting(const std::string& meetingId, const bool verbose, std::ostream* catering_planning_output)
{
    //REQUIRE(!meetingId.empty(), "Meeting id is empty");
    Meeting* meeting = getMeetingById(meetingId);
    REQUIRE(meeting, "Meeting must exist.");
    REQUIRE(meeting->isProperlyInitialized(), "Meeting must be properly initialized.");

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
    if (!meeting->isOnline())
    {
        meeting->participantsToRoomsSize.push_back({meeting->getParticipantCount(), meeting->getRoom()->getCapacity()});


        if (meeting->cateringNeeded())
        {
            if (catering_planning_output)
            {
                *catering_planning_output << "Catering for meeting \'" << meeting->toString() << "\' at " << meeting->getDateTime() <<
                " in " << meeting->getRoom()->getCampus()->toString() << ", " << meeting->getRoom()->getBuilding()->toString() << ", "
                << meeting->getRoom()->toString() << "." << std::endl;
            }
        }
    }
    emission += meeting->getEmissions();
    ENSURE(meeting->isCancelled() || meeting->isProcessed(), "Meeting must be processed");
}

void App::processAllMeetings(const bool verbose, std::ostream* catering_planning_output)
{

    std::vector<Meeting*> sortedMeetings;
    for (std::pair<std::string, Meeting*> m : meetings.getRawIdMap())
    {
        sortedMeetings.push_back(m.second);
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
    for (const Meeting* meeting : sortedMeetings)
    {
        ENSURE(meeting, "Meeting can not be null.");
        ENSURE(meeting->isProperlyInitialized(), "Meeting must be properly initialized.");
        processSingleMeeting(meeting->getId(), verbose, catering_planning_output);
    }
    //TODO add ensure
}

void App::addCampus(Campus* campus)
{
    REQUIRE(campus, "The provided campus cannot be null.");
    REQUIRE(campus->isProperlyInitialized(), "Campus must be properly initialized by the constructor.");
    REQUIRE(getCampus(campus->getId()) == nullptr, "Campus id must be unique.");


    campuses.insert({campus->getId(), campus});

    ENSURE(hasCampus(campus), "The campus must be added to the App.");
}

Campus* App::getCampus(const std::string& campusId) const
{
    REQUIRE(!campusId.empty(), "The provided campus id cannot be empty");
    const Campuses::const_iterator& it = campuses.find(campusId);

    if (it == campuses.end()) return nullptr;

    ENSURE(it->second, "Campus must not be null.");
    ENSURE(it->second->getId() == campusId, "Campus must have the right Id.");
    return it->second;
}

bool App::hasCampus(const Campus* campus) const
{
    REQUIRE(campus, "campus cannot be null.");
    return campus && getCampus(campus->getId()) == campus;
}

bool App::hasRoom(const Room* room) const
{
    REQUIRE(room, "room cannot be null.");
    return room && getRoom(room->getId()) == room;
}

bool App::hasMeeting(const Meeting* meeting) const
{
    REQUIRE(meeting, "meeting cannot be null.");
    return meeting && getMeetingById(meeting->getId()) == meeting;
}

bool App::hasUser(const User* user) const
{
    REQUIRE(user, "user cannot be null.");
    return user && getUser(user->getId()) == user;
}

void App::addBuilding(Building* building)
{
    REQUIRE(building, "The provided building cannot be null.");
    REQUIRE(building->isProperlyInitialized(), "Building must be properly initialized by the constructor.");
    REQUIRE(getBuilding(building->getId()) == nullptr, "Building ids must be unique.");
    const Campus* campus = building->getCampus();
    REQUIRE(campus, "Buildings campus can't be null.");
    REQUIRE(hasCampus(campus), "Buildings campus '%s' must be registered first.", campus->getId().c_str());

    buildings.insert({building->getId(), building});

    ENSURE(hasBuilding(building), "The building must be added to the App");
}

Building* App::getBuilding(const std::string& buildingId) const
{
    REQUIRE(!buildingId.empty(), "The provided building id cannot be empty");
    const Buildings::const_iterator& it = buildings.find(buildingId);

    if (it == buildings.end()) return nullptr;

    ENSURE(it->second, "Building must not be null.");
    ENSURE(it->second->getId() == buildingId, "Building must have the right id.");
    return it->second;
}

bool App::hasBuilding(const Building* building) const
{
    REQUIRE(building, "building cannot be null.");
    return building && getBuilding(building->getId()) == building;
}

void App::addRoom(Room* room)
{
    REQUIRE(room, "The provided room cannot be null.");
    REQUIRE(room->isProperlyInitialized(), "Room must be properly initialized by the constructor.");
    REQUIRE(getRoom(room->getId()) == nullptr, "Room id must be unique.");
    const Building* building = room->getBuilding();
    REQUIRE(building, "Rooms building cannot be null.");
    REQUIRE(hasBuilding(building), "Rooms building '%s' must be registered first.", building->getId().c_str());
    const Campus* campus = room->getCampus();
    REQUIRE(campus, "Rooms campus can't be null.");
    REQUIRE(hasCampus(campus), "Rooms campus '%s' must be registered first.", campus->getId().c_str());

    rooms.insert({room->getId(), room}); // Add room

    ENSURE(hasRoom(room), "The room must be added to the App");
}

Room* App::getRoom(const std::string& roomId) const
{
    const Rooms::const_iterator& it = rooms.find(roomId);

    if (it == rooms.end()) return nullptr;

    ENSURE(it->second, "Room must not be null");
    ENSURE(it->second->getId() == roomId, "Room must have the right id.");
    return it->second;
}

const Rooms& App::getAllRooms() const
{
    return rooms;
}


bool App::isRoomOccupied(const std::string& roomId, const DateTime& date_time)
{
    const Room* r = getRoom(roomId);
    REQUIRE(r, "The room must be registered.");
    REQUIRE(date_time.isProperlyInitialized(), "DateTime must be properly initialized");

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
    REQUIRE(m, "This meeting must exist.");

    std::list<Meeting*>& possible_conflicts = meetings.getByDateTime(m->getDateTime());

    for (Meeting* possible_conflict : possible_conflicts)
    {
        ENSURE(possible_conflict->getDateTime() == m->getDateTime(), "Meetings must properly be found by date");
        if (possible_conflict != m &&
            possible_conflict->isProcessed() &&
            (!m->isOnline() && possible_conflict->getRoom() == m->getRoom())
        )
            return possible_conflict;
    }

    return nullptr;
}


void App::addMeeting(Meeting* meeting)
{
    REQUIRE(meeting, "Meeting cannot be null.");
    const Room* room = meeting->getRoom();
    REQUIRE(room, "Meetings room cannot be null.");
    REQUIRE(hasRoom(room), "Meetings room '%s' must be registered first.", room->getId().c_str());
    if (meeting->getOrder() == 0)
    {
        int order = meetings.getRawIdMap().size() + 1;
        meeting->setOrder(order);
        //std::cout << "Set order of meeting " << meeting->getId() << "  to " << order << std::endl;
    }
    meetings.add(meeting);
    ENSURE(hasMeeting(meeting), "Meeting must be added to the App.");
}


Meeting* App::getMeetingById(const std::string& meetingId) const
{
    return meetings.getById(meetingId);
}

std::list<Meeting*>& App::getMeetingsByDateTime(const DateTime& meetingDateTime)
{
    //ENSURE(meetings.getRawDateMap().contains(meetingDateTime), "DateTime must exist in MeetingRegistry.");
    return meetings.getByDateTime(meetingDateTime);
}

const MeetingRegistry& App::getMeetingRegistry() const
{
    return meetings;
}


void App::addUser(User* user)
{
    REQUIRE(user != nullptr, "User can not be null.");
    REQUIRE(user->isProperlyInitialized(), "User must be properly initialized.");
    REQUIRE(getUser(user->getId()) == nullptr, "User ids must be unique.");

    users.insert({user->getId(), user});

    REQUIRE(hasUser(user), "User must be added to the App.");
}

User* App::getUser(const std::string& userId) const
{
    const Users::const_iterator& it = users.find(userId);

    if (it == users.end()) return nullptr;

    ENSURE(it->second, "User must not be null.");
    ENSURE(it->second->getId() == userId, "User must have the correct id.");
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
    REQUIRE(m, "This meeting must exist: '%s'", meetingId.c_str());
    REQUIRE(u, "This user must exist: '%s'", userId.c_str());

    m->addParticipant(u);

    ENSURE(m->getParticipant(userId) == u, "Participant '%s' must be added to meeting '%s'.", userId.c_str(), meetingId.c_str());
    ENSURE(u->getMeetingById(meetingId) == m, "Meeting '%s' must be added to user '%s'.", meetingId.c_str(), userId.c_str());
}


bool App::isUserOccupied(const std::string& userId, const DateTime& date_time)
{
    const User* u = getUser(userId);
    REQUIRE(u, "User must be registered.");
    REQUIRE(date_time.isProperlyInitialized(), "DateTime must be properly initialized");

    std::list<Meeting*>& possible_occupations = getMeetingsByDateTime(date_time);

    for (Meeting* possible_occupation : possible_occupations)
    {
        if (possible_occupation == nullptr || !possible_occupation->isProcessed()) continue;

        const User* has_participant = possible_occupation->getParticipant(userId);
        REQUIRE(has_participant == nullptr || has_participant == u, "User found must be the correct one");

        if (has_participant == nullptr) continue;

        return true;
    }

    return false;
}


App::~App()
{
    for (const std::pair<const std::string, Campus*>& c : campuses ) delete c.second;
    for (const std::pair<const std::string, Building*>& b : buildings ) delete b.second;
    for (const std::pair<const std::string, Room*>& r : rooms) delete r.second;

    for (const std::pair<const std::string, User*>& u : users ) delete u.second;
    for (const Catering* c : caterings) delete c;

    for (const std::pair<const std::string, Meeting*>& m : meetings.getRawIdMap()) delete m.second;

    delete parser;
    delete output;
}