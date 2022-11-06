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
#include <string.h>
#include <ecrt.h>
#include "log.h"
#include "ecat_handler.h"

/****************************************************************************/
static ec_master_t *master_ = NULL;
/****************************************************************************/
int EtherCATinit(EcatConfig *config){
    unsigned int ver = ecrt_version_magic();
    log_trace("Initializing EtherCAT ecrt_version_magic %u", ver);

    master_ = ecrt_request_master(config->master_index);
    if (!master_){
        log_error("Didn't load /dev/EtherCAT%i, unable to continue", config->master_index);
        log_error("Try 'sudo ethercat master' to verify etherlabs installation is working");
        return -1;
    }

    return 0;
    
out_release_master:
	log_error("Releasing master");
	ecrt_release_master(master_);
    return -1;
}
/****************************************************************************/
int EtherCATcyclic(){
    return 0;
}