#include <gtest/gtest.h>

#include "operators_manager.hpp"
#include "call.hpp"
#include "config.hpp"
#include "cdr.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio.hpp>

#include <string>


namespace net = boost::asio;
namespace posix_time = boost::posix_time;

class OperatorsManagerTest : public ::testing::Test {
protected:
    std::jthread worker;
    net::io_context io_context;
    OperatorsManager operators_manager{io_context, 1, 10};
public:
    OperatorsManagerTest() {
        std::jthread thr([this]{io_context.run();});
        worker = std::move(thr);
        CDR::instance().initialize("tests/test_cdr.txt");
        operators_manager.start();
    }
    ~OperatorsManagerTest() {
        operators_manager.stop();
        io_context.stop();
    }

};


TEST_F(OperatorsManagerTest, HandleCallWithQueueFull) {
    Config::instance().initialize("tests/test_configs/config_with_cdh_off.json");

    for (int i = 0; i < 15; i++) {
        Call call(std::to_string(i));
        operators_manager.handle_call(call);
    }
    Call call("1234567891");

    ASSERT_FALSE(operators_manager.handle_call(call));
    ASSERT_EQ(CDR::instance().get_last_status(), CDR::Status::OVERLOAD);

}

TEST_F(OperatorsManagerTest, HandleCallWithDuplicateHandlingOn) {
    Config::instance().initialize("tests/test_configs/config_with_cdh_on.json");

    Call call("1234567890");
    

    ASSERT_TRUE(operators_manager.handle_call(call));
    ASSERT_TRUE(operators_manager.handle_call(call));
    ASSERT_EQ(CDR::instance().get_last_status(), CDR::Status::CALL_DUPLICATION);



}

TEST_F(OperatorsManagerTest, HandleCallWithDuplicateHandlingoff) {
    Config::instance().initialize("tests/test_configs/config_with_cdh_off.json");

    Call call("1234567890");
    
    ASSERT_TRUE(operators_manager.handle_call(call));
    ASSERT_FALSE(operators_manager.handle_call(call));
    ASSERT_EQ(CDR::instance().get_last_status(), CDR::Status::ALREADY_IN_QUEUE);


}



int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
