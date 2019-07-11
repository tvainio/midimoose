// tests.cpp
#include "clock_divider.h"
#include <gtest/gtest.h> 

int called = 0;
int calledTrue = 0;
int calledFalse = 0;

void cb(bool state) {
    state == true? calledTrue++:calledFalse++;
    called++;
}

class ClockDividerTest : public testing::Test
{
public:
    
    virtual void SetUp() {
	called=0;
	calledTrue = 0;
	calledFalse = 0;
    }
 
    virtual void TearDown() {
    }
};


TEST_F(ClockDividerTest, callsCbAFterFirstTickThenAFterHalfWay) {
    ClockDivider s(4, cb);
    s.tick();
    ASSERT_EQ(1, called);
    ASSERT_EQ(1, calledTrue);
    ASSERT_EQ(0, calledFalse);
    s.tick();
    s.tick();
    ASSERT_EQ(2, called);
    ASSERT_EQ(1, calledTrue);
    ASSERT_EQ(1, calledFalse);
}

TEST_F(ClockDividerTest, resets) {
    ClockDivider s(4, cb);
    s.tick();
    ASSERT_EQ(1, called);
    ASSERT_EQ(1, calledTrue);
    ASSERT_EQ(0, calledFalse);
    s.tick();
    s.tick();
    ASSERT_EQ(2, called);
    ASSERT_EQ(1, calledTrue);
    ASSERT_EQ(1, calledFalse);
}

TEST_F(ClockDividerTest, callsCbAlsoDuringSecondCycle) {
    ClockDivider s(4, cb);
    s.tick();
    s.tick();
    s.tick();
    s.tick();
    s.tick();
    ASSERT_EQ(3, called);
    ASSERT_EQ(2, calledTrue);
    ASSERT_EQ(1, calledFalse);
    s.tick();
    s.tick();
    ASSERT_EQ(4, called);
    ASSERT_EQ(2, calledTrue);
    ASSERT_EQ(2, calledFalse);
}

 
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
