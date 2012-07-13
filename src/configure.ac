dnl
dnl Author:
dnl   jose luis pulido burbano
dnl
dnl Copyright:
dnl   José Luis Pulido Burbano (Universidad del Valle), 2009
dnl
dnl Large parts taken from the CP(Graph) configure.ac versión 0.1


AC_REVISION([$Revision: 272 $])
AC_PREREQ(2.53)
AC_INIT(CPGRAPH, 0.2, josel28_20@hotmail.com)
AC_CONFIG_SRCDIR(graph.hh)

dnl the names of the generated dlls
AC_SUBST(GRAPH,     "graph")


AC_ARG_WITH(boost-include, 
            [  --with-boost-include=DIR     Specify where to find the boost include files
                             (default=/usr/include)], 
	    boost_include=$withval,
	    boost_include=/usr/include)

AC_SUBST(BOOST_DIR,"$boost_include")
HDRS=`find . -name \*.icc -or -name \*.hh` 
#AC_SUBST(GRAPHHDR,$HDRS) # does not work
../../config.status --file Makefile.in:Makefile.in.in
AC_CONFIG_COMMANDS([Makefile.in]) 

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
