#pragma once

#include <stdint.h>

/**
 * EcatSm:
 *
 * Sync manager, https://infosys.beckhoff.com/content/1033/tc3_io_intro/1344982411.html?id=1069631237599749139
 */
typedef struct _EcatSm EcatSm;
typedef EcatSm *EcatSmPtr;
struct _EcatSm{
    uint32_t            Index;           // order of specificatio in device decsription
    uint32_t            Enable;          // 1 for input, 9 for output, messagebox?
    uint32_t            StartAddress;    // Determines from which address the Sync Channel is active.
    uint32_t            ControlByte;     // ? Configuration data written to the SyncManager. The expression in parentheses starts with the number "1" or with the number "3". Number "1" means that the corresponding Sync Channel operates in 1-buffer mode, number "3" means that the corresponding Sync Channel operates in 3-buffer mode. The 1-buffer mode is also called mailbox mode.
    uint32_t            DefaultSize;     // Length of the Sync Channel in bytes. If the length is 0, the Sync Channel is not enabled.
};

/**
 * EcatPdo:
 *
 * Device recieve, PLC output
 */
typedef struct _EcatPdo EcatPdo;
typedef EcatPdo *EcatPdoPtr;
struct _EcatPdo{
    const char          *pdotype;                           // RxPdo or TxPdo
    uint16_t            sm;                                 // sm index
    const char          *fixed;                             // fixed attr
    const char          *mandatory;                         // mandatory attr
    uint16_t            index;                             // pdo index
    const char          *name;                              // pdo name
    uint16_t            entryindex;                         // entry index
    const char          *subindex;                          // entry subindex
    uint8_t             bitlen;                             // bit length, 1, 8, 16, 32
    const char          *entryname;                         // entryname, arbritary, usually Input/Output, but can be descriptive
    const char          *datatype;                          // datatype, BOOL for bitlen 1, others...
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
    struct _EcatPdo         *RxPDO;                         // RxPDO collection
    uint32_t                TxPdo_count;                    // number of specified TxPDO
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

/* set up ethercat comms */
int EtherCATinit(EcatConfig *config);
/* run cyclic task forever */
int EtherCATcyclic();
