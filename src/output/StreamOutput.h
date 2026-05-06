//
// Created by lucas on 4/23/26.
//

#ifndef MEETING_PLANNER_STREAMOUTPUT_H
#define MEETING_PLANNER_STREAMOUTPUT_H

#include <iostream>

#include "Output.h"

class StreamOutput : public Output {
public:

    /**
     *
     * @pre REQUIRE(stream, "'stream' can't be null")
     *
     */
    StreamOutput(std::ostream& stream = std::cout);

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

    void printMeetingCO2(const Meeting* meeting) const override;
    void printMeetingsCO2(const MeetingRegistry& registry) const override;

private:
    std::ostream& stream;
};



#endif //MEETING_PLANNER_STREAMOUTPUT_H
