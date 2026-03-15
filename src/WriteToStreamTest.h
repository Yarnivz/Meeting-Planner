//
// Created by lucas on 2026-03-13.
//

#ifndef MEETING_PLANNER_WRITETOSTREAMTESTS_H
#define MEETING_PLANNER_WRITETOSTREAMTESTS_H

#include "gtest/gtest.h"
#include "FileUtils.h"
#include "App.h"
#include <fstream>

class WriteToStreamTest : public ::testing::Test {
protected:
};



TEST_F(WriteToStreamTest, HappyDay1) {
    App p = App();
    EXPECT_TRUE(p.isProperlyInitialized());

    Date date1 = Date(2026, 1, 2);
    Date date2 = Date(2026, 3, 4);

    p.addRoom(new Room("Room 1", "r1", 20));
    p.addRoom(new Room("Room 2", "r2", 20));
    p.addRoom(new Room("Empty Room", "empty", 1));

    p.addMeeting(new Meeting("Meeting 1", "m1", "r1" , date1));
    p.addMeeting(new Meeting("Meeting 2", "m2", "r1", date2));

    p.addMeeting(new Meeting("Meeting 3", "m3", "r2", date1));
    p.addMeeting(new Meeting("Meeting 4", "m4", "r2", date1));

    p.addParticipation(new Participation("John Doe", "m3"));
    p.addParticipation(new Participation("Peter Selie", "m1"));

    const std::string actual = "test-files/WriteToStreamTests.HappyDay1-actual.txt";
    const std::string expected = "test-files/WriteToStreamTests.HappyDay1-expected.txt";


    std::ofstream f = std::ofstream(actual);
    p.writeToStream(f);

    EXPECT_TRUE(file_exists(actual));
    EXPECT_TRUE(file_exists(expected));

    EXPECT_TRUE(file_compare(actual, expected));
}

TEST_F(WriteToStreamTest, HappyDay2) {
    App p = App();
    EXPECT_TRUE(p.isProperlyInitialized());

    Date date1 = Date(2026, 12, 12);
    Date date2 = Date(2026, 1, 1);

    p.addRoom(new Room("M.G.025", "MG025", 20));
    p.addRoom(new Room("G.T.103", "GT103", 20));

    p.addMeeting(new Meeting("Important Meeting 1", "m1", "MG025" , date1));
    p.addMeeting(new Meeting("Important Meeting 2", "m2", "MG025", date2));

    p.addParticipation(new Participation("John Doe", "m1"));
    p.addParticipation(new Participation("Peter Selie", "m1"));
    p.addParticipation(new Participation("Jane Doe", "m1"));

    p.addParticipation(new Participation("Alice", "m2"));
    p.addParticipation(new Participation("Bob", "m2"));
    p.addParticipation(new Participation("Charlie", "m2"));
    p.addParticipation(new Participation("David", "m2"));

    const std::string actual = "test-files/WriteToStreamTests.HappyDay2-actual.txt";
    const std::string expected = "test-files/WriteToStreamTests.HappyDay2-expected.txt";


    std::ofstream f = std::ofstream(actual);
    p.writeToStream(f);

    EXPECT_TRUE(file_exists(actual));
    EXPECT_TRUE(file_exists(expected));

    EXPECT_TRUE(file_compare(actual, expected));
}

TEST_F(WriteToStreamTest, Empty) {
    App p = App();
    EXPECT_TRUE(p.isProperlyInitialized());


    const std::string actual = "test-files/WriteToStreamTests.Empty-actual.txt";


    std::ofstream f = std::ofstream(actual);
    p.writeToStream(f);

    EXPECT_TRUE(file_exists(actual));
    EXPECT_TRUE(file_is_empty(actual));
}

TEST_F(WriteToStreamTest, Processed) {
    App p = App();
    EXPECT_TRUE(p.isProperlyInitialized());

    Date date1 = Date(2026, 12, 12);
    Date date2 = Date(2026, 1, 1);

    p.addRoom(new Room("M.G.025", "MG025", 20));
    p.addRoom(new Room("G.T.103", "GT103", 20));

    p.addMeeting(new Meeting("Important Meeting 1", "m1", "MG025" , date1));
    p.addMeeting(new Meeting("Important Meeting 2", "m2", "MG025", date2));

    p.addMeeting(new Meeting("Conflict", "m3", "MG025", date1));

    p.addParticipation(new Participation("John Doe", "m1"));
    p.addParticipation(new Participation("Peter Selie", "m1"));
    p.addParticipation(new Participation("Jane Doe", "m1"));

    p.addParticipation(new Participation("Alice", "m2"));
    p.addParticipation(new Participation("Bob", "m2"));

    p.addParticipation(new Participation("Charlie", "m3"));
    p.addParticipation(new Participation("David", "m3"));

    p.processAllMeetings(false);

    const std::string actual = "test-files/WriteToStreamTests.Processed-actual.txt";
    const std::string expected = "test-files/WriteToStreamTests.Processed-expected.txt";


    std::ofstream f = std::ofstream(actual);
    p.writeToStream(f);

    EXPECT_TRUE(file_exists(actual));
    EXPECT_TRUE(file_exists(expected));

    EXPECT_TRUE(file_compare(actual, expected));
}


#endif //MEETING_PLANNER_WRITETOSTREAMTESTS_H