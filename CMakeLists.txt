cmake_minimum_required(VERSION 3.0.0)

include(${CMAKE_CURRENT_SOURCE_DIR}/avr-toolchain.cmake)

include(${CMAKE_CURRENT_SOURCE_DIR}/project.cmake)


project(${AVR_PROJECT_NAME} VERSION ${AVR_PROJECT_VERSION})

enable_language(ASM C CXX)

add_executable(${AVR_PROJECT_NAME}.elf
    ${AVR_PROJECT_SOURCES}
)


add_custom_command(
		TARGET ${AVR_PROJECT_NAME}.elf POST_BUILD
		COMMAND ${CMAKE_OBJCOPY} -O ihex ${AVR_PROJECT_NAME}.elf ${AVR_PROJECT_NAME}.hex
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generate .hex extension file firmware")

add_custom_command(
        TARGET ${AVR_PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary ${AVR_PROJECT_NAME}.elf ${AVR_PROJECT_NAME}.bin
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generate .bin extension file firmware")

add_custom_command(
        TARGET ${AVR_PROJECT_NAME}.elf 
        COMMAND ${CMAKE_OBJCOPY} -O binary ${AVR_PROJECT_NAME}.elf ${AVR_PROJECT_NAME}.bin
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generate .bin extension file firmware")

SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
include(upload.cmake)