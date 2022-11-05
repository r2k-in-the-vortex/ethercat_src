add_executable(${PROJECT_NAME} main.c)

add_subdirectory(parser)
add_subdirectory(xml_conf_parser)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    parser
    xml_conf_parser
  )