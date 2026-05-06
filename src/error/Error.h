//
// Created by lucas on 5/6/26.
//

#ifndef MEETING_PLANNER_ERROR_H
#define MEETING_PLANNER_ERROR_H
#include <string>
#include "parser/Elements.h"

class Error
{
public:
    Error() = default;
    virtual ~Error() = default;

    virtual void uniqueId(const CampusElement& campus) const = 0;
    virtual void uniqueId(const BuildingElement& building) const = 0;
    virtual void uniqueId(const RoomElement& room) const = 0;
    virtual void uniqueId(const MeetingElement& meeting) const = 0;

    virtual void nonExistentCampus( const BuildingElement& building ) const = 0;
    virtual void nonExistentCampus( const RoomElement& room ) const = 0;
    virtual void nonExistentCampus( const CateringElement& catering ) const = 0;

    virtual void nonExistentBuilding( const RoomElement& room ) const = 0;

    virtual void nonExistentRoom( const MeetingElement& meeting ) const = 0;
    virtual void nonExistentRoom( const RenovationElement& renovation ) const = 0;

    virtual void campusBuildingMismatch(const RoomElement& room, const std::string& other_campus) const = 0;

    virtual void nonExistentMeeting( const ParticipationElement& participation ) const = 0;

    virtual void externalsNotAllowed( const ParticipationElement& participation ) const = 0;

    virtual void duplicateParticipation( const ParticipationElement& participation ) const = 0;

    virtual void externalMismatch( const ParticipationElement& participation ) const = 0;
};


#endif //MEETING_PLANNER_ERROR_H
