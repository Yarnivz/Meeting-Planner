//
// Created by Yarni on 2/26/2026.
//

#ifndef MEETING_PLANNER_APP_H
#define MEETING_PLANNER_APP_H

#include <string>

#include "objects/MeetingRegistry.h"
#include "objects/Room.h"
#include "objects/Meeting.h"
#include "parser/Parser.h"
#include "output/Output.h"
#include "objects/User.h"
#include "objects/Catering.h"
#include "TypeDefs.h"

class Catering;

class App
{
public:
    /**
     * @brief Constructor of the App class.
     */
    App(Parser* parser = nullptr, Output* output = nullptr);

    /**
     * @brief Checks whether the App was properly initialized by the constructor.
     * @return bool indicating result
     */
    bool isProperlyInitialized() const;

    /**
    * @brief Parse an xml file, and registers all items specified in that file.
    *
    * Read in and parse an xml file.
    * All meetings, rooms and participations specified in the xml file are registered in the app.
    *
    * If there is an error with a single item, the parser will attempt to recover and skip over that item.
    * If an unrecoverable error occurs, the parser exits without changing anything.
    *
    * @param filename of the .xml file to parse
    * @param errStream stream to print errors to in case they appear. Defaults to the standard console error stream.
    */

    //TODO: Delete errStream and use parser errStream
    void parseFile(const std::string& filename, std::ostream& errStream = std::cerr);

    /**
     * @brief Print all meetings and rooms to a stream.
     *
     * Prints all registered meetings and rooms to the app's output.
     *
     *
     */
    void writeToStream();

    /**
     * @brief Registers a Campus to the app.
     *
     * Registers a new 'Campus' object.
     * The App class expects ownership of the Campus pointer passed in
     *
     *
     * @param campus to register
     *
     * @post The campus was added to the App.
     */
    void addCampus(Campus* campus);

    /**
     * @brief Retrieve a registered Campus based on its id.
     *
     *
     * @param campusId Identifier of the campus to retrieve
     * @return a pointer to the room with given id if it exists, nullptr otherwise
     *
     * @post The found campus must have the right id.
     */
    Campus* getCampus(const std::string& campusId) const;

    /**
     * @brief Checks whether the Apps campus map contains the corresponding campus
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
     * @param building to register
     *
     * @post The building was added to the App.
     */
    void addBuilding(Building* building);

    /**
     * @brief Retrieve a registered Building based on its id.
     *
     *
     * @param buildingId Identifier of the building to retrieve
     * @return a pointer to the room with given id if it exists, nullptr otherwise
     *
     * @post The found building must have the right id.
     */
    Building* getBuilding(const std::string& buildingId) const;

    /**
     * @brief Checks whether the Apps buildings map contains the corresponding building
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
     * @param room to register
     *
     * @post The room was added to the App
     */
    void addRoom(Room* room);

    /**
     * @brief Retrieve a registered room based on its id.
     *
     * @param roomId Identifier of the room to retrieve
     */
    Room* getRoom(const std::string& roomId) const;

    /**
     * @brief Checks whether the Apps rooms map contains the corresponding room
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
     *
     *
     * @param meetingId Identifier of the meeting to check
     * @return pointer to the conflicting meeting; nullptr if no conflict exists
     *
     * @post Something went wrong. Looking meetings up by date failed.
     */
    Meeting* findConflictingMeeting(const std::string& meetingId);


    /**
     * @brief Register and plan a meeting.
     *
     * Register and plan a new meeting.
     * The App expects ownership of the Meeting* passed in.
     *
     * @param meeting The meeting to plan and register
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
     */
    void addUser(User* user);
    /**
     * @brief Gets the user that corresponds to the id from the Apps map
     * @param userId the users id
     *
     *
     * @return the user itself
     */
    User* getUser(const std::string& userId) const;
    bool hasUser(const User* user) const;
    const Users& getAllUsers() const;


    /**
     * @brief Adds the user that corresponds to the user id to the meeting that corresponds to the meeting id
     *
     * @param userId The id of the user
     * @param meetingId The id of the meeting
     *
     * @post Something went wrong. The participant '%s' was not added to meeting '%s'.
     * @post Something went wrong. The meeting '%s' was not added to user '%s'.
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
    ~App();

private:

    Parser* parser;
    Output* output;

    Campuses campuses;
    Buildings buildings;
    Rooms rooms;
    MeetingRegistry meetings;
    Users users;
    std::list<Catering*> caterings;
    float emission;


    void* init_check_this_ptr = nullptr;
};


#endif //MEETING_PLANNER_APP_H
