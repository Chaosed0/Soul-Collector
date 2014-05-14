cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")

project(Soul-Collector)

# Version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 1)

file(GLOB src
	"SCollector/SCollector/*.h"
	"SCollector/SCollector/*.cpp"
)

if(CMAKE_BUILD_TYPE MATCHES Debug)
	set(SFML_SUFFIX -d)
endif(CMAKE_BUILD_TYPE MATCHES Debug)

add_executable(SCollector ${src})
target_link_libraries(SCollector sfml-window${SFML_SUFFIX})
target_link_libraries(SCollector sfml-system${SFML_SUFFIX})
target_link_libraries(SCollector sfml-graphics${SFML_SUFFIX})
target_link_libraries(SCollector sfml-audio${SFML_SUFFIX})
target_link_libraries(SCollector sfgui)
target_link_libraries(SCollector tmxparser)
target_link_libraries(SCollector z)
target_link_libraries(SCollector tinyxml)
