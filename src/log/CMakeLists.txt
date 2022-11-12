set(LIB_NAME log)

set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall --std=c++11 -O3 -fPIC" )
set( CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wall -O3 -fPIC" )
add_library(${LIB_NAME} "")

target_sources(${LIB_NAME}
  PRIVATE
    log.c
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/log.h
  )

target_include_directories(${LIB_NAME}
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )