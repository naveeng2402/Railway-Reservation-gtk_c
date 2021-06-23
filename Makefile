default:
	make res
	gcc -g main.c resources/rsc.c -o bin/app `pkg-config --cflags --libs gtk+-3.0 sqlite3` -export-dynamic -Wall && bin/app

res:
	cd resources ; glib-compile-resources --target=rsc.c --generate-source rsc_config.xml ; cd ..

cmake:
	make res;
	mkdir -p build ;
	cmake -S . -B build/;
	cd build/; make ; ./Railway_reservation; cd ..

build_clean:
	rm -rf build/*
	