build:
	@echo "Building..."
	@gcc -fPIC -c pretty_print.c -o pretty_print.o
	@gcc -shared -Wl,-soname,libprettyprint.so.1 -o libprettyprint.so.1.0 pretty_print.o
	@rm pretty_print.o
	@echo "Installing..."
	@mv libprettyprint.so.1.0 /usr/local/lib/
	@cp pretty_print.h /usr/local/include/
	@ldconfig
