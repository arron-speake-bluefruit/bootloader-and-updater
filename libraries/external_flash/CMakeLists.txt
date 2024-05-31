project(external_flash)

add_library(${PROJECT_NAME} OBJECT
    "${PROJECT_SOURCE_DIR}/source/external_flash.c"
    "${PROJECT_SOURCE_DIR}/source/microchip_sst25vf080b.c"
)
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(${PROJECT_NAME} drivers)
