ifndef RTEMS_DIR
	        export RTEMS_DIR=${HOME}/sabia_bsw/rtems-4.10/build/sparc-rtems/leon3/lib/
endif

CC = sparc-rtems-gcc

CFLAGS = -O0 -g3 -Wall -msoft-float -c -fmessage-length=0 -B$(RTEMS_DIR) -DRASTA #-DDEBUG

LDFLAGS = -msoft-float -B$(RTEMS_DIR)


LIBS = -L$(RTEMS_DIR)/../../c/leon3/cpukit/libmisc/shell/ -L$(RTEMS_DIR)/../../c/leon3/cpukit/libpci/

all:
	make -C src $@

clean:
	make -C src $@

