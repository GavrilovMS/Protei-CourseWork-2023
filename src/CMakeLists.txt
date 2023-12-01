cmake_minimum_required(VERSION 3.0.0)
project(ProteiCourseWork VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(target_name ${PROJECT_NAME})
set(source_list
    main.cpp
    service.hpp
    service.cpp
    config/config.hpp
    config/config.cpp
    cdr/cdr.hpp
    cdr/cdr.cpp
    operators_manager/calls_queue/calls_queue.hpp
    operators_manager/calls_queue/calls_queue.cpp
    operators_manager/calls_queue/ts_queue.hpp
    operators_manager/calls_queue/call.hpp
    operators_manager/calls_queue/call.cpp
    operators_manager/operators_manager.hpp
    operators_manager/operators_manager.cpp
    operators_manager/operator.hpp
    operators_manager/operator.cpp
    controller/controller.hpp
    controller/controller.cpp
    io_manager/io_manager.hpp
    io_manager/io_manager.cpp
)

add_executable(${target_name} ${source_list})

target_link_libraries(${target_name}
    PRIVATE
        Boost::system
        spdlog::spdlog 
        nlohmann_json::nlohmann_json
        GTest::gtest 
)

target_include_directories(${target_name}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        cdr
        config
        io_manager
        operators_manager
        operators_manager/calls_queue
        controller
        utils
)

configure_file(${CMAKE_SOURCE_DIR}/configs/config.json ${CMAKE_BINARY_DIR}/src/config.json COPYONLY)