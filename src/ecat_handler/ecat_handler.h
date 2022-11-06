/**
 * Ethercat config data:
 *
 */
typedef struct _EcatConfig EcatConfig;
typedef EcatConfig *EcatConfigPtr;
struct _EcatConfig{
    unsigned int  master_index;     // EtherCAT master index, /dev/EtherCAT0 is 0, /dev/EtherCAT1 is 1 etc
};

/* set up ethercat comms */
int EtherCATinit(EcatConfig *config);
/* run cyclic task forever */
int EtherCATcyclic();