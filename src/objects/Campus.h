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
     * @pre REQUIRE(!name.empty(), "Name cannot be empty")
     * @pre REQUIRE(!id.empty(), "Id cannot be empty")
     *
     * @param name the name of this campus
     * @param id the unique identifier of this campus that can be used to call this object from the app.
     *
     * @post ENSURE(isProperlyInitialized(), "Campus must be properly initialized.")
     * @post ENSURE(this->name == name, "Name must be added to this Campus object")
     * @post ENSURE(getId() == id, "Id must be added to this Campus object")
     */
    Campus(const std::string& name, const std::string& id);

    /**
     * @brief prevents the campus object from being copied
     */
    Campus(const Campus&) = delete;
    Campus& operator=(const Campus&) = delete;

    /**
     * @brief Checks whether this Campus was properly initialized by the constructor.
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
     * @pre REQUIRE(campus.isProperlyInitialized(), "Campus must be properly initialized")
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
     * @pre REQUIRE(c != nullptr, "Catering cannot be null")
     * @pre REQUIRE(c->isProperlyInitialized(), "Catering must be properly initialized")
     * @pre REQUIRE(c->getCampus() == this, "Caterings campus does not match this.")
     *
     */
    ~Campus();

private:

    /**
     *
     * @pre REQUIRE(c != nullptr, "Catering cannot be null")
     * @pre REQUIRE(c->isProperlyInitialized(), "Catering must be properly initialized")
     * @pre REQUIRE(c->getCampus() == this, "Caterings campus does not match this.")
     *
     */
    void _addCatering(Catering* c);

    std::string name;
    std::string id;

    std::list<Catering*> caterings;

    void* init_check_this_ptr;
};


#endif //MEETING_PLANNER_CAMPUS_H
