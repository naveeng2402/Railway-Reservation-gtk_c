default:
	make res
	gcc -g main.c functions.c slots.c sqlite_funcs.c thread_funcs.c  resources/rsc.c -o bin/app `pkg-config --cflags --libs gtk+-3.0 sqlite3 python3-embed` -export-dynamic -Wall && bin/app

win:
	make res
	gcc -g main.c functions.c slots.c sqlite_funcs.c thread_funcs.c  resources/rsc.c -o bin/app.exe `pkg-config --cflags --libs gtk+-3.0 sqlite3 python3-embed` -Wall && bin/app.exe

res:
	cd resources ; glib-compile-resources --target=rsc.c --generate-source rsc_config.xml ; cd ..

cmake:
	make res;
	mkdir -p build ;
	cmake -S . -B build/;
	cd build/; make ; ./Railway_reservation; cd ..

build_clean:
	rm -rf build
	