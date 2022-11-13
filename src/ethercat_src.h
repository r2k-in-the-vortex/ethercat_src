// configure parametrized via config file, look under conf/ethercatcfg.txt

#ifdef __cplusplus
extern "C"{
#endif 

typedef u_int8_t *(*boolvar_call_back) (int, int);
typedef u_int8_t *(*int8var_call_back) (int);
typedef u_int16_t *(*int16var_call_back) (int);
typedef void (*type_logger_callback) (unsigned char*);

int ethercat_configure(char *paramsfile, type_logger_callback);
int ethercat_callcyclic(int buffersize, 
    boolvar_call_back bool_input, 
    boolvar_call_back bool_output, 
    int8var_call_back byte_input, 
    int8var_call_back byte_output, 
    int16var_call_back word_input, 
    int16var_call_back word_output);

#ifdef __cplusplus
}
#endif

