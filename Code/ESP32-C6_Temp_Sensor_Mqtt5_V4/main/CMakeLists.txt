idf_component_register(
    SRCS 
        "app_main.c"
        "io/gpio_init.c"
        "io/i2c_init.c"
        "mqtt/mqtt.c"
        "mqtt/wifi.c"
        "sht/measure_sensor.c"

                    
                    
    INCLUDE_DIRS 
        "."
        "../includes/"
        "../includes/io/"
        "../includes/mqtt/"
        "../includes/sht/"
)
