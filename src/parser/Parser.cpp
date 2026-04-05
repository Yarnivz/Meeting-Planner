//
// Created by s0255946 on 18/03/2026.
//

#include "Parser.h"


const std::list<RoomElement>& Parser::parsedRooms() const { return parsed_rooms; }
void Parser::clearRooms() { parsed_rooms = {}; }

const std::list<MeetingElement>& Parser::parsedMeetings() const { return parsed_meetings; }
void Parser::clearMeetings() { parsed_meetings = {}; }

const std::list<ParticipationElement>& Parser::parsedParticipations() const { return parsed_participations; }
void Parser::clearParticipations() { parsed_participations = {}; }
