CXX=g++
CXXFLAGS=-g $(shell pkg-config --cflags libpressio_opt) $(shell pkg-config --cflags libpressio_cxx) $(shell pkg-config --cflags ompi-cxx)
LDFLAGS=$(shell pkg-config --libs libpressio_opt)  $(shell pkg-config --libs libpressio_cxx) $(shell pkg-config --libs ompi-cxx)
run_fraz:
check: test
	./test
clean:
	$(RM) test
