# Author Gr�goire Dooms
# Copied from Gecode Makefile.in
# The gecode configure script makes the Makefile.in from this Makefile.in.in
# Then the cp(Graph) configure script makes the Makefile from the Makefile.in

# get variables from top Makefile
#
gecode_top_srcdir = $(top_srcdir)
cpgraph_top_srcdir = $(curdir)

RMF = rm -f

KERNELDLL := $(KERNELDLL:%=../../%)
SETDLL := $(SETDLL:%=../../%)
INTDLL := $(INTDLL:%=../../%)
SUPPORTOBJ := $(SUPPORTOBJ:%=../../%)

ifeq "$(LIBPREFIX)" "$(LINKPREFIX)"
LINKSET:=../../$(LINKSET)
LINKKERNEL:=../../$(LINKKERNEL)
LINKINT:=../../$(LINKINT)
LINKMM:=../../$(LINKMM)
LINKSEARCH:=../../$(LINKSEARCH)
LINKALL := $(LINKMM) $(LINKSET) $(LINKINT) $(LINKSEARCH) $(LINKKERNEL)
else
EXTRALINKFLAGS := -L../..
endif

#
# GRAPH COMPONENTS
#

GRAPHSRC0 = view/arcnode.cc 
GRAPHHDR0 = view/outadjsets.icc view/constant.icc view/arcnode.hh view/nodearcsets.icc graphutils.icc  var.icc view.icc graphutils.h view/boundsgraphs.icc view/prop.icc path/path.icc path/pathgraphs.icc path.hh path.icc
#GRAPHHDR0 = @GRAPHHDR@


GRAPHSRC		= $(GRAPHSRC0)
GRAPHHDR		= graph.hh $(GRAPHHDR0)
GRAPHOBJ		= $(GRAPHSRC:%.cc=%$(OBJSUFFIX))
GRAPHFLAGS      = -I/usr/include -I../.. -I../../gecode $(EXTRALINKFLAGS)
GRAPHDLL		= $(LIBPREFIX)graph$(DLLSUFFIX)
GRAPHSTATICLIB	= $(LIBPREFIX)graph$(STATICLIBSUFFIX)
GRAPHLIB		= $(LIBPREFIX)graph$(LIBSUFFIX)
LINKGRAPH      	=  $(LINKPREFIX)graph$(LINKSUFFIX)  ../../examples/support/example.cc  ../../examples/support/options.cc
#GRAPHBUILDDIRS	= view

ifeq "$(LIBPREFIX)" "$(LINKPREFIX)"
LINKGRAPH := $(LINKGRAPH)
else
EXTRALINKFLAGS := -L../..
endif

LINKALL := $(LINKALL) $(LINKGRAPH)

#
# GRAPH EXAMPLES
#

GRAPHEXAMPLESRC0 = cpgraph-basic cpgraph-complement cpgraph-instUB cpgraph-path cpgraph-path2 cpgraph-pathcost cpgraph-instUB ejemplo5  ejemplo6  ejemplo8 
GRAPHEXAMPLESRC  = $(GRAPHEXAMPLESRC0:%=examples/%.cc)
GRAPHEXAMPLEOBJ  = $(GRAPHEXAMPLESRC:%.cc=%$(OBJSUFFIX))
GRAPHEXAMPLESBJ  = $(GRAPHEXAMPLESRC:%.cc=%$(SBJSUFFIX))
GRAPHEXAMPLEEXE  = $(GRAPHEXAMPLESRC:%.cc=%$(EXESUFFIX))
GRAPHEXAMPLEBUILDDIRS = examples/

GRAPHEXAMPLEHDR0 = $(gecode_top_srcdir)/examples/support.hh $(gecode_top_srcdir)/examples/support/example.icc $(gecode_top_srcdir)/examples/support/options.cc

GRAPHEXAMPLEHDR  = $(GRAPHEXAMPLEHDR0:%=../../examples/%) $(GRAPHHDR)

ALLLIB :=  $(ALLLIB) $(GRAPHLIB) 


LIBTARGETS = $(GRAPHDLL) $(GRAPHLIB)

all : $(LIBTARGETS) $(GRAPHEXAMPLEEXE)

#
# Object targets
#


examples/%$(OBJSUFFIX): examples/%.cc
	$(CXX) $(CXXFLAGS) $(EXAMPLES_EXTRA_CXXFLAGS) $(GRAPHFLAGS) \
	-c -o $@  $<
examples/%$(SBJSUFFIX): examples/%.cc
	$(CXX) $(CXXFLAGS) $(EXAMPLES_EXTRA_CXXFLAGS) $(GRAPHFLAGS) \
	-S -o $@  $<

%$(OBJSUFFIX): %.cc
	$(CXX) $(CXXFLAGS) -DGECODE_BUILD_GRAPH $(GRAPHFLAGS) \
	-c -o $@  $<
%$(SBJSUFFIX): %.cc
	$(CXX) $(CXXFLAGS) -DGECODE_BUILD_GRAPH  $(GRAPHFLAGS) \
	-S -o $@  $<

#
# DLL Targets
#

ifeq "$(DLLSUFFIX)" "$(LIBSUFFIX)" 
#linux 
$(GRAPHDLL): $(GRAPHOBJ) $(KERNELDLL) $(INTDLL) $(SETDLL)
	$(CXX) $(DLLFLAGS) $(GRAPHOBJ)  $(GRAPHFLAGS) \
		-L. $(LINKKERNEL) $(LINKINT) \
		-o $(GRAPHDLL) &&\
	$(CREATELINK) $@ $(@:%$(DLLSUFFIX)=%$(SOLINKSUFFIX)) && \
	$(CREATELINK) $@ $(@:%$(DLLSUFFIX)=%$(SOSUFFIX))
else
#win
$(GRAPHDLL) $(GRAPHLIB): $(GRAPHOBJ) $(KERNELDLL) $(INTDLL) $(SETDLL)
	$(CXX) $(DLLFLAGS) $(GRAPHOBJ)  $(GRAPHFLAGS) \
		-L. $(LINKKERNEL) $(LINKINT) $(LINKSET) \
		-o $(GRAPHDLL)
endif

# 
# EXE targets
#
#


.PRECIOUS: examples/%$(OBJSUFFIX)
examples/%$(EXESUFFIX): examples/%$(OBJSUFFIX) $(SUPPORTOBJ) $(ALLIB)
	$(CXX) -o $@ $< $(SUPPORTOBJ) $(DLLPATH) $(CXXFLAGS) $(GRAPHFLAGS) \
	$(LDFLAGS) $(LINKALL)
CXXFLAGS := $(CXXFLAGS) $(GRAPHFLAGS) 

	
.PHONY: doc
doc: $(GRAPHHDR) $(GRAPHSRC) 
	mkdir -p doc/html
	doxygen doxygen.conf

#
# Autoconf
#

configure: configure.ac 
	autoconf
config.status: configure
	../../config.status --recheck
	./config.status --recheck
Makefile: Makefile.in config.status
	./config.status
Makefile.in : Makefile.in.in ../../config.status
	../../config.status --file $@:$<


.PHONY: clean veryclean distclean
localclean:
	find  examples -perm +u+x -not -type d -exec rm '{}' ';'
	rm libgecodegraph.so

clean:
	$(RMF) $(GRAPHOBJ) $(GRAPHSBJ)

veryclean: clean
	$(RMF) $(LIBTARGETS)
	$(RMF) $(LIBTARGETS:%$(DLLSUFFIX)=%.exp) $(LIBTARGETS:%$(DLLSUFFIX)=%.lib)
	$(RMF) $(LIBTARGETS:%$(DLLSUFFIX)=%.ilk) $(LIBTARGETS:%$(DLLSUFFIX)=%.pdb)
	$(RMF) $(GRAPHEXAMPLEEXE:%$(EXESUFFIX)=%.pdb)
	$(RMF) $(GRAPHEXAMPLEEXE:%$(EXESUFFIX)=%$(OBJSUFFIX))
	$(RMF) $(GRAPHEXAMPLEEXE:%$(EXESUFFIX)=%$(SBJSUFFIX))
	$(RMF) doc CPGraphReference.chm 
	$(RMF) $(GRAPHEXAMPLEEXE)
	$(RMF) $(GRAPHDLL)

distclean: veryclean
	$(RMF) config.log config.status Makefile.in
