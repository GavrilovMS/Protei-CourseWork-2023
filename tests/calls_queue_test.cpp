#include <gtest/gtest.h>

#include "calls_queue.hpp"
#include "call.hpp"

TEST(CallsQueueTest, PushPopTest) {
    CallsQueue queue(5);
    Call call1("1234567890");
    Call call2("9876543210");

    queue.push(call1);
    queue.push(call2);

    Call poppedcall;
    queue.wait_and_pop(poppedcall);

    ASSERT_EQ(poppedcall.get_phone_number(), "1234567890");
}

TEST(CallsQueueTest, FindIndexTest) {
    CallsQueue queue(5);
    Call call1("1234567890");
    Call call2("9876543210");

    queue.push(call1);
    queue.push(call2);

    size_t index = queue.find_index("1234567890");

    ASSERT_EQ(index, 0);
}

TEST(CallsQueueTest, EraseTest) {
    CallsQueue queue(5);
    Call call1("1234567890");
    Call call2("9876543210");

    queue.push(call1);
    queue.push(call2);

    size_t index = queue.find_index("1234567890");
    queue.erase(index);

    index = queue.find_index("1234567890");

    ASSERT_EQ(index, 5);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}