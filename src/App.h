//
// Created by User on 2/26/2026.
//

#ifndef MEETING_PLANNER_APP_H
#define MEETING_PLANNER_APP_H
#include <string>
#include <unordered_map>
#include "Room.h"
#include "Meeting.h"
#include "Participation.h"

class App {
public:
    App();
    void parseFile(std::string filename);
    void writeToStream(std::ostream stream);
    void addRoom(Room* room);
    Room* getRoom(std::string id);
    void addMeeting(Meeting* meeting);
    Meeting* getMeeting(std::string id);
    void addParticipation(Participation* participation);
    Participation* getParticipation(std::string id);
    void processMeetings();
    ~App();
private:
    std::unordered_map<std::string, Room*> rooms;
    std::unordered_map<std::string, Meeting*> meetings;
    std::unordered_map<std::string, Participation*> participations;
};


#endif //MEETING_PLANNER_APP_H