#pragma once

#include <stdint.h>
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
