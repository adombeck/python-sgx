NAME=attestation
SO_NAME=_$(NAME)_swig.so

all: $(SO_NAME)

PROJECT_HOME = ../..

include $(PROJECT_HOME)/Makefile.config

SDK_LIB = $(SDK)/lib64
LIB = $(PROJECT_HOME)/lib
UTILS = $(PROJECT_HOME)/utils

SRC = $(LIB)/memset_s.c $(LIB)/rdrand.c $(LIB)/sgx_aes_gcm.cpp $(LIB)/sgx_create_report.cpp $(LIB)/sgx_get_key.cpp $(LIB)/sgx_read_rand.cpp $(LIB)/trts.c $(LIB)/trts_pic.S

LIBS = $(SDK_LIB)/libsgx_tcrypto.a $(SDK_LIB)/libsgx_tservice.a
# $(SDK_LIB)/libsgx_tkey_exchange.a $(SDK_LIB)/libsgx_ukey_exchange.a $(SDK_LIB)/libsgx_tsetjmp.a $(SDK_LIB)/libsgx_tstdcxx.a

INCLUDES = -I$(SDK)/include -I$(SDK_GIT_DIR)/common/inc -I$(SDK_GIT_DIR)/common/inc/internal -I$(SDK_GIT_DIR)/external/crypto_px/include -I$(SDK_GIT_DIR)/sdk/trts/linux -I$(SDK_GIT_DIR)/external/rdrand -I$(SDK_GIT_DIR)/sdk/tseal

$(NAME).i:
	sed -e "s:\$$(NAME):$(NAME):g" $(NAME).i.template > $(NAME).i

$(NAME)_wrap.c: $(NAME).i
	swig -python -I$(SDK)/include $<

$(NAME)_wrap.o: $(NAME)_wrap.c
	gcc -Wall -fPIC -c $^ -I$(SDK)/include -I/usr/include/python3.5 -ldl

$(NAME).o: $(NAME).c $(SRC)
	gcc -Wall -fPIC -c $^ -I$(LIB) $(INCLUDES) -I/usr/include/python3.5

$(SO_NAME): $(NAME).o $(NAME)_wrap.o
	gcc -Wall -shared *.o $(LIBS) -lsgx_uae_service -o $@


call_tables.o: call_tables.c
	gcc -g -Wall -fPIC -c $^ -iquote$(SDK)/include/tlibc -I$(SDK)/include

a.out: $(NAME).o call_tables.o
	gcc -g -Wall $^ \
		-I$(SDK)/include \
		-I/usr/include/python3.5 \
		-shared-libgcc \
		-L$(SDK)/lib64 \
		-Wl,-Bstatic \
		-lprotobuf \
		-Wl,--whole-archive -lsgx_trts -Wl,--no-whole-archive \
		-Wl,--start-group \
			-lsgx_tstdc \
			-lsgx_tstdcxx \
			-lsgx_tkey_exchange \
			-lsgx_tcrypto \
			-lsgx_tservice \
		-Wl,--end-group \
		-Wl,-Bsymbolic \
		-Wl,--defsym,__ImageBase=0 \
		-Wl,--allow-multiple-definition \
		-Wl,-Bdynamic -lsgx_uae_service -lsgx_ukey_exchange




.PHONY: clean
clean:
	rm -rf $(NAME).i $(NAME)_wrap.c $(SO_NAME) $(NAME)_swig.py *.o
