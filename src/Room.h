//
// Created by User on 2/26/2026.
//

#ifndef MEETING_PLANNER_ROOM_H
#define MEETING_PLANNER_ROOM_H
#include <string>


class Room {
public:
    Room(const std::string &name, const std::string &id, unsigned capacity);
    bool isProperlyInitialized() const;

    const std::string& getId();
    const std::string& toString();
    friend std::ostream& operator <<(std::ostream& os, Room& room);
    ~Room();
private:
    std::string name;
    std::string id;
    unsigned capacity;

    void* init_check_this_ptr;
};


#endif //MEETING_PLANNER_ROOM_H