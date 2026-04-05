//
// Created by lucas on 4/5/26.
//

#ifndef MEETING_PLANNER_MEETINGREGISTRY_H
#define MEETING_PLANNER_MEETINGREGISTRY_H

#include <cstddef>
#include <unordered_map>
#include "objects/Date.h"
#include "objects/Meeting.h"

struct DateHash {
    std::size_t operator()(const Date& d) const;
};

class MeetingRegistry {
public:
    void add(Meeting* meeting);

    Meeting* getById(const std::string& id);
    Meeting* getByDate(const Date& date);

    void removeById(const std::string& id);
    const std::unordered_map<std::string, Meeting*>& getRawIdMap() const;
    const std::unordered_map<Date, Meeting*, DateHash>& getRawDateMap() const;
    void removeByDate(const Date& date);

private:

    std::unordered_map<std::string, Meeting*> by_id;
    std::unordered_map<Date, Meeting*, DateHash> by_date;

};



#endif //MEETING_PLANNER_MEETINGREGISTRY_H
