//
// Created by Lucas on 2026-03-13.
//

#ifndef MEETING_PLANNER_WRITETOSTREAMTESTS_H
#define MEETING_PLANNER_WRITETOSTREAMTESTS_H

#include "gtest/gtest.h"
#include "FileUtils.h"
#include "App.h"
#include <fstream>

class TestWriteToStream : public ::testing::Test
{
protected:
};


TEST_F(TestWriteToStream, HappyDay1)
{
    const std::string actual = "test-files/WriteToStreamTests.HappyDay1-actual.txt";
    const std::string expected = "test-files/WriteToStreamTests.HappyDay1-expected.txt";
    std::ofstream f = std::ofstream(actual);

    App p = App(nullptr, &f);
    ASSERT_TRUE(p.isProperlyInitialized());

    DateTime date1 = DateTime(2026, 1, 2, 13);
    DateTime date2 = DateTime(2026, 3, 4, 14);

    Room *r1, *r2, *r3;
    p.addRoom(r1 = new Room("Room 1", "r1", 20));
    p.addRoom(r2 = new Room("Room 2", "r2", 20));
    p.addRoom(r3 = new Room("Empty Room", "empty", 1));

    ASSERT_EQ(r1, p.getRoom("r1"));
    ASSERT_EQ(r2, p.getRoom("r2"));
    ASSERT_EQ(r3, p.getRoom("empty"));

    Meeting *m1, *m2, *m3, *m4;
    p.addMeeting(m1 = new Meeting("Meeting 1", "m1", r1, date1));
    p.addMeeting(m2 = new Meeting("Meeting 2", "m2", r1, date2));
    p.addMeeting(m3 = new Meeting("Meeting 3", "m3", r2, date1));
    p.addMeeting(m4 = new Meeting("Meeting 4", "m4", r2, date1));

    ASSERT_EQ(m1, p.getMeetingById("m1"));
    ASSERT_EQ(m2, p.getMeetingById("m2"));
    ASSERT_EQ(m3, p.getMeetingById("m3"));
    ASSERT_EQ(m4, p.getMeetingById("m4"));

    User* u1 = new User("John Doe");
    User* u2 = new User("Peter Selie");
    p.addUser(u1);
    p.addUser(u2);
    ASSERT_EQ(u1, p.getUser("John Doe"));
    ASSERT_EQ(u2, p.getUser("Peter Selie"));

    m3->addParticipant(u1);
    m1->addParticipant(u2);

    p.writeToStream();

    ASSERT_TRUE(file_exists(actual));
    ASSERT_TRUE(file_exists(expected));

    EXPECT_TRUE(file_compare(actual, expected));
}

TEST_F(TestWriteToStream, HappyDay2)
{
    const std::string actual = "test-files/WriteToStreamTests.HappyDay2-actual.txt";
    const std::string expected = "test-files/WriteToStreamTests.HappyDay2-expected.txt";
    std::ofstream f = std::ofstream(actual);

    App p = App(nullptr, &f);
    EXPECT_TRUE(p.isProperlyInitialized());

    DateTime date1 = DateTime(2025, 12, 12, 20);
    DateTime date2 = DateTime(2026, 1, 1, 21);

    Room *r1, *r2;
    p.addRoom(r1 = new Room("M.G.025", "MG025", 20));
    p.addRoom(r2 = new Room("G.T.103", "GT103", 20));

    p.addMeeting(new Meeting("Important Meeting 1", "m1", r1, date1));
    p.addMeeting(new Meeting("Important Meeting 2", "m2", r1, date2));

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

    p.addUserToMeeting("John Doe", "m1");
    p.addUserToMeeting("Peter Selie", "m1");
    p.addUserToMeeting("Jane Doe", "m1");

    p.addUserToMeeting("Alice", "m2");
    p.addUserToMeeting("Bob", "m2");
    p.addUserToMeeting("Charlie", "m2");
    p.addUserToMeeting("David", "m2");

    p.writeToStream();

    EXPECT_TRUE(file_exists(actual));
    EXPECT_TRUE(file_exists(expected));

    EXPECT_TRUE(file_compare(actual, expected));
}

TEST_F(TestWriteToStream, Empty)
{
    const std::string actual = "test-files/WriteToStreamTests.Empty-actual.txt";
    std::ofstream f = std::ofstream(actual);

    App p = App(nullptr, &f);
    EXPECT_TRUE(p.isProperlyInitialized());

    p.writeToStream();

    EXPECT_TRUE(file_exists(actual));
    EXPECT_TRUE(file_is_empty(actual));
}

TEST_F(TestWriteToStream, Processed)
{
    const std::string actual = "test-files/WriteToStreamTests.Processed-actual.txt";
    const std::string expected = "test-files/WriteToStreamTests.Processed-expected.txt";
    std::ofstream f = std::ofstream(actual);

    App p = App(nullptr, &f);
    EXPECT_TRUE(p.isProperlyInitialized());

    DateTime date1 = DateTime(2025, 12, 12, 0);
    DateTime date2 = DateTime(2026, 1, 1, 11);

    Room *r1, *r2;
    p.addRoom(r1 = new Room("M.G.025", "MG025", 20));
    p.addRoom(r2 = new Room("G.T.103", "GT103", 20));

    Meeting* m1 = new Meeting("Important Meeting 1", "m1", r1, date1);
    Meeting* m2 = new Meeting("Important Meeting 2", "m2", r1, date2);
    Meeting* m3 = new Meeting("Conflict", "m3", r1, date1);

    // Meetings added first get priority
    p.addMeeting(m1);
    p.addMeeting(m2);
    p.addMeeting(m3);

    User *u1, *u2, *u3, *ua, *ub, *uc, *ud;
    p.addUser(u1 = new User("John Doe"));
    p.addUser(u2 = new User("Peter Selie"));
    p.addUser(u3 = new User("Jane Doe"));
    p.addUser(ua = new User("Alice"));
    p.addUser(ub = new User("Bob"));
    p.addUser(uc = new User("Charlie"));
    p.addUser(ud = new User("David"));

    u1->addMeeting(m1);
    u2->addMeeting(m1);
    u3->addMeeting(m1);

    ua->addMeeting(m2);
    ub->addMeeting(m2);

    uc->addMeeting(m3);
    ud->addMeeting(m3);

    p.processAllMeetings(false);


    p.writeToStream();

    EXPECT_TRUE(file_exists(actual));
    EXPECT_TRUE(file_exists(expected));

    EXPECT_TRUE(file_compare(actual, expected));
}


#endif //MEETING_PLANNER_WRITETOSTREAMTESTS_H