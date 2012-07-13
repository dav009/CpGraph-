/*Copyright (c) 2005, Grégoire Dooms ( Université catholique de Louvain )
All rights reserved.*/

#include "graph.hh"
#include "stlutility.icc"
#include "examples/support.hh"

#include "graphutils.icc"


#undef TRACE
// no debug
#define TRACE(A)
#define TRACE_CHOICE(A) A
using namespace Gecode::Graph;
typedef Gecode::Graph::Graph GG;

typedef GraphBDSingle<pair<int,int> > GBD;

/** \brief GraphBounds class used for branching on a constrained path problem.
 *
 *  The heuristic is adapted from:
 *  Sparse heuristic from Pesant G, Gengreau M, Potvin J-Y and Rousseau J-M,
 *  "An Exact Constraint Logic Programming Algorithm for the Traveling Salesman
 *  Problem with Time Windows", Transportation Science 32, 12-29 (1998). page 17.
 *  \relates CPGraphSimplePathHeur CPGraphSimplePathHeur2var
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

        /// The real branch m ethod
        PathHeurBoundsG( GView &g): BoundsGraphs<GView>(g) {}
        //AGREGE UN CONST SOLO PARA PROBAR
        pair<bool,GBD*> branch(const Branching *b)const {
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
             //   TRACE_CHOICE(cout << g<< endl);
                if (bestedge == make_pair(-1,-1)){
               //         TRACE_CHOICE(cout << "end of branching" << endl ;)
                        return make_pair(false,(GBD*)NULL);
                }
                //TRACE_CHOICE(cout << "best:"<<bestedge.first << " " << bestedge.second << ", score:" << bestscore <<endl);
                return make_pair(true,new GBD( b,bestedge,inc));
        }

        /// as  an example here is the code for something closer to the original heuristic (for outneighbours)
        void pesant_et_al()
        {
                int vi_out, vi_out_score ; // the best node for the outneighbors
                // find out degrees
                vector<int> od(num_vertices(UB));
                BGL_FORALL_VERTICES(v, UB, GG){
                        od[UB[v].index] = out_degree(v,UB);
                };
                // get min val
                int min = minimum(od.begin(),od.end());
                // find the indexes with the min val
                vector<int> V;
                for(vector<int>::iterator cur=find(od.begin(), od.end(), min); cur!=od.end(); cur=find(cur, od.end(), min)){
                        V.push_back(cur-od.begin());
                }
                if (min==1) return *V.begin();

                // compute E the set of outneigh of nodes of V
                set<int> E;
                for(vector<int>::iterator i=V.begin(); i!=V.end(); ++i){
                        int vi = *i;
                        BGL_FORALL_OUTEDGES(UB_v[vi], e, UB, GG){
                                E.insert(UB[target(e,UB)].index);
                        }
                }

                // compute e_sharp for all e in E
                map<int,int> e_sharp;
                for(set<int>::iterator i=E.begin(); i!=E.end(); i++){
                        int ei = *i;
                        // local functions are not allowed by C++
                        struct _has_e_in_neigh {
                                int ei;
                                _has_e_in_neigh(int ei): ei(ei) {}
                                bool operator()(int vi){
                                        return edge(UB_v[vi], UB_v[ei], UB).second;
                                }
                        } has_e_in_neigh(ei);
                        e_sharp[ei] = count_if(V.begin(), V.end(), has_e_in_neigh);
                }

                //compute the sum of e_sharp for every v
                vector<int> sum_e_sharp;
                for(vector<int>::iterator i=V.begin(); i!=V.end(); ++i){
                        int vi = *i;
                        int s = 0;
                        BGL_FORALL_OUTEDGES(UB_v[vi], e, UB, GG){
                                int ei = UB[target(e,UB)].index;
                                s += e_sharp[ei];
                        }
                        sum_e_sharp.push_back(s);
                }

                unsigned int min_index = minimum_index(sum_e_sharp.begin(), sum_e_sharp.end());

                // the best node for outneighbors is:
                vi_out = V[min_index];
                vi_out_score = sum_e_sharp[min_index];
        }
};

/** \brief Example to test the branchings with OutAdjSetsGraphView
 * \ingroup Examples
 * */
class CPGraphSimplePathHeur: public Example {
        private:
                OutAdjSetsGraphView g1;
        public:
                /// Constructor with unused options
                CPGraphSimplePathHeur(const Options& opt):  g1(this,loadGraph("g2.txt")) {
                        Gecode::Graph::path(this,g1,0,5);
                       branch<OutAdjSetsGraphView, PathHeurBoundsG<OutAdjSetsGraphView> >(this,g1);
                }

                /// Constructor for cloning \a s
                CPGraphSimplePathHeur(bool share, CPGraphSimplePathHeur& s) : Example(share,s){
                        g1.update(this, share, s.g1);
                }
                /// Copying during cloning
                virtual Space*
                        copy(bool share) {
                                return new CPGraphSimplePathHeur(share,*this);
                        }
                /// Print the solution
                virtual void
                        print(std::ostream &os) {

                                os << "\tg1 = " << g1 << std::endl;
                        }
};

int main(int argc , char** argv) {
        /*En la siguiente linea se cambio de objeto Options a SizeOptions el cual es propio de Gecode 2.1.1,
         esto dado que Options de Gecode 1.0.0 incluia el atributo size, pero en Gecode 2.1.1 este atributo
         lo incluye es una nueva clase que hereda de Options que es SizeOptions
         además mientras que en Options de Gecode 1.0.0
         los atributos  icl, solutions y parse son asignados y obtenidos directamente, en SizeOptions esto se hace mediante
         metodos, asi que esto tambien se modifico.*/
        SizeOptions opt("CPGraphSimplePathHeur");
        opt.icl(ICL_DOM);
        opt.solutions(0);
        opt.parse(argc, argv);
        if(opt.size() == 2) {
        } else {
                Example::run<CPGraphSimplePathHeur,DFS>(opt);
        }



        return 0;
}
