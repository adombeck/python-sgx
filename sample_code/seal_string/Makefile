all: seal_data seal_data.manifest.sgx

seal_data: seal_data.c /home/adrian/projects/my-sgx-lib/tSeal.cpp /home/adrian/projects/my-sgx-lib/tSeal_util.cpp /home/adrian/projects/my-sgx-lib/sgx_create_report.cpp /home/adrian/projects/my-sgx-lib/sgx_read_rand.cpp /home/adrian/projects/my-sgx-lib/tSeal_internal.cpp /home/adrian/projects/my-sgx-lib/memset_s.c /home/adrian/projects/my-sgx-lib/rdrand.c /home/adrian/projects/my-sgx-lib/sgx_get_key.cpp /home/adrian/projects/my-sgx-lib/sgx_aes_gcm.cpp /home/adrian/projects/my-sgx-lib/libsgx_tcrypto.a /home/adrian/projects/my-sgx-lib/ereport.S
	gcc $^ -I/home/adrian/projects/my-sgx-lib -ldl -o seal_data
	rm -rf seal_data.manifest.sgx

seal_data.manifest: 
	./create_manifest.py

seal_data.manifest.sgx: seal_data.manifest
	./sign_manifest.py



.PHONY: clean
clean:
	rm -rf seal_data seal_data.manifest seal_data.manifest.sgx seal_data.token seal_data.sig
