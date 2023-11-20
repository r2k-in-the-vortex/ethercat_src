/****************************************************************************/
//    ecat_test                                                             //
//    Copyright (C) 2022  Rainer Kordmaa                                    //
//                                                                          //
//    This program is free software: you can redistribute it and/or modify  //
//    it under the terms of the GNU General Public License as published by  //
//    the Free Software Foundation, either version 3 of the License, or     //
//    (at your option) any later version.                                   //
//                                                                          //
//    This program is distributed in the hope that it will be useful,       //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of        //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         //
//    GNU General Public License for more details.                          //
//                                                                          //
//    You should have received a copy of the GNU General Public License     //
//    along with this program.  If not, see <http://www.gnu.org/licenses/>. //
/****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include "xml_conf_parser.h"
#include "ecat_handler.h"
#include "log.h"
#include "ethercat_src.h"
/****************************************************************************/
static char *enableparam;
static char *xmlfilenameparam;
static char *ethercat_device_indexparam;
static char *verboseparam;
static char *configonlyparam;
static type_logger_callback openplc_logger = NULL;
static EcatConfig config;

int log_callback(char *message){ 
    if(openplc_logger != NULL)openplc_logger(message);
    return 0;
}

int checkforparam(char *line, char *prefix, char **dataout){
    int len = strlen(prefix);
    if(!strncmp(line, prefix, len)){
        char *value = line+len*sizeof(char);
        char *term = '\0';
        int valuelen = strlen(value);
        char *buffer;
        buffer = malloc(sizeof(char) * (valuelen+1)*sizeof(char));
        if(buffer == NULL){
            log_error("malloc failes");
        }
        if(valuelen){
            buffer[0] = 0;
            strncat(buffer, value, valuelen-1);
            buffer[valuelen] = 0;
        }
        dataout[0] = buffer;
    }
    return 0;
}

int parseparam(char *line){
    int len = strlen(line);
    if(len == 0)return 0;
    if(line[0] == '#')return 0;
    checkforparam(line, "enable=", &enableparam);
    checkforparam(line, "xmlfilename=", &xmlfilenameparam);
    checkforparam(line, "ethercat_device_index=", &ethercat_device_indexparam);
    checkforparam(line, "verbose=", &verboseparam);
    checkforparam(line, "configonly=", &configonlyparam);
    return 0;
}

int ethercat_configure_byxml(char *xmlfilename, int ethercat_device_index, int verbose, int configonly)
{
    int ret = 0;
    
    log_set_level(1);
    // set verbose output DEBUG
    if(verbose){
        log_trace("TRACE output");
        log_set_level(0);
    } 

    log_trace("ai.filename_arg: %s", xmlfilename);
    log_trace("ai.verbose_flag: %d", verbose);
    log_trace("ai.confonly_flag: %d", configonly);
    if (!ethercat_device_index)ethercat_device_index = 0;
    log_trace("ai.index_arg: %d", ethercat_device_index);
    
    
    config.master_index = ethercat_device_index;
    config.config_only_flag = configonly;
    
    if(parse_xml_config(xmlfilename, &config)){
        log_error("failed to parse configuration file");
        return -1;
    }

    EtherCATinit(&config, &log_callback);
    return ret;
}
int ethercat_configure(char *paramsfile, type_logger_callback logfptr){
    if(paramsfile == NULL){
        log_error("Params file NULL");
    }
    openplc_logger = logfptr;
    log_trace("reading file '%s'", paramsfile);
    log_set_level(0);
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    fp = fopen(paramsfile,"r");

    if(fp){
        while ((read = getline(&line, &len, fp)) != -1) {
            parseparam(line);
        }
        free(line);
    }
    else{
        log_error("Failed to open file '%s'", paramsfile);
        return -1;
    }
    fclose(fp);

    int ethercat_device_index = atoi(ethercat_device_indexparam);
    int verbose = atoi(verboseparam);
    int configonly = atoi(configonlyparam);
    int enable = atoi(enableparam);
    if (!enable)return 0;
    
    log_trace("xmlfilenameparam: %s", xmlfilenameparam);
    log_trace("ethercat_device_indexparam: %s", ethercat_device_indexparam);
    log_trace("verboseparam: %s", verboseparam);
    log_trace("configonlyparam: %s", configonlyparam);
    
    ethercat_configure_byxml(xmlfilenameparam, ethercat_device_index, verbose, configonly);
    free(enableparam);
    free(xmlfilenameparam);
    free(ethercat_device_indexparam);
    free(verboseparam);
    free(configonlyparam);
    return 0;
}

int ethercat_callcyclic(int buffersize, 
    boolvar_call_back bool_input, 
    boolvar_call_back bool_output, 
    int8var_call_back byte_input, 
    int8var_call_back byte_output, 
    int16var_call_back word_input, 
    int16var_call_back word_output, 
    int32var_call_back dword_input, 
    int32var_call_back dword_output, 
    int64var_call_back lword_input, 
    int64var_call_back lword_output){
    return EtherCATcyclic(buffersize, bool_input, bool_output, byte_input, byte_output, word_input, word_output, dword_input, dword_output, lword_input, lword_output);
}

int ethercat_read_sdo(){
    printf("calling ethercat_read_sdo()\n");
    return 7;
}

int ethercat_write_sdo(){
    printf("calling ethercat_write_sdo()\n");
    return 7;
}

/* Close all resources */
void ethercat_terminate_src(){
    terminate_handler();
    terminate_xml_parsed_conf();
}
/****************************************************************************/