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
    Parser(const std::string& filename);
    virtual void Parse(const std::ostream& errorStream);

    const std::list<Room*>& getRooms() const;
    const std::list<Meeting*>& getMeetings() const;
    const std::list<Participation*>& getParticipations() const;

    virtual ~Parser() = default;
protected:
    std::string filename;
    std::list<Room*> rooms;
    std::list<Meeting*> meetings;
    std::list<Participation*> participations;
};
#endif //PARSER_H
