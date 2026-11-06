cmake_minimum_required(VERSION 3.20)
project(libseq VERSION 1.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(LIBSEQ_BUILD_TESTS "Build the tests" ON)

add_library(libseq
    src/bytes.cpp
    src/engine.cpp
    src/timemap.cpp
    src/wrk_read.cpp
    src/wrk_write.cpp
    src/playback.cpp
    src/smf_read.cpp
    src/smf_write.cpp
)
target_include_directories(libseq PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
set(LIBSEQ_WARNINGS $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic -Wshadow -Wold-style-cast -Wconversion -Wsign-conversion>)
target_compile_options(libseq PRIVATE ${LIBSEQ_WARNINGS})
set_target_properties(libseq PROPERTIES OUTPUT_NAME seq) # libseq.a, not liblibseq.a
add_library(libseq::libseq ALIAS libseq)

if(LIBSEQ_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests/runner)
endif()

install(TARGETS libseq EXPORT libseqTargets)
install(DIRECTORY include/ DESTINATION include)
