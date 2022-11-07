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
/****************************************************************************/

/** Task period in ns. */
#define PERIOD_NS   (1000000)

#define MAX_SAFE_STACK (8 * 1024) /* The maximum stack size which is
                                     guranteed safe to access without
                                     faulting */

/****************************************************************************/

/* Constants */
#define NSEC_PER_SEC (1000000000)
#define FREQUENCY (NSEC_PER_SEC / PERIOD_NS)

/****************************************************************************/

// EtherCAT
static ec_master_t *master = NULL;
static ec_master_state_t master_state = {};

static ec_domain_t *domain1 = NULL;
static ec_domain_state_t domain1_state = {};

// process data
static uint8_t *domain1_pd = NULL;


/****************************************************************************/
int ConfigureSlave();
/****************************************************************************/
int EtherCATinit(EcatConfig *config){
    unsigned int ver = ecrt_version_magic();
    log_trace("Initializing EtherCAT ecrt_version_magic %u", ver);

    if (!config->config_only_flag){
        master = ecrt_request_master(config->master_index);
        if (!master){
            log_error("Didn't load /dev/EtherCAT%i, unable to continue", config->master_index);
            log_error("Try 'sudo ethercat master' to verify etherlabs installation is working");
            return -1;
        }
    }
    log_trace("/dev/EtherCAT%i open", config->master_index);

    // support for multiple domains in future?
    if (!config->config_only_flag){
        domain1 = ecrt_master_create_domain(master);
        if (!domain1) {
            log_error("Failed to create domain");
            goto out_release_master;
        }
    }
    log_trace("Domain created");

    // configure slaves
    for (int i = 0; i < config->slave_count; i++) {
        log_trace("Slave %i %s", i, config->slavesConfig[i].type);
        //log_trace("rev %08X", config->slavesConfig[i].product_revision);
        //ecrt_master_slave_config()
        ;
        // to be implemented
    }


    log_trace("Activating master...");
    if (!config->config_only_flag){
        if (ecrt_master_activate(master)) {
            log_trace("Failed to activate master");
            goto out_release_master;
        }
        log_trace("Process data memory...");
        if (!(domain1_pd = ecrt_domain_data(domain1))) {
            log_trace("Failed to get Process data memory");
            goto out_release_master;
        }
    }

    log_trace("/dev/EtherCAT%i set up, ready to start cyclic", config->master_index);
    return 0;
    
out_release_master:
    if (!config->config_only_flag){
        log_error("Releasing master");
        ecrt_release_master(master);
    }
    return -1;
}
/****************************************************************************/
int EtherCATcyclic(){
    return 0;
}