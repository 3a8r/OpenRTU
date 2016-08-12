all:
# Make dnp3 library
	cd lib/dnp3 && echo "I'm in lib/dnp3" && \
	cmake CMakeLists.txt && \
	make && cd ../..;

#make libmodbus library

	cd lib/libmodbus && echo "I'm in lib/libmodbus" && \
	./autogen.sh && ./configure && make install	

#make sqlite3 library
	cd lib/sqlite3 && echo "I'm in lib/sqlite3" && \
	make

clean:
	cd lib/dnp3 && echo "I'm in lib/dnp3" && \
	make clean

