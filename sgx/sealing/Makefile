NAME=sealing

all: $(NAME) $(NAME).manifest.sgx _$(NAME).so

PROJECT_HOME = ../..

include $(PROJECT_HOME)/Makefile.config

SDK_LIB = $(SDK)/lib64
LIB = $(PROJECT_HOME)/lib
UTILS = $(PROJECT_HOME)/utils

SRC = $(LIB)/memset_s.c $(LIB)/rdrand.c $(LIB)/sgx_aes_gcm.cpp $(LIB)/sgx_create_report.cpp $(LIB)/sgx_get_key.cpp $(LIB)/sgx_read_rand.cpp $(LIB)/trts.c $(LIB)/trts_pic.S

LIBS = $(SDK_LIB)/libsgx_tcrypto.a $(SDK_LIB)/libsgx_tservice.a
# $(SDK_LIB)/libsgx_tkey_exchange.a $(SDK_LIB)/libsgx_ukey_exchange.a $(SDK_LIB)/libsgx_tsetjmp.a $(SDK_LIB)/libsgx_tstdcxx.a

INCLUDES = -I$(SDK)/include -I$(SDK_GIT_DIR)/common/inc/internal -I$(SDK_GIT_DIR)/external/crypto_px/include -I$(SDK_GIT_DIR)/sdk/trts/linux -I$(SDK_GIT_DIR)/external/rdrand -I$(SDK_GIT_DIR)/sdk/tseal

$(NAME): $(NAME).o $(SRC) $(LIBS)
	gcc -Wall $^ $(INCLUDES) -ldl -o $@

$(NAME)_wrap.c: $(NAME).i
	swig -python -I$(LIB) $(INCLUDES) $<

$(NAME).o: $(NAME).c $(NAME)_wrap.c $(SRC)
	gcc -Wall -fPIC -c $^ -I$(LIB) $(INCLUDES) -I/usr/include/python3.5 -ldl

_$(NAME).so: $(NAME).o
	gcc -Wall -shared *.o $(LIBS) -o $@

$(NAME).manifest:
	rm -rf $(NAME).manifest.sgx
	python3 $(PROJECT_HOME)/utils/create_manifest.py
#	$(UTILS)/create_manifest.py

$(NAME).manifest.sgx: $(NAME).manifest
	$(UTILS)/sign_manifest.py


.PHONY: clean
clean:
	rm -rf $(NAME) $(NAME).manifest $(NAME).manifest.sgx $(NAME).token $(NAME).sig $(NAME)_wrap.c _$(NAME).so $(NAME).py *.o
