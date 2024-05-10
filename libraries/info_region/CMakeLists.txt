project(info_region)

add_library(${PROJECT_NAME} OBJECT "${PROJECT_SOURCE_DIR}/info_region.c")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(${PROJECT_NAME} flash_copy)
