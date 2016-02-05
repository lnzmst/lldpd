# git clone https://github.com/lnzmst/lldpd.git
rebuild: clean
	mkdir build;
	cd build; CC=gcc CXX=g++ cmake ..
	cd build; make
	cd build; make install

clean:
	rm -rf usr build
	find . -name '*~' | xargs rm -f
