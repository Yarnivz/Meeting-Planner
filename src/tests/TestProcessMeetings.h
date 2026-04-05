//
// Created by lucas on 2026-03-15.
//

#ifndef MEETING_PLANNER_PROCESSMEETINGSTESTS_H
#define MEETING_PLANNER_PROCESSMEETINGSTESTS_H

#include "gtest/gtest.h"
#include "App.h"

class ProcessMeetingsTest : public ::testing::Test
{
protected:
};


TEST_F(ProcessMeetingsTest, HappyDay)
{
    App p = App(nullptr, nullptr);
    EXPECT_TRUE(p.isProperlyInitialized());

    p.addRoom(new Room("M.G.025", "MG025", 40));
    p.addRoom(new Room("G.T.103", "GT103", 100));

    Meeting* m1 = new Meeting("Important Meeting", "M1", "MG025", Date(2026, 1, 1));
    Meeting* m2 = new Meeting("Important Meeting 2", "M2", "GT103", Date(2026, 1, 1));
    Meeting* m3 = new Meeting("Conflict", "M3", "MG025", Date(2026, 1, 1));
    Meeting* m4 = new Meeting("Conflict 2", "M4", "GT103", Date(2026, 1, 1));
    p.addMeeting(m1);
    p.addMeeting(m2);
    p.addMeeting(m3);
    p.addMeeting(m4);
    ASSERT_EQ(m1, p.getMeeting("M1"));
    ASSERT_EQ(m2, p.getMeeting("M2"));
    ASSERT_EQ(m3, p.getMeeting("M3"));
    ASSERT_EQ(m4, p.getMeeting("M4"));

    p.processSingleMeeting("M1", false);
    EXPECT_TRUE(m1->isProcessed());
    EXPECT_TRUE(m2->isUnProcessed());
    EXPECT_TRUE(m3->isUnProcessed());
    EXPECT_TRUE(m4->isUnProcessed());

    p.processSingleMeeting("M2", false);
    EXPECT_TRUE(m1->isProcessed());
    EXPECT_TRUE(m2->isProcessed());
    EXPECT_TRUE(m3->isUnProcessed());
    EXPECT_TRUE(m4->isUnProcessed());

    p.processSingleMeeting("M3", false);
    EXPECT_TRUE(m1->isProcessed());
    EXPECT_TRUE(m2->isProcessed());
    EXPECT_TRUE(m3->isCancelled());
    EXPECT_TRUE(m4->isUnProcessed());
    EXPECT_EQ("conflict with meeting M1", m3->getCancellationReason());

    p.processSingleMeeting("M4", false);
    EXPECT_TRUE(m1->isProcessed());
    EXPECT_TRUE(m2->isProcessed());
    EXPECT_TRUE(m3->isCancelled());
    EXPECT_TRUE(m4->isCancelled());
    EXPECT_EQ("conflict with meeting M1", m3->getCancellationReason());
    EXPECT_EQ("conflict with meeting M2", m4->getCancellationReason());
}

TEST_F(ProcessMeetingsTest, Conflicts)
{
    App p = App(nullptr, nullptr);
    EXPECT_TRUE(p.isProperlyInitialized());

    const std::string r1 = "MG025";
    const std::string r2 = "GT103";

    p.addRoom(new Room("M.G.025", r1, 40));
    p.addRoom(new Room("G.T.103", r2, 100));

    const Date d1 = Date(2026, 1, 1);
    const Date d2 = Date(2026, 3, 3);

    std::vector<Meeting*> l1 = {};
    std::vector<Meeting*> l2 = {};
    std::vector<Meeting*> l3 = {};
    std::vector<Meeting*> l4 = {};

    for (size_t i = 0; i < 30; ++i)
    {
        std::string id = "A" + std::to_string(i);
        l1.push_back(new Meeting("Meeting " + id, id, r1, d1));
    }
    for (size_t i = 0; i < 30; ++i)
    {
        std::string id = "B" + std::to_string(i);
        l2.push_back(new Meeting("Meeting " + id, id, r2, d1));
    }
    for (size_t i = 0; i < 30; ++i)
    {
        std::string id = "C" + std::to_string(i);
        l3.push_back(new Meeting("Meeting " + id, id, r2, d2));
    }
    for (size_t i = 0; i < 30; ++i)
    {
        std::string id = "D" + std::to_string(i);
        l4.push_back(new Meeting("Meeting " + id, id, r1, d2));
    }

    for (size_t i = 0; i < 30; ++i)
    {
        p.addMeeting(l1[i]);
        ASSERT_EQ(l1[i], p.getMeeting(l1[i]->getId()));
        p.processSingleMeeting(l1[i]->getId(), false);

        p.addMeeting(l2[i]);
        ASSERT_EQ(l1[i], p.getMeeting(l1[i]->getId()));
        p.processSingleMeeting(l2[i]->getId(), false);

        p.addMeeting(l3[i]);
        ASSERT_EQ(l1[i], p.getMeeting(l1[i]->getId()));
        p.processSingleMeeting(l3[i]->getId(), false);

        p.addMeeting(l4[i]);
        ASSERT_EQ(l1[i], p.getMeeting(l1[i]->getId()));
        p.processSingleMeeting(l4[i]->getId(), false);
    }

    EXPECT_TRUE(l1[0]->isProcessed());
    EXPECT_TRUE(l2[0]->isProcessed());
    EXPECT_TRUE(l3[0]->isProcessed());
    EXPECT_TRUE(l4[0]->isProcessed());

    for (size_t i = 1; i < 30; ++i) {
        EXPECT_TRUE(l1[i]->isCancelled());
        EXPECT_TRUE(l2[i]->isCancelled());
        EXPECT_TRUE(l3[i]->isCancelled());
        EXPECT_TRUE(l4[i]->isCancelled());

        static const std::string creason1 = "conflict with meeting " + l1[0]->getId();
        static const std::string creason2 = "conflict with meeting " + l2[0]->getId();
        static const std::string creason3 = "conflict with meeting " + l3[0]->getId();
        static const std::string creason4 = "conflict with meeting " + l4[0]->getId();

        EXPECT_EQ(creason1, l1[i]->getCancellationReason());
        EXPECT_EQ(creason2, l2[i]->getCancellationReason());
        EXPECT_EQ(creason3, l3[i]->getCancellationReason());
        EXPECT_EQ(creason4, l4[i]->getCancellationReason());
    }
}

TEST_F(ProcessMeetingsTest, Order)
{
    App p = App(nullptr, nullptr);
    EXPECT_TRUE(p.isProperlyInitialized());

    p.addRoom(new Room("M.G.025", "MG025", 40));

    Meeting* m1 = new Meeting("Important Meeting", "M1", "MG025", Date(2026, 1, 1));
    Meeting* m2 = new Meeting("Importanter Meeting", "M2", "MG025", Date(2026, 1, 1));
    Meeting* m3 = new Meeting("Importantest Meeting", "M3", "MG025", Date(2026, 1, 1));
    m1->setOrder(100);
    m2->setOrder(0);
    m3->setOrder(-2);

    p.addMeeting(m3);
    p.addMeeting(m1);
    p.addMeeting(m2);
    EXPECT_EQ(m1, p.getMeeting("M1"));
    EXPECT_EQ(m2, p.getMeeting("M2"));
    EXPECT_EQ(m3, p.getMeeting("M3"));
    EXPECT_TRUE(m1->isUnProcessed());
    EXPECT_TRUE(m2->isUnProcessed());
    EXPECT_TRUE(m3->isUnProcessed());

    p.processAllMeetings(false);
    EXPECT_TRUE(m1->isCancelled());
    EXPECT_TRUE(m2->isCancelled());
    EXPECT_TRUE(m3->isProcessed());

    EXPECT_EQ("conflict with meeting M3", m1->getCancellationReason());
    EXPECT_EQ("conflict with meeting M3", m2->getCancellationReason());
}

TEST_F(ProcessMeetingsTest, ParseOrder)
{
    App p = App(new XmlParser(), nullptr);
    EXPECT_TRUE(p.isProperlyInitialized());

    p.parseFile("./test-files/ConflictMeetings2.xml");

    Meeting* a1 = p.getMeeting("A1");
    Meeting* a2 = p.getMeeting("A2");
    Meeting* a3 = p.getMeeting("A3");
    Meeting* b1 = p.getMeeting("B1");
    Meeting* b2 = p.getMeeting("B2");
    Meeting* b3 = p.getMeeting("B3");
    ASSERT_NE(nullptr, a1);
    ASSERT_NE(nullptr, a2);
    ASSERT_NE(nullptr, a3);
    ASSERT_NE(nullptr, b1);
    ASSERT_NE(nullptr, b2);
    ASSERT_NE(nullptr, b3);

    p.processAllMeetings(false);


    EXPECT_TRUE(a1->isProcessed());
    EXPECT_TRUE(b1->isProcessed());

    EXPECT_TRUE(a2->isCancelled());
    EXPECT_TRUE(b3->isCancelled());
    EXPECT_TRUE(b2->isCancelled());
    EXPECT_TRUE(b3->isCancelled());

    EXPECT_EQ("conflict with meeting A1", a2->getCancellationReason());
    EXPECT_EQ("conflict with meeting A1", a3->getCancellationReason());
    EXPECT_EQ("conflict with meeting B1", b2->getCancellationReason());
    EXPECT_EQ("conflict with meeting B1", b3->getCancellationReason());
}

TEST_F(ProcessMeetingsTest, ContractViolation)
{
    App p = App(nullptr, nullptr);

    p.addRoom(new Room("r", "r", 20));

    p.addMeeting(new Meeting("m", "m", "r", Date(2026, 1, 1)));

    //double use
    EXPECT_NO_FATAL_FAILURE(p.processSingleMeeting("m", false));
    EXPECT_DEATH(p.processSingleMeeting("m", false), "");

    //same for processAllMeetings
    EXPECT_DEATH(p.processAllMeetings(false), "");

    //process fake meeting
    EXPECT_DEATH(p.processSingleMeeting("doesnt-exist", false), "");
}

#endif //MEETING_PLANNER_PROCESSMEETINGSTESTS_H