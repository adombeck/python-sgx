NAME=sealing

all: $(NAME) $(NAME).manifest.sgx _$(NAME).so

LIB = /home/adrian/projects/python-sgx/lib
SDK = /opt/intel/sgxsdk
SDK_LIB = $(SDK)/lib64
SGX_GIT_REPO = /home/adrian/projects/linux-sgx

SRC = $(LIB)/memset_s.c $(LIB)/rdrand.c $(LIB)/sgx_aes_gcm.cpp $(LIB)/sgx_create_report.cpp $(LIB)/sgx_get_key.cpp $(LIB)/sgx_read_rand.cpp $(LIB)/trts.c $(LIB)/trts_pic.S

LIBS = $(SDK_LIB)/libsgx_tcrypto.a $(SDK_LIB)/libsgx_tservice.a
# $(SDK_LIB)/libsgx_tkey_exchange.a $(SDK_LIB)/libsgx_ukey_exchange.a $(SDK_LIB)/libsgx_tsetjmp.a $(SDK_LIB)/libsgx_tstdcxx.a

INCLUDES = -I$(SDK)/include -I$(SGX_GIT_REPO)/common/inc/internal -I$(SGX_GIT_REPO)/external/crypto_px/include -I$(SGX_GIT_REPO)/sdk/trts/linux -I$(SGX_GIT_REPO)/external/rdrand -I$(SGX_GIT_REPO)/sdk/tseal

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
	./create_manifest.py

$(NAME).manifest.sgx: $(NAME).manifest
	./sign_manifest.py


.PHONY: clean
clean:
	rm -rf $(NAME) $(NAME).manifest $(NAME).manifest.sgx $(NAME).token $(NAME).sig $(NAME)_wrap.c _$(NAME).so $(NAME).py *.o
