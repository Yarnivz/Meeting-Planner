//
// Created by User on 2/26/2026.
//

#ifndef MEETING_PLANNER_MEETING_H
#define MEETING_PLANNER_MEETING_H
#include <string>
#include "Date.h"

class Meeting {
public:
    /**
     * @brief Creates the Meeting class.
     * The process of users gathering in a specific room for a specific purpose for a certain amount of time on a set date.
     * A meeting can only be initialized using the label, id, room id and date parameters as these are required.
     * @param label title of the meeting
     * @param id identifier of the meeting
     * @param roomId identifier of the room where the meeting takes place
     * @param date of when the meeting takes/took place
     * @param order order in which the meeting is added to the system
     */
    Meeting(const std::string &label, const std::string &id, const std::string &roomId, const Date &date = Date(), int order = 0);

    bool isProperlyInitialized() const;

    /**
     * @brief Id getter.
     *
     * @return the identifier of this meeting
     */
    const std::string& getId() const;

    /**
     * @brief Room id getter.
     *
     * @return the identifier of the room where this meeting takes place
     */
    const std::string& getRoomId() const;

    /**
     * @brief Date getter.
     *
     * @return the date of when this meeting takes place
     */
    const Date& getDate() const;

    /**
     * @brief Converts the meeting class to a readable string format.
     *
     * @return the meeting in string format
     */
    const std::string& toString() const;

    /**
     * @brief Standard stream operator using the toString function to append the meeting to the stream.
     * @param os stream to write on
     * @param meeting object to write on the stream
     * @return the stream
     */
    friend std::ostream& operator<<(std::ostream &os, const Meeting& meeting);
    void process();
    ~Meeting();
protected:

    int getOrder() const;

private:
    std::string label;
    std::string id;
    std::string room;
    Date date;
    int order;

    void* init_check_this_ptr;
};


#endif //MEETING_PLANNER_MEETING_H