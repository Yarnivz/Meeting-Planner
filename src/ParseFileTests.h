//
// Created by Yarni on 3/13/2026.
//

#ifndef MEETING_PLANNER_PARSEFILETESTS_H
#define MEETING_PLANNER_PARSEFILETESTS_H
#include <fstream>
#include <gtest/gtest.h>

#include "App.h"

class ParseFileTests : public ::testing::Test {
};

TEST_F(ParseFileTests, HappyDay1) {

        std::ofstream errLog("./logs/HappyDay1-Errors.log");
        std::string meeting_id = "Meeting_478463";



        App* app = new App();
        EXPECT_TRUE(app->isProperlyInitialized());
        app->parseFile("./test-files/HappyDay1.xml", errLog);

        //Test room #1
        Room* room = app->getRoom("M.G.025");
        EXPECT_EQ("M.G.025", room->toString());
        EXPECT_EQ(55u, room->getCapacity());

        //Test room #2
        room = app->getRoom("Room98732");
        EXPECT_EQ("M.G.023", room->toString());
        EXPECT_EQ(10u, room->getCapacity());

        //Test meeting
        Meeting* meeting = app->getMeeting(meeting_id);
        EXPECT_EQ("Weekly meeting", meeting->toString());
        EXPECT_EQ("Room98732", meeting->getRoomId());
        EXPECT_EQ(Date(2026,5,22), meeting->getDate());

        //Test participation
        const Participations* participationsByMeeting = app->getParticipationsByMeeting(meeting_id);
        const Participations* participationsByUser = app->getParticipationsByUser("Peter Selie");
        EXPECT_EQ(1u, participationsByMeeting->size());
        EXPECT_EQ(1u, participationsByUser->size());

        delete app;
}


TEST_F(ParseFileTests, HappyDay2) {


        std::ofstream errLog("./logs/HappyDay2-Errors.log");
        std::string meeting1 = "Meeting_478463";
        std::string meeting2 = "Meeting_465831";



        App* app = new App();
        EXPECT_TRUE(app->isProperlyInitialized());
        app->parseFile("./test-files/HappyDay2.xml");

        //Test room #1
        Room* room = app->getRoom("id_026");
        EXPECT_EQ(room->toString(), "M.G.026");
        EXPECT_EQ(room->getCapacity(), 30u);

        //Test room #2
        room = app->getRoom("Room98732");
        EXPECT_EQ("M.G.023", room->toString());
        EXPECT_EQ(10u, room->getCapacity());

        //Test room #3
        room = app->getRoom("Aula23");
        EXPECT_EQ("M.A.043", room->toString());
        EXPECT_EQ(46u, room->getCapacity());

        //Test meeting #1
        Meeting* meeting = app->getMeeting(meeting1);
        EXPECT_EQ("Weekly Meeting", meeting->toString());
        EXPECT_EQ("Aula23", meeting->getRoomId());
        EXPECT_EQ(Date(2026,5,22), meeting->getDate());

        //Test meeting #2
        meeting = app->getMeeting(meeting2);
        EXPECT_EQ(meeting->toString(), "Sales Report 2025");
        EXPECT_EQ(meeting->getRoomId(), "Room98732");
        EXPECT_EQ(Date(2026,1,3), meeting->getDate());

        //Test participations meeting #1
        Participations* p = app->getParticipationsByMeeting(meeting1);
        EXPECT_EQ(3u, p->size());

        //Length Test participations meeting #2
        p = app->getParticipationsByMeeting(meeting2);
        EXPECT_EQ(1u, p->size());

        //Value Test participations user #1
        p = app->getParticipationsByUser("Peter Selie");
        EXPECT_EQ(meeting1, p->back()->getMeetingId());

        //Value Test participations user #2
        p = app->getParticipationsByUser("John Cena");
        EXPECT_EQ(meeting1, p->back()->getMeetingId());

        //Value Test participations user #3
        p = app->getParticipationsByUser("Arnold Schwarzenegger");
        EXPECT_EQ(meeting1, p->back()->getMeetingId());

        //Value Test participations user #4
        p = app->getParticipationsByUser("Mick Turner");
        EXPECT_EQ(meeting2, p->back()->getMeetingId());

        delete app;
}

TEST_F(ParseFileTests, InvalidData1) {


        std::stringstream errMsg;
        std::string errMsgLine;



        App* app = new App();
        EXPECT_TRUE(app->isProperlyInitialized());
        app->parseFile("./test-files/InvalidData1.xml", errMsg);

        //Test room #1
        Room* room = app->getRoom("M.G.025");
        EXPECT_EQ("M.G.025", room->toString());
        EXPECT_EQ(55u, room->getCapacity());

        //Test room #2
        room = app->getRoom("Room98732");
        EXPECT_FALSE(app->getRoom("Room98732"));
        std::getline(errMsg, errMsgLine);
        EXPECT_EQ("Room capacity needs to be larger than 0. Room will not be added.", errMsgLine);


        delete app;
}


#endif //MEETING_PLANNER_PARSEFILETESTS_H