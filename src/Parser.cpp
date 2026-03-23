//
// Created by s0255946 on 18/03/2026.
//

#include "Parser.h"


Parser::Parser(const std::string& filename): filename(filename) {}

const std::list<Room *> & Parser::getRooms() const { return rooms; }

const std::list<Meeting *> & Parser::getMeetings() const { return meetings; }

const std::list<Participation *> & Parser::getParticipations() const { return participations; }
