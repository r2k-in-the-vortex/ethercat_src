add_library(${PROJECT_NAME} SHARED ethercat_src.c)
add_executable(${PROJECT_NAME}_test main.c)

set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(xml_conf_parser)
add_subdirectory(ecat_handler)
add_subdirectory(log)

target_sources(${PROJECT_NAME}
  PRIVATE
    ethercat_src.c
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/ethercat_src.h
  )

target_include_directories(${PROJECT_NAME}
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    xml_conf_parser
    ecat_handler
    log
)
target_link_libraries(${PROJECT_NAME}_test
  PRIVATE
    ${PROJECT_NAME}
    xml_conf_parser
    ecat_handler
    log
)

install(TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})