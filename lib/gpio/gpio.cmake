file(GLOB DRIVERS_GPIO_SOURCES "${CMAKE_CURRENT_LIST_DIR}/**.c")

if(DEFINED DRIVERS_GPIO_AVR)
include(${CMAKE_CURRENT_LIST_DIR}/avr/avr-gpio.cmake)
endif()

set(DRIVERS_GPIO_HEADERS "${CMAKE_CURRENT_LIST_DIR}")

target_include_directories(${TARGET_BINARY} PUBLIC
    ${DRIVERS_GPIO_HEADERS}
)

set(drivers_gpio_SOURCES
    ${DRIVERS_GPIO_SOURCES}
)
    
target_sources(${TARGET_BINARY} PRIVATE
    ${drivers_gpio_SOURCES}
)
