all: get_report get_report.manifest.sgx

get_report.manifest:
	./create_manifest.py

get_report.manifest.sgx: get_report.manifest
	./sign_manifest.py

get_report: get_report.c Enclave.cpp /home/adrian/projects/my-sgx-lib/ereport.S 
	@gcc $^ -I/home/adrian/projects/my-sgx-lib -o get_report

.PHONY: clean
clean:
	rm -rf get_report get_report.manifest get_report.manifest.sgx get_report.token get_report.sig
