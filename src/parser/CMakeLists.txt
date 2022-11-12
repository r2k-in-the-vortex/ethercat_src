set(LIB_NAME parser)

add_library(${LIB_NAME} "")

target_sources(${LIB_NAME}
  PRIVATE
    cmdline.c
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/cmdline.h
  )

target_include_directories(${LIB_NAME}
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )


  