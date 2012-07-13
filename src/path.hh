
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

#ifndef PATH_HH
#define PATH_HH

#include "kernel.hh"
#include "int.hh"
#include <map>
#include <boost/utility.hpp>
//using namespace Gecode;
//using namespace std;
//using namespace boost;

namespace Gecode { namespace Graph {
/** \brief Propagator for the simple path constraint
 *  
 *   Post the constraint Path(g,N1,N2) : Graph \f$ g \f$ has a Simple Path Between Node N1 and N2 
 *
 * \relates PathBoundsGraphs
 */
template <class GView>
class PathPropag : public  Propagator {
        protected:
                GView g; ///Graph View
                int start; ///Start Node
		int end; ///End Node
        public :
                PathPropag(Space* home, bool share, PathPropag& p);
        /// Destructor for cancelling
                ~PathPropag(void);
        /// Constructor for posting
                PathPropag(Space* home, GView g, int st, int en);
        /// perform the copy upon space cloning
                virtual Actor*      copy(Space*,bool);
        // return the cost of propagation
                virtual PropCost    cost(ModEventDelta med) const;
        /// perform the filtering
                virtual ExecStatus  propagate(Space* home, ModEventDelta med);
        /// Post propagator g is a simple path from node \a start to node \a end
                static ExecStatus post(Space* home, GView &g,int start, int end);
        //! Update Gecode 2.1.1 Delete the Propagator and return its size
        /*!
                    Update Gecode 2.1.1.
                    This function
                    Cancels subscription of the graphView to the PathPropag and Deletes the PathPropag propagator
        */
        virtual size_t dispose(Space* home);
};
/** \brief Propagator for the cost-based filtering for a simple-path
 *   Post the constraint Path(g,N1,N2,W, Costs) : Graph \f$ g \f$ has a Simple Path from Node \f$N1\f$ to \f$N2\f$ 
 *   with Cost \f$W\f$ and the Arcs Costs are described in \f$Costs\f$.
 * \relates PathBoundsGraphs
 */
template <class GView>
class PathCostPropag : public  Propagator {
        protected:
                GView g;///< the graph view on which the propagator is posted
                int start, end;
                /** todo keep a pointer to the map instead of the map
                 * allocated by  PathCostPropag, and kept through reference counting.
                 * copy increments, destructor decrements and frees if count==0
                 * --> Use a smart pointer  */
                map <pair<int,int>,int> ecosts; ///Arcs Costs 
                Int::IntView w; ///< the IntView for the total cost of the graph
        public :
                /// Copy constructor for cloning the propagator
                PathCostPropag(Space* home, bool share, PathCostPropag& p);
                /// Destructor
                ~PathCostPropag();
                /// Post the constraint g has a Simple Path from \f$st\f$  to \f$en\f$  with cost \f$W\f$ . The cost of arcs are described in \f$edgecosts\f$.
                PathCostPropag(Space* home, GView g, int st, int en, const map <pair<int,int>,int> &edgecosts, Int::IntView w);
                /// copy the propagator to new Space
                virtual Actor*      copy(Space*,bool);
                /// cost of propagation
               virtual PropCost   cost(ModEventDelta med) const;
                /// perform propagation(Update Gecode 2.1.1)
                /*!
                            Update Gecode 2.1.1.
							ModEventDelta was added in order to be able to handle Advisors in the       future.
                */
                virtual ExecStatus  propagate(Space* home, ModEventDelta med);
                /// Posts the propagator in \f$g\f$ exists a path from \f$st\f$ to \f$en\f$ and according to the \f$edgecosts\f$ its total cost is \f$w\f$
                static ExecStatus post(Space* home, GView &g,int start, int end, Int::IntView w, const map <pair<int,int>,int> &edgecosts ) ;
                
                //! Update Gecode 2.1.1 Delete the Propagator and return its size
                 /*!
                             Update Gecode 2.1.1.
                             This function
                             Cancels subscription of the graphView to the PathCostPropag and Deletes the PathCostPropag propagator
                 */
                virtual size_t dispose(Space* home);
};

/** \brief Propagator for a node degree=1 constraint
 *
 * each node of the graph has a in/out degree of 1 iff it is part of the graph
 * except for start and end which have resp. only outdegree and indegree equal to 1.
 */
template <class GView>
class PathDegreePropag : public  Propagator {
        protected:
                GView g; ///Gview
                int start, end; ///Start and End Nodes
        public :
		  /// Copy constructor for cloning the propagator
                PathDegreePropag(Space* home, bool share, PathDegreePropag& p);
                ~PathDegreePropag(void);
		///each node of the graph has a in/out degree of 1 iff it is part of the graph except for start and end which have resp. only outdegree and indegree equal to 1.
                PathDegreePropag(Space* home, GView &g, int st, int end);
                virtual Actor*      copy(Space*,bool);
                virtual PropCost    cost(ModEventDelta med) const;
                /// perform propagation(Update Gecode 2.1.1)
                /*!
                            Update Gecode 2.1.1.
							ModEventDelta was added in order to be able to handle Advisors in the future.
                */
                virtual ExecStatus  propagate(Space* home, ModEventDelta med);

                static ExecStatus post(Space* home, GView &g,int start, int end) ;
 size_t dispose(Space* home);
};


/** \brief Propagator for a simple constraint
 */
template <class GView>
class Simple : public  Propagator {
        protected:
                GView g;
        public :
                Simple(Space* home, bool share, Simple& p);
                virtual size_t dispose (Space *home);
                Simple(Space* home, GView &g);
                virtual Actor*      copy(Space*,bool);
                virtual PropCost    cost(ModEventDelta med) const;
                virtual ExecStatus  propagate(Space*,ModEventDelta med);
                static ExecStatus post(Space* home, GView &g) ;
};


/** \brief Propagator for a symmetric constraint
 *If  the graph \f$G\f$ has a directed Arc from \f$N1\f$ to \f$N2\f$
 *then an Arc from \f$N2\f$ to \f$N1\f$ must exist.
 */
template <class GView>
class Symmetric : public  Propagator {
        protected:
                GView g;
	        ///Stores which Arcs have to be removed in the next propagation
		std::vector< std::pair<int,int> > remArcs;
		///Stores which Arcs have to be added in the next propagation
		std::vector< std::pair<int,int> > addArcs;
		///Stores which Nodes have to be added in the next propagation
	         std::vector< int> addnodes;
		///Stores which Nodes have to be removed in the next propagation
	         std::vector< int> remnodes;
		///A flag which tells when the propagator is propagating. It tells the propagator's advisors not to queu the propagator when the changes are caused by the same propagator.
		int activo;
		
	


        public :
		///counts how many times this propagator has been called
        	int cuenta;
		///Propagator's advisor's Council
        	Council< nameAdvisor2 > 	co1;
        	int nodoFuente;
        	int nodoFinal;
        	int nodo;
		///Propagator's state variable, a 0 value, indicates complete propagation(propagation with scanning)       	
        	int indicador;

		

		 /// Copy constructor for cloning the propagator
                Symmetric(Space* home, bool share, Symmetric& p);
                virtual size_t dispose (Space *home);
                Symmetric(Space* home, GView &g);
                virtual Actor*      copy(Space*,bool);
		///Return the propagators cost
                virtual PropCost    cost(ModEventDelta med) const;
		///Propagation Function
                virtual ExecStatus  propagate(Space*,ModEventDelta med);
		/// Post that graph \f$g\f$ is symmetric
                static ExecStatus post(Space* home, GView &g) ;
		//advise function
                Gecode::ExecStatus  advise(Space* home, Advisor* _a, const Delta* d);
                     

};


/** \brief Post a simple path constraint on the graph view \f$g\f$, in \f$g\f$ must exist a simple path from \f$start\f$ to \f$end\f$ with cost \f$w\f$.
 *
 * The propagator uses cost filtering based on the upper bound of w and updates the lower bound of w.
 * \ingroup TaskModel
 */
template <class GView>
void path(Space* home, GView &g, int start, int end, const map <pair<int,int>,int> &edgecosts , IntVar w);

/** \brief Post a path constraint on the graph view \f$g\f$, in \f$g\f$ must exist a simple path from \f$start\f$ to \f$end\f$
 * \ingroup TaskModel
 */
template <class GView>
void path(Space* home, GView &g, int start, int end);

} }
#include "path.icc"
#endif
