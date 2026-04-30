//
// Created by s0255946 on 18/03/2026.
//

#ifndef PARSER_H
#define PARSER_H
#include <list>
#include <iostream>
#include <string>
#include "./objects/DateTime.h"

//TODO: add documentation

enum class ElementType
{
    CAMPUS,
    BUILDING,
    ROOM,
    MEETING,
    PARTICIPATION
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
    DATE,
    HOUR,
    EXTERNALS,
    MEETING,
    USER,
    EXTERNAL,
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
    bool externals_allowed;
    bool catering_needed;
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

struct ParseObject
{
    std::string identifier, name, label, campus_id, building_id, room_id, user_id, meeting_id;
    int capacity;
    int year = -1;
    int month = -1;
    int day = -1;
    int hour = -1;
    bool externals, external;
};

class Parser
{
public:
    explicit Parser(std::ostream& errorStream = std::cerr);

    virtual void parse(const std::string& filename) = 0;

    const std::list<CampusElement>& parsedCampuses() const;

    void clearCampuses();

    const std::list<BuildingElement>& parsedBuildings() const;

    void clearBuildings();

    const std::list<RoomElement>& parsedRooms() const;

    void clearRooms();

    const std::list<MeetingElement>& parsedMeetings() const;

    void clearMeetings();

    const std::list<ParticipationElement>& parsedParticipations() const;

    void clearParticipations();

    const std::list<CateringElement>& parsedCaterings() const;

    void clearCaterings();

    virtual ~Parser() = default;

protected:
    std::list<CampusElement> parsed_campuses;
    std::list<BuildingElement> parsed_buildings;
    std::list<RoomElement> parsed_rooms;
    std::list<MeetingElement> parsed_meetings;
    std::list<ParticipationElement> parsed_participations;
    std::list<CateringElement> parsed_caterings;
    ParseObject parseObject;
    std::ostream& errorStream;
};
#endif //PARSER_H