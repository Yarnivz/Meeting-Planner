//
// Created by lucas on 5/6/26.
//

#ifndef MEETING_PLANNER_ELEMENTS_H
#define MEETING_PLANNER_ELEMENTS_H
#include <string>

#include "objects/DateTime.h"

enum class ElementType
{
    CAMPUS,
    BUILDING,
    ROOM,
    MEETING,
    PARTICIPATION,
    CATERING,
    RENOVATION
};

enum class PropType
{
    IDENTIFIER,
    NAME,
    LABEL,
    CAMPUS,
    BUILDING,
    CAPACITY,
    ROOM,
    ONLINE,
    DATE,
    HOUR,
    EXTERNALS,
    MEETING,
    USER,
    EXTERNAL,
    CO2,
    CATERINGNEEDED,
    STARTDATE,
    ENDDATE,
};

struct CampusElement
{
    std::string name, id;
};

struct BuildingElement
{
    std::string name, id, campus_id;
};

struct MeetingElement
{
    std::string label, id, room_id;

    DateTime date_time;
    bool externals_allowed, catering_needed, online;

};

struct RoomElement
{
    std::string name, id, campus_id, building_id;
    int capacity;
};

struct ParticipationElement
{
    std::string meeting, user;
    bool external;
};


struct CateringElement
{
    std::string campus_id;
    float co2_count;
};

struct RenovationElement
{
    std::string room;
    Date start_date, end_date;
};

struct ParseObject
{
    std::string identifier, name, label, campus_id, building_id, room_id, user_id, meeting_id;
    int capacity;
    int hour = -1;
    float co2_count;
    bool externals, external, online, catering_needed;
    Date date = Date(1, 1, 1);
    Date start_date, end_date;
};

#endif //MEETING_PLANNER_ELEMENTS_H
