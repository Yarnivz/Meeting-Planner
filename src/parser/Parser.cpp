//
// Created by s0255946 on 18/03/2026.
//

#include "Parser.h"


Parser::Parser(std::ostream& errorStream): errorStream(errorStream) {}

const std::list<RoomElement>& Parser::parsedRooms() const { return parsed_rooms; }
void Parser::clearRooms() { parsed_rooms = {}; }

const std::list<MeetingElement>& Parser::parsedMeetings() const { return parsed_meetings; }
void Parser::clearMeetings() { parsed_meetings = {}; }

const std::list<ParticipationElement>& Parser::parsedParticipations() const { return parsed_participations; }
void Parser::clearParticipations() { parsed_participations = {}; }

const std::list<CateringElement>& Parser::parsedCaterings() const { return parsed_caterings; }
void Parser::clearCaterings() { parsed_caterings = {}; }
