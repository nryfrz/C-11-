cmake_minimum_required(VERSION 3.0.0)
project(main VERSION 0.1.0 LANGUAGES C CXX)



add_executable(main main.cpp)

target_link_libraries(main pthread)
target_link_libraries(main mysqlclient)

include(CTest)
enable_testing()

