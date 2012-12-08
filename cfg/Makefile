-include ../Makefile.config

PROJDIR = ..
WORKDIR = $(PROJDIR)/cfg

CPPFLAGS := $(CPPFLAGS) -I$(BOOST_DIR) -I$(PROJDIR)
CXXFLAGS := $(CXXFLAGS) -O2 -g
LDFLAGS := $(CXXFLAGS)

ifeq ($(OSTYPE), msys)
	LDFLAGS := $(LDFLAGS) -static-libgcc -static-libstdc++
endif

CXXFILES = \
	cfg.cpp \
	cfg0.cpp \
	cfg1.cpp \
	cfg2.cpp \
	cfg3.cpp \
	cfg4.cpp \
	cfg_version.cpp

OBJFILES := $(CXXFILES:%.cpp=%.o)

BOOST_LIBFILES := $(BOOST_LIBFILES) -lboost_system$(LIBBOOST_SUFFIX) -lboost_filesystem$(LIBBOOST_SUFFIX) -lboost_program_options$(LIBBOOST_SUFFIX) -lboost_regex$(LIBBOOST_SUFFIX)

LIBFILES = $(PROJDIR)/toppers/itronx/libitronx.a \
	$(PROJDIR)/toppers/oil/liboil.a \
	$(PROJDIR)/toppers/libtoppers.a \

all: cfg

cfg: $(OBJFILES) $(LIBFILES)
	$(CXX) $(LDFLAGS) -o cfg $(OBJFILES) -L$(LIBBOOST_DIR) $(LIBFILES) $(BOOST_LIBFILES)

depend:
	$(CXX) $(CPPFLAGS) -M $(CXXFILES) > Makefile.depend

clean:
	-rm -f *.o cfg *.exe

-include Makefile.depend
