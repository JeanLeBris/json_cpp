SRCNAMES= example_1.cpp
SRC= $(foreach srcname, $(SRCNAMES), $(SRCDIR)/$(srcname))
OBJNAMES= $(SRCNAMES:.cpp=.o)
OBJ= $(foreach objname, $(OBJNAMES), $(OBJDIR)/$(objname))

examples: obj bin $(OBJNAMES)
	$(COPYFILE) ..$(FILE_SLASH)$(BINDIR)$(FILE_SLASH)* .$(FILE_SLASH)$(BINDIR)

%.o:
	$(CC) -c $(SRCDIR)/$(@:.o=.cpp) -o $(OBJDIR)/$@
	$(CC) $(OBJDIR)/$@ -L ../$(BINDIR) -l $(EXEC) -l $(DEPENDENCIES) -o $(BINDIR)/$(@:.o=.exe)
# The "-l $(DEPENDENCIES)" should not be necessary in the case of a compilation with shared libraries, but it is necessary in the case of a compilation with static libraries

.PHONY: examples

bin:
	mkdir bin

obj:
	mkdir obj

clean:
	$(RMFILE) $(OBJDIR)$(FILE_SLASH)*
	$(RMDIR) $(OBJDIR)
	$(RMFILE) $(BINDIR)$(FILE_SLASH)*
	$(RMDIR) $(BINDIR)
