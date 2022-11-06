add_executable(${PROJECT_NAME} main.c)

set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(parser)
add_subdirectory(xml_conf_parser)
add_subdirectory(ecat_handler)
add_subdirectory(log)


target_link_libraries(${PROJECT_NAME}
  PRIVATE
    parser
    xml_conf_parser
    ecat_handler
    log
  )