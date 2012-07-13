  
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

#ifndef BINARYSIMPLE_HH
#define BINARYSIMPLE_HH
#include "kernel.hh"
//using namespace Gecode;
namespace Gecode { namespace Graph {


/** \brief Base class for binary propagators (Update Gecode 2.1.1)
   *
   * Allows the Implementation of a Binary Propagator whose Views are CpGraph Graph views
   * for the implementation of a particular constraint.
   *
 * */
template <class GDV1, PropCond p1, class GDV2, PropCond p2>
class BinaryGraphPropagator: public Propagator {
        protected:
		///First Graph View
                GDV1 g1;
		///First Graph View
                GDV2 g2;
                /// Constructor for cloning
                BinaryGraphPropagator(Space* home,  bool share, BinaryGraphPropagator& p);
        public :
                /// Constructor for posting
                BinaryGraphPropagator(Space* home, GDV1 &g1, GDV2 &g2);

     		/// Constructor for rewriting \a p during cloning
                BinaryGraphPropagator(Space* home,bool share, Propagator& p, GDV1 &y1, GDV2 &y2);

                /// Tells the estimated cost of propgation
                virtual PropCost cost(ModEventDelta med) const;

                /// Delete the Propagator and return its size(Update Gecode 2.1.1)
                /**
                            This function
                            Cancels subscription of the graphViews  and Deletes the BinaryGraphPropagator propagator
                 * */
                virtual size_t dispose(Space* home);

                ///Return specification given a variable map m and a name.( Update Gecode 2.1.1 )
                Reflection::ActorSpec spec(const Space* home, Reflection::VarMap& m, const Support::Symbol& name) const;


                /// Post the binary propagator
                static ExecStatus post(Space* home, GDV1 &g1, GDV2 &g2);
};

/** \brief Propagator for the Complement graph binary constraint.
 *
 * A graph is the complement of an other if their adj matrices sum to all ones.
 *
 * Posts The constraint: \f$ g_1 \f$ is Complemnt of  \f$ g_2 \f$ that is.
 * - \f$ g_1 \f$ and \f$ g_2 \f$ have the same set of Nodes.
 * - Each Arc which Belongs to the Complete Graph built over their Set of Nodes belongs to exactly one of the two graphs.
 */
template <class GDV1, class GDV2>
struct ComplementPropag: public BinaryGraphPropagator<GDV1,  Gecode::PC_GEN_NONE, GDV2, Gecode::PC_GEN_NONE> {
        using BinaryGraphPropagator<GDV1,  Gecode::PC_GEN_NONE, GDV2, Gecode::PC_GEN_NONE>::g1;
        using BinaryGraphPropagator<GDV1,  Gecode::PC_GEN_NONE, GDV2,  Gecode::PC_GEN_NONE>::g2;
        using BinaryGraphPropagator<GDV1,  Gecode::PC_GEN_NONE, GDV2, Gecode::PC_GEN_NONE>::cost;
        protected:
                /// Constructor for cloning
                ComplementPropag(Space*, bool share, ComplementPropag&);
        public:
		///Propagator's state variable, a 0 value, indicates complete propagation(propagation with scanning)       	 
		int indicador;
	       ///Propagator's advisor's Council
		Council< nameAdvisor2 > 	co1;
        	int nodoFuente;
        	int nodoFinal;
        	int nodo;
               ///counts how many times this propagator has been called
        	int cuenta;

                ///Stores which Arcs of G2 have to be removed in the next propagation
		std::vector< std::pair<int,int> > remArcsG2;
                ///Stores which Arcs of G1 have to be removed in the next propagation
		std::vector< std::pair<int,int> > remArcsG1;
                ///Stores which Arcs of G1 have to be added in the next propagation
		std::vector< std::pair<int,int> > addArcsG1;
                ///Stores which Arcs of G2 have to be added in the next propagation
		std::vector< std::pair<int,int> > addArcsG2;
	         std::vector< int> addnodes;
	         std::vector< int> remnodes;
		///A flag which tells when the propagator is propagating. It tells the propagator's advisors not to queu the propagator when the changes are caused by the same propagator.
		int activo;

        	/// Constructor for posting
                ComplementPropag(Space*,GDV1&,GDV2&);
                /// Perform the filtering steps of the constraint(Update Gecode 2.1.1)
                /**
			ModDelta Argument was added during Updated.
                 * */
                virtual ExecStatus  propagate(Space *home, ModEventDelta med);
                /// Post the constraint \a g1 = Complement(\a g2)
                static  ExecStatus  post(Space*,GDV1& g1,GDV2& g2);
                /// Copy propagator to new Space
                virtual Actor*  copy(Space* home,bool share) ;
 		Gecode::ExecStatus  advise(Space* home, Advisor* _a, const Delta* d);
                /// disposes the Propagator
                virtual size_t dispose(Space* home);


                      	 
                       
};
/** \brief Propagator for the Undirected Connected Constraint (Gecode 2.1.1 Update)
 *
 *  
 *
 * Post The constraint: \f$ g_2 \f$ is the undirected graph obtained by ignoring the directions of  \f$ g_1 \f$ 
 */
template <class GDV1, class GDV2>
struct UndirectedPropag: public BinaryGraphPropagator<GDV1, Gecode::Graph::PC_GRAPH_ANY, GDV2, Gecode::Graph::PC_GRAPH_ANY> {
        using BinaryGraphPropagator<GDV1, Gecode::Graph::PC_GRAPH_ANY, GDV2, Gecode::Graph::PC_GRAPH_ANY>::g1;
        using BinaryGraphPropagator<GDV1, Gecode::Graph::PC_GRAPH_ANY, GDV2, Gecode::Graph::PC_GRAPH_ANY>::g2;
        using BinaryGraphPropagator<GDV1, Gecode::Graph::PC_GRAPH_ANY, GDV2, Gecode::Graph::PC_GRAPH_ANY>::cost;
        using BinaryGraphPropagator<GDV1, Gecode::Graph::PC_GRAPH_ANY, GDV2, Gecode::Graph::PC_GRAPH_ANY>::dispose;
        protected:
                /// Constructor for cloning
                UndirectedPropag(Space*, bool share, UndirectedPropag&);
        public:
                /// Constructor for posting
                UndirectedPropag(Space*,GDV1&,GDV2&);
                /// Perform the filtering steps of the constraint
                virtual ExecStatus  propagate(Space*,ModEventDelta med);
                /// Post The constraint: \f$ g_2 \f$ is the undirected graph obtained by ignoring the directions of  \f$ g_1 \f$ 
                static  ExecStatus  post(Space*,GDV1& g1,GDV2& g2);
                /// Copy propagator to new Space
                virtual Actor*  copy(Space* home,bool share) ;
};

/** posts a constraint for g1 is the complement of g2
 * \ingroup TaskModel
 */
template <class GDV1, class GDV2>
void complement(Space * home, GDV1 &g1, GDV2 &g2);


/** \brief Propagator for the Subgraph Constraint (Gecode 2.1.1 Update)
 *
 * Subgraph(\f$ g_1 \f$,\f$ g_2\f$ )
 * Posts The constraint: \f$ g_1 \f$ is a subgraph of  \f$ g_2 \f$ 
 */
template <class GDV1, class GDV2>
struct SubgraphPropag: public BinaryGraphPropagator<GDV1, Gecode::PC_GEN_NONE, GDV2, Gecode::PC_GEN_NONE> {
        using BinaryGraphPropagator<GDV1, Gecode::PC_GEN_NONE, GDV2, Gecode::PC_GEN_NONE>::g1;
        using BinaryGraphPropagator<GDV1, Gecode::PC_GEN_NONE, GDV2, Gecode::PC_GEN_NONE>::g2;
        using BinaryGraphPropagator<GDV1, Gecode::PC_GEN_NONE, GDV2, Gecode::PC_GEN_NONE>::cost;
        using BinaryGraphPropagator<GDV1, Gecode::PC_GEN_NONE, GDV2, Gecode::PC_GEN_NONE>::dispose;
        protected:
                /// Constructor for cloning
                SubgraphPropag(Space*, bool share, SubgraphPropag&);
        public:
                ///Propagator's state variable, a 0 value, indicates complete propagation(propagation with scanning)
		int indicador;
	       ///Propagator's advisor's Council
		Council< nameAdvisor2 > 	co1;
        	int nodoFuente;
        	int nodoFinal;
        	int nodo;
               ///counts how many times this propagator has been called
        	int cuenta;

                ///Stores which Arcs of G1 have to be removed in the next propagation
		std::vector< std::pair<int,int> > remArcs;
 		///Stores which Arcs of  G2 have to be added in the next propagation
		std::vector< std::pair<int,int> > addArcs;
               ///Stores which Nodes G2 of have to be added in the next propagation
	        std::vector< int> addnodes;
	        ///Stores which Nodes of G1 have to be removed in the next propagation
		std::vector< int> remnodes;
		///A flag which tells when the propagator is propagating. It tells the propagator's advisors not to queu the propagator when the changes are caused by the same propagator.
		int activo;
	
		

                /// Constructor for posting
                SubgraphPropag(Space*,GDV1&,GDV2&);
                /// Perform the filtering steps of the constraint
                virtual ExecStatus  propagate(Space*,ModEventDelta med);
                /// Post the constraint \a \f$ g_1 \f$ is a subgraph of \f$ g_2 \f$ 
                static  ExecStatus  post(Space*,GDV1& g1,GDV2& g2);
                /// Copy propagator to new Space
                virtual Actor*  copy(Space* home,bool share) ;
		///Advise Function
                Gecode::ExecStatus  advise(Space* home, Advisor* _a, const Delta* d);
};

/** Post a constraint for \f$ g_1 \f$  is a  subgraph of \f$ g_2 \f$ 
 * \ingroup TaskModel
 */
template <class GDV1, class GDV2>
void subgraph(Space * home, GDV1 &g1, GDV2 &g2);

} }
#include "binarysimple.icc"
#endif
