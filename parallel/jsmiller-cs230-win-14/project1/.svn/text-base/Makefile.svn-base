
all: floyd tests 

.PHONY: floyd tests

floyd: 
	$(MAKE) -C src/

tests: 
	$(MAKE) -C tests/

clean: 
	$(MAKE) clean -C tests/
	$(MAKE) clean -C src/ 
