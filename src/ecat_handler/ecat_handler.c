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

static ec_slave_config_t **sc_slaves = NULL;
static ec_slave_config_state_t **sc_salves_states = NULL;


// process data
static uint8_t *domain1_pd = NULL;


/****************************************************************************/
int ConfigureSlave(EcatConfig *config, SlaveConfig *slave, ec_slave_config_t *sc){
    log_trace("Slave %s %i|%i %i 0x%08X", slave->type, slave->alias, slave->position, slave->vendor_id, slave->product_code);
    if (!config->config_only_flag){
        sc = ecrt_master_slave_config(master, slave->alias, slave->position, slave->vendor_id, slave->product_code);
        if (!sc){
            log_error("Failed to get slave configuration");
            return -1;
        }
    }
    // cofigure pdo-s
    // Digital out ------------------------

    static ec_pdo_entry_info_t el2004_channels[] = {
        {0x3001, 1, 1}, // Value 1
        {0x3001, 2, 1}, // Value 2
        {0x3001, 3, 1}, // Value 3
        {0x3001, 4, 1}  // Value 4
    };

    static ec_pdo_info_t el2004_pdos[] = {
        {0x1600, 1, &el2004_channels[0]},
        {0x1601, 1, &el2004_channels[1]},
        {0x1602, 1, &el2004_channels[2]},
        {0x1603, 1, &el2004_channels[3]}
    };

    static ec_sync_info_t el2004_syncs[] = {
        {0, EC_DIR_OUTPUT, 4, el2004_pdos},
        {1, EC_DIR_INPUT},
        {0xff}
    };

    if (!config->config_only_flag){

        if (ecrt_slave_config_pdos(sc, EC_END, el2004_syncs)){
            log_error("Failed to configure PDO-s");
            return -1;
        }
    }
    return 0;
}
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
    
    sc_slaves = (ec_slave_config_t**) malloc(config->slave_count * sizeof(ec_slave_config_t*));
    sc_salves_states = (ec_slave_config_state_t**) malloc(config->slave_count * sizeof(ec_slave_config_state_t*));
    if (sc_slaves == NULL || sc_salves_states == NULL){
        log_error("Failed to alloc");
        goto out_release_master;
    }

    // configure slaves
    log_trace("Slave n type alias|position vendor_id product_code");
    for (int i = 0; i < config->slave_count; i++) {
        SlaveConfig *slave = &config->slavesConfig[i];
        slave->position = i;
        if (ConfigureSlave(config, slave, sc_slaves[i])){
            log_error("Failed to configure slave");
            goto out_release_master;
        }
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