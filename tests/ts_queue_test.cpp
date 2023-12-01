#include <gtest/gtest.h>

#include <thread>

#include "ts_queue.hpp"

TEST(TSQueueTest, SizeTest) {
    TSQueue<int> queue(10);
    size_t size = queue.get_size();
    ASSERT_EQ(10, size);
}

TEST(TSQueueTest, FullTest) {
    TSQueue<int> queue(3);
    queue.push(1);
    queue.push(2);
    queue.push(3);
    ASSERT_TRUE(queue.is_full_protected());
}

TEST(TSQueueTest, EmptyAndDoneTest) {
    TSQueue<std::string> queue(3);
    ASSERT_FALSE(queue.is_empty_and_done());

    queue.wake_and_done();
    ASSERT_TRUE(queue.is_empty_and_done());
}

TEST(TSQueueTest, PushPopTest) {
    TSQueue<int> queue(3);
    queue.push(1);
    queue.push(2);
    queue.push(3);

    int data;
    ASSERT_TRUE(queue.wait_and_pop(data));
    ASSERT_EQ(1, data);

    ASSERT_TRUE(queue.wait_and_pop(data));
    ASSERT_EQ(2, data);

    ASSERT_TRUE(queue.wait_and_pop(data));
    ASSERT_EQ(3, data);
}

TEST(TSQueueTest, EmptyWaitTest) {
    TSQueue<char> queue(2);
    std::jthread producer([&queue](){
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        queue.push('a');
        queue.push('b');
        queue.wake_and_done();
    });

    char data;
    ASSERT_TRUE(queue.wait_and_pop(data));
    ASSERT_EQ('a', data);

    ASSERT_TRUE(queue.wait_and_pop(data));
    ASSERT_EQ('b', data);

    ASSERT_FALSE(queue.wait_and_pop(data));
}

TEST(TSQueueTest, FullWaitTest) {
    TSQueue<char> queue(2);
    std::jthread producer([&queue](){
        queue.push('a');
        queue.push('b');
        queue.wake_and_done();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    char data;
    ASSERT_TRUE(queue.wait_and_pop(data));
    ASSERT_EQ('a', data);

    ASSERT_TRUE(queue.wait_and_pop(data));
    ASSERT_EQ('b', data);

    ASSERT_FALSE(queue.wait_and_pop(data));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}