add_executable(robsize
    robsize/Main.cpp
    robsize/Robsize.cpp
    robsize/CacheMissTest.cpp
    robsize/Config.cpp
    )

target_link_libraries(robsize
    asmjit::asmjit
    )