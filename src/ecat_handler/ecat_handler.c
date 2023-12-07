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
#include <ctype.h>
#include "log.h"
#include <ecrt.h>
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
static PdoRegistryEntry         *RxPdoRegistry      = NULL;
static PdoRegistryEntry         *TxPdoRegistry      = NULL;
static ec_pdo_entry_reg_t       *domain1_regs       = NULL; 
static int                      domain1_regs_idx    = 0; 
static int                      RxPdoRegistry_idx   = 0; 
static int                      TxPdoRegistry_idx   = 0; 

static ec_slave_config_t        **sc_slaves         = NULL;
static ec_slave_config_state_t  **sc_salves_states  = NULL;

static ec_sync_info_t           **slave_syncs       = NULL;
static ec_pdo_entry_info_t      **rx_entries        = NULL;
static ec_pdo_info_t            **rx_pdos           = NULL;
static ec_pdo_entry_info_t      **tx_entries        = NULL;
static ec_pdo_info_t            **tx_pdos           = NULL;
static logger_callback          *plclogger          = NULL;
static char                     plcloggerbuffer[1024];


static int                      rxregistry_count    = 0;
static int                      txregistry_count    = 0;
static int                      config_done         = 0;

static int                      msgonce             = 0;
// for some reason slaves don't start at all if non-zero data is sent too early
// have to think of a better solution, if one slave is in fault state or whatever
// the rest must still be able to work
static int                      init_done           = 0;

static EcatConfig               *config             = NULL;
static int                      wcincompletecounter = 0;
static char                     tempbuf[20];

// process data
static uint8_t *domain1_pd = NULL;


/****************************************************************************/
int RegisterRxInDomain(SlaveConfig *slave, ec_pdo_entry_info_t *entry, EcatPdo *pdo){
    domain1_regs[domain1_regs_idx].alias                    =           slave->alias;
    domain1_regs[domain1_regs_idx].position                 =           slave->position;
    domain1_regs[domain1_regs_idx].vendor_id                =           slave->vendor_id;
    domain1_regs[domain1_regs_idx].product_code             =           slave->product_code;
    domain1_regs[domain1_regs_idx].index                    =           entry->index;
    domain1_regs[domain1_regs_idx].subindex                 =           entry->subindex;
    domain1_regs[domain1_regs_idx].offset                   =           &RxPdoRegistry[RxPdoRegistry_idx].offset;
    domain1_regs[domain1_regs_idx].bit_position             =           &RxPdoRegistry[RxPdoRegistry_idx].bit_position;

    RxPdoRegistry[RxPdoRegistry_idx].slaveposition          =           slave->position;
    RxPdoRegistry[RxPdoRegistry_idx].slavename              =           slave->name;
    RxPdoRegistry[RxPdoRegistry_idx].slavetype              =           slave->type;
    RxPdoRegistry[RxPdoRegistry_idx].bitlength              =           entry->bit_length;
    RxPdoRegistry[RxPdoRegistry_idx].pdoname                =           pdo->name;
    RxPdoRegistry[RxPdoRegistry_idx].pdoidx                 =           RxPdoRegistry_idx;

    domain1_regs_idx++;
    RxPdoRegistry_idx++;
    return 0;
}
int RegisterTxInDomain(SlaveConfig *slave, ec_pdo_entry_info_t *entry, EcatPdo *pdo){
    domain1_regs[domain1_regs_idx].alias                    =           slave->alias;
    domain1_regs[domain1_regs_idx].position                 =           slave->position;
    domain1_regs[domain1_regs_idx].vendor_id                =           slave->vendor_id;
    domain1_regs[domain1_regs_idx].product_code             =           slave->product_code;
    domain1_regs[domain1_regs_idx].index                    =           entry->index;
    domain1_regs[domain1_regs_idx].subindex                 =           entry->subindex;
    domain1_regs[domain1_regs_idx].offset                   =           &TxPdoRegistry[TxPdoRegistry_idx].offset;
    domain1_regs[domain1_regs_idx].bit_position             =           &TxPdoRegistry[TxPdoRegistry_idx].bit_position;

    TxPdoRegistry[TxPdoRegistry_idx].slaveposition          =           slave->position;
    TxPdoRegistry[TxPdoRegistry_idx].slavename              =           slave->name;
    TxPdoRegistry[TxPdoRegistry_idx].slavetype              =           slave->type;
    TxPdoRegistry[TxPdoRegistry_idx].bitlength              =           entry->bit_length;
    TxPdoRegistry[TxPdoRegistry_idx].pdoname                =           pdo->name;
    TxPdoRegistry[TxPdoRegistry_idx].pdoidx                 =           TxPdoRegistry_idx;

    domain1_regs_idx++;
    TxPdoRegistry_idx++;
    return 0;
}

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

    ec_sync_info_t *ss = (ec_sync_info_t*) malloc((slave->sm_count + 1) * sizeof(ec_sync_info_t));
    slave_syncs[slave->position] = ss;

    ec_pdo_entry_info_t *rxe = (ec_pdo_entry_info_t*) malloc(slave->RxPdo_count * sizeof(ec_pdo_entry_info_t));
    rx_entries[slave->position] = rxe;

    ec_pdo_info_t *rxp = (ec_pdo_info_t*) malloc(slave->RxPdo_count * sizeof(ec_pdo_info_t));
    rx_pdos[slave->position] = rxp;

    ec_pdo_entry_info_t *txe = (ec_pdo_entry_info_t*) malloc(slave->TxPdo_count * sizeof(ec_pdo_entry_info_t));
    tx_entries[slave->position] = txe;

    ec_pdo_info_t *txp = (ec_pdo_info_t*) malloc(slave->TxPdo_count * sizeof(ec_pdo_info_t));
    tx_pdos[slave->position] = txp;


    ss[slave->sm_count] = (ec_sync_info_t){0xff};  // list termination
    for (int i = 0;i < slave->sm_count;i++) {
        ss[i].index    = i;       // indexes
        ss[i].n_pdos   = 0;       // to be counted
        ss[i].dir   = EC_DIR_INPUT;       // set default
        ss[i].watchdog_mode   = EC_WD_DEFAULT;       // set default
    }

    // rx pdos
    for (int i = 0;i < slave->RxPdo_count;i++){
        EcatPdo rxpdo = slave->RxPDO[i];
        rxe[i].subindex = i + 1;
        rxe[i].index = rxpdo.entryindex;
        rxe[i].bit_length = rxpdo.bitlen;
        rxp[i].entries = &rxe[i];
        rxp[i].index = rxpdo.index;
        rxp[i].n_entries = 1;
        ss[rxpdo.sm].dir = EC_DIR_OUTPUT;
        /*
        if for example 
        22:52:44 Sync master 0 EC_DIR_OUTPUT
        22:52:46 PDO index 1/1  0x1A00 entryindex 0x3101 subindex 1 bitlen 8
        22:52:56 Sync master 1 EC_DIR_OUTPUT
        22:53:03 PDO index 1/1  0x1A01 entryindex 0x3101 subindex 2 bitlen 8

        then assuming RxPDO entries are in correct order

        if one entry sm0, another sm1 and then again sm0, then problem
        */
        if (ss[rxpdo.sm].n_pdos == 0)ss[rxpdo.sm].pdos = rxp + i;
        ss[rxpdo.sm].n_pdos++;
        if(RegisterRxInDomain(slave, &rxe[i], &rxpdo)){
            log_error("Failed to register RxPDO in domain");
            return -1;
        }
    }
    
    // tx pdos
    for (int i = 0;i < slave->TxPdo_count;i++){
        EcatPdo txpdo = slave->TxPDO[i];
        txe[i].subindex = i + 1;
        txe[i].index = txpdo.entryindex;
        txe[i].bit_length = txpdo.bitlen;
        txp[i].entries = &txe[i];
        txp[i].index = txpdo.index;
        txp[i].n_entries = 1;
        ss[txpdo.sm].dir = EC_DIR_INPUT;
        if (ss[txpdo.sm].n_pdos == 0)ss[txpdo.sm].pdos = txp + i;
        ss[txpdo.sm].n_pdos++;
        if(RegisterTxInDomain(slave, &txe[i], &txpdo)){
            log_error("Failed to register TxPDO in domain");
            return -1;
        }
    }

    for (int i = 0; i < slave->sm_count;i++){
        char *dir = "[error invalid direction]";
        if (ss[i].dir == EC_DIR_INPUT)dir = "EC_DIR_INPUT";
        if (ss[i].dir == EC_DIR_OUTPUT)dir = "EC_DIR_OUTPUT";
        log_trace("Sync master %i %s", i, dir);
        for (int j = 0; j < ss[i].n_pdos; j++){
            log_trace("PDO index %i/%i  0x%4X entryindex 0x%4X subindex %i bitlen %i", 
                j + 1,
                ss[i].n_pdos,
                ss[i].pdos[j].index, 
                ss[i].pdos[j].entries[0].index, 
                ss[i].pdos[j].entries[0].subindex, 
                ss[i].pdos[j].entries[0].bit_length);
        }
    }

    if (!config->config_only_flag){

        if (ecrt_slave_config_pdos(sc, EC_END, ss)){
            log_error("Failed to configure PDO-s");
            return -1;
        }
    }
    log_trace("Slave configured");
    return 0;
}

int NameTypeAndPrefix(PdoRegistryEntry *pdo, char *name, char *prefix, char *dtype, char *comment){
    strcpy(name, pdo->pdoname);
    int i=0;
    while (name[i])
    {
        if (isspace(name[i])) 
            name[i]='_';
        i++;
    }
    // supported types on PLC side are limited
    if (pdo->bitlength == 1){
        strcpy(dtype, "BOOL");
        strcpy(prefix, "X");
    } else if (pdo->bitlength == 8){
        strcpy(dtype, "BYTE");
        strcpy(prefix, "B");
    } else if (pdo->bitlength == 16){
        strcpy(dtype, "UINT");
        strcpy(prefix, "W");
    } else if (pdo->bitlength == 32){
        strcpy(dtype, "DINT");
        strcpy(prefix, "D");
    } else if (pdo->bitlength == 64){
        strcpy(dtype, "LINT");
        strcpy(prefix, "L");
    } else{
        log_error("Bitlen %i not implemented and not supported", pdo->bitlength);
        return -1;
    }
    sprintf(comment, "(* %s *)", pdo->slavename);
    return 0;
}

int PlcInputOutputPrintout(int rxregistry_count, int txregistry_count){

    plclogger("PLC IO Printout, copy this to VAR..END_VAR of main program or something\n");
    plclogger("Variable names may be modified at will as long as the adresses and variable sizes remain the same\n");
    
    for (int i = 0;i < txregistry_count;i++){
        PdoRegistryEntry pdo = TxPdoRegistry[i];
        //Input1 AT %IX0.0 : BOOL;
        char name[strlen(pdo.pdoname)];
        char dtype[10];
        char prefix[10];
        char comment[255];
        if(NameTypeAndPrefix(&pdo, name, prefix, dtype, comment)){
            log_error("PLC variable printout failed");
            return -1;
        }
        
        sprintf(plcloggerbuffer, "Slave%i_%s AT %%I%s%i.0 : %s; %s\n", pdo.slaveposition, name, prefix, pdo.pdoidx, dtype, comment);
        
        if(plclogger != NULL)plclogger(plcloggerbuffer);
    }
    for (int i = 0;i < rxregistry_count;i++){
        PdoRegistryEntry pdo = RxPdoRegistry[i];
        //Output1 AT %QX0.0 : BOOL;
        char name[strlen(pdo.pdoname)];
        char dtype[10];
        char prefix[10];
        char comment[255];
        if(NameTypeAndPrefix(&pdo, name, prefix, dtype, comment)){
            log_error("PLC variable printout failed");
            return -1;
        }

        sprintf(plcloggerbuffer, "Slave%i_%s AT %%Q%s%i.0 : %s; %s\n", pdo.slaveposition, name, prefix, pdo.pdoidx, dtype, comment);
        
        if(plclogger != NULL)plclogger(plcloggerbuffer);
    }
    return 0;
}
/****************************************************************************/
int EtherCATinit(EcatConfig *configin, logger_callback logger){
    plclogger = logger;
    wcincompletecounter = 0;

    if(plclogger != NULL)plclogger("Configuring EtherCAT\n");
    config = configin;
    config_done = 0;
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

    // count registers
    rxregistry_count = 0;
    txregistry_count = 0;
    for (int i = 0; i < config->slave_count; i++){
        rxregistry_count += config->slavesConfig[i].RxPdo_count;
        txregistry_count += config->slavesConfig[i].TxPdo_count;
    }
    domain1_regs = (ec_pdo_entry_reg_t*) malloc((rxregistry_count + txregistry_count) * sizeof(ec_pdo_entry_reg_t));
    //static PdoRegistryEntry         *TxPdoRegistry      = NULL;
    RxPdoRegistry = (PdoRegistryEntry*) malloc(rxregistry_count * sizeof(PdoRegistryEntry));
    TxPdoRegistry = (PdoRegistryEntry*) malloc(txregistry_count * sizeof(PdoRegistryEntry));

    log_trace("Rx Registry count %i", rxregistry_count);
    log_trace("Tx Registry count %i", txregistry_count);
    
    sc_slaves = (ec_slave_config_t**) malloc(config->slave_count * sizeof(ec_slave_config_t*));
    sc_salves_states = (ec_slave_config_state_t**) malloc(config->slave_count * sizeof(ec_slave_config_state_t*));

    slave_syncs = (ec_sync_info_t**) malloc(config->slave_count * sizeof(ec_sync_info_t*));
    rx_entries = (ec_pdo_entry_info_t**) malloc(config->slave_count * sizeof(ec_pdo_entry_info_t*));
    rx_pdos = (ec_pdo_info_t**) malloc(config->slave_count * sizeof(ec_pdo_info_t*));
    tx_entries = (ec_pdo_entry_info_t**) malloc(config->slave_count * sizeof(ec_pdo_entry_info_t*));
    tx_pdos = (ec_pdo_info_t**) malloc(config->slave_count * sizeof(ec_pdo_info_t*));


    if (sc_slaves == NULL || 
    sc_salves_states == NULL || 
    domain1_regs == NULL || 
    slave_syncs == NULL || 
    rx_entries == NULL || 
    rx_pdos == NULL || 
    tx_entries == NULL || 
    tx_pdos == NULL
    ){
        log_error("Failed to alloc");
        goto out_release_master;
    }

    // configure slaves
    log_trace("Slave n type alias|position vendor_id product_code");
    for (int i = 0; i < config->slave_count; i++) {
        SlaveConfig *slave = &config->slavesConfig[i];
        slave->position = i;
        slave->alias = 0;
        if (ConfigureSlave(config, slave, sc_slaves[i])){
            log_error("Failed to configure slave");
            goto out_release_master;
        }
    }
    log_trace("Slaves configured");

    if (!config->config_only_flag){
        if (ecrt_domain_reg_pdo_entry_list(domain1, domain1_regs)) {
            log_error("PDO entry registration failed!");
            return -1;
        }
    }
    log_trace("PDO registry complete");
    log_trace("RxPDO registries for linking with PLC");
    for (int i = 0;i < rxregistry_count;i++){
        PdoRegistryEntry pdo = RxPdoRegistry[i];
        log_trace("%i slave %i %s | %s | %i bits", pdo.pdoidx, pdo.slaveposition, pdo.slavename, pdo.pdoname, pdo.bitlength);
    }
    log_trace("TxPDO registries for linking with PLC");
    for (int i = 0;i < txregistry_count;i++){
        PdoRegistryEntry pdo = TxPdoRegistry[i];
        log_trace("%i slave %i %s | %s | %i bits", pdo.pdoidx, pdo.slaveposition, pdo.slavename, pdo.pdoname, pdo.bitlength);
    }

    if(PlcInputOutputPrintout(rxregistry_count, txregistry_count)){
        goto out_release_master;
    }

    log_trace("Activating master...");
    if (!config->config_only_flag){
        if (ecrt_master_activate(master)) {
            log_error("Failed to activate master");
            goto out_release_master;
        }
        log_trace("Process data memory...");
        if (!(domain1_pd = ecrt_domain_data(domain1))) {
            log_error("Failed to get Process data memory");
            goto out_release_master;
        }
    }

    log_trace("/dev/EtherCAT%i set up, ready to start cyclic", config->master_index);
    config_done = 1;
    return 0;
    
out_release_master:
    if (!config->config_only_flag){
        log_error("Releasing master");
        ecrt_release_master(master);
    }
    return -1;
}
/****************************************************************************/
void check_domain1_state(void)
{
    ec_domain_state_t ds;

    ecrt_domain_state(domain1, &ds);

    if (ds.working_counter != domain1_state.working_counter){
        sprintf(plcloggerbuffer, "Domain1: WC %u.\n", ds.working_counter);
        log_trace(plcloggerbuffer);
        if(plclogger != NULL)plclogger(plcloggerbuffer);
    }
    if (ds.wc_state != domain1_state.wc_state){
        sprintf(plcloggerbuffer, "Domain1: State %u.\n", ds.wc_state);
        log_trace(plcloggerbuffer);
        if(plclogger != NULL)plclogger(plcloggerbuffer);
    }

    domain1_state = ds;
}

void check_master_state(void)
{
    ec_master_state_t ms;

    ecrt_master_state(master, &ms);

    if (ms.slaves_responding != master_state.slaves_responding){
        sprintf(plcloggerbuffer, "%u slave(s).\n", ms.slaves_responding);
        log_trace(plcloggerbuffer);
        if(plclogger != NULL)plclogger(plcloggerbuffer);
    }
    if (ms.al_states != master_state.al_states){

        /*
        - Bit 0: \a INIT
        - Bit 1: \a PREOP
        - Bit 2: \a SAFEOP
        - Bit 3: \a OP
        */
        if(ms.al_states == 1)strcpy(tempbuf, "INIT");
        else if(ms.al_states == 2)strcpy(tempbuf, "PREOP");
        else if(ms.al_states == 4)strcpy(tempbuf, "SAFEOP");
        else if(ms.al_states == 8)strcpy(tempbuf, "OP");
        else sprintf(tempbuf, "0x%02X", ms.al_states);


        sprintf(plcloggerbuffer, "AL states: %s.\n", tempbuf);
        log_trace(plcloggerbuffer);
        if(plclogger != NULL)plclogger(plcloggerbuffer);
    }
    if (ms.link_up != master_state.link_up){
        sprintf(plcloggerbuffer, "Link is %s.\n", ms.link_up ? "up" : "down");
        log_trace(plcloggerbuffer);
        if(plclogger != NULL)plclogger(plcloggerbuffer);
    }

    master_state = ms;
}

char *boolarrayptr1(void *ptr, int i1){
    return ptr + i1 * sizeof(uint8_t) * 8 * 8;
}

int EtherCATcyclic(int buffersize, 
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

    if(!config_done)return 0;
    if(!msgonce){
        sprintf(plcloggerbuffer, config->config_only_flag ? "EtherCAT simulation only\n" : "enabled\n");
        log_trace(plcloggerbuffer);
        if(plclogger != NULL)plclogger(plcloggerbuffer);
    }
    if(buffersize < rxregistry_count || buffersize < txregistry_count){
        log_error("PDO count rxregistry_count=%i or txregistry_count=%i too large for buffersize=%i increase buffer", rxregistry_count, txregistry_count, buffersize);
        return -1;
    }

    if(!config->config_only_flag){
        // receive process data
        ecrt_master_receive(master);
        ecrt_domain_process(domain1);

        // check process data state
        check_domain1_state();

        // optional
        check_master_state();
    } else init_done = 1;

    if(!init_done){
        if (domain1_state.wc_state == EC_WC_COMPLETE){
            init_done = 1;
            log_trace("Domain1: WC %u.", domain1_state.working_counter);
            log_trace("Domain1: State %u.", domain1_state.wc_state);
            log_trace("%u slave(s).", master_state.slaves_responding);
            log_trace("AL states: 0x%02X.", master_state.al_states);
            log_trace("Link is %s.", master_state.link_up ? "up" : "down");
        } else {
            wcincompletecounter++;
            if (wcincompletecounter > 500) init_done = 1;
            if (wcincompletecounter % 50 == 0)log_trace("domain1_state.wc_state != EC_WC_COMPLETE"); 
        }
    }
	// for some mysterious reason writing data too early prevents slaves responding correctly
    // needs a better fix in future
    if(init_done){
        // read inputs
        for (int i = 0;i < txregistry_count;i++){
            PdoRegistryEntry pdo = TxPdoRegistry[i];
            
            if(pdo.bitlength == 1){
                uint8_t readval = config->config_only_flag ? 0 : EC_READ_BIT(domain1_pd + pdo.offset, pdo.bit_position);
                uint8_t *ptr = bool_input(i, 0);
                if(!config->config_only_flag){
                    *ptr = readval;
                } else {
                    // simulate input for testing
                    if(i == 0){
                        uint8_t *ptrout = bool_output(i, 0);
                        *ptr = *ptrout;
                    }
                }
            } else if(pdo.bitlength == 8){
                uint8_t *ptr = byte_input(i);
                if(!config->config_only_flag && ptr){
                    *ptr = EC_READ_U8(domain1_pd + pdo.offset);
                }
            } else if(pdo.bitlength == 16){
                uint16_t *ptr = word_input(i);
                if(!config->config_only_flag && ptr){
                    *ptr = EC_READ_U16(domain1_pd + pdo.offset);
                }
            } else if(pdo.bitlength == 32){
                uint32_t *ptr = dword_input(i);
                if(!config->config_only_flag && ptr){
                    *ptr = EC_READ_U32(domain1_pd + pdo.offset);
                }
            } else if(pdo.bitlength == 64){
                uint64_t *ptr = lword_input(i);
                if(!config->config_only_flag && ptr){
                    *ptr = EC_READ_U64(domain1_pd + pdo.offset);
                }
            }
        }
        
        // write outputs
        for (int i = 0;i < rxregistry_count;i++){
            PdoRegistryEntry pdo = RxPdoRegistry[i];
            if(pdo.bitlength == 1){
                uint8_t *ptrout = bool_output(i, 0);
                if (*ptrout){
                    if(config->config_only_flag && i == 0) log_trace("OUT=1");
                    if(!config->config_only_flag) EC_WRITE_BIT(domain1_pd + pdo.offset, pdo.bit_position, 1);
                }else{
                    if(config->config_only_flag && i == 0) log_trace("OUT=0");
                    if(!config->config_only_flag) EC_WRITE_BIT(domain1_pd + pdo.offset, pdo.bit_position, 0);
                }
            } else if(pdo.bitlength == 8){
                uint8_t *ptrout = byte_output(i);
                if(!config->config_only_flag && ptrout){
                    EC_WRITE_U8(domain1_pd + pdo.offset, *ptrout);
                }
            } else if(pdo.bitlength == 16){
                uint16_t *ptrout = word_output(i);
                if(!config->config_only_flag && ptrout){
                    EC_WRITE_U16(domain1_pd + pdo.offset, *ptrout);
                }
            } else if(pdo.bitlength == 32){
                uint32_t *ptrout = dword_output(i);
                if(!config->config_only_flag && ptrout){
                    EC_WRITE_U32(domain1_pd + pdo.offset, *ptrout);
                }
            } else if(pdo.bitlength == 64){
                uint64_t *ptrout = lword_output(i);
                if(!config->config_only_flag && ptrout){
                    EC_WRITE_U64(domain1_pd + pdo.offset, *ptrout);
                }
            }
        }
    }
    if(!config->config_only_flag){
        // send process data
        ecrt_domain_queue(domain1);
        ecrt_master_send(master);
    }
    if(!msgonce){
        log_trace("cycle complete");
        msgonce = 1;
    }
    return 0;
}

// cleanup after 
void terminate_handler(){
    if(config == NULL)return;
    for (int i = 0; i < config->slave_count; i++){
        free(slave_syncs[i]);
        free(rx_entries[i]);
        free(rx_pdos[i]);
        free(tx_entries[i]);
        free(tx_pdos[i]);
    }
    free(domain1_regs);
    free(RxPdoRegistry);
    free(TxPdoRegistry);
    free(sc_slaves);
    free(sc_salves_states);
    free(slave_syncs);
    free(rx_entries);
    free(rx_pdos);
    free(tx_entries);
    free(tx_pdos);
}