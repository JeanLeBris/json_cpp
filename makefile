.PHONY: compile examples clean
# .SILENT: compile examples clean

export CC=g++
export CFLAGS=
export LDFLAGS=
export EXEC=json
SRCNAMES= json.cpp
export SRCDIR=src
SRC= $(foreach srcname, $(SRCNAMES), $(SRCDIR)/$(srcname))
OBJNAMES= $(SRCNAMES:.cpp=.o)
export OBJDIR=obj
OBJ= $(foreach objname, $(OBJNAMES), $(OBJDIR)/$(objname))
export BINDIR=bin
export DEPENDENCIES= json

EXAMPLESDIR=examples

ifeq ($(OS), Windows)
	RMDIR= rmdir
	RMFILE= del /s /q
	COPYFILE= copy
	SHARED_LIBRARY_EXT= dll
	FILE_SLASH=\\

else
ifeq ($(OS), Windows_NT)
	OS=Windows
	RMDIR= rmdir
	RMFILE= del /s /q
	COPYFILE= copy
	SHARED_LIBRARY_EXT= dll
	FILE_SLASH=\\

else
ifeq ($(OS), Linux)
	RMDIR= rmdir
	RMFILE= rm
	COPYFILE= cp
	SHARED_LIBRARY_EXT= so
	FILE_SLASH=/
endif
endif
endif

export RMDIR
export RMFILE
export COPYFILE
export SHARED_LIBRARY_EXT
export FILE_SLASH

ifeq ($(LIBRARY_TYPE), shared)
else
	ifeq ($(LIBRARY_TYPE), static)
	else
		LIBRARY_TYPE=shared
	endif
endif
export LIBRARY_TYPE

compile:bin obj $(OBJNAMES)
ifeq ($(LIBRARY_TYPE), shared)
ifeq ($(OS), Windows)
	$(CC) -fpic -shared $(OBJ) -o $(BINDIR)/lib$(EXEC).$(SHARED_LIBRARY_EXT) $(LDFLAGS)
endif
ifeq ($(OS), Linux)
	$(CC) -shared $(OBJ) -o $(BINDIR)/lib$(EXEC).$(SHARED_LIBRARY_EXT) $(LDFLAGS)
endif
endif
ifeq ($(LIBRARY_TYPE), static)
	ar rcs $(BINDIR)/lib$(EXEC).a $(OBJ) $(LDFLAGS)
endif

examples:compile
	@(cd $(EXAMPLESDIR) && $(MAKE) $@)
# 	cp $(BINDIR)/libargparse.dll $(EXAMPLESDIR)/$(BINDIR)/libargparse.dll

%.o:
ifeq ($(LIBRARY_TYPE), shared)
ifeq ($(OS), Windows)
	$(CC) -c $(SRCDIR)/$(@:.o=.cpp) -o $(OBJDIR)/$@ $(CFLAGS)
endif
ifeq ($(OS), Linux)
	$(CC) -c -fpic $(SRCDIR)/$(@:.o=.cpp) -o $(OBJDIR)/$@ $(CFLAGS)
endif
endif
ifeq ($(LIBRARY_TYPE), static)
	$(CC) -c $(SRCDIR)/$(@:.o=.cpp) -o $(OBJDIR)/$@ $(CFLAGS)
endif

bin:
	mkdir bin

obj:
	mkdir obj

clean:
	- $(RMFILE) $(OBJDIR)$(FILE_SLASH)*
	- $(RMDIR) $(OBJDIR)
	- $(RMFILE) $(BINDIR)$(FILE_SLASH)*
	- $(RMDIR) $(BINDIR)





# static:bin obj
# 	g++ -c ./src/deallocator.cpp -o ./obj/deallocator.o
# 	ar rcs ./bin/libdeallocator.a ./obj/deallocator.o

# dynamic:bin obj
# 	g++ -fPIC -c ./src/deallocator.cpp -o ./obj/deallocator.o
# 	g++ -shared -o ./bin/libdeallocator.dll ./obj/deallocator.o

# bin:
# 	mkdir bin

# obj:
# 	mkdir obj
