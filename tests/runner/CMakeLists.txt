# The script runner and the oracle tests: every suite listed in suites101.txt runs against
# libseq and must print exactly what TTSSEQ.DLL 1.01 printed (tests/data/ttsseq/101).
add_executable(ttsrun main.cpp probe.cpp api101.cpp)
target_link_libraries(ttsrun PRIVATE libseq)
target_compile_options(ttsrun PRIVATE ${LIBSEQ_WARNINGS})

set(TTSSEQ_DATA ${CMAKE_SOURCE_DIR}/tests/data/ttsseq/101)
file(STRINGS suites101.txt SUITES REGEX "^[a-zA-Z_0-9]+")
foreach(suite IN LISTS SUITES)
    add_test(NAME oracle101_${suite}
             COMMAND ttsrun --songs ${CMAKE_SOURCE_DIR}/tests/data/songs
                     ${TTSSEQ_DATA}/${suite}.txt ${TTSSEQ_DATA}/${suite}.out)
endforeach()
