all:
	cd lib/dnp3 && echo "I'm in lib/dnp3" && \
	cmake CMakeLists.txt && \
	make && cd ../..;

	cd lib/libmodbus && echo "I'm in lib/libmodbus" && \
	./autogen.sh && ./configure && make install	

	cd lib && mkdir -p bld && cd bld &&\
	../sqlite/configure && make && make sqlite3.c
clean:
	cd lib/dnp3 && echo "I'm in lib/dnp3" && \
	make clean

