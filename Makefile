-include Makefile.config

ifndef BOOST_DIR
	ifndef BOOST_VERSION
		BOOST_VERSION = 1_42_0
	endif
	ifeq ($(OSTYPE), msys)
		BOOST_DIR = /mingw/include/boost-$(BOOST_VERSION)
	else
		ifeq ($(OSTYPE), darwin*)
			BOOST_DIR = /opt/homebrew/include/boost-$(BOOST_VERSION)
		else
			BOOST_DIR = /usr/local/include/boost-$(BOOST_VERSION)
		endif
	endif
endif

# 非力なマシンなどでコンパイルに非常に長い時間がかかる、もしくはコンパイル時に
# ハングアップする場合には、-O0に変更してみてください。
# OPTIMIZE = -O0 -std=c++11
OPTIMIZE = -std=c++11

MODULES = toppers/itronx \
	toppers/oil

ifdef HAS_CFG_XML
	MODULES := $(MODULES) toppers/xml
	OPTIONS := $(OPTIONS) -DHAS_CFG_XML=1
endif

SUBDIRS  = toppers $(MODULES) cfg

all: $(SUBDIRS)

$(SUBDIRS)::
	$(MAKE) BOOST_DIR="$(BOOST_DIR)" LIBBOOST_SUFFIX="$(LIBBOOST_SUFFIX)" CXXFLAGS="$(OPTIMIZE) $(OPTIONS)" -C $@

depend:
	for subdir in $(SUBDIRS) ; do \
		if test -d $$subdir ; then \
			$(MAKE) BOOST_DIR="$(BOOST_DIR)" LIBBOOST_SUFFIX="$(LIBBOOST_SUFFIX)" depend -C $$subdir; \
		fi ; \
	done \

clean:
	for subdir in $(SUBDIRS) ; do \
		if test -d $$subdir ; then \
			$(MAKE) clean -C $$subdir ; \
		fi ; \
	done \

realclean: clean
	for subdir in $(SUBDIRS) ; do \
		if test -d $$subdir ; then \
			rm -f $$subdir/Makefile.depend ; \
		fi ; \
	done
	rm -f Makefile.config
