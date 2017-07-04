NAME=sealing
SO_NAME=_$(NAME)_swig.so

all: $(SO_NAME)

PROJECT_HOME = ../../..

include $(PROJECT_HOME)/Makefile.config

SWIG_LIB = $(PROJECT_HOME)/lib/swig

$(NAME).i:
	sed -e "s:\$$(NAME):$(NAME):g" $(NAME).i.template > $(NAME).i

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
		-lprotobuf \
		-Wl,--whole-archive -lsgx_trts -Wl,--no-whole-archive \
		-Wl,--start-group \
			-lsgx_tstdc \
			-lsgx_tkey_exchange \
			-lsgx_tcrypto \
			-lsgx_tservice \
		-Wl,--end-group \
		-Wl,-Bsymbolic \
		-Wl,--defsym,__ImageBase=0 \
		-Wl,-Bdynamic

call_tables.o: call_tables.c
	gcc -g -Wall -Werror  -fPIC -c $^ -I$(SDK)/include

.PHONY: clean
clean:
	rm -rf $(NAME).i $(NAME)_wrap.c $(SO_NAME) $(NAME)_swig.py *.o
