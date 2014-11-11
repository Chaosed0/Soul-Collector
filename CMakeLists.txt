cmake_minimum_required(VERSION 2.6)

project(Soul-Collector)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules/ ${CMAKE_MODULE_PATH})

find_package(SFML 2.1 REQUIRED system window graphics audio)
find_package(SFGUI REQUIRED)
find_package(ZLIB)
find_package(TinyXML)
find_package(TmxParser)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 1)

if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

file(GLOB src
	"SCollector/SCollector/*.h"
	"SCollector/SCollector/*.cpp"
)

add_executable(SCollector ${src})

include_directories(${SFML_INCLUDE_DIR} ${SFGUI_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR} ${TINYXML_INCLUDE_DIR} ${TMXPARSER_INCLUDE_DIR})
target_link_libraries(SCollector ${SFML_LIBRARIES} ${SFGUI_LIBRARY} ${ZLIB_LIBRARIES} ${TINYXML_LIBRARY} ${TMXPARSER_LIBRARY})
