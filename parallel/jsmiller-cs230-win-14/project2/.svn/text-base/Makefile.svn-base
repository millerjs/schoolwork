all: firewall tests


.PHONY: firewall tests

firewall: 
	$(MAKE) -C src/

tests: 
	$(MAKE) -C tests/

clean: 
	$(MAKE) clean -C src/ 
	$(MAKE) clean -C tests/ 
