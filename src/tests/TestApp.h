//
// Created by tishaak on 3/15/26.
//

#ifndef MEETING_PLANNER_APPTESTS_H
#define MEETING_PLANNER_APPTESTS_H
#include <gtest/gtest.h>
#include "App.h"

class TestApp : public ::testing::Test
{
protected:
};

TEST_F(TestApp, HappyDay1)
{
    App p = App(nullptr, nullptr);
    EXPECT_TRUE(p.isProperlyInitialized());

    DateTime date1 = DateTime(2026, 1, 2, 0);
    DateTime date2 = DateTime(2026, 3, 4, 0);

    Room* room1 = new Room("Room 1", "r1", 20);
    Room* room2 = new Room("Room 2", "r2", 20);
    Room* room3 = new Room("Room 3", "r3", 20);
    Room* room4 = new Room("Room 4", "r4", 20);
    Room* roome = new Room("Empty Room", "empty", 1);

    p.addRoom(room1);
    p.addRoom(room2);
    p.addRoom(room3);
    p.addRoom(room4);
    p.addRoom(roome);

    ASSERT_EQ(room1, p.getRoom("r1"));
    ASSERT_EQ(room2, p.getRoom("r2"));
    ASSERT_EQ(room3, p.getRoom("r3"));
    ASSERT_EQ(room4, p.getRoom("r4"));
    ASSERT_EQ(roome, p.getRoom("empty"));

    Meeting* m1 = new Meeting("Meeting 1", "m1", room1, date1);
    Meeting* m2 = new Meeting("Meeting 2", "m2", room1, date2);
    Meeting* m3 = new Meeting("Meeting 3", "m3", room2, date1);
    Meeting* m4 = new Meeting("Meeting 4", "m4", room3, date1);

    p.addMeeting(m1);
    p.addMeeting(m2);
    p.addMeeting(m3);
    p.addMeeting(m4);

    ASSERT_EQ(m1, p.getMeetingById("m1"));
    ASSERT_EQ(m2, p.getMeetingById("m2"));
    ASSERT_EQ(m3, p.getMeetingById("m3"));
    ASSERT_EQ(m4, p.getMeetingById("m4"));

    User* johndoe = new User("John Doe", "m3");
    User* davejone = new User("Dave Jones", "m2");

    p.addUser(johndoe);
    p.addUser(davejone);

    p.getMeetingById("m3")->addParticipant(johndoe);
    p.getMeetingById("m2")->addParticipant(davejone);

    ASSERT_EQ(johndoe, p.getUser("John Doe"));
    ASSERT_EQ(davejone, p.getUser("Dave Jones"));

    EXPECT_EQ(size_t(2), p.getAllUsers().size());

    EXPECT_EQ(johndoe, p.getMeetingById("m3")->getParticipant("John Doe"));
    EXPECT_EQ(m3, p.getUser("John Doe")->getMeetingById("m3"));
    EXPECT_EQ(davejone, p.getMeetingById("m2")->getParticipant("Dave Jones"));
    EXPECT_EQ(m2, p.getUser("Dave Jones")->getMeetingById("m2"));

    EXPECT_EQ(size_t(4), p.getMeetingRegistry().getRawIdMap().size());

    p.addMeeting(new Meeting("Meeting 5", "m5", room4, date1));

    EXPECT_EQ(size_t(5), p.getMeetingRegistry().getRawIdMap().size());


    EXPECT_EQ(size_t(5), p.getAllRooms().size());
    p.addRoom(new Room("Room 5", "r5", 20));
    EXPECT_EQ(size_t(6), p.getAllRooms().size());



    p.addUser(new User("Math Smith", "m2"));
    EXPECT_EQ(size_t(3), p.getAllUsers().size());
}

TEST_F(TestApp, RetrieveInvalid)
{
    App p = App(nullptr, nullptr);
    EXPECT_TRUE(p.isProperlyInitialized());

    EXPECT_EQ(nullptr, p.getRoom("r1111"));
    EXPECT_EQ(nullptr, p.getUser("Snit"));
    EXPECT_EQ(nullptr, p.getMeetingById("m89"));
    //EXPECT_EQ(std::list<Meeting*>{}, p.getMeetingsByDate(Date(1, 2, 3)));

    p.addMeeting(new Meeting("label", "id", new Room("name", "id", 123)));

    EXPECT_EQ(size_t(0), p.getMeetingById("id")->getParticipantCount());
    EXPECT_EQ(nullptr, p.getMeetingById("id")->getParticipant("john"));
}


#endif //MEETING_PLANNER_APPTESTS_H
