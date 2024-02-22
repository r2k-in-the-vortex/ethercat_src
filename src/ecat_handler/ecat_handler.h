#pragma once

#include <stdint.h>

/**
 * For registering all read/write pdos and their links to PLC
 *
 */
typedef struct _PdoRegistryEntry PdoRegistryEntry;
typedef PdoRegistryEntry *PdoRegistryEntryPtr;
struct _PdoRegistryEntry{
    unsigned int        offset;                             // PDO entry's (byte-)offset in the process data.
    unsigned int        bit_position;                       // bit position (0-7) within the \a offset
    unsigned int        slaveposition;                      // slave position in ethercat chain
    unsigned char       *slavename;                         // slave name
    unsigned char       *slavetype;                         // slave type
    unsigned int        bitlength;                          // number of bits
    unsigned int        pdoidx;                             // registry PDO index, use this to link PLC
    char                *pdoname;                           // pdo name, example 'Channel 6'
};

/**
 * EcatSm:
 *
 * Sync manager, https://infosys.beckhoff.com/content/1033/tc3_io_intro/1344982411.html?id=1069631237599749139
 */
typedef struct _EcatSm EcatSm;
typedef EcatSm *EcatSmPtr;
struct _EcatSm{
    uint32_t            Index;                              // order of specificatio in device decsription
    uint32_t            Enable;                             // 1 for input, 9 for output, messagebox?
    uint32_t            StartAddress;                       // Determines from which address the Sync Channel is active.
    uint32_t            ControlByte;                        // ? Configuration data written to the SyncManager. The expression in parentheses starts with the number "1" or with the number "3". Number "1" means that the corresponding Sync Channel operates in 1-buffer mode, number "3" means that the corresponding Sync Channel operates in 3-buffer mode. The 1-buffer mode is also called mailbox mode.
    uint32_t            DefaultSize;                        // Length of the Sync Channel in bytes. If the length is 0, the Sync Channel is not enabled.
};

/**
 * EcatPdoEntry:
 *
 * Device recieve, PLC output
 */
typedef struct _EcatPdoEntry EcatPdoEntry;
typedef EcatPdoEntry *EcatPdoEntryPtr;
struct _EcatPdoEntry{
    uint16_t            entryindex;                         // entry index
    uint8_t             subindex;                           // entry subindex
    uint8_t             bitlen;                             // bit length, 1, 8, 16, 32
    char                *entryname;                         // entryname, arbritary, usually Input/Output, but can be descriptive
    char                *datatype;                          // datatype, BOOL for bitlen 1, others...
};

/**
 * EcatPdo:
 *
 * Device recieve, PLC output
 */
typedef struct _EcatPdo EcatPdo;
typedef EcatPdo *EcatPdoPtr;
struct _EcatPdo{
    char                                *pdotype;                           // RxPdo or TxPdo
    uint16_t                            sm;                                 // sm index
    char                                *fixed;                             // fixed attr
    char                                *mandatory;                         // mandatory attr
    uint16_t                            index;                              // pdo index
    char                                *name;                              // pdo name
    uint16_t                            entrycount;                         // how many entries
    struct _EcatPdoEntry                *entries;                           // entires
};

/**
 * Ethercat slave config data:
 * Slaves are addressed with the \a alias and \a position parameters.
 * - If \a alias is zero, \a position is interpreted as the desired slave's
 *   ring position.
 * - If \a alias is non-zero, it matches a slave with the given alias. In this
 *   case, \a position is interpreted as ring offset, starting from the
 *   aliased slave, so a position of zero means the aliased slave itself and a
 *   positive value matches the n-th slave behind the aliased one.
 */
typedef struct _SlaveConfig SlaveConfig;
typedef SlaveConfig *SlaveConfigPtr;
struct _SlaveConfig{
    unsigned char           *type;                          // Type of the slave, example EL2008
    unsigned char           *name;                          // Name of the slave, example EL2008 8K. Dig. Ausgang 24V, 0.5A
    uint16_t                alias;                          // Slave alias. */
    uint16_t                position;                       // Slave position. */
    uint32_t                vendor_id;                      // Expected vendor ID. */
    uint32_t                product_code;                   // Expected product code. */
    uint32_t                product_revision;               // Expected product code revision. */
    uint32_t                sm_count;                       // number of specified Sync managers
    struct _EcatSm          *Sm;                            // Sm collection
    uint32_t                RxPdo_count;                    // number of specified RxPDO
    uint32_t                RxEntry_count;                  // number of specified Rx entries
    struct _EcatPdo         *RxPDO;                         // RxPDO collection
    uint32_t                TxPdo_count;                    // number of specified TxPDO
    uint32_t                TxEntry_count;                  // number of specified Tx entries
    struct _EcatPdo         *TxPDO;                         // TxPDO collection
};

/**
 * Ethercat config data:
 *
 */
typedef struct _EcatConfig EcatConfig;
typedef EcatConfig *EcatConfigPtr;
struct _EcatConfig{
    unsigned int            config_only_flag;               // if not 0 will only run through configuration without accessing device, for  and testing
    unsigned int            master_index;                   // EtherCAT master index, /dev/EtherCAT0 is 0, /dev/EtherCAT1 is 1 etc
    unsigned int            slave_count;                    // count of slaves
    struct _SlaveConfig     *slavesConfig;                  // configuration of slaves
};


#ifdef __cplusplus
extern "C"{
#endif 

typedef u_int8_t *(*boolvar_call_back) (int, int);
typedef u_int8_t *(*int8var_call_back) (int);
typedef u_int16_t *(*int16var_call_back) (int);
typedef u_int32_t *(*int32var_call_back) (int);
typedef u_int64_t *(*int64var_call_back) (int);
typedef int logger_callback(char*);


/* set up ethercat comms */
int EtherCATinit(EcatConfig *config, logger_callback logger);

/* run cyclic task forever */
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
        int64var_call_back lword_output);

void terminate_handler();

#ifdef __cplusplus
}
#endif