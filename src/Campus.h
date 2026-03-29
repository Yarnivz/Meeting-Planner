//
// Created by User on 3/29/2026.
//

#ifndef MEETING_PLANNER_CAMPUS_H
#define MEETING_PLANNER_CAMPUS_H
#include <string>


class Campus {
public:
    /**
     * @brief Creates the Campus class.
     * This is the object that houses a bunch of different buildings that have rooms inside of them to hold meetings.
     * A campus needs to be initialized using its name and identifier.
     *
     * @param name the name of this campus
     * @param id the unique identifier of this campus that can be used to call this object from the app.
     */
    Campus(const std::string& name, const std::string& id);

    /**
     * @brief Checks whether this Room was properly initialized by the constructor.
     * @return bool indicating result
     */
    bool isProperlyInitialized() const;

    /**
     * @brief Id getter.
     *
     * @return the identifier of this campus
     */
    const std::string& getId() const;

    /**
     * @brief Converts this campus object into a readable string format.
     * This is represented by its name.
     * @return the campus in string format
     */
    const std::string& toString() const;

    /**
     * @brief Standard stream operator using the toString function to append the campus to the stream.
     * @param os stream to write on
     * @param campus object to write on the stream
     * @return the stream
     */
    friend std::ostream& operator <<(std::ostream& os, const Campus& campus);

    /**
     * @brief Default destructor.
     */
    ~Campus();

private:
    std::string name;
    std::string id;

    void* init_check_this_ptr;
};


#endif //MEETING_PLANNER_CAMPUS_H