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
#include "libxml/parser.h"
#include "xml_conf_parser.h"
#include "log.h"
#include "ecat_handler.h"

/****************************************************************************/
/****************************************************************************/
static EcatConfig *ecat_config;

char *strmcpy(char *src){
    char *dst;
    dst = (char*)malloc((strlen(src) + 1) * sizeof(char));
    strcpy(dst, src);
    return dst;
}

xmlNode * getNextNodeNamed(xmlNode *start, char *name){
    xmlNode *node;
    for (node = start; node; node = node->next){
        if (node->name == NULL)return NULL;
        if(strcmp(node->name, name) == 0){
            return node;
        }
    }
    return NULL;
}
int countNodesNamed(xmlNode *start, char *name){
    xmlNode *node;
    int count = 0;
    for (node = start; node; node = node->next){
        if (node->name == NULL)return 0;
        if(strcmp(node->name, name) == 0){
            count++;
        }
    }
    return count;
}
// gets exactly one node named name, NULL if start NULL, more that one element with name or none
xmlNode * getSingularnodeNamed(xmlNode *start, char *name){
    if (start == NULL)return NULL;
    int nodecount = countNodesNamed(start, name);
    if (nodecount != 1)return NULL;
    xmlNode *node = getNextNodeNamed(start, name);
    return node;
}
char * getNodeTextContent(xmlNode *node){
    if (node == NULL)return NULL;
    if (node->children == NULL)return NULL;
    xmlNode *text = getSingularnodeNamed(node->children, "text");
    if (text == NULL)return NULL;
    return text->content;
}
char * getAttrTextContent(xmlAttr *attr){
    if (attr == NULL)return NULL;
    xmlNode *text = getSingularnodeNamed(attr->children, "text");
    if (text == NULL)return NULL;
    return text->content;
}
char * getAttributeValueNamed(xmlNode *element, char *name){
    xmlAttr *attr;
    for (attr = element->properties; attr; attr = attr->next){
        if(strcmp(attr->name, name) == 0){
            return getAttrTextContent(attr);
        }
    }
    return NULL;
}
uint32_t hexstrtoint32(char *str){
    //log_trace("converting %s", str);
    if (strlen(str) < 3){
        log_error("invalid string '%s'", str);
        return 0;
    }
    char *pend;
    return strtol(str+2, &pend, 16);
}

/****************************************************************************/
int getVendorId(xmlNode *slaveinfo, uint32_t *vendorid){
    char *ptr;
    xmlNode *vendornode = getSingularnodeNamed(slaveinfo->children, "Vendor");
    xmlNode *idnode = getSingularnodeNamed(vendornode->children, "Id");
    char *text = getNodeTextContent(idnode);
    if(text == NULL) return -1;
    *vendorid = strtol(text, &ptr, 10);
    if(&vendorid != 0) return 0;
    fprintf(stdout, "\t vendor id not found\n");
    return -1;
}

int ParseDeviceType(xmlNode *typedata, SlaveConfig *config){
    if(typedata == NULL){
        log_error("Typenode NULL");
        return -1;
    }
    char *strproductcode = getAttributeValueNamed(typedata, "ProductCode");
    char *strrevisionNo = getAttributeValueNamed(typedata, "RevisionNo");

    config->product_code = hexstrtoint32(strproductcode);
    config->product_revision = hexstrtoint32(strrevisionNo);

    config->type = strmcpy(getNodeTextContent(typedata));
    log_trace("%s product code 0x%08X, rev 0x%08X", config->type, config->product_code, config->product_revision);
    return 0;
}

int ParseDeviceName(xmlNode *namenode, char **name){
    if(namenode == NULL){
        log_error("name node not found");
        return -1;
    }
    *name = namenode->children->content;
    if(*name == NULL){
        log_error("no name parsed");
        return -1;
    }
    log_trace("%s", *name);
    return 0;
}

int ParseSm(xmlNode *node, EcatSm *sm){
    char *ptr;
    char *Enable        = getAttributeValueNamed(node, "Enable");
    char *StartAddress  = getAttributeValueNamed(node, "StartAddress");
    char *ControlByte   = getAttributeValueNamed(node, "ControlByte");
    char *DefaultSize   = getAttributeValueNamed(node, "DefaultSize");

    sm->Enable          = strtol(Enable, &ptr, 10);
    sm->StartAddress    = hexstrtoint32(StartAddress);
    sm->ControlByte     = hexstrtoint32(ControlByte);
    sm->DefaultSize     = strtol(DefaultSize, &ptr, 10);

    log_trace("Sm %i %i 0x%X 0x%X %i", sm->Index, sm->Enable, sm->StartAddress, sm->ControlByte, sm->DefaultSize);
}

int ParsePdo(xmlNode *node, EcatPdo *pdo){
    char *ptr;
    xmlNode *entry;
    pdo->pdotype = strmcpy((char *)node->name);
    char *sm = getAttributeValueNamed(node, "Sm");
    pdo->sm = (uint16_t)strtol(sm, &ptr, 10);
    pdo->fixed = strmcpy(getAttributeValueNamed(node, "Fixed"));
    pdo->mandatory = strmcpy(getAttributeValueNamed(node, "Mandatory"));
    char *index = getNodeTextContent(getSingularnodeNamed(node->children, "Index"));
    pdo->index = (uint16_t)hexstrtoint32(index);
    pdo->name = strmcpy(getNodeTextContent(getSingularnodeNamed(node->children, "Name")));

    pdo->entrycount = countNodesNamed(node->children, "Entry");
    pdo->entries = (EcatPdoEntry*) malloc(pdo->entrycount * sizeof(EcatPdoEntry));

    log_trace("%s %i %s %s 0x%X %s", pdo->pdotype, pdo->sm, pdo->fixed, pdo->mandatory, pdo->index, pdo->name);
    entry = getNextNodeNamed(node->children, "Entry");
    for (int i = 0; i<pdo->entrycount; i++){
        EcatPdoEntry *pdoentry = &pdo->entries[i];
        
        char *entryindex = getNodeTextContent(getSingularnodeNamed(entry->children, "Index"));
        
        pdoentry->entryindex = (uint16_t)hexstrtoint32(entryindex);
        char *subindex = getNodeTextContent(getSingularnodeNamed(entry->children, "SubIndex"));
        pdoentry->subindex = (uint8_t)strtol(subindex, &ptr, 10);
        char *bitlen = getNodeTextContent(getSingularnodeNamed(entry->children, "BitLen"));
        pdoentry->bitlen = (uint8_t)strtol(bitlen, &ptr, 10);
        pdoentry->entryname = strmcpy(getNodeTextContent(getSingularnodeNamed(entry->children, "Name")));
        pdoentry->datatype = strmcpy(getNodeTextContent(getSingularnodeNamed(entry->children, "DataType")));
        entry = getNextNodeNamed(entry->next, "Entry");
        log_trace("%s 0x%X %i %i %s", pdoentry->entryname, pdoentry->entryindex, pdoentry->subindex, pdoentry->bitlen, pdoentry->datatype);
    }
    return 0;
}

int ParseDescriptions(xmlNode *descriptions, SlaveConfig *config){
    // data out
    char *producttype, *productcode, *revisionNo, *name;

    // parse
    log_trace("parsing descriptions");
    // <Descriptions><Devices><Device>the good stuff inside here</Device></Devices></Descriptions>
    // one element of devices, inside one device, any deviation from that is not implemented
    xmlNode *first_child, *node, *start;
    xmlNode *devices = getSingularnodeNamed(descriptions->children, "Devices");
    xmlNode *device = getSingularnodeNamed(devices->children, "Device");
    xmlNode *typenode = getSingularnodeNamed(device->children, "Type");
    xmlNode *namenode = getSingularnodeNamed(device->children, "Name");

    if(ParseDeviceType(typenode, config))return -1;

    log_trace("code  0x%08X | rev 0x%08X", config->product_code, config->product_revision);

    if(ParseDeviceName(namenode, &name))return -1;

    // syncmanager count
    config->sm_count = countNodesNamed(device->children, "Sm");
    log_trace("Sm count %i", config->sm_count);
    EcatSm *Sms;
    Sms = (EcatSm*) malloc(config->sm_count * sizeof(EcatSm));
    if (Sms == NULL){
        log_error("failed to alloc");
        return -1;
    }
    start = device->children;
    for (int i = 0; i< config->sm_count;i++){
        start = getNextNodeNamed(start, "Sm");
        Sms[i].Index = i;
        ParseSm(start, &Sms[i]);
        start = start->next;
    }

    config->RxPdo_count = countNodesNamed(device->children, "RxPdo");
    config->TxPdo_count = countNodesNamed(device->children, "TxPdo");
    config->RxEntry_count = 0;
    config->TxEntry_count = 0;
    log_trace("RxPdos %i | TxPdos %i", config->RxPdo_count, config->TxPdo_count);

    EcatPdo *RxPdos, *TxPdos;
    RxPdos = (EcatPdo*) malloc(config->RxPdo_count * sizeof(EcatPdo));
    TxPdos = (EcatPdo*) malloc(config->TxPdo_count * sizeof(EcatPdo));
    if (RxPdos == NULL || TxPdos == NULL){
        log_error("failed to alloc");
        return -1;
    }
    start = device->children;
    for (int i = 0; i< config->RxPdo_count;i++){
        start = getNextNodeNamed(start, "RxPdo");
        ParsePdo(start, &RxPdos[i]);
        config->RxEntry_count += RxPdos[i].entrycount;
        start = start->next;
    }
    start = device->children;
    for (int i = 0; i< config->TxPdo_count;i++){
        start = getNextNodeNamed(start, "TxPdo");
        ParsePdo(start, &TxPdos[i]);
        config->TxEntry_count += TxPdos[i].entrycount;
        start = start->next;
    }

    config->name = strmcpy(name);
    config->Sm = Sms;
    config->RxPDO = RxPdos;
    config->TxPDO = TxPdos;
    return 0;
}

int ParseSlave(xmlNode *slaveinfo, SlaveConfig *config){
    log_trace("parsing slave %s", slaveinfo->name);
    xmlNode *first_child, *node;
    uint32_t vendorid;

    if(getVendorId(slaveinfo, &vendorid) != 0){
        log_error("failed to get vendor id %ld", vendorid);
        return -1;
    }
    log_trace("vendor id %ld", vendorid);
    config->vendor_id = vendorid;

    char expectedDescriptionsElementName[] = "Descriptions"; 

    first_child = slaveinfo->children;
    for (node = first_child; node; node = node->next){
        if(strcmp(node->name, expectedDescriptionsElementName) == 0) {
            if(ParseDescriptions(node, config))return -1;
        }
    }

    return 0;
}
/****************************************************************************/
int parse_xml_config(char *filename, EcatConfig *newconfig){
    log_trace("parsing file %s", filename);
    ecat_config = newconfig;
    xmlDoc         *document;
    xmlNode        *root, *first_child, *node;

    document = xmlReadFile(filename, NULL, XML_PARSE_PEDANTIC);
    if(!document){
        log_error("Reading file failed, fp NULL");
        return -1;
    }
    root = xmlDocGetRootElement(document);

    char expectedRootName[] = "EtherCATInfoList"; 
    char expectedSlaveName[] = "EtherCATInfo"; 

    log_trace("Root is <%s> (%i)", root->name, root->type);
    if(strcmp(root->name, expectedRootName) != 0){
        log_error("Invalid configuration file, expected root name %s but got %s", expectedRootName, root->name);
        return -1;
    }

    first_child = root->children;

    ecat_config->slave_count = countNodesNamed(first_child, expectedSlaveName);
    
    
    SlaveConfig *slavesConfig = (SlaveConfig*) malloc(ecat_config->slave_count * sizeof(SlaveConfig));
    
    uint32_t *test = &slavesConfig[0].product_revision;

    ecat_config->slavesConfig = slavesConfig;
    if (ecat_config->slavesConfig == NULL){
        log_error("failed to alloc");
        return -1;
    }

    int slaveindex = 0;
    for (node = first_child; node; node = node->next) {
        if(strcmp(node->name, expectedSlaveName) == 0){
            SlaveConfig conf;
            if(ParseSlave(node, &conf)){
                log_error("Failed to parse slave");
                return -1;
            }
            ecat_config->slavesConfig[slaveindex] = conf;
            slaveindex++;
        }
    }
    xmlFreeDoc(document);
    return 0;
}

void terminate_entry(EcatPdoEntry *entry){
    free(entry->datatype);
    free(entry->entryname);
}

void terminate_pdo(EcatPdo *pdo){
    for (int i = 0; i < pdo->entrycount; i++) terminate_entry(&pdo->entries[i]);
    free(pdo->entries);
    free(pdo->fixed);
    free(pdo->mandatory);
    free(pdo->name);
    free(pdo->pdotype);
}

void terminate_slaveconfig(SlaveConfig *s){
    for (int i = 0; i < s->RxPdo_count;i++)terminate_pdo(&s->RxPDO[i]);
    for (int i = 0; i < s->TxPdo_count;i++)terminate_pdo(&s->TxPDO[i]);
    free(s->Sm);
    free(s->RxPDO);
    free(s->TxPDO);
    free(s->name);
    free(s->type);
}

void terminate_xml_parsed_conf(){
    if(ecat_config == NULL)return;
    for (int i = 0; i < ecat_config->slave_count;i++){
        terminate_slaveconfig(&ecat_config->slavesConfig[i]);
    }
    free(ecat_config->slavesConfig);
}
/****************************************************************************/