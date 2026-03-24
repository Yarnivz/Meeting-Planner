//
// Created by s0255946 on 18/03/2026.
//

#ifndef PARSER_H
#define PARSER_H
#include <list>
#include <ostream>
#include <string>

#include "Meeting.h"
#include "Participation.h"
#include "Room.h"


class Parser {
public:
    explicit Parser() = default;

    virtual void parse(const std::string& filename, std::ostream& errorStream) = 0;

    const std::list<Room*>& parsedRooms() const;

    void clearRooms();

    const std::list<Meeting*>& parsedMeetings() const;

    void clearMeetings();

    const std::list<Participation*>& parsedParticipations() const;

    void clearParticipations();

    virtual ~Parser() = default;
protected:
    std::list<Room*> rooms;
    std::list<Meeting*> meetings;
    std::list<Participation*> participations;
};
#endif //PARSER_H
