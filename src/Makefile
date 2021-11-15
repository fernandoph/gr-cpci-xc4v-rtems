ifndef RTEMS_DIR
        export RTEMS_DIR=${HOME}/sabia_bsw/rtems-4.10/build/sparc-rtems/leon3/lib/
endif


CC = sparc-rtems-gcc

INCLUDES = ../include/

CFLAGS = -O0 -g3 -Wall -msoft-float -c -fmessage-length=0 -B$(RTEMS_DIR) -DRASTA -I$(INCLUDES) -pg#-DDEBUG

LDFLAGS = -msoft-float -B$(RTEMS_DIR) -I$(INCLUDES)

TARGETDIR = build

CSOURCES = $(wildcard *.c)

COBJECTS = $(patsubst %.c, $(TARGETDIR)/%.o, $(CSOURCES))

EXECUTABLES = $(patsubst %.c, $(TARGETDIR)/%, $(CSOURCES))


all: $(EXECUTABLES)
	@rm -rf $(TARGETDIR)/*.o

$(EXECUTABLES): $(COBJECTS) 
	@echo ' '
	@echo 'Building target: $@'
	@mkdir -p "$(TARGETDIR)"
#       @echo $(patsubst %, /%, $^)
	$(CC)  $(LDFLAGS) $(patsubst $(TARGETDIR)/%.o, %.c, $^) -o $@
	@rm -rf $(COBJECTS)
	@echo 'Finished building target $@'
	@echo "load ../$@\nrun\nexit" > "$@.cmd"
	@echo ' '

$(COBJECTS):
	@echo ' '
	@echo 'Building obj: $@'
	@mkdir -p $(TARGETDIR)
#       @echo 'Invoking SPARC RTEMS C Compiler'
#       @$(CC) $(INCLUDES) $(CFLAGS) $(patsubst $(TARGETDIR)/%.o, src/%.c, $@) -o $@
	@$(CC) $(CFLAGS) $(patsubst $(TARGETDIR)/%.o, %.c, $@) -o $@
	@echo ' '

clean:
	@echo 'Cleaning...'
	@rm -rf $(TARGETDIR)
	@echo 'Finished successfully'
