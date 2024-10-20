file(GLOB DRIVERS_GPIO_AVR_SOURCES "${CMAKE_CURRENT_LIST_DIR}/**.c")

set(DRIVERS_GPIO_AVR_HEADERS "${CMAKE_CURRENT_LIST_DIR}")

target_include_directories(${TARGET_BINARY} PUBLIC
    ${DRIVERS_GPIO_AVR_HEADERS}
)

set(drivers_avr_gpio_SOURCES
    ${DRIVERS_GPIO_AVR_SOURCES}
)
    
target_sources(${TARGET_BINARY} PRIVATE
    ${drivers_avr_gpio_SOURCES}
)
