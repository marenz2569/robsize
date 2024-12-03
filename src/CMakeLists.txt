add_executable(robsize
    robsize/Main.cpp
    robsize/Config.cpp
    )

target_link_libraries(robsize
    asmjit::asmjit
    )