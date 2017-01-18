NAME=python3

all: $(NAME).manifest.sgx

PROJECT_HOME = ..
UTILS = $(PROJECT_HOME)/utils

$(NAME).manifest:
	rm -rf $(NAME).manifest.sgx
	$(UTILS)/create_manifest.py

$(NAME).manifest.sgx: $(NAME).manifest
	$(UTILS)/sign_manifest.py


.PHONY: clean
clean:
	rm -rf $(NAME).manifest $(NAME).manifest.sgx $(NAME).token $(NAME).sig 

