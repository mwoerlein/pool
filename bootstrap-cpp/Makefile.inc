CC=g++
CFLAGS= -fno-rtti -fno-exceptions -fno-pie -m32 -I$(INCDIR) -DCXX_VERSION$(CXX_VERSION) -DLINUX_ENV
ASFLAGS= --32
MAKEOP= -s
#MAKEOP= 

VERSION=$(shell g++ --version)
ifeq ($(VERSION), 2.95.4)
CXX_VERSION=$(shell g++ --version | sed "s/\./\_/g")
else
CXX_VERSION=$(shell g++ --version | grep g++ | cut -d ' ' -f4 | sed "s/\./\_/g")
endif

LIBDIR=./lib
SRCDIR=./src
OBJDIR=./obj
BINDIR=./bin
INCDIR=./include
INSTALLDIR=../bin

$(LIBDIR)/%.a:
	@make $(MAKEOP) -f ./Makefile.libs LIB=$@ $@

$(LIBDIR) $(OBJDIR) $(BINDIR):
	@mkdir -p $@
