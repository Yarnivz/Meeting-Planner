//
// Created by Lucas on 2026-03-05.
//
#pragma once

#include <gtest/gtest.h>

#include "objects/DateTime.h"

class TestDateTime : public ::testing::Test
{
protected:
    DateTime date_;
};

TEST_F(TestDateTime, HappyDay)
{
    using YMDH = struct
    {
        int y;
        unsigned m;
        unsigned d;
        unsigned h;
    };

    YMDH data[] = {
        {2025, 10, 2, 10}, {2027, 1, 12, 13},
        {2023, 12, 30, 0}, {2031, 7, 23, 23}
    };

    for (const YMDH& t : data)
    {
        DateTime d = DateTime(t.y, t.m, t.d, t.h);
        EXPECT_TRUE(d.isProperlyInitialized());
        EXPECT_EQ(d.getYear(), t.y);
        EXPECT_EQ(d.getMonth(), t.m);
        EXPECT_EQ(d.getDay(), t.d);
        EXPECT_EQ(d.getHour(), t.h);
    }
}

TEST_F(TestDateTime, CopyConstructor)
{
    DateTime d1 = DateTime();
    DateTime d2 = d1;

    EXPECT_TRUE(d2.isProperlyInitialized());
    EXPECT_EQ(d1.getDay(), d2.getDay());
    EXPECT_EQ(d1.getYear(), d2.getYear());
    EXPECT_EQ(d1.getMonth(), d2.getMonth());
}

TEST_F(TestDateTime, FailConstructor)
{
    EXPECT_DEATH(DateTime(1, -1, -23, -3), "");
    EXPECT_DEATH(DateTime(0, 0, 0, 0), "");
    EXPECT_DEATH(DateTime(2025, 12, 0, 12), "");
    EXPECT_DEATH(DateTime(2025, 11, 31, 12), "");
    EXPECT_DEATH(DateTime(2025, 12, 32, 12), "");
    EXPECT_DEATH(DateTime(2025, 0, 31, 12), "");
    EXPECT_DEATH(DateTime(2025, 2, 29, 12), "");
    EXPECT_DEATH(DateTime(2024, 2, 30, 12), "");

    //Hour
    EXPECT_DEATH(DateTime(2024, 2, 30, 24), "");
    EXPECT_DEATH(DateTime(2024, 2, 30, -1), "");
    EXPECT_DEATH(DateTime(2024, 2, 30, 1234567), "");
}

TEST_F(TestDateTime, EdgeCases)
{
    // Schrikkeljaren
    DateTime correct = DateTime(2024, 2, 29, 14);
    EXPECT_TRUE(correct.isProperlyInitialized());
    EXPECT_EQ(2024, correct.getYear());
    EXPECT_EQ(2u, correct.getMonth());
    EXPECT_EQ(29u, correct.getDay());
    EXPECT_EQ(14u, correct.getHour());

    EXPECT_DEATH(DateTime(2025, 2, 29, 12), "");

    //Verschillende maanden
    DateTime correct2 = DateTime(2024, 3, 31, 13);
    EXPECT_TRUE(correct2.isProperlyInitialized());
    EXPECT_EQ(2024, correct2.getYear());
    EXPECT_EQ(3u, correct2.getMonth());
    EXPECT_EQ(31u, correct2.getDay());
    EXPECT_EQ(13u, correct2.getHour());

    DateTime correct3 = DateTime(2024, 4, 30, 20);
    EXPECT_TRUE(correct3.isProperlyInitialized());
    EXPECT_EQ(2024, correct3.getYear());
    EXPECT_EQ(4u, correct3.getMonth());
    EXPECT_EQ(30u, correct3.getDay());
    EXPECT_EQ(20u, correct3.getHour());

    EXPECT_DEATH(DateTime(2025, 4, 31, 1), "");

    //Juli en Augustus beide 31 dagen lang
    DateTime juli = DateTime(2024, 7, 31, 12);
    EXPECT_TRUE(juli.isProperlyInitialized());
    EXPECT_EQ(2024, juli.getYear());
    EXPECT_EQ(7u, juli.getMonth());
    EXPECT_EQ(31u, juli.getDay());
    EXPECT_EQ(12u, juli.getHour());

    DateTime aug = DateTime(2024, 8, 31, 12);
    EXPECT_TRUE(aug.isProperlyInitialized());
    EXPECT_EQ(2024, aug.getYear());
    EXPECT_EQ(8u, aug.getMonth());
    EXPECT_EQ(31u, aug.getDay());
}