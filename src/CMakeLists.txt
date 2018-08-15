cmake_minimum_required(VERSION 2.6)

PROJECT(magent C)
ADD_DEFINITIONS(-Os -ggdb -Wall -Werror --std=gnu99 -Wmissing-declarations)
ADD_DEFINITIONS("-DWITH_TLS") 

SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

SET(SOURCES main.c mosq.c ubus.c utils.c)

SET(LIBS ubox ubus blobmsg_json json-c)

ADD_EXECUTABLE(magent ${SOURCES})

TARGET_LINK_LIBRARIES(magent ${LIBS})

INSTALL(TARGETS magent RUNTIME DESTINATION bin)

