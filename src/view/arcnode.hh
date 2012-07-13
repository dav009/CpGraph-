
/*
 *  Main authors:
 *     Gr�goire Dooms <dooms@info.ucl.ac.be>
 *
 *  Copyright:
 *     Gr�goire Dooms (Universit� catholique de Louvain), 2005
 *
 *  Last modified:
 *     $Date: 2005-11-29 10:57:21 +0100 (Tue, 29 Nov 2005) $
 *     $Revision: 271 $
 *
 *  This file is part of CP(Graph)
 *
 *  See the file "contribs/graph/LICENSE" for information on usage and
 *  redistribution of this file, and for a
 *     DISCLAIMER OF ALL WARRANTIES.
 *
 */

#ifndef ARCNODE_H
#define ARCNODE_H

#include <vector>
#include <map>
#include <utility>
#include "gecode/kernel.hh"
#include "int.hh"
//#include "graph.hh"
//using namespace std;
//using namespace Gecode;


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
namespace Gecode { namespace Graph {

/** \brief A class for encoding pairs of nodes (pairs of ints) as ints.
 */ 
GECODE_GRAPH_EXPORT 
class ArcNode {
        private:
               std::vector<std::pair<int,int> > nodes;
               std::map<std::pair<int,int>,int> ids;
        public:
          /// constructor
          GECODE_GRAPH_EXPORT     ArcNode();
          /// constructor With arcs from complete graph 
          GECODE_GRAPH_EXPORT     ArcNode(int s);
          /// constructor With initial arcs
          GECODE_GRAPH_EXPORT     ArcNode(const std::vector<std::pair<int,int> > &arcs);
          /// adds arcs to the mapping
          GECODE_GRAPH_EXPORT     void add_arcs(const std::vector<std::pair<int,int> > &arcs);
          /// return the arc id for this arc
          GECODE_GRAPH_EXPORT     int id(int,int) const;
          /// return the end node ids for this arc id
          GECODE_GRAPH_EXPORT     std::pair<int,int> end_nodes(int) const;
          /// posts a constraint linking an arc to two nodes
          GECODE_GRAPH_EXPORT     void arcNode(Space * home, Int::IntView arc, Int::IntView tail, Int::IntView head) const;
          /// tells if a,b is an arc
          GECODE_GRAPH_EXPORT     bool has_arc(int a, int b) const;
          /// returns max id of arcs
          GECODE_GRAPH_EXPORT     int max_arc() const;
};
       //AGREGADA POR EL EQUIPO de la version PERDIDA
GECODE_GRAPH_EXPORT
class DefaultArcNode {
        private:
                int numNodes;
        public:
               /// constructor
            GECODE_GRAPH_EXPORT    DefaultArcNode();
               /// constructor with arcs from complete graph 
            GECODE_GRAPH_EXPORT    DefaultArcNode(int s);
               /// return the arc id for this arc
           GECODE_GRAPH_EXPORT     int id(int,int) const;
               /// return the end node ids for this arc id
            GECODE_GRAPH_EXPORT    std::pair<int,int> end_nodes(int) const;
               /// posts a constraint linking an arc to two nodes
            GECODE_GRAPH_EXPORT    void arcNode(Space * home, Int::IntView arc, Int::IntView tail, Int::IntView head) const;
               /// tells if a,b is an arc
           GECODE_GRAPH_EXPORT     bool has_arc(int a, int b) const;
               /// returns max id of arcs
           GECODE_GRAPH_EXPORT     int max_arc() const;
};

} } 
#endif
