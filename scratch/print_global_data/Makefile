NAME=main

all: exec

PROJECT_HOME = ../..

UTILS = $(PROJECT_HOME)/utils

include $(PROJECT_HOME)/Makefile.config

INCLUDES = -I$(SDK)/include

$(NAME).o: $(NAME).c
	gcc -Wall -Werror -fPIC -c $^ $(INCLUDES) -I/usr/include/python3.5 -I$(PROJECT_HOME)/graphene/Pal/src

call_tables.o: call_tables.c
	gcc -g -Wall -Werror  -fPIC -c $^ -I$(SDK)/include

# gcc -g -Wall -O2 -std=gnu99 -fgnu89-inline -fno-builtin -nostdlib -I../include/pal -I../lib File.c ../src/user_start.o .lib/graphene-lib.a ../src/libpal.so Preload1.so Preload2.so ../include/pal/pal.h -o File

exec: $(NAME).o call_tables.o
	gcc -g -Wall -Werror $^ -o $@ \
		-I$(SDK)/include \
		-I/usr/include/python3.5 \
		-I$(PROJECT_HOME)/graphene/Pal/include/pal \
		-I$(PROJECT_HOME)/graphene/Pal/lib \
		$(PROJECT_HOME)/graphene/Pal/src/libpal.so \
		-shared-libgcc \
		-L$(SDK)/lib64 \
		-Wl,--whole-archive -lsgx_trts -Wl,--no-whole-archive \
		-Wl,--start-group \
			-lsgx_tstdc \
			-lsgx_tcrypto \
        -Wl,--end-group \
        -Wl,-Bstatic \
		-Wl,-Bsymbolic \
		-Wl,--defsym,__ImageBase=0 \
		-Wl,--allow-multiple-definition \
		-Wl,-Bdynamic

	$(UTILS)/create_manifest.py
	$(UTILS)/sign_manifest.py


.PHONY: clean
clean:
	rm -rf *.o exec *.sig *.token *.manifest *.manifest.sgx
