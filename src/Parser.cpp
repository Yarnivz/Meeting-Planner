//
// Created by s0255946 on 18/03/2026.
//

#include "Parser.h"



const std::list<Room *> & Parser::parsedRooms() const { return rooms; }
void Parser::clearRooms() { rooms = {};}

const std::list<Meeting *> & Parser::parsedMeetings() const { return meetings; }
void Parser::clearMeetings() { meetings = {};}

const std::list<Participation *> & Parser::parsedParticipations() const { return participations; }
void Parser::clearParticipations() { participations = {};}
