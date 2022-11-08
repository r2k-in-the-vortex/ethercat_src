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
static ec_master_t              *master             = NULL;
static ec_master_state_t        master_state        = {};

static ec_domain_t              *domain1            = NULL;
static ec_domain_state_t        domain1_state       = {};

static ec_slave_config_t        **sc_slaves         = NULL;
static ec_slave_config_state_t  **sc_salves_states  = NULL;

static ec_sync_info_t           *slave_syncs        = NULL;
static ec_pdo_entry_info_t      *rx_entries         = NULL;
static ec_pdo_info_t            *rx_pdos            = NULL;
static ec_pdo_entry_info_t      *tx_entries         = NULL;
static ec_pdo_info_t            *tx_pdos            = NULL;


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

    //ec_sync_info_t slave_syncs[slave->sm_count + 1];
    slave_syncs = (ec_sync_info_t*) malloc((slave->sm_count + 1) * sizeof(ec_sync_info_t));
    //ec_pdo_entry_info_t rx_entries[slave->RxPdo_count];
    rx_entries = (ec_pdo_entry_info_t*) malloc(slave->RxPdo_count * sizeof(ec_pdo_entry_info_t));
    //ec_pdo_info_t rx_pdos[slave->RxPdo_count];
    rx_pdos = (ec_pdo_info_t*) malloc(slave->RxPdo_count * sizeof(ec_pdo_info_t));
    //ec_pdo_entry_info_t tx_entries[slave->TxPdo_count];
    tx_entries = (ec_pdo_entry_info_t*) malloc(slave->TxPdo_count * sizeof(ec_pdo_entry_info_t));
    //ec_pdo_info_t tx_pdos[slave->TxPdo_count];
    tx_pdos = (ec_pdo_info_t*) malloc(slave->TxPdo_count * sizeof(ec_pdo_info_t));


    slave_syncs[slave->sm_count] = (ec_sync_info_t){0xff};  // list termination
    for (int i = 0;i < slave->sm_count;i++) {
        slave_syncs[i].index    = i;       // indexes
        slave_syncs[i].n_pdos   = 0;       // to be counted
    }

    // rx pdos
    for (int i = 0;i < slave->RxPdo_count;i++){
        EcatPdo rxpdo = slave->RxPDO[i];
        rx_entries[i].subindex = i + 1;
        rx_entries[i].index = rxpdo.entryindex;
        rx_entries[i].bit_length = rxpdo.bitlen;
        rx_pdos[i].entries = &rx_entries[i];
        rx_pdos[i].index = rxpdo.index;
        rx_pdos[i].n_entries = 1;
        slave_syncs[rxpdo.sm].dir = EC_DIR_OUTPUT;
        slave_syncs[rxpdo.sm].n_pdos++;
        slave_syncs[rxpdo.sm].pdos = rx_pdos;
    }
    
    // tx pdos
    for (int i = 0;i < slave->TxPdo_count;i++){
        EcatPdo txpdo = slave->TxPDO[i];
        tx_entries[i].subindex = i + 1;
        tx_entries[i].index = txpdo.entryindex;
        tx_entries[i].bit_length = txpdo.bitlen;
        tx_pdos[i].entries = &tx_entries[i];
        tx_pdos[i].index = txpdo.index;
        tx_pdos[i].n_entries = 1;
        slave_syncs[txpdo.sm].dir = EC_DIR_INPUT;
        slave_syncs[txpdo.sm].n_pdos++;
        slave_syncs[txpdo.sm].pdos = tx_pdos;
    }

    for (int i = 0; i < slave->sm_count;i++){
        char *dir = "[error invalid direction]";
        if (slave_syncs[i].dir == EC_DIR_INPUT)dir = "EC_DIR_INPUT";
        if (slave_syncs[i].dir == EC_DIR_OUTPUT)dir = "EC_DIR_OUTPUT";
        log_trace("Sync master %i %s", i, dir);
        for (int j = 0; j < slave_syncs[i].n_pdos; j++){
            log_trace("PDO index %i/%i  0x%4X entryindex 0x%4X subindex %i bitlen %i", 
                j + 1,
                slave_syncs[i].n_pdos,
                slave_syncs[i].pdos[j].index, 
                slave_syncs[i].pdos[j].entries[0].index, 
                slave_syncs[i].pdos[j].entries[0].subindex, 
                slave_syncs[i].pdos[j].entries[0].bit_length);
        }
        log_trace("Slave done");
    }

    if (!config->config_only_flag){

        if (ecrt_slave_config_pdos(sc, EC_END, slave_syncs)){
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