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
#include "xml_conf_parser.h"
#include "ecat_handler.h"
#include "log.h"
#include "ethercat_src.h"
#include <time.h>


#define BUFFER_SIZE 1024
/****************************************************************************/

uint8_t shortvar = 0;
uint16_t longvar = 0;
uint32_t dwordvar = 0;
uint64_t lwordvar = 0;
uint8_t *bool_input_call_back(int a, int b){ return &shortvar; }
uint8_t *bool_output_call_back(int a, int b){ return &shortvar; }
uint8_t *byte_input_call_back(int a){ return &shortvar; }
uint8_t *byte_output_call_back(int a){ return &shortvar; }
uint16_t *int_input_call_back(int a){ return &longvar; }
uint16_t *int_output_call_back(int a){ return &longvar; }
uint32_t *dint_input_call_back(int a){ return &dwordvar; }
uint32_t *dint_output_call_back(int a){ return &dwordvar; }
uint64_t *lint_input_call_back(int a){ return &lwordvar; }
uint64_t *lint_output_call_back(int a){ return &lwordvar; }
void logger_callbackf(unsigned char *msg){ printf("PLC log: %s", msg); }

//-----------------------------------------------------------------------------
// Helper function - Makes the running thread sleep for the ammount of time
// in milliseconds
//-----------------------------------------------------------------------------
void sleepms(int milliseconds)
{
	struct timespec ts;
	ts.tv_sec = milliseconds / 1000;
	ts.tv_nsec = (milliseconds % 1000) * 1000000;
	nanosleep(&ts, NULL);
}

int main (int argc, char **argv){
    type_logger_callback logger = logger_callbackf; 
    ethercat_configure(argv[1], logger);
    boolvar_call_back bool_input_callback = bool_input_call_back;
    boolvar_call_back bool_output_callback = bool_output_call_back;
    int8var_call_back byte_input_callback = byte_input_call_back;
    int8var_call_back byte_output_callback = byte_output_call_back;
    int16var_call_back int_input_callback = int_input_call_back;
    int16var_call_back int_output_callback = int_output_call_back;
    int32var_call_back dint_input_callback = dint_input_call_back;
    int32var_call_back dint_output_callback = dint_output_call_back;
    int64var_call_back lint_input_callback = lint_input_call_back;
    int64var_call_back lint_output_callback = lint_output_call_back;

    for (int i = 0; i < 100; i++){
        ethercat_callcyclic(BUFFER_SIZE, 
            bool_input_callback, 
            bool_output_callback, 
            byte_input_callback, 
            byte_output_callback, 
            int_input_callback, 
            int_output_callback, 
            dint_input_callback, 
            dint_output_callback, 
            lint_input_callback, 
            lint_output_callback);
            
        sleepms(100);
    }

    ethercat_terminate_src();
    return 0;
}

/****************************************************************************/