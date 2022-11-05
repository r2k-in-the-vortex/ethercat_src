add_executable(${PROJECT_NAME} main.c)

add_subdirectory(parser)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    parser
  )