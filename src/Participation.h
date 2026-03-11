//
// Created by User on 2/26/2026.
//

#ifndef MEETING_PLANNER_PARTICIPATION_H
#define MEETING_PLANNER_PARTICIPATION_H
#include <string>

class Participation {
public:
    /**
     * @brief Creates the Participation class.
     * This is the link between a user and the meeting that they participate in.
     * The participation class has to be initialized using a username and a meeting id.
     *
     * @param user username of the user that participates in the meeting
     * @param meeting meeting id of the meeting that is being participated in
     */
    Participation(const std::string &user, const std::string &meeting);

    bool isProperlyInitialized() const;

    /**
     * @brief User getter.
     *
     * @return the username of the user involved in this participation
     */
    std::string getUser();

    /**
     * @brief Meeting id getter.
     *
     * @return the identifier of the meeting involved in this participation
     */
    std::string getMeetingId();

    /**
     * @brief Default destructor.
     */
    ~Participation();
private:
    std::string user;
    std::string meeting;

    void* init_check_this_ptr = nullptr;
};


#endif //MEETING_PLANNER_PARTICIPATION_H