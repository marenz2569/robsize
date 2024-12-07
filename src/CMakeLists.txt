# Core library
add_library(robsizecore STATIC
    robsize/Robsize.cpp
    robsize/CacheMissTest.cpp
    robsize/Config.cpp
    )

target_link_libraries(robsizecore
    asmjit::asmjit
    )

# robsize executable
add_executable(robsize
    robsize/Main.cpp
    )

target_link_libraries(robsize
    Threads::Threads
    robsizecore
    )