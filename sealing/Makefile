NAME=sealing

all: $(NAME) $(NAME).manifest.sgx _$(NAME).so

LIB = /home/adrian/projects/python-sgx/lib

SRC = $(LIB)/memset_s.c $(LIB)/rdrand.c $(LIB)/sgx_aes_gcm.cpp $(LIB)/sgx_create_report.cpp $(LIB)/sgx_get_key.cpp $(LIB)/sgx_read_rand.cpp $(LIB)/trts.c $(LIB)/trts_pic.S $(LIB)/tSeal_aad.cpp $(LIB)/tSeal.cpp $(LIB)/tSeal_internal.cpp $(LIB)/tSeal_util.cpp
LIBS = /opt/intel/sgxsdk/lib64/libsgx_tcrypto.a
INCLUDES = -I/opt/intel/sgxsdk/include -I/home/adrian/projects/linux-sgx/common/inc/internal -I/home/adrian/projects/linux-sgx/external/crypto_px/include -I/home/adrian/projects/linux-sgx/sdk/trts/linux -I/home/adrian/projects/linux-sgx/external/rdrand -I/home/adrian/projects/linux-sgx/sdk/tseal

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
