all: locks 

.PHONY: locks 

locks: 
	$(MAKE) -C src/

tests: locks
	$(MAKE) -C tests/

clean: 
	$(MAKE) clean -C src/ 
	$(MAKE) clean -C tests/ 
