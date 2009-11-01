build:
	@echo "Building..."
	@cc -std=c99 -O2 -fPIC -c pretty_print.c
	@cc -std=c99 -O2 -shared -Wl,-soname,libprettyprint.so.1 -o libprettyprint.so.1.0 pretty_print.o
	@rm pretty_print.o
	@echo "Installing..."
	@mv libprettyprint.so.1.0 /usr/local/lib/
	@cp pretty_print.h /usr/local/include/
	@ldconfig
