#include "ecat_handler.h"

#ifdef __cplusplus
extern "C"{
#endif 

/*
 * XML file in, config out
 */
int parse_xml_config(char *filename, EcatConfig *config);
void terminate_xml_parsed_conf();


#ifdef __cplusplus
}
#endif