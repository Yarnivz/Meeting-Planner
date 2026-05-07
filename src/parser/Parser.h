//
// Created by s0255946 on 18/03/2026.
//

#ifndef PARSER_H
#define PARSER_H
#include <list>
#include <iostream>
#include <string>

#include "Elements.h"

//TODO: add documentation



class Parser
{
public:
    explicit Parser(std::ostream& errorStream = std::cerr);

    virtual void parse() = 0;

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

    const std::list<RenovationElement>& parsedRenovations() const;

    void clearRenovations();

    virtual ~Parser() = default;

protected:
    std::list<CampusElement> parsed_campuses;
    std::list<BuildingElement> parsed_buildings;
    std::list<RoomElement> parsed_rooms;
    std::list<MeetingElement> parsed_meetings;
    std::list<ParticipationElement> parsed_participations;
    std::list<CateringElement> parsed_caterings;
    std::list<RenovationElement> parsed_renovations;
    ParseObject parseObject;
    std::ostream& errorStream;
};
#endif //PARSER_H
