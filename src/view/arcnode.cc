
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

#include "arcnode.hh"
#include <algorithm>
#include "kernel.hh"
#include "boost/tuple/tuple.hpp" // for tie

using namespace std;
using namespace boost;
namespace Gecode { namespace Graph {


// constructor
DefaultArcNode::DefaultArcNode(){

}

DefaultArcNode::DefaultArcNode(int size):numNodes(size){ }

// tells if a,b is an arc
bool DefaultArcNode::has_arc(int a, int b) const{
        return a<numNodes && b< numNodes;
}

// max id of arcs
int DefaultArcNode::max_arc() const{
        return numNodes*numNodes-1;
}

// return the arc id for this arc
int DefaultArcNode::id(int a,int b) const{
        return a*numNodes+b;
}

// return the node ids for this arc id
pair<int,int> DefaultArcNode::end_nodes(int i) const{
        assert(i>=0);
        return make_pair(i/numNodes,i%numNodes);
}

// constructor
ArcNode::ArcNode(){

}
ArcNode::ArcNode(const vector<pair<int,int> > &arcs){
        add_arcs(arcs);
}

ArcNode::ArcNode(int size){
        for (int i=0; i<size; i++){
                for (int j=0; j<size; j++){
                        pair<int,int> p =make_pair(i,j);
                        nodes.push_back(p);
                        ids[p] = nodes.size()-1;
                }
        }
}
// adds arcs to the mapping
void ArcNode::add_arcs(const vector<pair<int,int> > &arcs){
        for (vector<pair<int,int> >::const_iterator i=arcs.begin() ; i!= arcs.end(); ++i){
                if (ids.find(*i)==ids.end()){
                        pair<int,int> p =make_pair(i->first,i->second) ;
                        nodes.push_back(p);
                        ids[p] = nodes.size()-1;
                }
        }
}

// tells if a,b is an arc
bool ArcNode::has_arc(int a, int b) const{
        return ids.end() != ids.find(make_pair(a,b));
}

// max id of arcs
int ArcNode::max_arc() const{
        return nodes.size()-1;
}

// return the arc id for this arc
int ArcNode::id(int a,int b) const{
        map<pair<int,int>,int>::const_iterator i = ids.find(make_pair(a,b));
        assert(i!=ids.end());
        return i->second;
}

// return the node ids for this arc id
pair<int,int> ArcNode::end_nodes(int i) const{
        assert(i<static_cast<int>(nodes.size()));
        return nodes[i];
}


// Constraint 
//
/// \brief Propagator for ArcNode(a,n1,n2): \f$a=(n1,n2)\f$

class ArcNodeCstr : public TernaryPropagator<Int::IntView,Int::PC_INT_DOM> {
        const ArcNode *a;

        public:
        	virtual ~ArcNodeCstr()
        	{
        	}
        	
        /// Constructor for posting 
        ArcNodeCstr(const ArcNode *a, Space * home , Int::IntView x0, Int::IntView x1, Int::IntView x3)
                : TernaryPropagator<Int::IntView,Int::PC_INT_DOM>(home,x0,x1,x2), a(a)  {}




                
        /// Post propagator \f$a=(n1,n2)\f$ 
        static ExecStatus 
                post(const ArcNode *a, Space* home, Int::IntView x0, Int::IntView x1, Int::IntView x2) {
                        (void) new (home) ArcNodeCstr(a,home,x0,x1,x2);
                 
                        return ES_OK;
                }

        /// Constructor for cloning \a p 
        forceinline
                ArcNodeCstr(Space* home, bool share, ArcNodeCstr& p)
                : TernaryPropagator<Int::IntView,Int::PC_INT_DOM>(home,share, p), a(p.a) {}

        /// Copy propagator during cloning
        virtual Actor*
                copy(Space* home, bool share) {
                        return new (home) ArcNodeCstr(home,share,*this);
                }

        /// Return cost
        PropCost
                cost(void) const {
                        return PC_TERNARY_HI;
                }


        
        /// Perform propagation
        /*!
         * Update Gecode 2.1.1
         * ModEventDelta argument was added, this allows to use advisors in the future
         * */
       virtual  ExecStatus //esta funcion se modifico para que tubiera el modEvenDelta necesario por el nuevo Gecode 2.1.1
                propagate(Space* home,ModEventDelta med) {
                       /** First of all propag from arc to nodes */
                       Int::ViewValues<Int::IntView> arcR(x0);
                       int numVals = x0.size();
		       vector<int> headVals(numVals);
                       vector<int> tailVals(numVals);
                       for (int i=0; arcR(); ++arcR, ++i) {
                               boost::tie(headVals[i],tailVals[i]) = a->end_nodes(arcR.val());
                       }
                       sort(tailVals.begin(),tailVals.end()); 
                       IntSet tailDS(&tailVals[0],numVals);
                       IntSetRanges tailRDS(tailDS);
                       ///UPDATE Gecode 2.1.1, inter function was replaced for inter_r
                       GECODE_ME_CHECK(x1.inter_r(home,tailRDS));
                       sort(headVals.begin(),headVals.end()); 
                       IntSet headDS(&headVals[0],numVals);
                       IntSetRanges headRDS(tailDS);
                       ///UPDATE Gecode 2.1.1, inter function was replaced for inter_r
                       GECODE_ME_CHECK(x2.inter_r(home,headRDS));

                       /** then propag from nodes to arcs */
                       vector<int> arcVals;
                       Int::ViewValues<Int::IntView> tailR(x1);
                       Int::ViewValues<Int::IntView> headR(x2);
                       for (; tailR(); ++tailR){
                            for (; headR(); ++headR){
                                    if (a->has_arc(tailR.val(),headR.val())){
                                            arcVals.push_back(a->id(tailR.val(),headR.val()));
                                    }
                            }
                       }
                       sort(arcVals.begin(),arcVals.end()); 
                       IntSet arcDS(&headVals[0],numVals);
                       IntSetRanges arcRDS(arcDS);
                       GECODE_ME_CHECK(x0.inter_r(home,arcRDS));
                       return ES_FIX;
                }

};

/// Posts a constraint linking an arc to two nodes
void ArcNode::arcNode(Space * home, Int::IntView arc, Int::IntView tail, Int::IntView head) const{
    ArcNodeCstr::post(this,home,arc, tail, head);
}


} }
