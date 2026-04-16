//
// Created by Lucas on 2026-03-05.
//
#pragma once

#include <gtest/gtest.h>

#include "objects/Date.h"

class TestDate : public ::testing::Test
{
protected:
    Date date_;
};

TEST_F(TestDate, HappyDay)
{
    using YMD = struct
    {
        int y;
        unsigned m;
        unsigned d;
    };

    YMD data[] = {
        {2025, 10, 2}, {2027, 1, 12},
        {2023, 12, 30}, {2031, 7, 23}
    };

    for (const YMD& t : data)
    {
        Date d = Date(t.y, t.m, t.d);
        EXPECT_TRUE(d.isProperlyInitialized());
        EXPECT_EQ(t.y, d.getYear());
        EXPECT_EQ(t.m, d.getMonth());
        EXPECT_EQ(t.d, d.getDay());
    }
}

TEST_F(TestDate, CopyConstructor)
{
    Date d1 = Date();
    Date d2 = d1;

    EXPECT_TRUE(d2.isProperlyInitialized());
    EXPECT_EQ(d1.getDay(), d2.getDay());
    EXPECT_EQ(d1.getYear(), d2.getYear());
    EXPECT_EQ(d1.getMonth(), d2.getMonth());
}

TEST_F(TestDate, FailConstructor)
{
    EXPECT_DEATH(Date(1, -1, -23), "");
    EXPECT_DEATH(Date(0, 0, 0), "");
    EXPECT_DEATH(Date(2025, 12, 0), "");
    EXPECT_DEATH(Date(2025, 12, 0), "");
    EXPECT_DEATH(Date(2025, 11, 31), "");
    EXPECT_DEATH(Date(2025, 12, 32), "");
    EXPECT_DEATH(Date(2025, 0, 31), "");
    EXPECT_DEATH(Date(2025, 2, 29), "");
    EXPECT_DEATH(Date(2024, 2, 30), "");
}

TEST_F(TestDate, EdgeCases)
{
    // Schrikkeljaren
    Date correct = Date(2024, 2, 29);
    EXPECT_TRUE(correct.isProperlyInitialized());
    EXPECT_EQ(2024, correct.getYear());
    EXPECT_EQ(2u, correct.getMonth());
    EXPECT_EQ(29u, correct.getDay());

    EXPECT_DEATH(Date(2025, 2, 29), "");

    //Verschillende maanden
    Date correct2 = Date(2024, 3, 31);
    EXPECT_TRUE(correct2.isProperlyInitialized());
    EXPECT_EQ(2024, correct2.getYear());
    EXPECT_EQ(3u, correct2.getMonth());
    EXPECT_EQ(31u, correct2.getDay());

    Date correct3 = Date(2024, 4, 30);
    EXPECT_TRUE(correct3.isProperlyInitialized());
    EXPECT_EQ(2024, correct3.getYear());
    EXPECT_EQ(4u, correct3.getMonth());
    EXPECT_EQ(30u, correct3.getDay());

    EXPECT_DEATH(Date(2025, 4, 31), "");

    //Juli en Augustus beide 31 dagen lang
    Date juli = Date(2024, 7, 31);
    EXPECT_TRUE(juli.isProperlyInitialized());
    EXPECT_EQ(2024, juli.getYear());
    EXPECT_EQ(7u, juli.getMonth());
    EXPECT_EQ(31u, juli.getDay());

    Date aug = Date(2024, 8, 31);
    EXPECT_TRUE(aug.isProperlyInitialized());
    EXPECT_EQ(2024, aug.getYear());
    EXPECT_EQ(8u, aug.getMonth());
    EXPECT_EQ(31u, aug.getDay());
}