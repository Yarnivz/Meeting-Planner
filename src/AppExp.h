//
// Created by Yarni on 2/26/2026.
//

#ifndef MEETING_PLANNER_APP_H
#define MEETING_PLANNER_APP_H
#include <list>
#include <string>
#include <unordered_map>

#include "objects/Room.h"
#include "objects/Meeting.h"
#include "parser/Parser.h"
#include "objects/Participation.h"

using Meetings = std::unordered_map<std::string, Meeting*>;
using Rooms = std::unordered_map<std::string, Room*>;
using Participations = std::list<Participation*>;


using MeetingsByRoomMap = std::unordered_map<std::string, Meetings>;
using ParticipationsByUserMap = std::unordered_map<std::string, Participations>;
using ParticipationsByMeetingMap = std::unordered_map<std::string, Participations>;

class App
{
public:
    /**
     * @brief Constructor of the App class.
     */
    App(Parser* parser = nullptr, std::ostream* output = nullptr);

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
     * @pre App doesnt have an output attached.
     * 
     */
    void writeToStream();

    /**
     * @brief Register a room.
     *
     * Registers a new 'Room' object.
     * The App class expects ownership of the Room pointer passed in.
     *
     * 
     * @pre The provided room cannot be null.
     * @pre Room needs to be properly initialized by the constructor.
     * @pre Room id has to be unique.
     * 
     * @param room to register
     * 
     * @post The room was not added to the App
     */
    void addRoom(Room* room);

    /**
     * @brief Retrieve a registered room based on its id.
     *
     * @param roomId of the room to retrieve
     * @return a pointer to the room with given id if it exists, nullptr otherwise
     * 
     * @post Something went wrong. The room which was found did not have the right id.
     */
    Room* getRoom(const std::string& roomId);

    /**
     * @brief Retrieve a map of all registered rooms
     *
     * Retrieves a map of all registered rooms.
     * With key == room-id and value == room-pointer
     *
     * @return a map of all registered rooms
     * 
     * @post rooms contains no room
     */
    const Rooms& getAllRooms() const;

    /**
     * @brief Check if the room with given id is occupied by another meeting, on the given date.
     *
     * Checks if the room with given id is occupied by another meeting, on the given date.
     *
     * A room is occupied iff:
     *   -A meeting takes place in that room.
     *   -That meeting takes place on the given date
     *   -The meeting was registered as 'done' by the meeting processor
     *
     * 
     * @pre This room doesnt exist.
     * 
     * @param roomId of the room to check
     * @param date to check
     * @return bool indicating the result
     */
    bool isRoomOccupied(const std::string& roomId, const Date& date);

    /**
     * @brief Find a meeting which would conflict with the given meeting.
     *
     * A meeting would conflict iff:
     *    -Another meeting takes place on the same date, in the same room
     *    -That meeting was registered as 'done"
     *
     * 
     * @pre This meeting doesn't exist.
     * 
     * @param meetingId of the meeting to check
     * @return pointer to the conflicting meeting; nullptr if no conflict exists
     * 
     * @post Encountered a meeting with a roomId which doesnt exist.
     */
    Meeting* findConflictingMeeting(const std::string& meetingId);


    /**
     * @brief Register and plan a meeting.
     *
     * Register and plan a new meeting.
     * The App expects ownership of the Meeting* passed in.
     *
     * 
     * @pre Meeting can not be null.
     * @pre Meeting needs to be properly initialized.
     * @pre Meetings Id needs to be unique!
     * @pre The meeting has to take place in a room which was already registered.
     * @pre     // REQUIRE(
     * 
     * @param meeting to plan and register
     * 
     * @post Something went wrong, the meeting list was not found.
     * @post Something went wrong, The meeting was not added to the App
     * @post Something went wrong, the meeting was not added to the App
     */
    void addMeeting(Meeting* meeting);

    /**
     * @brief Retrieve a meeting based on its id and the id of the room it takes place in.
     *
     * @param meetingId of the meeting to retrieve
     * @param roomId of the room the meeting takes place in
     * @return a pointer to the correct meeting. Returns nullptr if the meeting was not found for that room.
     * 
     * @post Something went wrong, The meeting which was found did not have the correct id.
     */
    Meeting* getMeetingInRoom(const std::string& meetingId, const std::string& roomId);

    /**
     * @brief Retrieve a meeting based on its id.
     *
     * @param meetingId of the meeting to retrieve
     * @return a pointer to the meeting; nullptr if the meeting was not found.
     * 
     * @post Something went wrong, The meeting which was found did not have the correct id.
     */
    Meeting* getMeeting(const std::string& meetingId);

    /**
     * @brief Retrieve a map of all registered meetings
     *
     * Retrieves a map of all registered meetings.
     * With key == meeting-id and value == meeting-pointer
     *
     * @return a map of all registered meetings
     * 
     * @post all_meetings contains no meetings
     */
    const Meetings& getAllMeetings() const;


    /**
     * @brief Retrieve a map of all registered meetings in a given room
     *
     * Retrieves a map of all registered meetings that take place in the given room.
     * With key == meeting-id and value == meeting-pointer.
     *
     * @param roomId of the room to search
     * @return a map of all registered meetings
     * 
     * @post Something went wong while going through the meetings.
     */
    const Meetings* getMeetingsByRoom(const std::string& roomId);


    /**
     * @brief Retrieve a canceled meeting based on its id.
     *
     * @param meetingId of the meeting to retrieve
     * @return a pointer to the meeting; nullptr if the meeting was not found, or if it wasn't canceled.
     * 
     * @post Something went wrong, The meeting which was found did not have the correct id.
     */
    Meeting* getCanceledMeeting(const std::string& meetingId);
    /**
     * @brief Retrieves the cancellation reason of the appropriate meeting.
     *
     * Uses the meeting Id to retrieve the cancellation reason from a map that stores all of them, tied to the meetingId key.
     *
     * 
     * @pre That meeting does not exist or it isn't cancelled
     * 
     * @param meetingId that's tied to meetings appropriate cancellation reason.
     * 
     * @post Something went wrong, The meeting was found but the cancellation reason wasn't.
     * @post Something went wrong, The wrong cancellation reason was retrieved.
     */
    const std::string& getCancellationReason(const std::string& meetingId);

    /**
     * @brief Retrieve a processed meeting based on its id.
     *
     * @param meetingId of the meeting to retrieve
     * @return a pointer to the meeting; nullptr if the meeting was not found, or if it wasn't processed.
     * 
     * @post Something went wrong, The meeting which was found did not have the correct id.
     */
    Meeting* getDoneMeeting(const std::string& meetingId);

    /**
     * @brief Retrieve an unprocessed future meeting based on its id.
     *
     * @param meetingId of the meeting to retrieve
     * @return a pointer to the meeting; nullptr if the meeting was not found, or if it was already processed.
     * 
     * @post Something went wrong, The meeting which was found did not have the correct id.
     */
    Meeting* getFutureMeeting(const std::string& meetingId);

    /**
     * @brief Cancel a meeting.
     *
     * @param meetingId of the meeting to cancel.
     * @param reason for the cancellation (may be left empty)
     */
    void cancelMeeting(const std::string& meetingId, const std::string& reason = "");

    /**
     * @brief Uncancel a meeting.
     *
     * 
     * @pre This meeting does not exist.
     * @pre This meeting was not canceled.
     * @pre This meeting was already done.
     * @pre This meeting is not in the list of unprocessed meetings.
     * 
     * @param meetingId of the meeting to uncancel.
     * 
     * @post Something went wrong. The meeting was not uncanceled.
     */
    void uncancelMeeting(const std::string& meetingId);

    /**
     * @brief 'Does' a meeting.
     *
     * 
     * @pre This meeting does not exist.
     * @pre This meeting was canceled.
     * @pre This meeting was already done.
     * @pre This meeting is not in the list of unprocessed meetings.
     * 
     * @param meetingId of the meeting to do.
     * 
     * @post Something went wrong. The meeting was not done.
     * @post Something went wrong. The meeting was not deleted from future (unprocessed) meetings
     */
    void doMeeting(const std::string& meetingId);

    /**
     * @brief 'Undoes' a meeting.
     *
     * 
     * @pre This meeting does not exist.
     * @pre This meetings was canceled.
     * @pre This meeting was not done.
     * 
     * @param meetingId of the meeting to undo.
     * 
     * @post Something went wrong. The meeting was not undone.
     */
    void undoMeeting(const std::string& meetingId);


    /**
     * @brief Register a participation.
     *
     * Register a new participation.
     * The App expects ownership of the Participation* passed in.
     *
     * 
     * @pre Participation can not be null.
     * @pre Participation needs to be properly initialized.
     * @pre Could not add participation: The requested meeting does not exist.
     * @pre Could not add participation: This user already participates in another meeting.
     * @pre Could not add participation: The room the meeting takes place in was full.
     * @pre This rooms is already occupied.
     * @pre This user already participates in another meeting.
     * 
     * @param participation to register
     * 
     * @post Something went wrong. Encountered a meeting with invalid room id.
     * @post Something went wrong. Found meeting in App but it did not have an assigned room.
     * @post Something went wrong. The list of participations by meeting went out of sync.
     * @post Something went wrong. The participation was not added to the global list.
     * @post Something went wrong. The participation was not added to the list by user.
     * @post Something went wrong. The participation was not added to the list by app.
     */
    void addParticipation(Participation* participation);


    /**
     * @brief Retrieve a list of all participations concerning the given user.
     *
     * @param userId of the user for which to return all participations
     * @return a const pointer to the requested list, if it exists; nullptr otherwise (if the user has no registered participations).
     */
    Participations* getParticipationsByUser(const std::string& userId);

    /**
     * @brief Retrieve a list of all participations concerning the given meeting.
     *
     * @param meetindId of the meeting for which to return all participations
     * @return a const pointer to the requested list, if it exists; nullptr otherwise (if the meeting has no registered participations)
     */
    Participations* getParticipationsByMeeting(const std::string& meetindId);


    /**
     * @brief Retrieve a list of all registered participations
     *
     * @return a list of all registered participations
     */
    const Participations& getAllParticipations() const;


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
     * @param userId of the user to check
     * @param date to check
     * @return bool indicating the result
     * 
     * @post Something went wrong. Encountered a participation without a valid meeting Id.
     */
    bool isUserOccupied(const std::string& userId, const Date& date);

    /**
     *@brief Check this planned meeting for conflicting rooms/dates.
     *
     * Retrieves a meeting using its Id and uses it check wether other planned meetings of the same room/date conflicts with this one and cancels/plans it accordingly.
     *
     * @param meetingId Id of the meeting to retrieve.
     * @param verbose Prints text to console when enabled. This option is enabled by default.
     *
     * @contracts
     * REQUIRE(meeting, "This meeting doesn't exist.");
     * @n REQUIRE(meeting->isProperlyInitialized(), "Meeting needs to be properly initialized.");
     * @n ENSURE(meetingProcessed, "Meeting hasn't been processed");
     */
    void processSingleMeeting(const std::string& meetingId, bool verbose = true);
    /**
     * @brief Checks all planned meeting entries for conflicting rooms/dates.
     *
     * Temporarily sorts all meetings by date (or order if the dates are the same) then runs @ref processSingleMeeting for each meeting.
     *
     * @param verbose Prints text to console when enabled. This option is enabled by default.
     * @contracts
     * REQUIRE(currentMeeting, "Meeting can not be null.");
     * @n REQUIRE(currentMeeting->isProperlyInitialized(), "Meeting needs to be properly initialized.");
     * @n ENSURE(current_date.ok(), "Date creation failed. Date validity check did not pass.");
     */
    void processAllMeetings(bool verbose = true);
    ~App();

private:
    void writeMeeting(std::ostream& onStream, const Meeting* meeting);
    void writeRoom(std::ostream& onStream, const Room* room);

    Meetings* _getMutMeetingsByRoom(const std::string& roomId);
    Participations* _getMutParticipationsByUser(const std::string& userId);

    Participations* _getMutParticipationsByMeeting(const std::string& meetingId);

    Parser* parser;
    std::ostream* output;

    Rooms rooms;

    std::vector<std::pair<unsigned int, unsigned int>> participantsToRoomsSize;;

    Meetings all_meetings;
    //TODO: delete datastructures below and use class attributes instead
    Meetings future_meetings;
    Meetings ongoing_meetings;
    Meetings cancelling_meetings;
    //TODO: delete and make attribute
    std::unordered_map<std::string, std::string> canceled_meeting_reasons;

    //TODO: delete datastructure and implement in function in class/app
    MeetingsByRoomMap meetings_by_room;

    //TODO: delete datastructures and implement in function in class/app
    Participations all_participations;
    ParticipationsByUserMap participations_by_user;
    ParticipationsByMeetingMap participations_by_meeting;

    void* init_check_this_ptr = nullptr;
};


#endif //MEETING_PLANNER_APP_H
