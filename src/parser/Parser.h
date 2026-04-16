//
// Created by s0255946 on 18/03/2026.
//

#ifndef PARSER_H
#define PARSER_H
#include <list>
#include <ostream>
#include <string>
#include "./objects/DateTime.h"

//TODO: add documentation

struct MeetingElement
{
    std::string label, id, room_id;
    DateTime date_time;
};

struct RoomElement
{
    std::string name, id;
    int capacity;
};

struct ParticipationElement
{
    std::string meeting, user;
};

class Parser
{
public:
    explicit Parser() = default;

    virtual void parse(const std::string& filename, std::ostream& errorStream) = 0;

    const std::list<RoomElement>& parsedRooms() const;

    void clearRooms();

    const std::list<MeetingElement>& parsedMeetings() const;

    void clearMeetings();

    const std::list<ParticipationElement>& parsedParticipations() const;

    void clearParticipations();

    virtual ~Parser() = default;

protected:
    std::list<RoomElement> parsed_rooms;
    std::list<MeetingElement> parsed_meetings;
    std::list<ParticipationElement> parsed_participations;
};
#endif //PARSER_H