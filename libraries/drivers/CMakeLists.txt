project(drivers)

add_library(${PROJECT_NAME} OBJECT
    "${PROJECT_SOURCE_DIR}/source/flash_copy.c"
    "${PROJECT_SOURCE_DIR}/source/flash.c"
    "${PROJECT_SOURCE_DIR}/source/gpio.c"
    "${PROJECT_SOURCE_DIR}/source/nvic.c"
    "${PROJECT_SOURCE_DIR}/source/rcc.c"
    "${PROJECT_SOURCE_DIR}/source/scb.c"
    "${PROJECT_SOURCE_DIR}/source/syscfg.c"
    "${PROJECT_SOURCE_DIR}/source/tim.c"
    "${PROJECT_SOURCE_DIR}/source/usart.c"
)

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")
