NAME=attestation
SO_NAME=_$(NAME)_swig.so

all: $(SO_NAME)

PROJECT_HOME = ../..

include $(PROJECT_HOME)/Makefile.config

SDK_LIB = $(SDK)/lib64
LIB = $(PROJECT_HOME)/lib
UTILS = $(PROJECT_HOME)/utils

SRC = $(LIB)/memset_s.c $(LIB)/rdrand.c $(LIB)/sgx_aes_gcm.cpp $(LIB)/sgx_create_report.cpp $(LIB)/sgx_get_key.cpp
# $(LIB)/sgx_read_rand.cpp $(LIB)/trts.c $(LIB)/trts_pic.S

LIBS = $(SDK_LIB)/libsgx_tcrypto.a $(SDK_LIB)/libsgx_tservice.a $(SDK_LIB)/libsgx_tstdc.a $(SDK_LIB)/libsgx_trts.a
# $(SDK_LIB)/libsgx_tkey_exchange.a $(SDK_LIB)/libsgx_ukey_exchange.a $(SDK_LIB)/libsgx_tsetjmp.a $(SDK_LIB)/libsgx_tstdcxx.a

INCLUDES = -I$(SDK)/include -I$(SDK_GIT_DIR)/common/inc -I$(SDK_GIT_DIR)/common/inc/internal -I$(SDK_GIT_DIR)/external/crypto_px/include -I$(SDK_GIT_DIR)/sdk/trts/linux -I$(SDK_GIT_DIR)/external/rdrand -I$(SDK_GIT_DIR)/sdk/tseal

$(NAME)_wrap.c: $(NAME).i
	@#swig -python -I$(LIB) $(INCLUDES) $<
	swig -python -I/opt/intel/sgxsdk/include $<

$(NAME).o: $(NAME).c $(NAME)_wrap.c #$(SRC)
	@#gcc -Wall -fPIC -c $^ -I$(LIB) $(INCLUDES) -I/usr/include/python3.5 -ldl
	g++ -g -fpie -I/usr/include/python3.5 -ldl -I/opt/intel/sgxsdk/include -c $^ 
	cc -g -fpie -I/usr/include/python3.5 -I/opt/intel/sgxsdk/include -I/opt/intel/sgxsdk/include/tlibc -I/opt/intel/sgxsdk/include/stlport -c Enclave/Enclave_t.c -o Enclave/Enclave_t.o

$(SO_NAME): $(NAME).o
	@#gcc -Wall -shared -fPIC -lstdc++ -Wl,--defsym,__ImageBase=0 *.o $(LIBS) -o $@
	g++ -g Enclave/Enclave_t.o $(NAME).o -o libenclave.so -nodefaultlibs -nostartfiles -L/opt/intel/sgxsdk/lib64 -Wl,--whole-archive -lsgx_trts -Wl,--no-whole-archive -Wl,--start-group -lsgx_tstdc -lsgx_tstdcxx -lsgx_tcrypto -lsgx_tservice -Wl,--end-group -Wl,-pie,-eenclave_entry -Wl,--defsym,__ImageBase=0


.PHONY: clean
clean:
	rm -rf $(NAME)_wrap.c $(SO_NAME) $(NAME)_swig.py *.o
