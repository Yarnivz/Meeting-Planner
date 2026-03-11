//
// Created by User on 2/26/2026.
//

#ifndef MEETING_PLANNER_MEETING_H
#define MEETING_PLANNER_MEETING_H
#include <string>
#include "Date.h"

class Meeting {
public:
    Meeting(const std::string &label, const std::string &id, const std::string &roomId, const Date &date = Date());
    bool isProperlyInitialized() const;

    const std::string& getId() const;
    const std::string& getRoomId() const;
    const Date& getDate() const;

    const std::string& toString();
    friend std::ostream& operator<<(std::ostream &os, Meeting& meeting);
    void process();
    ~Meeting();
private:
    std::string label;
    std::string id;
    std::string room;
    Date date;

    void* init_check_this_ptr;
};


#endif //MEETING_PLANNER_MEETING_H