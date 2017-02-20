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
	swig -python -I$(SDK)/include $<

$(NAME)_wrap.o: $(NAME)_wrap.c
	gcc -Wall -fPIC -c $^ -I$(SDK)/include -I/usr/include/python3.5 -ldl

test_t.o: test_t.c
	gcc -g -fPIC -c test_t.c -iquote/opt/pysgx/sgxsdk/include/tlibc -I/opt/pysgx/sgxsdk/include 

$(NAME).o: $(NAME).c
	gcc -g -fPIC -c $^ -I/opt/pysgx/sgxsdk/include -I/usr/include/python3.5 

# XXX: Re-add -Wl,--no-undefined and include all required static libraries
$(SO_NAME): $(NAME).o  test_t.o $(NAME)_wrap.o
	gcc -g -shared $^ -o $@ -shared-libgcc -L/opt/pysgx/sgxsdk/lib64 -Wl,--whole-archive -lsgx_trts -Wl,--no-whole-archive -Wl,--start-group -lsgx_tstdc -lsgx_tstdcxx -lsgx_tkey_exchange -lsgx_tcrypto -lsgx_tservice -Wl,--end-group -Wl,-Bsymbolic -Wl,--defsym,__ImageBase=0 -Wl,--allow-multiple-definition
	

.PHONY: clean
clean:
	rm -rf $(NAME)_wrap.c $(SO_NAME) $(NAME)_swig.py *.o
