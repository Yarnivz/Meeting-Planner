//
// Created by lucas on 4/23/26.
//

#ifndef MEETING_PLANNER_STREAMOUTPUT_H
#define MEETING_PLANNER_STREAMOUTPUT_H

#include <iostream>

#include "Output.h"

class StreamOutput : public Output {
public:
    StreamOutput(std::ostream* stream = &std::cout);

    void printMeeting(const Meeting* meeting) override;
    void printMeetings(const MeetingRegistry& registry) override;
    void printRoom(const Room* room) override;
    void printRooms(const Rooms& rooms) override;
    void printCampus(const Campus* campus) override;

private:
    std::ostream* stream;
};



#endif //MEETING_PLANNER_STREAMOUTPUT_H
