//
// Created by tishaak on 3/15/26.
//

#ifndef MEETING_PLANNER_APPTESTS_H
#define MEETING_PLANNER_APPTESTS_H
#include <gtest/gtest.h>
#include "App.h"

class AppTests : public ::testing::Test {
protected:
};

TEST_F(AppTests, HappyDay1) {
    App p = App();
    EXPECT_TRUE(p.isProperlyInitialized());

    Date date1 = Date(2026, 1, 2);
    Date date2 = Date(2026, 3, 4);

    p.addRoom(new Room("Room 1", "r1", 20));
    p.addRoom(new Room("Room 2", "r2", 20));
    p.addRoom(new Room("Room 3", "r3", 20));
    p.addRoom(new Room("Room 4", "r4", 20));
    p.addRoom(new Room("Empty Room", "empty", 1));

    p.addMeeting(new Meeting("Meeting 1", "m1", "r1" , date1));
    p.addMeeting(new Meeting("Meeting 2", "m2", "r1", date2));

    p.addMeeting(new Meeting("Meeting 3", "m3", "r2", date1));
    p.addMeeting(new Meeting("Meeting 4", "m4", "r3", date1));

    Participation* john = new Participation("John Doe", "m3");
    p.addParticipation(john);
    p.addParticipation(new Participation("Dave Jones", "m2"));


    EXPECT_TRUE(p.getMeetingsByRoom("r4")->empty());
    EXPECT_EQ(int(p.getAllMeetings().size()), 4);
    p.addMeeting(new Meeting("Meeting 5", "m5", "r4" , date1));
    EXPECT_EQ(int(p.getAllMeetings().size()), 5);
    EXPECT_TRUE(!p.getMeetingsByRoom("r4")->empty());
    EXPECT_EQ(int(p.getAllRooms().size()), 5);
    p.addRoom(new Room("Room 5", "r5", 20));
    EXPECT_EQ(int(p.getAllRooms().size()), 6);
    EXPECT_EQ(int(p.getAllParticipations().size()), 2);
    p.addParticipation(new Participation("Math Smith", "m2"));
    EXPECT_EQ(int(p.getAllParticipations().size()), 3);


    //look later on why a crash happens but it doesnt detect it
    //ASSERT_DEATH(p.getMeetingsByRoom("r1111"), "program continues torun despite getting an invalid room");
    EXPECT_TRUE(!p.getParticipationsByMeeting("m3")->empty());

    //look further into same crash detection issue with those
    //EXPECT_DEATH(p.getParticipationsByUser("unknown"), "program continues to run despite getting the user from an invalid name");
    //ASSERT_DEATH(p.getParticipationsByMeeting("shadowrealm"), "program continues torun despite getting a user from an invalid meeting");
}


#endif //MEETING_PLANNER_APPTESTS_H