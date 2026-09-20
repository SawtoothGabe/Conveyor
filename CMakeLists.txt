cmake_minimum_required(VERSION 4.1)
project(Conveyor)

set(CMAKE_CXX_STANDARD 26)

include(FetchContent)

FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG        asio-1-30-0
    GIT_SHALLOW    TRUE
)
set(ASIO_NO_HANDLER_AS_HANDLER ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(asio)

file(GLOB_RECURSE SOURCES "Source/*.cpp" "Source/*.hpp")

add_executable(Conveyor ${SOURCES})

target_include_directories(Conveyor PRIVATE "Source" ${asio_SOURCE_DIR}/asio/include)
target_compile_definitions(Conveyor PRIVATE ASIO_NO_HANDLER_AS_HANDLER)

if(WIN32)
    set(OPTIONAL_LIBS ws2_32 wsock32)
endif ()

find_package(OpenSSL REQUIRED)

target_link_libraries(Conveyor PRIVATE stdc++exp OpenSSL::SSL OpenSSL::Crypto ${OPTIONAL_LIBS})
