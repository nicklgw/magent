cmake_minimum_required(VERSION 2.6)

PROJECT(mosquitto-uloop C)
ADD_DEFINITIONS(-Os -ggdb -Wall -Werror --std=gnu99 -Wmissing-declarations)

SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

SET(SOURCES main.c mosq.c ubus.c utils.c)

SET(LIBS ubox ubus blobmsg_json json-c)

ADD_EXECUTABLE(mosquitto-uloop ${SOURCES})

TARGET_LINK_LIBRARIES(mosquitto-uloop ${LIBS})

INSTALL(TARGETS mosquitto-uloop RUNTIME DESTINATION bin)

