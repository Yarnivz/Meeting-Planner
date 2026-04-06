//
// Created by lucas on 4/5/26.
//

#ifndef MEETING_PLANNER_MEETINGREGISTRY_H
#define MEETING_PLANNER_MEETINGREGISTRY_H

#include <cstddef>
#include <list>
#include <unordered_map>
#include "objects/DateTime.h"
#include "objects/Meeting.h"

struct DateTimeHash {
    std::size_t operator()(const DateTime& d) const;
};

class MeetingRegistry {
public:
    void add(Meeting* meeting);

    Meeting* getById(const std::string& id);
    std::list<Meeting*>& getByDateTime(const DateTime& date);

    void removeById(const std::string& id);
    const std::unordered_map<std::string, Meeting*>& getRawIdMap() const;
    const std::unordered_map<DateTime, std::list<Meeting*>, DateTimeHash>& getRawDateMap() const;
    //void removeByDate(const Date& date);

private:

    std::unordered_map<std::string, Meeting*> by_id;
    std::unordered_map<DateTime, std::list<Meeting*>, DateTimeHash> by_date;

};



#endif //MEETING_PLANNER_MEETINGREGISTRY_H
