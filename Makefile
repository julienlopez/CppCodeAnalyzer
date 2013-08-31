CHK_DIR_EXISTS= test -d
MKDIR= mkdir -p
DIR= $(dir $(lastword $(MAKEFILE_LIST)))

all: tests StarSimulator

Debug: all
Release: all

StarSimulator:
	@$(CHK_DIR_EXISTS) bin || $(MKDIR) bin
	@$(CHK_DIR_EXISTS) obj || $(MKDIR) obj
	cd src && make

tests:
	@$(CHK_DIR_EXISTS) tests/bin || $(MKDIR) tests/bin
	@$(CHK_DIR_EXISTS) tests/obj || $(MKDIR) tests/obj
	@$(CHK_DIR_EXISTS) tests/obj_tests || $(MKDIR) tests/obj_tests
	cd tests && make
	
clean:
	rm -rf obj
	rm -rf obj_tests
	
mrproper: clean
	rm -rf bin
	rm -rf obj
	rm -rf obj_tests
