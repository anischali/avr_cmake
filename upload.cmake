include(project.cmake)

set(AVRDUDE_EXE avrdude)
set(AVRDUDE_SERIAL_PORT /dev/ttyUSB0)
set(AVR_FIRMWARE_BINARY "${CMAKE_CURRENT_BINARY_DIR}/${AVR_PROJECT_NAME}.hex")

add_custom_target(
    upload
    DEPENDS ${AVR_PROJECT_NAME}.elf
    COMMAND ${AVRDUDE_EXE} -c arduino -P ${AVRDUDE_SERIAL_PORT} -b 115200 -p ${AVR_PROJECT_DEVICE} -U flash:w:${AVR_FIRMWARE_BINARY}:i
    COMMENT "Uploads a firmware to a given MCU"
)

add_custom_target(
    erase
    COMMAND ${AVRDUDE_EXE} -c arduino -P ${AVRDUDE_SERIAL_PORT} -b 115200 -p ${AVR_PROJECT_DEVICE} -e
    COMMENT "Erase a given MCU"
)
