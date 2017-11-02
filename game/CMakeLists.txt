# td
file(GLOB includefiles
"include/*.hpp"
)

file(GLOB sourcefiles
"source/*.cpp"
)

add_executable(td
        ${includefiles}
        ${sourcefiles}
)

include_directories("thirdparty/include/"
                    "include/")

target_link_libraries(td  
debug "${CMAKE_SOURCE_DIR}/game/thirdparty/lib/debug64/sfml-main-d.lib"
debug "${CMAKE_SOURCE_DIR}/game/thirdparty/lib/debug64/sfml-window-d.lib"
debug "${CMAKE_SOURCE_DIR}/game/thirdparty/lib/debug64/sfml-system-d.lib"
debug "${CMAKE_SOURCE_DIR}/game/thirdparty/lib/debug64/sfml-graphics-d.lib"
debug "${CMAKE_SOURCE_DIR}/game/thirdparty/lib/debug64/sfml-audio-d.lib"
debug "${CMAKE_SOURCE_DIR}/game/thirdparty/lib/debug64/sfml-network-d.lib")

target_link_libraries(td 
optimized "${CMAKE_SOURCE_DIR}/game/thirdparty/lib/release64/sfml-main.lib"
optimized "${CMAKE_SOURCE_DIR}/game/thirdparty/lib/release64/sfml-window.lib"
optimized "${CMAKE_SOURCE_DIR}/game/thirdparty/lib/release64/sfml-system.lib"
optimized "${CMAKE_SOURCE_DIR}/game/thirdparty/lib/release64/sfml-graphics.lib"
optimized "${CMAKE_SOURCE_DIR}/game/thirdparty/lib/release64/sfml-audio.lib"
optimized "${CMAKE_SOURCE_DIR}/game/thirdparty/lib/release64/sfml-network.lib")