// configure parametrized via config file, look under conf/ethercatcfg.txt

#ifdef __cplusplus
extern "C"{
#endif 

typedef u_int8_t *(*boolvar_call_back) (int, int);
typedef u_int8_t *(*int8var_call_back) (int);
typedef u_int16_t *(*int16var_call_back) (int);
typedef u_int32_t *(*int32var_call_back) (int);
typedef u_int64_t *(*int64var_call_back) (int);
typedef void (*type_logger_callback) (unsigned char*);

// Setup before realtime
int ethercat_configure(char *paramsfile, type_logger_callback);

// Realtime data exchange
int ethercat_callcyclic(int buffersize, 
    boolvar_call_back bool_input, 
    boolvar_call_back bool_output, 
    int8var_call_back byte_input, 
    int8var_call_back byte_output, 
    int16var_call_back word_input, 
    int16var_call_back word_output, 
    int32var_call_back dword_input, 
    int32var_call_back dword_output, 
    int64var_call_back lword_input, 
    int64var_call_back lword_output);
    
// CoE access
int ethercat_read_sdo();
int ethercat_write_sdo();

// Clean up resources
void ethercat_terminate_src();

#ifdef __cplusplus
}
#endif

