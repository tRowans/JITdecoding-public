add_executable(JITdecoderToric mainToric.cpp decoderToric.cpp toricCodeSlice.cpp decoderGeneric.cpp)
add_executable(JITdecoderRhombic1 mainRhombic1.cpp decoderRhombic1.cpp rhombic1.cpp decoderGeneric.cpp)
add_executable(JITdecoderRhombic2 mainRhombic2.cpp decoderRhombic2.cpp rhombic2.cpp decoderGeneric.cpp)

add_library(JITdecoderToric_lib decoderToric.cpp toricCodeSlice.cpp decoderGeneric.cpp)
add_library(JITdecoderRhombic1_lib decoderRhombic1.cpp rhombic1.cpp decoderGeneric.cpp)
add_library(JITdecoderRhombic2_lib decoderRhombic2.cpp rhombic2.cpp decoderGeneric.cpp)

include_directories(../lib/blossom5)

target_link_libraries(JITdecoderToric PUBLIC PerfectMatching)
target_link_libraries(JITdecoderRhombic1 PUBLIC PerfectMatching)
target_link_libraries(JITdecoderRhombic2 PUBLIC PerfectMatching)
