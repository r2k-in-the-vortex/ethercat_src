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
        if (node->name == NULL)return NULL;
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

/****************************************************************************/
int getVendorId(xmlNode *slaveinfo, long *vendorid){
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

int ParseDeviceType(xmlNode *typedata, char *producttype, char *productcode, char *revisionNo){
    if(typedata == NULL){
        fprintf(stdout, "\t Typenode NULL\n");
        return -1;
    }
    productcode = getAttributeValueNamed(typedata, "ProductCode");
    revisionNo = getAttributeValueNamed(typedata, "RevisionNo");
    producttype = getNodeTextContent(typedata);
    fprintf(stdout, "\t %s product code %s, rev %s\n", producttype, productcode, revisionNo);
    return 0;
}
int ParseDeviceName(xmlNode *namenode, char *name){
    if(namenode == NULL){
        fprintf(stdout, "\t name node not found\n");
        return -1;
    }
    name = namenode->children->content;
    if(name == NULL){
        fprintf(stdout, "\t no name parsed\n");
        return -1;
    }
    fprintf(stdout, "\t %s\n", name);
    return 0;
}

int ParseDescriptions(xmlNode *descriptions){
    // data out
    char *producttype, *productcode, *revisionNo, *name;

    // parse
    fprintf(stdout, "\t parsing descriptions\n");
    // <Descriptions><Devices><Device>the good stuff inside here</Device></Devices></Descriptions>
    // one element of devices, inside one device, any deviation from that is not implemented
    char expectedTypeElementName[] = "Type"; 
    char expectedNameElementName[] = "Name"; 
    char expectedSmElementName[] = "Sm"; 
    char expectedRxPDOElementName[] = "RxPdo"; 
    char expectedTxPDOElementName[] = "TxPdo"; 
    xmlNode *first_child, *node;
    xmlNode *devices = getSingularnodeNamed(descriptions->children, "Devices");
    xmlNode *device = getSingularnodeNamed(devices->children, "Device");
    xmlNode *typenode = getSingularnodeNamed(device->children, "Type");
    xmlNode *namenode = getSingularnodeNamed(device->children, "Name");

    if(ParseDeviceType(typenode, producttype, productcode, revisionNo))return -1;

    if(ParseDeviceName(namenode, name))return -1;
    /*
    devices = NULL;
    device = NULL;
    
    // find devices
    devices = getNextNodeNamed(descriptions->children, "Devices");
    if(devices == NULL){
        fprintf(stdout, "\t <Devices> not found\n");
        return -1;
    }
    
    // find device
    device = getNextNodeNamed(devices->children, "Device");
    if(device == NULL){
        fprintf(stdout, "\t <Device> not found\n");
        return -1;
    }
    // device has elements of interest - type, name, sm(don't know what that does, but it's probably important), RxPDO(outputs), TxPDO(inputs)
    // find device
    first_child = device->children;
    for (node = first_child; node; node = node->next){
        if(strcmp(node->name, expectedTypeElementName) == 0){
            if(ParseDeviceType(node, producttype, productcode, revisionNo))return -1;
        }/* else if(strcmp(node->name, expectedNameElementName) == 0) {
            if(ParseDeviceName(node))return -1;
        } else if(strcmp(node->name, expectedSmElementName) == 0) {
            ; // what is Sm good for?
        } else if(strcmp(node->name, expectedRxPDOElementName) == 0) {
            if(ParseDeviceRxPDO(node))return -1;
        } else if(strcmp(node->name, expectedTxPDOElementName) == 0) {
            if(ParseDeviceTxPDO(node))return -1;
        }
    }
    */
    return 0;
}

int ParseSlave(xmlNode *slaveinfo){
    fprintf(stdout, "\t parsing slave %s\n", slaveinfo->name);
    xmlNode *first_child, *node;
    long vendorid;

    if(getVendorId(slaveinfo, &vendorid) != 0){
        fprintf(stdout, "\t failed to get vendor id %ld\n", vendorid);
        return -1;
    }
    fprintf(stdout, "\t vendor id %ld\n", vendorid);

    char expectedDescriptionsElementName[] = "Descriptions"; 

    first_child = slaveinfo->children;
    for (node = first_child; node; node = node->next){
        if(strcmp(node->name, expectedDescriptionsElementName) == 0) {
            if(ParseDescriptions(node))return -1;
        }
    }
    

    return 0;
}
/****************************************************************************/
int parse_xml_config(char *filename){
    printf("parsing file %s\n", filename);
    
    xmlDoc         *document;
    xmlNode        *root, *first_child, *node;

    document = xmlReadFile(filename, NULL, 0);
    root = xmlDocGetRootElement(document);

    char expectedRootName[] = "EtherCATInfoList"; 
    char expectedSlaveName[] = "EtherCATInfo"; 

    fprintf(stdout, "Root is <%s> (%i)\n", root->name, root->type);
    if(strcmp(root->name, expectedRootName) != 0){
        fprintf(stdout, "Invalid configuration file, expected root name %s but got %s\n", expectedRootName, root->name);
        return -1;
    }

    first_child = root->children;
    for (node = first_child; node; node = node->next) {
        
        if(strcmp(node->name, expectedSlaveName) == 0){
            if(ParseSlave(node)){
                // failed to parse slave
                return -1;
            }
        }
    }
    fprintf(stdout, "...\n");
    
    return 0;
}
/****************************************************************************/