//
// Created by Yarni on 3/13/2026.
//

#ifndef MEETING_PLANNER_PARSEFILETESTS_H
#define MEETING_PLANNER_PARSEFILETESTS_H
#include <fstream>
#include <gtest/gtest.h>

#include "FileUtils.h"
#include "../App.h"
#include "../parser/XmlParser.h"

class TestParseFile : public ::testing::Test {
};

TEST_F(TestParseFile, HappyDay1) {

        std::ofstream errLog("./logs/HappyDay1-Errors.log");
        std::string meeting_id = "Meeting_478463";


        XmlParser* xml_parser = new XmlParser();
        App* app = new App(xml_parser, nullptr);
        EXPECT_TRUE(app->isProperlyInitialized());

        app->parseFile("./test-files/HappyDay1.xml", errLog);

        //Test room #1
        Room* room = app->getRoom("M.G.025");
        ASSERT_NE(nullptr, room);
        EXPECT_EQ("M.G.025", room->toString());
        EXPECT_EQ(55u, room->getCapacity());

        //Test room #2
        room = app->getRoom("Room98732");
        ASSERT_NE(nullptr, room);
        EXPECT_EQ("M.G.023", room->toString());
        EXPECT_EQ(10u, room->getCapacity());

        //Test meeting
        Meeting* meeting = app->getMeeting(meeting_id);
        ASSERT_NE(nullptr, meeting);
        EXPECT_EQ("Weekly meeting", meeting->toString());
        EXPECT_EQ("Room98732", meeting->getRoomId());
        EXPECT_EQ(Date(2026,5,22), meeting->getDate());

        //Test participation
        const Participations* participationsByMeeting = app->getParticipationsByMeeting(meeting_id);
        const Participations* participationsByUser = app->getParticipationsByUser("Peter Selie");
        ASSERT_NE(nullptr, participationsByMeeting);
        ASSERT_NE(nullptr, participationsByUser);
        EXPECT_EQ(1u, participationsByMeeting->size());
        EXPECT_EQ(1u, participationsByUser->size());

        delete app;
}


TEST_F(TestParseFile, HappyDay2) {


        std::ofstream errLog("./logs/HappyDay2-Errors.log");
        std::string meeting1 = "Meeting_478463";
        std::string meeting2 = "Meeting_465831";




        App* app = new App(new XmlParser(), &std::cout);

        EXPECT_TRUE(app->isProperlyInitialized());
        app->parseFile("./test-files/HappyDay2.xml", errLog);

        //Test room #1
        Room* room = app->getRoom("id_026");
        ASSERT_NE(nullptr, room);
        EXPECT_EQ(room->toString(), "M.G.026");
        EXPECT_EQ(room->getCapacity(), 30u);

        //Test room #2
        room = app->getRoom("Room98732");
        ASSERT_NE(nullptr, room);
        EXPECT_EQ("M.G.023", room->toString());
        EXPECT_EQ(10u, room->getCapacity());

        //Test room #3
        room = app->getRoom("Aula23");
        ASSERT_NE(nullptr, room);
        EXPECT_EQ("M.A.043", room->toString());
        EXPECT_EQ(46u, room->getCapacity());

        //Test meeting #1
        Meeting* meeting = app->getMeeting(meeting1);
        ASSERT_NE(nullptr, meeting);
        EXPECT_EQ("Weekly Meeting", meeting->toString());
        EXPECT_EQ("Aula23", meeting->getRoomId());
        EXPECT_EQ(Date(2026,5,22), meeting->getDate());

        //Test meeting #2
        meeting = app->getMeeting(meeting2);
        ASSERT_NE(nullptr, meeting);
        EXPECT_EQ(meeting->toString(), "Sales Report 2025");
        EXPECT_EQ(meeting->getRoomId(), "Room98732");
        EXPECT_EQ(Date(2026,1,3), meeting->getDate());

        //Test participations meeting #1
        Participations* p = app->getParticipationsByMeeting(meeting1);
        ASSERT_NE(nullptr, p);
        EXPECT_EQ(3u, p->size());

        //Length Test participations meeting #2
        p = app->getParticipationsByMeeting(meeting2);
        ASSERT_NE(nullptr, p);
        EXPECT_EQ(1u, p->size());

        //Value Test participations user #1
        p = app->getParticipationsByUser("Peter Selie");
        ASSERT_NE(nullptr, p);
        EXPECT_EQ(meeting1, p->back()->getMeetingId());

        //Value Test participations user #2
        p = app->getParticipationsByUser("John Cena");
        ASSERT_NE(nullptr, p);
        EXPECT_EQ(meeting1, p->back()->getMeetingId());

        //Value Test participations user #3
        p = app->getParticipationsByUser("Arnold Schwarzenegger");
        ASSERT_NE(nullptr, p);
        EXPECT_EQ(meeting1, p->back()->getMeetingId());

        //Value Test participations user #4
        p = app->getParticipationsByUser("Mick Turner");
        ASSERT_NE(nullptr, p);
        EXPECT_EQ(meeting2, p->back()->getMeetingId());

        delete app;
}

TEST_F(TestParseFile, InvalidData1) {

        const std::string actual = "./test-files/InvalidData1-errors-actual.txt";
        const std::string expected = "./test-files/InvalidData1-errors-expected.txt";

        std::ofstream errLog(actual);

        const std::string meeting1 = "Meeting_478463";
        const std::string meeting2 = "Meeting_514203";
        const std::string user = "Peter Selie";



        App* app = new App(new XmlParser(), nullptr);
        EXPECT_TRUE(app->isProperlyInitialized());
        app->parseFile("./test-files/InvalidData1.xml", errLog);

        //Test room #1
        Room* room = app->getRoom("M.G.025");
        ASSERT_NE(nullptr, room);
        EXPECT_EQ("M.G.025", room->toString());
        EXPECT_EQ(55u, room->getCapacity());

        //Test room #2
        room = app->getRoom("Room98732");
        EXPECT_EQ(nullptr, room);

        //Test meeting #1
        Meeting* meeting = app->getMeeting(meeting1);
        EXPECT_EQ(nullptr, meeting);

        //Test meeting #2
        meeting = app->getMeeting(meeting2);
        EXPECT_EQ(nullptr, meeting);

        //Test participation
        Participations* p = app->getParticipationsByUser(user);
        EXPECT_EQ(nullptr, p);

        //Test errors
        EXPECT_TRUE(file_compare(actual, expected));


        delete app;
}

TEST_F(TestParseFile, InvalidData2) {

        std::string actual = "./test-files/InvalidData2-errors-actual.txt";
        std::string expected = "./test-files/InvalidData2-errors-expected.txt";

        std::ofstream errLog(actual);

        std::string room1 = "mg025";
        std::string room2 = "gt123";
        std::string meeting1 = "m656";
        std::string meeting2 = "m720";
        std::string meeting3 = "m999";
        std::string user1 = "Peter Selie";
        std::string user2 = "Freddy Gonzalez";




        App* app = new App(new XmlParser(), nullptr);
        EXPECT_TRUE(app->isProperlyInitialized());
        app->parseFile("./test-files/InvalidData2.xml", errLog);

        //Test room #1
        Room* room = app->getRoom(room1);
        EXPECT_NE(nullptr, room);
        EXPECT_EQ("M.G.025", room->toString());
        EXPECT_EQ(25u, room->getCapacity());

        //Test room #2
        room = app->getRoom(room2);
        EXPECT_NE(nullptr, room);
        EXPECT_EQ("G.T.123", room->toString());
        EXPECT_EQ(100u, room->getCapacity());

        //Test room #3
        EXPECT_EQ(2u, app->getAllRooms().size());

        //Test meeting #1
        Meeting* meeting = app->getMeeting(meeting1);
        EXPECT_NE(nullptr, meeting);
        EXPECT_EQ("Movie Marathon", meeting->toString());
        EXPECT_EQ(room1, meeting->getRoomId());
        EXPECT_EQ(Date(2026,6,13), meeting->getDate());

        //Test meeting #2
        meeting = app->getMeeting(meeting2);
        EXPECT_NE(nullptr, meeting);
        EXPECT_EQ("Very important meeting", meeting->toString());
        EXPECT_EQ(room2, meeting->getRoomId());
        EXPECT_EQ(Date(2026,6,13), meeting->getDate());

        //Test meeting #3
        meeting = app->getMeeting(meeting3);
        EXPECT_EQ(nullptr, meeting);
        EXPECT_EQ(2u, app->getAllMeetings().size());

        //Test participations
        Participations p = app->getAllParticipations();
        EXPECT_EQ(1u, p.size());
        EXPECT_EQ(meeting1, p.back()->getMeetingId());
        EXPECT_EQ(user1, p.back()->getUser());

        //Test errors
        EXPECT_TRUE(file_compare(actual, expected));

        delete app;
}

TEST_F(TestParseFile, InvalidXml) {

        App* app = new App(new XmlParser(), &std::cout);
        EXPECT_TRUE(app->isProperlyInitialized());


        EXPECT_DEATH(app->parseFile("./test-files/InvalidXml.xml"), "");

        delete app;
}

TEST_F(TestParseFile, FileNotFound) {

        App* app = new App(new XmlParser(), &std::cout);
        EXPECT_TRUE(app->isProperlyInitialized());


        EXPECT_DEATH(app->parseFile("./test-files/FileThatDoesntExist.xml"),"");

        delete app;
};


#endif //MEETING_PLANNER_PARSEFILETESTS_H