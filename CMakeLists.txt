set(SOURCES
    consul_client.h
    consul_client.cpp
    service_info.h
    service_info.cpp
)

include_directories("${PROJECT_SOURCE_DIR}/src")

# libhv
set(HV_LIBRARY /usr/local/lib/libhv.so)

add_library(consul-cpp ${SOURCES})

target_include_directories(consul-cpp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(consul-cpp PUBLIC
    ${HV_LIBRARY}
)
