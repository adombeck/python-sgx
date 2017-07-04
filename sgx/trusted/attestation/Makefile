NAME=attestation
SO_NAME=_$(NAME)_swig.so

all: $(SO_NAME)

PROJECT_HOME = ../../..

UTILS = $(PROJECT_HOME)/utils

include $(PROJECT_HOME)/Makefile.config

SWIG_LIB = $(PROJECT_HOME)/lib/swig

$(NAME).i:
	echo "// Generated from $(NAME).i.template\n" > $(NAME).i
	sed -e "s:\$$(NAME):$(NAME):g" $(NAME).i.template >> $(NAME).i
	chmod 444 $(NAME).i

$(NAME)_wrap.c: $(NAME).i
	swig -Wall -Werror -python -I$(SDK)/include -I$(SWIG_LIB) $<

$(NAME)_wrap.o: $(NAME)_wrap.c
	gcc -Wall -Werror -fPIC -c $^ -I$(SDK)/include -I$(SWIG_LIB) -I/usr/include/python3.5 -ldl

$(NAME).o: $(NAME).c
	gcc -Wall -Werror -fPIC -c $^ -I$(SDK)/include -I/usr/include/python3.5

$(SO_NAME): $(NAME).o $(NAME)_wrap.o call_tables.o
	gcc -g -Wall -Werror  -shared $^ -o $@ \
		-I$(SDK)/include \
		-I/usr/include/python3.5 \
		-shared-libgcc \
		-L$(SDK)/lib64 \
		-Wl,-Bstatic \
		-Wl,--whole-archive -lsgx_trts -Wl,--no-whole-archive \
		-Wl,--start-group \
			-lsgx_tstdc \
			-lsgx_tkey_exchange \
			-lsgx_tcrypto \
			-lsgx_tservice \
		-Wl,--end-group \
		-Wl,-Bsymbolic \
		-Wl,--defsym,__ImageBase=0 \
		-Wl,-Bdynamic -lsgx_uae_service -lsgx_ukey_exchange

call_tables.o: call_tables.c
	gcc -g -Wall -Werror  -fPIC -c $^ -I$(SDK)/include

exec: $(NAME).o call_tables.o
	gcc -g -Wall -Werror $^ -o $@ \
		-I$(SDK)/include \
		-I/usr/include/python3.5 \
		-shared-libgcc \
		-L$(SDK)/lib64 \
		-Wl,-Bstatic \
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
		-Wl,-Bdynamic -lsgx_uae_service -lsgx_ukey_exchange

	$(UTILS)/create_manifest.py
	$(UTILS)/sign_manifest.py


.PHONY: clean
clean:
	rm -rf $(NAME).i $(NAME)_wrap.c $(SO_NAME) $(NAME)_swig.py *.o exec
