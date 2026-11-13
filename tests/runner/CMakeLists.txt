# The script runner and the oracle tests: every suite listed in suites.txt runs against
# libseq and must print exactly what TTSSEQ.DLL 2.10 printed (tests/data/ttsseq/210), and send the
# same MIDI (SUITE.midi; none when there is no such file).
add_executable(ttsrun main.cpp probe.cpp api.cpp api_events.cpp api_files.cpp api_marks.cpp api_play.cpp api_record.cpp api_time.cpp)
target_link_libraries(ttsrun PRIVATE libseq)
target_compile_options(ttsrun PRIVATE ${LIBSEQ_WARNINGS})

set(TTSSEQ_DATA ${CMAKE_SOURCE_DIR}/tests/data/ttsseq/210)
file(STRINGS suites.txt SUITES REGEX "^[a-zA-Z_0-9]+")
foreach(suite IN LISTS SUITES)
    add_test(NAME oracle_${suite}
             COMMAND ttsrun --songs ${CMAKE_SOURCE_DIR}/tests/data/songs
                     --out ${CMAKE_CURRENT_BINARY_DIR}/out/${suite} --files ${TTSSEQ_DATA}/${suite}
                     --midi ${TTSSEQ_DATA}/${suite}.midi
                     ${TTSSEQ_DATA}/${suite}.txt ${TTSSEQ_DATA}/${suite}.out)
endforeach()
