add_executable(hello_dormant_gpio
        hello_dormant_gpio.c
        )

target_link_libraries(hello_dormant_gpio pico_stdlib hardware_sleep)

# create map/bin/hex file etc.
pico_add_extra_outputs(hello_dormant_gpio)