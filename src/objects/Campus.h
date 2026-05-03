//
// Created by Yarni on 3/29/2026.
//

#ifndef MEETING_PLANNER_CAMPUS_H
#define MEETING_PLANNER_CAMPUS_H
#include <list>
#include <string>


class Catering;

class Campus
{
    friend Catering;
public:
    /**
     * @brief Creates the Campus class.
     * This is the object that houses a bunch of different buildings that have rooms inside of them to hold meetings.
     * A campus needs to be initialized using its name and identifier.
     *
     * @pre Name cannot be empty
     * @pre Id cannot be empty
     *
     * @param name the name of this campus
     * @param id the unique identifier of this campus that can be used to call this object from the app.
     *
     * @post Campus must be properly initialized.
     * @post Name must be added to this Campus object
     * @post Id must be added to this Campus object
     */
    Campus(const std::string& name, const std::string& id);

    /**
     * @brief prevents the campus object from being copied
     */
    Campus(const Campus&) = delete;
    Campus& operator=(const Campus&) = delete;

    /**
     * @brief Checks whether this Room was properly initialized by the constructor.
     * @return bool indicating result
     */
    bool isProperlyInitialized() const;


    /**
     * @brief Id getter.
     *
     *
     * @return the identifier of this campus
     */
    const std::string& getId() const;

    /**
     * @brief Converts this campus object into a readable string format.
     * This is represented by its name.
     *
     *
     * @return the campus in string format
     */
    const std::string& toString() const;

    /**
     * @brief Standard stream operator using the toString function to append the campus to the stream.
     *
     *
     *
     * @pre Campus must be properly initialized
     *
     * @param os stream to write on
     * @param campus object to write on the stream
     *
     * @return the stream
     */
    friend std::ostream& operator<<(std::ostream& os, const Campus& campus);

    const std::list<Catering*>& getCaterings() const;

    /**
     * @brief Default destructor.
     *
     * @pre Catering cannot be null
     * @pre Catering must be properly initialized
     * @pre Caterings campus does not match this.
     *
     */
    ~Campus();

private:

    /**
     *
     *@pre Catering cannot be null
     *@pre Catering must be properly initialized
     *@pre Caterings campus does not match this.
     *
     */
    void _addCatering(Catering* c);

    std::string name;
    std::string id;

    std::list<Catering*> caterings;

    void* init_check_this_ptr;
};


#endif //MEETING_PLANNER_CAMPUS_H
