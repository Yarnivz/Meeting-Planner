//
// Created by User on 2/26/2026.
//

#ifndef MEETING_PLANNER_APP_H
#define MEETING_PLANNER_APP_H
#include <string>
#include <unordered_map>
#include <iostream>
#include <list>

#include "Room.h"
#include "Meeting.h"
#include "Participation.h"

class App {
public:
    /**
     * @brief Constructor of the App class.
     */
    App();

    void parseFile(std::string filename);
    void writeToStream(std::ostream stream);

    /**
     * @brief Register a room.
     *
     * Registers a new 'Room' object.
     * The App class expects ownership of the Room* passed in.
     *
     * @param room room to register
     */
    void addRoom(Room* room);

    /**
     * @brief Retrieve a registered room based on its id.
     *
     * @param id id of the room to retrieve
     * @return a pointer to the room with given id if it exists, nullptr otherwise
     */
    Room* getRoom(const std::string& id);

    /**
     * @brief Register and plan a meeting.
     *
     * Register and plan a new meeting.
     * The App expects ownership of the Meeting* passed in.
     *
     * @param meeting meeting to plan and register
     */
    void addMeeting(Meeting* meeting);

    /**
     * @brief Retrieve a meeting based on its id.
     *
     * @param id id of the meeting to retrieve
     * @return a pointer to the meeting with given id if it exists, nullptr otherwise
     */
    Meeting* getMeeting(const std::string& id);

    /**
     * @brief Register a participation.
     *
     * Register a new participation.
     * The App expects ownership of the Participation* passed in.
     *
     * @param participation participation to register
     */
    void addParticipation(Participation* participation);

    /**
     * @brief Retrieve a list of all participations concerning the given user.
     *
     * @param user user for which to return all participations
     * @return a const pointer to the requested list, if it exists;\n
     *         nullptr otherwise (if the user has no registered participations)
     */
    const std::list<Participation *> *getParticipationList(const std::string &user);

    void processMeetings();
    ~App();
private:
    std::unordered_map<std::string, Room*> rooms;
    std::unordered_map<std::string, Meeting*> meetings;
    std::unordered_map<std::string, std::list<Participation*>> participation_lists;

};


#endif //MEETING_PLANNER_APP_H