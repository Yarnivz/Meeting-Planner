//
// Created by tishaak on 3/15/26.
//

#ifndef MEETING_PLANNER_APPTESTS_H
#define MEETING_PLANNER_APPTESTS_H
#include <gtest/gtest.h>
#include "../App.h"

class TestApp : public ::testing::Test {
protected:
};

TEST_F(TestApp, HappyDay1) {
    App p = App();
    EXPECT_TRUE(p.isProperlyInitialized());

    Date date1 = Date(2026, 1, 2);
    Date date2 = Date(2026, 3, 4);

    Room* room3 = new Room("Room 3", "r3", 20);
    p.addRoom(new Room("Room 1", "r1", 20));
    p.addRoom(new Room("Room 2", "r2", 20));
    p.addRoom(room3);
    p.addRoom(new Room("Room 4", "r4", 20));
    p.addRoom(new Room("Empty Room", "empty", 1));

    Meeting* meeting1 = new Meeting("Meeting 1", "m1", "r1" , date1);
    p.addMeeting(meeting1);
    p.addMeeting(new Meeting("Meeting 2", "m2", "r1", date2));

    p.addMeeting(new Meeting("Meeting 3", "m3", "r2", date1));
    p.addMeeting(new Meeting("Meeting 4", "m4", "r3", date1));

    Participation* john = new Participation("John Doe", "m3");
    p.addParticipation(john);
    p.addParticipation(new Participation("Dave Jones", "m2"));


    EXPECT_TRUE(p.getMeetingsByRoom("r4")->empty());
    EXPECT_EQ(int(p.getAllMeetings().size()), 4);
    EXPECT_EQ(p.getMeeting("m1"), meeting1);
    p.addMeeting(new Meeting("Meeting 5", "m5", "r4" , date1));
    EXPECT_EQ(int(p.getAllMeetings().size()), 5);
    EXPECT_TRUE(!p.getMeetingsByRoom("r4")->empty());


    EXPECT_EQ(int(p.getAllRooms().size()), 5);
    p.addRoom(new Room("Room 5", "r5", 20));
    EXPECT_EQ(int(p.getAllRooms().size()), 6);
    EXPECT_EQ(p.getRoom("r3"), room3);

    EXPECT_EQ(int(p.getAllParticipations().size()), 2);
    p.addParticipation(new Participation("Math Smith", "m2"));
    EXPECT_EQ(int(p.getAllParticipations().size()), 3);
    EXPECT_TRUE(!p.getParticipationsByMeeting("m3")->empty());
    EXPECT_TRUE(p.getParticipationsByMeeting("m3")->front() == john);
    EXPECT_TRUE(p.getParticipationsByUser("John Doe")->front() == john);
}

TEST_F(TestApp, RetrieveInvalid)
{
    App p = App();
    EXPECT_TRUE(p.isProperlyInitialized());

    EXPECT_EQ(p.getMeetingsByRoom("r1111"), nullptr);
    EXPECT_EQ(p.getParticipationsByUser("unknown thing"), nullptr);
    EXPECT_EQ(p.getParticipationsByMeeting("m89"), nullptr);
    EXPECT_EQ(p.getMeetingInRoom("m9", "r10"), nullptr);

    EXPECT_TRUE(p.getAllMeetings().empty());
    EXPECT_TRUE(p.getAllParticipations().empty());
    EXPECT_TRUE(p.getAllRooms().empty());
}


#endif //MEETING_PLANNER_APPTESTS_H