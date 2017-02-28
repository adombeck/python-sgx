NAME=attestation
SO_NAME=_$(NAME)_swig.so

all: $(SO_NAME)

PROJECT_HOME = ../..

include $(PROJECT_HOME)/Makefile.config

$(NAME).i:
	sed -e "s:\$$(NAME):$(NAME):g" $(NAME).i.template > $(NAME).i

$(NAME)_wrap.c: $(NAME).i
	swig -python -I$(SDK)/include $<

$(NAME)_wrap.o: $(NAME)_wrap.c
	gcc -g -Wall -fPIC -c $^ -I$(SDK)/include -I/usr/include/python3.5 -ldl

call_tables.o: call_tables.c
	gcc -g -Wall -fPIC -c $^ -iquote$(SDK)/include/tlibc -I$(SDK)/include

$(NAME).o: $(NAME).c
	gcc -g -Wall -fPIC -c $^ -I$(SDK)/include -I/usr/include/python3.5

# XXX: Re-add -Wl,--no-undefined and include all required static libraries
# XXX: Why does -Wl,-Bstatic -lprotobuf not load /usr/lib/x86_64-linux-gnu/libprotobuf.a statically?
$(SO_NAME): $(NAME).o  call_tables.o $(NAME)_wrap.o
	gcc -g -Wall -shared $^ -o $@ \
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
        -Wl,-Bdynamic -lsgx_uae_service

exec: $(NAME).o call_tables.o
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
        -Wl,-Bdynamic -lsgx_uae_service

.PHONY: clean
clean:
	rm -rf $(NAME).i $(NAME)_wrap.c $(SO_NAME) $(NAME)_swig.py *.o a.out
