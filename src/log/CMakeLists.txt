set(LIB_NAME log)

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