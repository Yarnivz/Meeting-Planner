//
// Created by tishaak on 5/14/26.
//

#ifndef MEETING_PLANNER_DOTOUTPUT_H
#define MEETING_PLANNER_DOTOUTPUT_H

#include <iostream>
#include <fstream>
#include <string>

#include "Output.h"

class DotOutput : public Output
{
public:
    DotOutput(const std::string& fileName = "./output.dot");
    ~DotOutput();

    void printUser(const User* user) const override;
    void printUsers(const Users& users) const override;
    void printMeeting(const Meeting* meeting) const override;
    void printMeetings(const MeetingRegistry& registry) const override;
    void printRoom(const Room* room) const override;
    void printRooms(const Rooms& rooms) const override;
    void printBuilding(const Building* building) const override;
    void printBuildings(const Buildings& buildings) const override;
    void printCampus(const Campus* campus) const override;
    void printCampuses(const Campuses& campuses) const override;

    void printMeetingCO2(const Meeting* meeting) const override {}
    void printMeetingsCO2(const MeetingRegistry& registry) const override {}

private:
    mutable std::ofstream stream;
    mutable std::unordered_map<std::string, Meeting*> meetingsMap;
};
#endif //MEETING_PLANNER_DOTOUTPUT_H
