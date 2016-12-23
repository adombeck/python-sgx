all: get_report get_report.manifest.sgx

get_report: get_report.c /home/adrian/projects/my-sgx-lib/ereport.S 
	gcc $^ -I/home/adrian/projects/my-sgx-lib -ldl -o get_report
	rm -rf get_report.manifest.sgx

get_report.manifest: 
	./create_manifest.py

get_report.manifest.sgx: get_report.manifest
	./sign_manifest.py



.PHONY: clean
clean:
	rm -rf get_report get_report.manifest get_report.manifest.sgx get_report.token get_report.sig
