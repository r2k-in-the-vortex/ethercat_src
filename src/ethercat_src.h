// configure parametrized via config file, look under conf/ethercatcfg.txt

#ifdef __cplusplus
extern "C"{
#endif 

int ethercat_configure(char *paramsfile);
int ethercat_callcyclic(int buffersize, uint8_t *bool_input, uint8_t *bool_output, uint8_t *byte_input, uint8_t *byte_output, uint16_t *word_input, uint16_t *word_output);

#ifdef __cplusplus
}
#endif

