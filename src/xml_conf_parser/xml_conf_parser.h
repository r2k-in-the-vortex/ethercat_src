#include "ecat_handler.h"


/*
 * XML file in, config out
 */
int parse_xml_config(char *filename, EcatConfig *config);

/**
 * EcatPdo:
 *
 * Device recieve, PLC output
 */
typedef struct _EcatPdo EcatPdo;
typedef EcatPdo *EcatPdoPtr;
struct _EcatPdo{
    const char          *pdotype;         // RxPdo or TxPdo
    const char          *sm;              // not sure what that is, but references Sm of device
    const char          *fixed;           // fixed attr
    const char          *mandatory;       // mandatory attr
    const char          *index;           // pdo index
    const char          *name;            // pdo name
    const char          *entryindex;      // entry index
    const char          *subindex;        // entry subindex
    const char          *bitlen;          // bit length, 1, 8, 16, 32
    const char          *entryname;       // entryname, arbritary, usually Input/Output, but can be descriptive
    const char          *datatype;        // datatype, BOOL for bitlen 1, others...
};
