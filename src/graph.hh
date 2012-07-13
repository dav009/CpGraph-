/*
 *  Main authors:
 *     Gr�goire Dooms <dooms@info.ucl.ac.be>
 *
 *  Copyright:
 *     Universit� catholique de Louvain, 2005
 *
 *  Last modified:
 *     $Date: 2006-05-22 22:19:52 +0200 (Mon, 22 May 2006) $
 *     $Revision: 476 $
 *
 *  This file is part of CP(Graph)
 *
 *  See the file "contribs/graph/LICENSE" for information on usage and
 *  redistribution of this file, and for a
 *     DISCLAIMER OF ALL WARRANTIES.
 *
 */

#ifndef __GECODE_GRAPH_HH__
#define __GECODE_GRAPH_HH__


#include "gecode/kernel.hh"
#if !defined(GECODE_STATIC_LIBS) && \
    (defined(__CYGWIN__) || defined(__MINGW32__) || defined(_MSC_VER))

#ifdef GECODE_BUILD_GRAPH
#define GECODE_GRAPH_EXPORT __declspec( dllexport )
#else
#define GECODE_GRAPH_EXPORT __declspec( dllimport )
#endif

#else
#ifdef GECODE_GCC_HAS_CLASS_VISIBILITY
#define GECODE_GRAPH_EXPORT __attribute__ ((visibility("default")))
#else

#define GECODE_GRAPH_EXPORT


#endif
#endif


    
#include "set.hh"
#include "int.hh"

using namespace Gecode;
using namespace Gecode::Set;
using namespace std;
#include "var.icc"
#include "view.icc"
#include "path.hh"
#include "binarysimple.hh"
#include "branch/branch.icc"

#endif //__GECODE_GRAPH_HH
