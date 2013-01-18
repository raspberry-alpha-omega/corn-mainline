default: ../../lib/$(PLATFORM)/$(MODULE).a

CFILES=$(wildcard *.c)
SFILES=$(wildcard *.s)

../../lib/$(PLATFORM)/$(MODULE).a: $(addprefix $(PLATFORM)/,$(subst .c,.o,$(CFILES))) $(addprefix $(PLATFORM)/,$(subst .s,.o,$(SFILES)) )
	$(ARCH)ar cr $@ $^

$(PLATFORM)/%.o: %.c
	$(ARCH)gcc -I. -I.. $(CFLAGS) -c -o $@ $<

$(PLATFORM)/%.o: %.s
	$(ARCH)as $(ASFLAGS) -o $@ $<

clean: FORCE
	rm -f $(foreach dir,$(FLAVOURS),$(FLAVOUR)/*.o)
	
FORCE:
