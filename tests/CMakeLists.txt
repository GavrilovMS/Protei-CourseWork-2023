cmake_minimum_required(VERSION 3.0.0)
project(ProteiCourseWork VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

configure_file(${CMAKE_SOURCE_DIR}/tests/test_configs/config_with_cdh_off.json ${CMAKE_BINARY_DIR}/tests/test_configs/config_with_cdh_off.json COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/tests/test_configs/config_with_cdh_on.json ${CMAKE_BINARY_DIR}/tests/test_configs/config_with_cdh_on.json COPYONLY)

set(target_name ts_queue_test)

add_executable(${target_name} ts_queue_test.cpp)

target_link_libraries(${target_name}
    PRIVATE
        GTest::gtest
)

target_include_directories(${target_name}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src/operators_manager/calls_queue
)



set(target_name calls_queue_test)

set(source_list
    calls_queue_test.cpp
    ${CMAKE_SOURCE_DIR}/src/operators_manager/calls_queue/calls_queue.hpp
    ${CMAKE_SOURCE_DIR}/src/operators_manager/calls_queue/calls_queue.cpp
    ${CMAKE_SOURCE_DIR}/src/operators_manager/calls_queue/call.hpp
    ${CMAKE_SOURCE_DIR}/src/operators_manager/calls_queue/call.cpp
    ${CMAKE_SOURCE_DIR}/src/config/config.hpp
    ${CMAKE_SOURCE_DIR}/src/config/config.cpp
    )

add_executable(${target_name} ${source_list})

target_link_libraries(${target_name}
    PRIVATE
        GTest::gtest
        Boost::system
        spdlog::spdlog 
        nlohmann_json::nlohmann_json 
)


target_include_directories(${target_name}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src/operators_manager/calls_queue
        ${CMAKE_SOURCE_DIR}/src/config
)



set(target_name operators_manager_test)

set(source_list
    operators_manager_test.cpp
    ${CMAKE_SOURCE_DIR}/src/operators_manager/operators_manager.hpp
    ${CMAKE_SOURCE_DIR}/src/operators_manager/operators_manager.cpp
    ${CMAKE_SOURCE_DIR}/src/operators_manager/operator.hpp
    ${CMAKE_SOURCE_DIR}/src/operators_manager/operator.cpp
    ${CMAKE_SOURCE_DIR}/src/operators_manager/calls_queue/calls_queue.hpp
    ${CMAKE_SOURCE_DIR}/src/operators_manager/calls_queue/calls_queue.cpp
    ${CMAKE_SOURCE_DIR}/src/operators_manager/calls_queue/call.hpp
    ${CMAKE_SOURCE_DIR}/src/operators_manager/calls_queue/call.cpp
    ${CMAKE_SOURCE_DIR}/src/config/config.hpp
    ${CMAKE_SOURCE_DIR}/src/config/config.cpp
    ${CMAKE_SOURCE_DIR}/src/cdr/cdr.hpp
    ${CMAKE_SOURCE_DIR}/src/cdr/cdr.cpp
    ${CMAKE_SOURCE_DIR}/src/io_manager/io_manager.hpp
    ${CMAKE_SOURCE_DIR}/src/io_manager/io_manager.cpp
    )

add_executable(${target_name} ${source_list})

target_link_libraries(${target_name}
    PRIVATE
        GTest::gtest
        Boost::system
        spdlog::spdlog 
        nlohmann_json::nlohmann_json 
)


target_include_directories(${target_name}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src/operators_manager/calls_queue
        ${CMAKE_SOURCE_DIR}/src/operators_manager
        ${CMAKE_SOURCE_DIR}/src/cdr
        ${CMAKE_SOURCE_DIR}/src/config
)

