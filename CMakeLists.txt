cmake_minimum_required(VERSION 2.8.12)

project (QELab_cable_testing)

#Include header files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src
					)

set(SOURCES main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/serial_port.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/cableTesting.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/button.cpp
	)

add_executable(QELab_cable_testing ${SOURCES})				

find_library(wiringPi_LIB wiringPi)
find_package( Threads )

target_link_libraries(QELab_cable_testing
                      ${CMAKE_THREAD_LIBS_INIT}
                     ${wiringPi_LIB}
                      )