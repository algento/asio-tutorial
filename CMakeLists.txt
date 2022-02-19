cmake_minimum_required(VERSION 3.10)
project(asio-tutorial)

## Compiler 설정 ---------------------------------------------------------------#
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for clangd
set(CMAKE_CXX_EXTENSIONS OFF) # disable compiler extension
set(CMAKE_BUILD_TYPE Debug)

## Variables ------------------------------------------------------------------#
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/modules)

## Dependency -----------------------------------------------------------------#
find_package(ASIO REQUIRED)

## target_binary --------------------------------------------------------------#
# Timer 1. Using a timer synchronoously
add_executable(sync_timer src/sync_timer.cpp)
target_include_directories(sync_timer PUBLIC ${ASIO_INCLUDE_DIR})

# Timer 2. Using a timer asynchronoously
add_executable(async_timer src/async_timer.cpp)
target_include_directories(async_timer PUBLIC ${ASIO_INCLUDE_DIR})

# Timerl 3. Binding arguments to handler
add_executable(bind_arguments_to_handler src/bind_arguments_to_handler.cpp)
target_include_directories(bind_arguments_to_handler PUBLIC ${ASIO_INCLUDE_DIR})

# Timer 4. Using a member function as a handler
add_executable(member_func_as_handler src/member_func_as_handler.cpp)
target_include_directories(member_func_as_handler PUBLIC ${ASIO_INCLUDE_DIR})

# Timer 5. Synchronising handlers in multithreaded programs
add_executable(sync_handler_in_multi_thread src/sync_handler_in_multi_thread.cpp)
target_include_directories(sync_handler_in_multi_thread PUBLIC ${ASIO_INCLUDE_DIR})

# Daytime. 1 - A synchronous TCP daytime client
add_executable(sync_tcp_daytime_client src/sync_tcp_daytime_client.cpp)
target_include_directories(sync_tcp_daytime_client PUBLIC ${ASIO_INCLUDE_DIR})

# Daytime. 2 - A synchronous TCP daytime server
add_executable(sync_tcp_daytime_server src/sync_tcp_daytime_server.cpp)
target_include_directories(sync_tcp_daytime_server PUBLIC ${ASIO_INCLUDE_DIR})

# Daytime. 3 - A asynchronous TCP daytime server
add_executable(async_tcp_daytime_server src/async_tcp_daytime_server.cpp)
target_include_directories(async_tcp_daytime_server PUBLIC ${ASIO_INCLUDE_DIR})

add_executable(tcp_server src/tcp_server.cpp)
target_include_directories(tcp_server PUBLIC ${ASIO_INCLUDE_DIR})

add_executable(tcp_client src/tcp_client.cpp)
target_include_directories(tcp_client PUBLIC ${ASIO_INCLUDE_DIR})

add_executable(async_tcp_server src/async_tcp_server.cpp)
target_include_directories(async_tcp_server PUBLIC ${ASIO_INCLUDE_DIR})

