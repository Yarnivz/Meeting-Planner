//
// Created by User on 2/26/2026.
//

#ifndef MEETING_PLANNER_APP_H
#define MEETING_PLANNER_APP_H
#include <list>
#include <string>
#include <unordered_map>

#include "Room.h"
#include "Meeting.h"
#include "Participation.h"

using Meetings = std::unordered_map<std::string, Meeting*>;
using Rooms = std::unordered_map<std::string, Room*>;
using Participations = std::list<Participation*>;


using MeetingsByRoomMap = std::unordered_map<std::string, Meetings>;
using ParticipationsByUserMap = std::unordered_map<std::string, Participations>;
using ParticipationsByMeetingMap = std::unordered_map<std::string, Participations>;

class App {
public:
    /**
     * @brief Constructor of the App class.
     */
    App();

    bool isProperlyInitialized() const;

    /**
    *@brief Extract data from xml file.
    *
    * Takes an xml filename as input and extracts the relevant data, then sets the relevant class variables to it.
    *
    * @param filename
    */
    void parseFile(const std::string& filename);
    void writeToStream(std::ostream& onStream);

    /**
     * @brief Register a room.
     *
     * Registers a new 'Room' object.
     * The App class expects ownership of the Room pointer passed in.
     *
     * @param room to register
     */
    void addRoom(Room* room);

    /**
     * @brief Retrieve a registered room based on its id.
     *
     * @param roomId of the room to retrieve
     * @return a pointer to the room with given id if it exists, nullptr otherwise
     */
    Room* getRoom(const std::string& roomId);

    /**
     * @brief Register and plan a meeting.
     *
     * Register and plan a new meeting.
     * The App expects ownership of the Meeting* passed in.
     *
     * @param meeting to plan and register
     */
    void addMeeting(Meeting* meeting);


    Meeting* getMeetingInRoom(const std::string& meetingId, const std::string& roomId);

    /**
     * @brief Retrieve a meeting based on its id.
     *
     * @param meetingId id of the meeting to retrieve
     * @return a pointer to the meeting with given id if it exists, nullptr otherwise
     */
    Meeting* getMeeting(const std::string& meetingId);

    Meeting *getCanceledMeeting(const std::string &meetingId);

    Meeting *getDoneMeeting(const std::string &meetingId);

    void cancelMeeting(const std::string &meetingId);

    void uncancelMeeting(const std::string &meetingId);

    void doMeeting(const std::string &meetingId);

    void undoMeeting(const std::string &meetingId);

    /**
     * @brief Register a participation.
     *
     * Register a new participation.
     * The App expects ownership of the Participation* passed in.
     *
     * @param participation to register
     */
    void addParticipation(Participation* participation);


    /**
     * @brief Retrieve a list of all participations concerning the given user.
     *
     * @param userId user for which to return all participations
     * @return a const pointer to the requested list, if it exists;\n
     *         nullptr otherwise (if the user has no registered participations)
     */
    //const std::list<Participation *> *getParticipationListByUser(const std::string &user);
    const Participations* getParticipationsByUser(const std::string &userId);

    const Participations *getParticipationsByMeeting(const std::string &meetindId);

    const Meetings *getMeetingsByRoom(const std::string &roomId);


    const Meetings& getAllMeetings() const;
    const Participations &getAllParticipations() const;

    const Rooms &getAllRooms() const;


    bool isRoomOccupied(const std::string& roomId, const Date& date);
    bool isUserOccupied(const std::string& userId, const Date& date);

    /**
    *@brief Check this planned meeting for conflicting rooms/dates.
    *
    * Retrieves a meeting using its Id and uses it check wether other planned meetings of the same room/date conflicts with this one and cancels/plans it accordingly.
    *
    *@param meetingId Id of the meeting to retrieve.
    */
    void processSingleMeeting(const std::string &meetingId);
    /**
    *@brief Checks all planned meeting entries for conflicting rooms/dates.
    *
    * Checks if all meeting entries plans conflict with eachother and cancels/plans it accordingly.
    */
    void processAllMeetings();
    ~App();


private:

    void writeMeeting(std::ostream& onStream, const Meeting* meeting);
    void writeRoom(std::ostream& onStream, const Room* room);

    Meetings *_getMutMeetingsByRoom(const std::string &roomId);
    Participations *_getMutParticipationsByUser(const std::string &userId);

    Participations *_getMutParticipationsByMeeting(const std::string &meetingId);


    Rooms rooms;

    Meetings all_meetings;
    Meetings ongoing_meetings;
    Meetings cancelling_meetings;

    MeetingsByRoomMap meetings_by_room;

    Participations all_participations;
    ParticipationsByUserMap participations_by_user;
    ParticipationsByMeetingMap participations_by_meeting;

    void* init_check_this_ptr = nullptr;
};


#endif //MEETING_PLANNER_APP_H