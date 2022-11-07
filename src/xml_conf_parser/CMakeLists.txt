set(LIB_NAME xml_conf_parser)

add_library(${LIB_NAME} "")

target_sources(${LIB_NAME}
  PRIVATE
    xml_conf_parser.c
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/xml_conf_parser.h
)
  


FIND_PACKAGE(LibXml2 ${DEPS_HINTS})
IF(NOT LIBXML2_LIBRARIES)
    FIND_LIBRARY(LibXml2 REQUIRED ${DEPS_HINTS})
ENDIF()

target_include_directories(${LIB_NAME}
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(${LIB_NAME}
  LibXml2::LibXml2
  log
  ecat_handler
)