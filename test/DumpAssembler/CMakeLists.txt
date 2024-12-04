add_executable(DumpAssembler Main.cpp)
target_link_libraries(DumpAssembler robsizecore)

add_ref_test(NAME DumpAssembler)