//
// Created by lucas on 4/5/26.
//

#ifndef MEETING_PLANNER_USER_H
#define MEETING_PLANNER_USER_H
#include <string>


class User {
public:
    User(const std::string& name, bool external = false);
    ~User() = default;

    /**
     * @brief Checks whether this User was properly initialized by the constructor.
     * @return bool indicating result
     */
    bool isProperlyInitialized() const;

    const std::string& getName() const;
    bool isExternal() const;
private:
    std::string name;
    bool external;

    void* init_check_this_ptr;
};



#endif //MEETING_PLANNER_USER_H
