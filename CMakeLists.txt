cmake_minimum_required(VERSION 3.0.0)
project(ProteiCourseWork VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(BUILD_SHARED_LIBS OFF)

include(${CMAKE_SOURCE_DIR}/cmake/conan.cmake)

conan_cmake_autodetect(settings)
conan_cmake_install(
    PATH_OR_REFERENCE ${CMAKE_SOURCE_DIR}
    INSTALL_FOLDER ${CMAKE_BINARY_DIR}
    BUILD missing
    REMOTE conancenter
    SETTINGS ${settings}
    ENV "CONAN_CMAKE_GENERATOR=${CMAKE_GENERATOR}"
    CONF "tools.cmake.cmaketoolchain:generator=${CMAKE_GENERATOR}"
)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

find_package(Boost 1.81.0 EXACT REQUIRED)
find_package(spdlog 1.12.0 EXACT REQUIRED)
find_package(nlohmann_json 3.11.2 EXACT REQUIRED)
find_package(GTest 1.14.0 EXACT REQUIRED)

add_subdirectory(src)