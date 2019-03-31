cmake_minimum_required (VERSION 2.8)

project(test)

aux_source_directory("../" DIR_SRC)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -Wall")

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})
include_directories("../")
include_directories("/usr/local/include")
include_directories("/usr/local/Cellar/openssl/1.0.2q/include")

link_directories("/usr/local/lib")
link_directories("/usr/local/Cellar/openssl/1.0.2q/lib")

add_executable(keytest test_sign_verify.cc ${DIR_SRC})
target_link_libraries(keytest ${GTEST_BOTH_LIBRARIES})
target_link_libraries(keytest ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(keytest secp256k1)
target_link_libraries(keytest ssl)
target_link_libraries(keytest crypto)

add_test(Test keytest)
enable_testing()
