//
// Created by Yarni on 2/26/2026.
//

#ifndef MEETING_PLANNER_APP_H
#define MEETING_PLANNER_APP_H

#include <string>
#include <fstream>

#include "objects/MeetingRegistry.h"
#include "objects/Room.h"
#include "objects/Meeting.h"
#include "parser/Parser.h"
#include "output/Output.h"
#include "objects/User.h"
#include "objects/Catering.h"
#include "TypeDefs.h"
#include "error/Error.h"

class Catering;

class App
{
public:
    /**
     * @brief Constructor of the App class.
     */
    App();

    /**
     * @brief Checks whether the App was properly initialized by the constructor.
     * @return bool indicating result
     */
    bool isProperlyInitialized() const;

    void parse(Parser&& parser, const Error& error);

    /**
    * @brief Parse an xml file, and registers all items specified in that file.
    *
    * Read in and parse an xml file.
    * All meetings, rooms and participations specified in the xml file are registered in the app.
    *
    * If there is an error with a single item, the parser will attempt to recover and skip over that item.
    * If an unrecoverable error occurs, the parser exits without changing anything.
    *
    * @param parser parser to use
    * @param error
    */
    void parse(Parser& parser, const Error& error);

    /**
     * @brief Print all meetings and rooms to a stream.
     *
     * Prints all registered meetings and rooms to the app's output.
     *
     *
     *
     * @pre REQUIRE(output, "App doesnt have an output attached.")
     *
     */
    void write(const Output& output) const;

    /**
     * @brief Registers a Campus to the app.
     *
     * Registers a new 'Campus' object.
     * The App class expects ownership of the Campus pointer passed in
     *
     *
     *
     * @pre REQUIRE(campus, "The provided campus cannot be null.")
     * @pre REQUIRE(campus->isProperlyInitialized(), "Campus must be properly initialized by the constructor.")
     * @pre REQUIRE(getCampus(campus->getId()) == nullptr, "Campus id must be unique.")
     *
     * @param campus to register
     *
     * @post ENSURE(hasCampus(campus), "The campus must be added to the App.")
     */
    void addCampus(Campus* campus);

    /**
     * @brief Retrieve a registered Campus based on its id.
     *
     *
     *
     * @pre REQUIRE(!campusId.empty(), "The provided campus id cannot be empty")
     *
     * @param campusId Identifier of the campus to retrieve
     * @post ENSURE(it->second, "Campus must not be null.")
     * @post ENSURE(it->second->getId() == campusId, "Campus must have the right Id.")
     */
    Campus* getCampus(const std::string& campusId) const;

    /**
     * @brief Checks whether the Apps campus map contains the corresponding campus
     *
     * @pre REQUIRE(campus, "campus cannot be null.")
     *
     * @param campus The campus to be found
     * @return bool indicating result
     */
    bool hasCampus(const Campus* campus) const;

    /**
     * @brief Registers a Building to the app.
     *
     * Registers a new 'Building' object.
     * The App class expects ownership of the Building pointer passed in
     *
     *
     *
     * @pre REQUIRE(building, "The provided building cannot be null.")
     * @pre REQUIRE(building->isProperlyInitialized(), "Building must be properly initialized by the constructor.")
     * @pre REQUIRE(getBuilding(building->getId()) == nullptr, "Building ids must be unique.")
     * @pre REQUIRE(campus, "Buildings campus can't be null.")
     * @pre REQUIRE(hasCampus(campus), "Buildings campus '%s' must be registered first.", campus->getId().c_str())
     *
     * @param building to register
     *
     * @post ENSURE(hasBuilding(building), "The building must be added to the App")
     */
    void addBuilding(Building* building);

    /**
     * @brief Retrieve a registered Building based on its id.
     *
     *
     *
     * @pre REQUIRE(!buildingId.empty(), "The provided building id cannot be empty")
     *
     * @param buildingId Identifier of the building to retrieve
     * @post ENSURE(it->second, "Building must not be null.")
     * @post ENSURE(it->second->getId() == buildingId, "Building must have the right id.")
     */
    Building* getBuilding(const std::string& buildingId) const;

    /**
     * @brief Checks whether the Apps buildings map contains the corresponding building
     *
     * @pre REQUIRE(building, "building cannot be null.")
     *
     * @param building The building to be found
     * @return bool indicating result
     */
    bool hasBuilding(const Building* building) const;
    /**
     * @brief Register a room.
     *
     * Registers a new 'Room' object.
     * The App class expects ownership of the Room pointer passed in.
     *
     *
     *
     *
     * @pre REQUIRE(room, "The provided room cannot be null.")
     * @pre REQUIRE(room->isProperlyInitialized(), "Room must be properly initialized by the constructor.")
     * @pre REQUIRE(getRoom(room->getId()) == nullptr, "Room id must be unique.")
     * @pre REQUIRE(building, "Rooms building cannot be null.")
     * @pre REQUIRE(hasBuilding(building), "Rooms building '%s' must be registered first.", building->getId().c_str())
     * @pre REQUIRE(campus, "Rooms campus can't be null.")
     * @pre REQUIRE(hasCampus(campus), "Rooms campus '%s' must be registered first.", campus->getId().c_str())
     *
     * @param room to register
     *
     * @post ENSURE(hasRoom(room), "The room must be added to the App")
     */
    void addRoom(Room* room);

    /**
     * @brief Retrieve a registered room based on its id.
     *
     * @param roomId Identifier of the room to retrieve
     *
     * @post ENSURE(it->second, "Room must not be null")
     * @post ENSURE(it->second->getId() == roomId, "Room must have the right id.")
     *
     * @return a pointer to the room with given id if it exists, nullptr otherwise
     */
    Room* getRoom(const std::string& roomId) const;

    /**
     * @brief Checks whether the Apps rooms map contains the corresponding room
     *
     * @pre REQUIRE(room, "room cannot be null.")
     *
     * @param room The room to be found
     * @return bool indicating result
     */
    bool hasRoom(const Room* room) const;

    /**
     * @brief Retrieve a map of all registered rooms
     *
     * Retrieves a map of all registered rooms.
     * With key == room-id and value == room-pointer
     *
     * @return a map of all registered rooms
     */
    const Rooms& getAllRooms() const;

    /**
     * @brief Check if the room with given id is occupied by another meeting, on the given date.
     *
     * Checks if the room with given id is occupied by another meeting, on the given date.
     *
     * A room is occupied if:
     *   -A meeting takes place in that room.
     *   -That meeting takes place on the given date
     *   -The meeting was registered as 'done' by the meeting processor
     *
     *
     *
     *
     * @pre REQUIRE(r, "The room must be registered.")
     * @pre REQUIRE(date_time.isProperlyInitialized(), "DateTime must be properly initialized")
     *
     * @param roomId Identifier of the room to check
     * @param date_time Date to check
     * @return bool indicating the result
     */
    bool isRoomOccupied(const std::string& roomId, const DateTime& date_time);

    /**
     * @brief Find a meeting which would conflict with the given meeting.
     *
     * A meeting would conflict iff:
     *    -Another meeting takes place on the same date, in the same room
     *    -That meeting was registered as 'done"
     *
     * @pre REQUIRE(m, "This meeting must exist.")
     *
     * @param meetingId Identifier of the meeting to check
     *
     * @post ENSURE(possible_conflict->getDateTime() == m->getDateTime(), "Meetings must properly be found by date")
     *
     * @return pointer to the conflicting meeting; nullptr if no conflict exists
     */
    Meeting* findConflictingMeeting(const std::string& meetingId);


    /**
     * @brief Register and plan a meeting.
     *
     * Register and plan a new meeting.
     * The App expects ownership of the Meeting* passed in.
     *
     *
     * @pre REQUIRE(meeting, "Meeting cannot be null.")
     * @pre REQUIRE(room, "Meetings room cannot be null.")
     * @pre REQUIRE(hasRoom(room), "Meetings room '%s' must be registered first.", room->getId().c_str())
     *
     * @param meeting The meeting to plan and register
     *
     * @post ENSURE(hasMeeting(meeting), "Meeting must be added to the App.")
     */
    void addMeeting(Meeting* meeting);

    /**
     * @brief Retrieve a meeting based on its id.
     *
     * @param meetingId of the meeting to retrieve
     * @return a pointer to the meeting; nullptr if the meeting was not found.
     */
    Meeting* getMeetingById(const std::string& meetingId) const;
    std::list<Meeting*>& getMeetingsByDateTime(const DateTime& meetingDateTime);

    /**
     *
     * @pre REQUIRE(meeting, "meeting cannot be null.")
     *
     */
    bool hasMeeting(const Meeting* meeting) const;

    /**
     * @brief Gets the MeetingRegistry from the App
     * @return the MeetingRegistry itself
     */
    const MeetingRegistry& getMeetingRegistry() const;


    /**
     * @brief add the user to the apps map of users
     *
     *
     * @pre REQUIRE(user != nullptr, "User can not be null.")
     * @pre REQUIRE(user->isProperlyInitialized(), "User must be properly initialized.")
     * @pre REQUIRE(getUser(user->getId()) == nullptr, "User ids must be unique.")
     * @pre REQUIRE(hasUser(user), "User must be added to the App.")
     *
     * @param user The user to be added
     *
     */
    void addUser(User* user);
    /**
     * @brief Gets the user that corresponds to the id from the Apps map
     * @param userId the users id
     *
     *
     *
     * @post ENSURE(it->second, "User must not be null.")
     * @post ENSURE(it->second->getId() == userId, "User must have the correct id.")
     *
     * @return the user itself
     */
    User* getUser(const std::string& userId) const;

    /**
     *
     * @pre REQUIRE(user, "user cannot be null.")
     *
     */
    bool hasUser(const User* user) const;
    const Users& getAllUsers() const;


    /**
     * @brief Adds the user that corresponds to the user id to the meeting that corresponds to the meeting id
     *
     *
     * @pre REQUIRE(m, "This meeting must exist: '%s'", meetingId.c_str())
     * @pre REQUIRE(u, "This user must exist: '%s'", userId.c_str())
     *
     * @param userId The id of the user
     * @param meetingId The id of the meeting
     *
     * @post ENSURE(m->getParticipant(userId) == u, "Participant '%s' must be added to meeting '%s'.", userId.c_str(), meetingId.c_str())
     * @post ENSURE(u->getMeetingById(meetingId) == m, "Meeting '%s' must be added to user '%s'.", meetingId.c_str(), userId.c_str())
     */
    void addUserToMeeting(const std::string& userId, const std::string& meetingId);

    /**
     * @brief Check if the user with given id is occupied by another meeting, on the given date.
     *
     * Checks if the user with given id is occupied by another meeting, on the given date.
     *
     * A user is occupied iff:
     *   -The user participates in some meeting.
     *   -That meeting takes place on the given date
     *   -The meeting was registered as 'done' by the meeting processor
     *
     *
     *
     *
     *
     * @pre REQUIRE(u, "User must be registered.")
     * @pre REQUIRE(date_time.isProperlyInitialized(), "DateTime must be properly initialized")
     * @pre REQUIRE(has_participant == nullptr || has_participant == u, "User found must be the correct one")
     *
     * @param userId Identifier of the user to check
     * @param date_time Date to check
     * @return bool indicating the result
     */
    bool isUserOccupied(const std::string& userId, const DateTime& date_time);

    /**
     *@brief Check this planned meeting for conflicting rooms/dates.
     *
     * Retrieves a meeting using its Id and uses it check whether other planned meetings of the same room/date conflicts with this one and cancels/plans it accordingly.
     *
     * @param meetingId Identifier of the meeting to retrieve.
     * @param verbose Prints text to console when enabled. This option is enabled by default.
     */
    void processSingleMeeting(const std::string& meetingId, bool verbose = true, std::ostream* catering_planning_output = nullptr);

    /**
     * @brief Checks all planned meeting entries for conflicting rooms/dates.
     *
     * Temporarily sorts all meetings by date (or order if the dates are the same) then runs @ref processSingleMeeting for each meeting.
     *
     * @param verbose Prints text to console when enabled. This option is enabled by default.
     */
    void processAllMeetings(bool verbose = true, std::ostream* catering_planning_output = nullptr);

    void dotOutput();
    ~App();

private:

    //Parser* parser;
    //Output* output;

    Campuses campuses;
    Buildings buildings;
    Rooms rooms;
    MeetingRegistry meetings;
    Users users;
    std::list<Catering*> caterings;
    float emission = 0.0f;


    void* init_check_this_ptr = nullptr;
};


#endif //MEETING_PLANNER_APP_H
