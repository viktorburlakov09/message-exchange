cmake_minimum_required(VERSION 3.20)


project(TestApp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(
    consumer_app 
    src/consumer/cli.cpp 
    src/consumer/consumer.cpp
)
target_include_directories(consumer_app PRIVATE src/consumer)

add_executable(
    producer_app 
    src/producer/cli.cpp 
    src/producer/producer.cpp
)
target_include_directories(producer_app PRIVATE src/producer)
