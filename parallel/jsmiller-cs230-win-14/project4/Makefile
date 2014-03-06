all: project4

.PHONY: project4 

project4: 
	$(MAKE) -C src/

test: project4
	$(MAKE)  -C tests/

clean: 
	$(MAKE) clean -C src/ 
	$(MAKE) clean -C tests/ 
