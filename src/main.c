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
#include "cmdline.h"
#include "xml_conf_parser.h"
#include "ecat_handler.h"
#include "log.h"
#include "ethercat_src.h"

#define BUFFER_SIZE 1024
/****************************************************************************/
u_int8_t shortvar = 0;
u_int16_t longvar = 0;
u_int8_t *bool_input_call_back(int a, int b){ return &shortvar; }
u_int8_t *bool_output_call_back(int a, int b){ return &shortvar; }
u_int8_t *byte_input_call_back(int a){ return &shortvar; }
u_int8_t *byte_output_call_back(int a){ return &shortvar; }
u_int16_t *int_input_call_back(int a){ return &longvar; }
u_int16_t *int_output_call_back(int a){ return &longvar; }
void logger_callbackf(unsigned char *msg){ printf("PLC log: %s", msg); }

int main (int argc, char **argv){
    type_logger_callback logger = logger_callbackf; 
    ethercat_configure(argv[1], logger);
    boolvar_call_back bool_input_callback = bool_input_call_back;
    boolvar_call_back bool_output_callback = bool_output_call_back;
    int8var_call_back byte_input_callback = byte_input_call_back;
    int8var_call_back byte_output_callback = byte_output_call_back;
    int16var_call_back int_input_callback = int_input_call_back;
    int16var_call_back int_output_callback = int_output_call_back;

    for (int i = 0; i < 10; i++){
        ethercat_callcyclic(BUFFER_SIZE, 
            bool_input_callback, 
            bool_output_callback, 
            byte_input_callback, 
            byte_output_callback, 
            int_input_callback, 
            int_output_callback);
    }

    terminate_handler();
    terminate_src();
    return 0;
}
int main2(int argc, char **argv)
{
    int ret = 0;
    struct gengetopt_args_info ai;
    if (cmdline_parser(argc, argv, &ai) != 0) {
        exit(1);
    }
    log_set_level(1);
    // set verbose output DEBUG
    if(ai.verbose_flag){
        log_trace("TRACE output");
        log_set_level(0);
    }

    log_trace("ai.filename_arg: %s", ai.filename_arg);
    log_trace("ai.verbose_flag: %d", ai.verbose_flag);
    log_trace("ai.confonly_flag: %d", ai.confonly_flag);
    if (!ai.index_given)ai.index_arg = 0;
    log_trace("ai.index_arg: %d", ai.index_arg);

    EcatConfig config;
    config.master_index = ai.index_arg;
    config.config_only_flag = ai.confonly_flag;
    
    if(parse_xml_config(ai.filename_arg, &config)){
        log_error("failed to parse configuration file");
        return -1;
    }

    EtherCATinit(&config, NULL);


    return ret;
}

/****************************************************************************/