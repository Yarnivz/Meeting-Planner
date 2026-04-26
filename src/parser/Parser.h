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
};

struct RoomElement
{
    std::string name, id;
    int capacity;
};

struct ParticipationElement
{
    std::string meeting, user;
    bool external;
};


class Parser
{
public:
    explicit Parser(std::ostream& errorStream = std::cerr);

    virtual void parse(const std::string& filename) = 0;

    const std::list<RoomElement>& parsedRooms() const;

    void clearRooms();

    const std::list<MeetingElement>& parsedMeetings() const;

    void clearMeetings();

    const std::list<ParticipationElement>& parsedParticipations() const;

    void clearParticipations();

    virtual ~Parser() = default;

protected:
    std::list<CampusElement> parsed_campuses;
    std::list<BuildingElement> parsed_buildings;
    std::list<RoomElement> parsed_rooms;
    std::list<MeetingElement> parsed_meetings;
    std::list<ParticipationElement> parsed_participations;
    std::ostream& errorStream;
};
#endif //PARSER_H