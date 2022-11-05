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
#include "cmdline.h"
#include "xml_conf_parser.h"
/****************************************************************************/

int main(int argc, char **argv)
{
    int ret = 0;
    struct gengetopt_args_info ai;
    if (cmdline_parser(argc, argv, &ai) != 0) {
        exit(1);
    }
    printf("ai.filename_arg: %s\n", ai.filename_arg);
    printf("ai.verbose_flag: %d\n", ai.verbose_flag);
    if (!ai.index_given)ai.index_arg = 0;
    printf("ai.index_arg: %d\n", ai.index_arg);
    
    parse_xml_config(ai.filename_arg);

    return ret;
}

/****************************************************************************/