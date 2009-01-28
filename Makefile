ifndef BOOST_DIR
	ifndef BOOST_VERSION
		BOOST_VERSION = 1_33_1
	endif
	ifeq ($(OSTYPE), msys)
		BOOST_DIR = /mingw/include/boost-$(BOOST_VERSION)
	else
		ifeq ($(shell echo $$OSTYPE), cygwin)
			BOOST_DIR = /usr/include/boost-$(BOOST_VERSION)
		else
			BOOST_DIR = /usr/local/include/boost-$(BOOST_VERSION)
		endif
	endif
endif

ifndef LIBBOOST_SUFFIX
	LIBBOOST_SUFFIX = -gcc-mt-s
endif

# 非力なマシンではコンパイルに非常に長い時間がかかる、もしくはコンパイル時に
# ハングアップする場合には、-O0に変更してみてください。
OPTIMIZE = -O2

SUBDIRS  = toppers \
	toppers/itronx \
	cfg

all: $(SUBDIRS)

$(SUBDIRS)::
	make BOOST_DIR="$(BOOST_DIR)" LIBBOOST_SUFFIX="$(LIBBOOST_SUFFIX)" CXXFLAGS="$(OPTIMIZE)" -C $@

depend:
	for subdir in $(SUBDIRS) ; do \
		if test -d $$subdir ; then \
			make BOOST_DIR="$(BOOST_DIR)" LIBBOOST_SUFFIX="$(LIBBOOST_SUFFIX)" depend -C $$subdir; \
		fi ; \
	done \

clean:
	for subdir in $(SUBDIRS) ; do \
		if test -d $$subdir ; then \
			make clean -C $$subdir ; \
		fi ; \
	done \

realclean: clean
	for subdir in $(SUBDIRS) ; do \
		if test -d $$subdir ; then \
			rm -f $$subdir/Makefile.depend ; \
		fi ; \
	done \

