//
// Created by lucas on 4/23/26.
//

#ifndef MEETING_PLANNER_OUTPUT_H
#define MEETING_PLANNER_OUTPUT_H


#include "TypeDefs.h"


class MeetingRegistry;
class Meeting;
class Room;
class Catering;

class Output {
public:
    Output() = default;
    virtual ~Output() = default;

    virtual void printUser( const User* user ) const = 0;
    virtual void printUsers( const Users& users ) const = 0;
    // virtual void printRenovation( const Renovation* renovation ) = 0;
    // virtual void printRenovations( const Renovations& renovations ) = 0;
    virtual void printMeeting( const Meeting* meeting ) const = 0;
    virtual void printMeetings( const MeetingRegistry& registry ) const = 0;
    virtual void printRoom( const Room* room ) const = 0;
    virtual void printRooms( const Rooms& rooms ) const = 0;
    virtual void printBuilding( const Building* building ) const = 0;
    virtual void printBuildings( const Buildings& buildings ) const = 0;
    virtual void printCampus( const Campus* campus ) const = 0;
    virtual void printCampuses( const Campuses& campuses ) const = 0;

    virtual void printMeetingCO2( const Meeting* meeting ) const = 0;
    virtual void printMeetingsCO2( const MeetingRegistry& registry ) const = 0;
};



#endif //MEETING_PLANNER_OUTPUT_H
