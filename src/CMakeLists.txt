add_library(robsizecore STATIC
    robsize/Robsize.cpp
    robsize/CacheMissTest.cpp
    robsize/Config.cpp
    )

add_executable(robsize
    robsize/Main.cpp
    )

target_link_libraries(robsizecore
    asmjit::asmjit
    )

target_link_libraries(robsize
    robsizecore
    )