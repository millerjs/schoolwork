DEBUG = -D_$(DF)
export DEBUG

all: project4 libhash_utils

.PHONY: project4 

libhash_utils: 
	$(MAKE) -C hash_utils/

project4: 
	$(MAKE) -C src/

test: project4
	echo $(DEBUG)
	$(MAKE) -C tests/

clean: 
	$(MAKE) clean -C src/ 
	$(MAKE) clean -C tests/
	$(MAKE) clean -C libhash_utils/ 
