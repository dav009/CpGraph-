/*Copyright (c) 2005, Grégoire Dooms ( Université catholique de Louvain )
All rights reserved.*/

#include "examples/support.hh"
#include "graphutils.icc"
#include "graph.hh"
#include "stlutility.icc"
#include "boost/tuple/tuple.hpp"
#include <iostream>

#undef TRACE
// no debug
#define TRACE(A)
//#define TRACE_CHOICE(A) A
#define TRACE_CHOICE(A)
using namespace Gecode::Graph;
using namespace std;
using namespace boost;
typedef Gecode::Graph::Graph GG;

typedef GraphBDSingle<pair<int,int> > GBD;

/** \brief GraphBounds class used for branching on a constrained path problem.
 *
 *  The heuristic is adapted from:
 *  Sparse heuristic from Pesant G, Gengreau M, Potvin J-Y and Rousseau J-M,
 *  "An Exact Constraint Logic Programming Algorithm for the Traveling Salesman
 *  Problem with Time Windows", Transportation Science 32, 12-29 (1998). page 17.
 *  \relates CPGraphPathCostHeur CPGraphPathCostHeur2var
 * \ingroup Examples
 * \ingroup TaskModelBranch
 */
template <class GView>
struct PathHeurBoundsG :  public BoundsGraphs<GView> {
        typedef GBD GraphBD; //< a Graph Branching must define the branching description used
        using BoundsGraphs<GView>::g; //< for debugging only
        using BoundsGraphs<GView>::UB; //< Upper bound graph
        using BoundsGraphs<GView>::UB_v;//< Upper bound node descriptors by internal id
        using BoundsGraphs<GView>::LB;//< Lower bound graph
        using BoundsGraphs<GView>::LB_v;//< Lower bound node descriptors by internal id

        /// The real branch method
        PathHeurBoundsG( GView &g): BoundsGraphs<GView>(g) {}
        pair<bool,GBD*> branch(const Branching *b){
                // We use an heuristic inspired from the following:
                // Sparse heuristic from Pesant G, Gengreau M, Potvin J-Y and Rousseau J-M,
                // "An Exact Constraint Logic Programming Algorithm for the Traveling Salesman
                // Problem with Time Windows", Transportation Science 32, 12-29 (1998). page 17.

                // find nodes V of minimum out-degree s,
                // if s==1 pick any in V
                // else
                //    for all nodes e in Union{outneigh(v)|v\in v},
                //        compute e# = #{v\in V | e\in outneigh(v)}
                //    choose the v which maximizes the \sum_{e\in outneigh(v)} e#
                //
                // Do the same in symetrical  in/out choose among them on the value of the sum.
                //
                // Once the node is chosen branch on all out/in arcs by order of cost
                // (depending if the node was chosen based on out/in going arcs).
                //
                // Our heuristic is based on a similar technique:
                // to count the number of prunings made by arc->node and degree constraints
                // we want to maximize that number of prunings.
                //
                // we can add or remove an arc.
                // 1) addition the addition of an arc will remove all other outgoing arcs and incoming arcs:
                // add (a,b) -> outdegree(a)+indegree(b)-2
                // 2) removal of an arc might remove the nodes
                // remove (a,b) -> if indegree(b) == 1 : outdegree(b)
                //                 if outdegree(a) == 1 : indegree(a)
                //           might also trigger inclusion of arcs: at most 2 so ignored
                //



                //compute the in and out degree of all nodes
                vector<int> od(num_vertices(UB));
                vector<int> id(num_vertices(UB));
                BGL_FORALL_EDGES(e, UB, GG){
                        int s,t;
                        t = UB[target(e,UB)].index;
                        s = UB[source(e,UB)].index;
                        od[s]++; id[t]++;
                }
                int bestscore = 0;
                bool inc=true; //< inc ? include : exclude
                pair<int,int> bestedge = make_pair(-1,-1);  // pair of id of nodes

                //search the unknown edges for a best score
                BGL_FORALL_EDGES(e, UB, GG){
                        //check for the presence of this edge in the lower bound
                        // id is used
                        int sid = UB[source(e,UB)].id;
                        int tid = UB[target(e,UB)].id;
                        TRACE(cout << "sid:"<<sid << " tid:" << tid << " e:" << e<<endl);
                        if (LB_v[sid] && LB_v[tid] && edge(LB_v[sid],LB_v[tid],LB).second){
                                TRACE(cout << "skip" << endl);
                                continue;
                        }
                        int s,t;
                        t = UB[target(e,UB)].index;
                        s = UB[source(e,UB)].index;
                        TRACE(cout << "s:"<<s << " t:" << t <<endl);
                        int sc; //score
                        // include
                        sc = od[s]+id[t]-2;
                        if (sc > bestscore){
                                bestscore = sc;
                                inc = true;
                                bestedge = make_pair(sid,tid); // id is used
                        }
                        // exclude
                        sc = 0;
                        sc += od[s]==1 ? id[s] : 0;
                        sc += id[t]==1 ? od[t] : 0;
                        if (sc > bestscore){
                                bestscore = sc;
                                inc = false;
                                bestedge = make_pair(sid,tid); // id is used
                        }
                        TRACE(cout << "best:"<<bestedge.first << " " << bestedge.second <<endl);
                }
                TRACE_CHOICE(cout << g<< endl);
                if (bestedge == make_pair(-1,-1)){
                        TRACE_CHOICE(cout << "end of bracnhing" << endl ;)
                                return make_pair(false,(GBD*)NULL);
                }
                TRACE_CHOICE(cout << "best:"<<bestedge.first << " " << bestedge.second << ", score:" << bestscore <<endl);
                return make_pair(true,new GBD(b,bestedge,inc));
        }

};

class PathCostOptions : public  SizeOptions {
        public:
                PathCostOptions(const char* s) : SizeOptions(s) {}
                int start;
                int end;
                pair<vector<int>,vector<pair<int,int> > > graph;
                vector<int> arc_costs;
                vector<int> interns;
};

PathCostOptions* loadParams(string filename, int start, int end, const char* name, int* interns=NULL, int numInterns=0){
        PathCostOptions * res = new PathCostOptions(name);
        res->start = start;
        res->end = end;
        vector<int> nodes;
        for (int i=0; i<numInterns; i++){
                res->interns.push_back(interns[i]);
        }
        typedef vector<tuple<int,int,int> > V3;
        V3 arcs;
        tie(nodes,arcs) = loadGraphWeights(filename);
        res->graph.first = nodes;
        for (V3::iterator i=arcs.begin(); i!=arcs.end(); i++){
                res->graph.second.push_back(make_pair(get<0>(*i), get<1>(*i)));
                res->arc_costs.push_back(get<2>(*i));
        }
        return res;
}

/** \brief Example to test the branchings with OutAdjSetsGraphView
 * \ingroup Examples
 * */


class CPGraphPathCostHeur: public Example {
        private:
                OutAdjSetsGraphView g1;
                IntVar w;
                int start; int end;
        public:
                /// Constructor taking PathCostOptions
                CPGraphPathCostHeur(const SizeOptions& opt): g1(this,(reinterpret_cast<const PathCostOptions *>(&opt))->graph), w(this,0,Int::Limits::max){
                        const PathCostOptions * o = reinterpret_cast<const PathCostOptions *>(&opt);
                        std::map<pair<int,int>,int> arc_costs;
                        for (unsigned int i=0; i<o->graph.second.size(); i++){
                                arc_costs[o->graph.second[i]] = o->arc_costs[i];
                        }
                        for (vector<int>::const_iterator i = o->interns.begin(); i!=o->interns.end();  i++){
                                GECODE_ME_FAIL(this,g1._nodeIn(this,*i));
                        }
                        start = o->start;
                        end = o->end;
                        path(this, g1, o->start, o->end, arc_costs, w);
                        branch<OutAdjSetsGraphView, PathHeurBoundsG<OutAdjSetsGraphView> >(this,g1);
                }
                /// Constructor for cloning \a s
                CPGraphPathCostHeur(bool share, CPGraphPathCostHeur& s) : Example(share,s){
                        g1.update(this, share, s.g1);
                        w.update(this, share, s.w);
                        start = s.start;
                        end = s.end;
                }
                /// Copying during cloning
                virtual Space*
                        copy(bool share) {
                                return new CPGraphPathCostHeur(share,*this);
                        }
                /// Print the solution
                virtual void
                        print(std::ostream &os) {
                                const vector<int> & path = get_path();
                                os << "g1 = " << path << std::endl;
                                os<< "path cost = " << w << std::endl;

                       }
                /// Constrain the space further for Branch and Bound
                virtual void
                        constrain(Space * s) {
                                rel(this, w, IRT_LE, static_cast<CPGraphPathCostHeur*>(s)->w.val());
                        }
                /// return the path as an order vector of node id
                virtual vector<int>
                        get_path() {
                                vector<int> path;
                                int cur=start;
                                path.push_back(start);
                                do {
                                        vector<int> nei;
                                        g1.outNeighboursUB(cur,nei);
                                        assert(nei.size()==1);
                                        cur=nei[0];
                                        vector<int>::const_iterator f = find(path.begin(),path.end(),cur);
                                        vector<int>::const_iterator not_found = path.end();
                                        path.push_back(cur);
                                        if (f!=not_found){
                                                cerr << "Error: there is a cycle :" << path << " " << cur << endl;
                                                break; // cur was already present --> cycle
                                        }
                                } while(cur != end );
                                return path;
                        }


};

int main(int argc , char** argv) {
        /*En la siguiente linea se cambio de objeto Options a SizeOptions el cual es propio de Gecode 2.1.1,
         esto dado que Options de Gecode 1.0.0 incluia el atributo size, pero en Gecode 2.1.1 este atributo
         lo incluye es una nueva clase que hereda de Options que es SizeOptions
         además mientras que en Options de Gecode 1.0.0
         los atributos  icl, solutions y parse son asignados y obtenidos directamente, en SizeOptions esto se hace mediante
         metodos, asi que esto tambien se modifico.*/
        SizeOptions * opt = loadParams("gpathcost.txt", 1,13, "CPGraphPathCostHeur");
        (*opt).icl(ICL_DOM);
        opt->solutions(0);      
        opt->parse(argc, argv);
        if(opt->size() == 2) {

           } else {

                   Example::run<CPGraphPathCostHeur,BAB>(*opt);
           }

        return 0;
}
