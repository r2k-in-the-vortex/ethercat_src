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
int ParseVendor(xmlNode *vendorinfo, long *vendorid){
    //fprintf(stdout, "\t parsing vendorinfo\n");
    // first child is supposed to be <Id>, with vendor value, 2 for Beckhoff
    xmlNode *first_child, *node;
    char expectedTag[] = "Id"; 
    int maxcontentlen = 5;
    char tag_Content[maxcontentlen];
    int res = -1;
    char *ptr;
    //long vendorid = 0;

    first_child = vendorinfo->children;
    for (node = first_child; node; node = node->next){
        if(strcmp(node->name, expectedTag) == 0){
            *vendorid = strtol(node->children->content, &ptr, 10);
            if(&vendorid != 0) res = 0;
        }
    }
    
    if (&vendorid == 0)fprintf(stdout, "\t vendor id not found\n");

    return res;
}
int ParseDescriptions(xmlNode *descriptions){
    fprintf(stdout, "\t parsing descriptions\n");
    return 0;
}

int ParseSlave(xmlNode *slaveinfo){
    fprintf(stdout, "\t parsing slave %s\n", slaveinfo->name);
    xmlNode *first_child, *node;
    long vendorid;

    char expectedVendorElementName[] = "Vendor"; 
    char expectedDescriptionsElementName[] = "Descriptions"; 

    first_child = slaveinfo->children;
    for (node = first_child; node; node = node->next){
        if(strcmp(node->name, expectedVendorElementName) == 0){
            if(ParseVendor(node, &vendorid))return -1;
        } else if(strcmp(node->name, expectedDescriptionsElementName) == 0) {
            if(ParseDescriptions(node))return -1;
        }
    }
    
    fprintf(stdout, "\t vendor id %i\n", vendorid);

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
        else{
            ; // why is it getting elements that arent there?
        }
    }
    fprintf(stdout, "...\n");
    
    return 0;
}
/****************************************************************************/