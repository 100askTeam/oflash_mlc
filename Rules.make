DEPFILES 	:= $(patsubst %.S,%.d,$(ASM_FILES)) $(patsubst %.c,%.d,$(C_FILES)) $(patsubst %.cpp,%.d,$(CPP_FILES))

OBJFILES 	:= $(patsubst %.S,%.o,$(ASM_FILES)) $(patsubst %.c,%.o,$(C_FILES)) $(patsubst %.cpp,%.o,$(CPP_FILES))


built-in.o : $(OBJFILES)  $(patsubst %/,%/built-in.o,$(SUBDIRS))
	$(LD) $(LDFLAGS) -r -o $@ $(OBJFILES) $(patsubst %/,%/built-in.o,$(SUBDIRS))

$(patsubst %/,%/built-in.o,$(SUBDIRS)): FORCE
	for i in $(SUBDIRS); do \
		$(MAKE) -C $$i || exit 1; \
		done

FORCE :;

dep : $(DEPFILES) FORCE
	for i in $(SUBDIRS); do \
                cd $$i; $(MAKE) dep; cd ..; \
		done

subdirs: FORCE
	for i in $(SUBDIRS); do \
		$(MAKE) -C $$i || exit 1 ; \
		done

subclean: FORCE
	for i in $(SUBDIRS); do \
		cd $$i; $(MAKE) subclean; cd ..; \
		done
	rm -f built-in.o $(OBJFILES)

subdistclean: FORCE
	for i in $(SUBDIRS); do \
		cd $$i; $(MAKE) subdistclean; cd ..; \
		done
	rm -f built-in.o $(OBJFILES) $(DEPFILES)

HAS_DEP	:= $(foreach file, $(DEPFILES), $(shell \
				if test -f $(file); then echo 'Y';\
				 else  echo 'N'; fi))
ifeq ($(findstring N, $(HAS_DEP)),)
include $(DEPFILES)
endif

%.d: %.S
	set -e; $(CC) -M $(CPPFLAGS) $< \
                  	| sed -e 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
                	[ -s $@ ] || rm -f $@


%.d: %.c
	set -e; $(CC) -M $(CPPFLAGS) $< \
                  	| sed -e 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
                	[ -s $@ ] || rm -f $@
%.d: %.cpp
	set -e; $(CPP) -M $(CPPFLAGS) $< \
                        | sed -e 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
                        [ -s $@ ] || rm -f $@
%.o:%.S
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $<

%.o:%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $<

%.o:%.cpp
	$(CPP) $(CPPFLAGS) $(CFLAGS) -o $@ $<
