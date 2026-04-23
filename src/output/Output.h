//
// Created by lucas on 4/23/26.
//

#ifndef MEETING_PLANNER_OUTPUT_H
#define MEETING_PLANNER_OUTPUT_H
#include <list>

#include "TypeDefs.h"


class MeetingRegistry;
class Meeting;
class Room;

class Output {
public:
    Output() = default;
    virtual ~Output() = default;

    virtual void printMeeting(const Meeting* meeting) = 0;
    virtual void printRoom( const Room* room ) = 0;
    virtual void printMeetings( const MeetingRegistry& registry ) = 0;
    virtual void printRooms( const Rooms& rooms ) = 0;
};



#endif //MEETING_PLANNER_OUTPUT_H
