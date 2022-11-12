set(LIB_NAME ecat_handler)

cmake_policy(SET CMP0079 NEW)

add_library(${LIB_NAME} "")

target_sources(${LIB_NAME}
  PRIVATE
    ecat_handler.c
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/ecat_handler.h
  )

target_include_directories(${LIB_NAME}
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )

# Add include directories for other libraries
set( INCLUDE_DIRS 
    /usr/local/src/ethercat/include     # EtherLAB EtherCAT master
  )

set (LINK_LIBS ethercat                 # EtherLAB EtherCAT master user API
  )

include_directories( ${INCLUDE_DIRS} )
target_link_directories(${LIB_NAME} PRIVATE 
  /usr/local/src/ethercat/lib
  /usr/local/lib
  )
target_link_libraries (${LIB_NAME} 
  PRIVATE
    ${LINK_LIBS} 
    log
  )