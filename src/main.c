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
/****************************************************************************/
int main (int argc, char **argv){
    ethercat_configure(argv[1], NULL);
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