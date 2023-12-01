# Documentation 
Полный отчет по проделанной работе находится в /docs

# Requirements
* CMake
* conan 1.61.0

# Build
(From project-directory)
```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ../
cmake --build ./src
cmake --build ./tests
```

# Execution
(From build-directory)
* Service
```bash
./src/ProteiCourseWork ./src/config.json
```
* Tests
```bash
./tests/ts_queue_test
./tests/calls_queue_test 
./tests/operators_manager_test 
```