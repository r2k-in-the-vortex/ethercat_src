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

    config->type = getNodeTextContent(typedata);
    log_trace("%s product code 0x%08X, rev 0x%08X", config->type, config->product_code, config->product_revision);
    return 0;
}

int ParseDeviceName(xmlNode *namenode, char *name){
    if(namenode == NULL){
        log_error("name node not found");
        return -1;
    }
    name = namenode->children->content;
    if(name == NULL){
        log_error("no name parsed");
        return -1;
    }
    log_trace("%s", name);
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
    xmlNode *entry;
    pdo->pdotype = node->name;
    pdo->sm = getAttributeValueNamed(node, "Sm");
    pdo->fixed = getAttributeValueNamed(node, "Fixed");
    pdo->mandatory = getAttributeValueNamed(node, "Mandatory");
    pdo->index = getNodeTextContent(getSingularnodeNamed(node->children, "Index"));
    pdo->name = getNodeTextContent(getSingularnodeNamed(node->children, "Name"));
    entry = getSingularnodeNamed(node->children, "Entry");
    pdo->entryindex = getNodeTextContent(getSingularnodeNamed(entry->children, "Index"));
    pdo->subindex = getNodeTextContent(getSingularnodeNamed(entry->children, "SubIndex"));
    pdo->bitlen = getNodeTextContent(getSingularnodeNamed(entry->children, "BitLen"));
    pdo->entryname = getNodeTextContent(getSingularnodeNamed(entry->children, "Name"));
    pdo->datatype = getNodeTextContent(getSingularnodeNamed(entry->children, "DataType"));
    log_trace("%s %s %s %s %s %s %s %s %s %s %s", pdo->pdotype, pdo->sm, pdo->fixed, pdo->mandatory, pdo->index, pdo->name, pdo->entryindex, pdo->subindex, pdo->bitlen, pdo->entryname, pdo->datatype);
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

    if(ParseDeviceName(namenode, name))return -1;

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
        start = start->next;
    }
    start = device->children;
    for (int i = 0; i< config->TxPdo_count;i++){
        start = getNextNodeNamed(start, "TxPdo");
        ParsePdo(start, &TxPdos[i]);
        start = start->next;
    }

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
int parse_xml_config(char *filename, EcatConfig *config){
    log_trace("parsing file %s", filename);
    
    xmlDoc         *document;
    xmlNode        *root, *first_child, *node;

    document = xmlReadFile(filename, NULL, 0);
    root = xmlDocGetRootElement(document);

    char expectedRootName[] = "EtherCATInfoList"; 
    char expectedSlaveName[] = "EtherCATInfo"; 

    log_trace("Root is <%s> (%i)", root->name, root->type);
    if(strcmp(root->name, expectedRootName) != 0){
        log_error("Invalid configuration file, expected root name %s but got %s", expectedRootName, root->name);
        return -1;
    }

    first_child = root->children;

    config->slave_count = countNodesNamed(first_child, expectedSlaveName);
    
    
    SlaveConfig *slavesConfig = (SlaveConfig*) malloc(config->slave_count * sizeof(SlaveConfig));
    //slavesConfig = (SlaveConfig*) malloc(config->slave_count * sizeof(SlaveConfig));
    uint32_t *test = &slavesConfig[0].product_revision;

    config->slavesConfig = slavesConfig;
    if (config->slavesConfig == NULL){
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
            config->slavesConfig[slaveindex] = conf;
            slaveindex++;
        }
    }
    
    return 0;
}
/****************************************************************************/