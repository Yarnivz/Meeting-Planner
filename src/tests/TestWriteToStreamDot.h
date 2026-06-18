//
// Created by Lucas on 2026-03-13.
//

#ifndef MEETING_PLANNER_TESTWRITETOSTREAMDOT_H
#define MEETING_PLANNER_TESTWRITETOSTREAMDOT_H

#include "gtest/gtest.h"
#include "FileUtils.h"
#include "App.h"
#include <fstream>

#include "output/DotOutput.h"

class TestWriteToStreamDot : public ::testing::Test
{
protected:
};


TEST_F(TestWriteToStreamDot, HappyDay1)
{
    const std::string actual = "test-files/TestWriteToStreamDot.HappyDay1-actual.dot";
    const std::string expected = "test-files/TestWriteToStreamDot.HappyDay1-expected.dot";

    App p = App();
    ASSERT_TRUE(p.isProperlyInitialized());

    DateTime date1 = DateTime(2026, 1, 2, 13);
    DateTime date2 = DateTime(2026, 3, 4, 14);

    Campus* campus1 = new Campus ("Middelheim", "M");
    Campus* campus2 = new Campus ("Groenenborger", "G");
    Building* building1 = new Building("Bib", "MG", campus1);
    Building* building2 = new Building("ZBlok", "GZ", campus2);

    p.addCampus(campus1);
    p.addBuilding(building1);
    p.addCampus(campus2);
    p.addBuilding(building2);

    Room *r1, *r2, *r3, *r4, *r5, *r6;
    p.addRoom(r1 = new Room("Room 1", "r1", 20, building1));
    p.addRoom(r2 = new Room("Room 2", "r2", 20, building1));
    p.addRoom(r3 = new Room("Empty Room 3", "empty", 1, building1));
    p.addRoom(r4 = new Room("Room 4", "r4", 20, building2));
    p.addRoom(r5 = new Room("Room 6", "r5", 20, building2));
    p.addRoom(r6 = new Room("Room 6", "r6", 100, building2));

    ASSERT_EQ(r1, p.getRoom("r1"));
    ASSERT_EQ(r2, p.getRoom("r2"));
    ASSERT_EQ(r3, p.getRoom("empty"));
    ASSERT_EQ(r4, p.getRoom("r4"));
    ASSERT_EQ(r5, p.getRoom("r5"));
    ASSERT_EQ(r6, p.getRoom("r6"));

    Meeting *m1, *m2, *m3, *m4, *m5, *m6, *m7, *m8;
    p.addMeeting(m1 = new Meeting("Meeting 1", "m1", r1, date1));
    p.addMeeting(m2 = new Meeting("Meeting 2", "m2", r1, date2));
    p.addMeeting(m3 = new Meeting("Meeting 3", "m3", r2, date1));
    p.addMeeting(m4 = new Meeting("Meeting 4", "m4", r2, date1));

    p.addMeeting(m5 = new Meeting("Meeting 5", "m5", r4, DateTime(2026, 11, 11, 5)));
    p.addMeeting(m6 = new Meeting("Meeting 6", "m6", r4, DateTime(2026, 11, 11, 6)));
    p.addMeeting(m7 = new Meeting("Meeting 7", "m7", r5, DateTime(2026, 11, 11, 7)));
    p.addMeeting(m8 = new Meeting("Meeting 8", "m8", r6, DateTime(2026, 11, 11, 8)));

    ASSERT_EQ(m1, p.getMeetingById("m1"));
    ASSERT_EQ(m2, p.getMeetingById("m2"));
    ASSERT_EQ(m3, p.getMeetingById("m3"));
    ASSERT_EQ(m4, p.getMeetingById("m4"));
    ASSERT_EQ(m5, p.getMeetingById("m5"));
    ASSERT_EQ(m6, p.getMeetingById("m6"));
    ASSERT_EQ(m7, p.getMeetingById("m7"));
    ASSERT_EQ(m8, p.getMeetingById("m8"));

    User* u1 = new User("John Doe");
    User* u2 = new User("Peter Selie");
    User* u3 = new User("Jane Doe");

    p.addUser(u1);
    p.addUser(u2);
    p.addUser(u3);
    ASSERT_EQ(u1, p.getUser("John Doe"));
    ASSERT_EQ(u2, p.getUser("Peter Selie"));
    ASSERT_EQ(u3, p.getUser("Jane Doe"));

    p.addUserToMeeting("John Doe", "m3");
    p.addUserToMeeting("Peter Selie", "m1");
    p.addUserToMeeting("Jane Doe", "m5");
    p.addUserToMeeting("Jane Doe", "m6");
    p.addUserToMeeting("Jane Doe", "m7");
    p.addUserToMeeting("Jane Doe", "m8");

    p.write(DotOutput(actual));

    ASSERT_TRUE(file_exists(actual));
    ASSERT_TRUE(file_exists(expected));

    EXPECT_TRUE(file_compare(actual, expected));
}

TEST_F(TestWriteToStreamDot, HappyDay2)
{
    const std::string actual = "test-files/TestWriteToStreamDot.HappyDay2-actual.dot";
    const std::string expected = "test-files/TestWriteToStreamDot.HappyDay2-expected.dot";

    App p = App();
    EXPECT_TRUE(p.isProperlyInitialized());

    DateTime date1 = DateTime(2025, 12, 12, 20);
    DateTime date2 = DateTime(2026, 1, 1, 21);

    //possibly diversify bulding and campus later for extra tests
    Campus* campus1 = new Campus ("Middelheim", "M");
    Building* building1 = new Building("Bib", "G", campus1);

    p.addCampus(campus1);
    p.addBuilding(building1);

    Room *r1, *r2;
    p.addRoom(r1 = new Room("M.G.025", "MG025", 20, building1));
    p.addRoom(r2 = new Room("G.T.103", "GT103", 15, building1));

    p.addMeeting(new Meeting("Important Meeting 1", "m1", r1, date1));
    p.addMeeting(new Meeting("Important Meeting 2", "m2", r1, date2));
    p.addMeeting(new Meeting("Less Important Meeting 3", "m3", r1, date1));

    User *jod, *ps, *jad, *a, *b, *c, *d;
    jod = new User("John Doe");
    ps = new User("Peter Selie");
    jad = new User("Jane Doe");

    a = new User("Alice");
    b = new User("Bob");
    c = new User("Charlie");
    d = new User("David");

    p.addUser(jod);
    p.addUser(ps);
    p.addUser(jad);
    p.addUser(a);
    p.addUser(b);
    p.addUser(c);
    p.addUser(d);

    p.addUserToMeeting("John Doe", "m3");
    p.addUserToMeeting("Peter Selie", "m1");
    p.addUserToMeeting("Jane Doe", "m3");

    p.addUserToMeeting("Alice", "m2");
    p.addUserToMeeting("Bob", "m2");
    p.addUserToMeeting("Charlie", "m2");
    p.addUserToMeeting("David", "m2");

    p.write(DotOutput(actual));

    EXPECT_TRUE(file_exists(actual));
    EXPECT_TRUE(file_exists(expected));

    EXPECT_TRUE(file_compare(actual, expected));
}

TEST_F(TestWriteToStreamDot, Empty)
{
    const std::string actual = "test-files/TestWriteToStreamDot.Empty-actual.dot";
    const std::string expected = "test-files/TestWriteToStreamDot.Empty-expected.dot";

    App p = App();
    EXPECT_TRUE(p.isProperlyInitialized());

    p.write(DotOutput(actual));

    EXPECT_TRUE(file_exists(expected));
    EXPECT_TRUE(file_exists(actual));
    EXPECT_TRUE(file_compare(actual, expected));
}

TEST_F(TestWriteToStreamDot, HappyDay3)
{
    const std::string actual = "test-files/TestWriteToStreamDot.HappyDay3-actual.dot";
    const std::string expected = "test-files/TestWriteToStreamDot.HappyDay3-expected.dot";

    App p = App();
    ASSERT_TRUE(p.isProperlyInitialized());

    DateTime date1 = DateTime(2026, 1, 2, 13);
    DateTime date2 = DateTime(2026, 3, 4, 14);

    Campus* campus1 = new Campus ("Middelheim", "M");
    Campus* campus2 = new Campus ("Groenenborger", "G");
    Campus* campus3 = new Campus ("Drie Eiken", "D");
    Campus* campus4 = new Campus ("Foo", "F");
    Campus* campus5 = new Campus ("Bar", "B");
    Campus* campus6 = new Campus ("Baz", "Bz");

    Building* building1 = new Building("Bib", "MG", campus1);
    Building* building3 = new Building("Blok A", "MA", campus1);
    Building* building2 = new Building("Blok Z", "GZ", campus2);
    Building* building4 = new Building("Blok A", "DA", campus3);

    p.addCampus(campus1);
    p.addCampus(campus2);
    p.addCampus(campus3);
    p.addCampus(campus4);
    p.addCampus(campus5);
    p.addCampus(campus6);

    p.addBuilding(building1);
    p.addBuilding(building2);
    p.addBuilding(building3);
    p.addBuilding(building4);

    Room *r1, *r2, *r3, *r4, *r5, *r6;
    p.addRoom(r1 = new Room("Room 1", "r1", 20, building1));
    p.addRoom(r2 = new Room("Room 2", "r2", 20, building1));
    p.addRoom(r3 = new Room("Empty Room 3", "empty", 1, building1));
    p.addRoom(r4 = new Room("Room 4", "r4", 20, building2));
    p.addRoom(r5 = new Room("Room 5", "r5", 20, building3));
    p.addRoom(r6 = new Room("Room 6", "r6", 100, building4));

    ASSERT_EQ(r1, p.getRoom("r1"));
    ASSERT_EQ(r2, p.getRoom("r2"));
    ASSERT_EQ(r3, p.getRoom("empty"));
    ASSERT_EQ(r4, p.getRoom("r4"));
    ASSERT_EQ(r5, p.getRoom("r5"));
    ASSERT_EQ(r6, p.getRoom("r6"));

    Meeting *m1, *m2, *m3, *m4, *m5, *m6, *m7, *m8;
    p.addMeeting(m1 = new Meeting("Meeting 1", "m1", r1, date1));
    p.addMeeting(m2 = new Meeting("Meeting 2", "m2", r1, date2));
    p.addMeeting(m3 = new Meeting("Meeting 3", "m3", r2, date1));
    p.addMeeting(m4 = new Meeting("Meeting 4", "m4", r2, date1));

    p.addMeeting(m5 = new Meeting("Meeting 5", "m5", r4, DateTime(2026, 11, 11, 5)));
    p.addMeeting(m6 = new Meeting("Meeting 6", "m6", r4, DateTime(2026, 11, 11, 6)));
    p.addMeeting(m7 = new Meeting("Meeting 7", "m7", r5, DateTime(2026, 11, 11, 7)));
    p.addMeeting(m8 = new Meeting("Meeting 8", "m8", r6, DateTime(2026, 11, 11, 8)));

    ASSERT_EQ(m1, p.getMeetingById("m1"));
    ASSERT_EQ(m2, p.getMeetingById("m2"));
    ASSERT_EQ(m3, p.getMeetingById("m3"));
    ASSERT_EQ(m4, p.getMeetingById("m4"));
    ASSERT_EQ(m5, p.getMeetingById("m5"));
    ASSERT_EQ(m6, p.getMeetingById("m6"));
    ASSERT_EQ(m7, p.getMeetingById("m7"));
    ASSERT_EQ(m8, p.getMeetingById("m8"));

    User* u1 = new User("John Doe");
    User* u2 = new User("Peter Selie");
    User* u3 = new User("Jane Doe");

    p.addUser(u1);
    p.addUser(u2);
    p.addUser(u3);
    ASSERT_EQ(u1, p.getUser("John Doe"));
    ASSERT_EQ(u2, p.getUser("Peter Selie"));
    ASSERT_EQ(u3, p.getUser("Jane Doe"));

    p.addUserToMeeting("Jane Doe", "m1");
    p.addUserToMeeting("Peter Selie", "m2");
    p.addUserToMeeting("John Doe", "m3");
    p.addUserToMeeting("John Doe", "m4");
    p.addUserToMeeting("Jane Doe", "m5");
    p.addUserToMeeting("John Doe", "m6");
    p.addUserToMeeting("Peter Selie", "m7");
    p.addUserToMeeting("Peter Selie", "m8");

    p.write(DotOutput(actual));

    ASSERT_TRUE(file_exists(actual));
    ASSERT_TRUE(file_exists(expected));

    EXPECT_TRUE(file_compare(actual, expected));
}


#endif //MEETING_PLANNER_TESTWRITETOSTREAMDOT_H