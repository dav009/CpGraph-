

#include "examples/support.hh"
#include "graphutils.icc"
#include "graph.hh"
using namespace Gecode::Graph;


/** \brief Example to test the Subgraph propagator with OutAdjSetsGraphView distributing in a naive way
 * \ingroup Examples
 * */
class CPGraphSubgraph: public Example {
        private:
                OutAdjSetsGraphView g1;
                OutAdjSetsGraphView g2;
        public:
                /// Constructor with unused options
               
                CPGraphSubgraph(const SizeOptions& opt):  g1(this,loadGraph("gsubgraph.txt"))  , g2(this,1){
                Gecode::Graph::subgraph(this,g1,g2);
                        g1.distrib(this);
                        g2.distrib(this);
                }
                /// Constructor for cloning \a s
                CPGraphSubgraph(bool share, CPGraphSubgraph& s) : Example(share,s){
                        g1.update(this, share, s.g1);

                        g2.update(this, share, s.g2);

                }
                /// Copying during cloning
                virtual Space*
                        copy(bool share) {
                                return new CPGraphSubgraph(share,*this);
                        }
                /// Print the solution
                virtual void
                        print(std::ostream &os) {


                                            os << std::endl << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;
                                           os << "g1 = " << g1 << std::endl;
                                           os<< "g2 = " << g2 << std::endl;
                                            os<< std::endl << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;


 }
};



/** \brief Example to test the  Subgraph propagator with NodeArcSetsGraphView distributing in a naive way 
 * \ingroup Examples
 * */

class CPGraphSubgraph2vars: public Example {
        private:
                ArcNode *an ; //used for member init
                NodeArcSetsGraphView g1;
                NodeArcSetsGraphView g2;
        public:
                /// Constructor with unused options
                CPGraphSubgraph2vars(const Options& opt): g1(this,loadGraph("gsubgraph.txt"))  , g2(this,1){

                        Gecode::Graph::subgraph(this,g1,g2);
                        g1.distrib(this);
                        g2.distrib(this);
                }
                /// Constructor for cloning \a s
                CPGraphSubgraph2vars(bool share, CPGraphSubgraph2vars& s) : Example(share,s){
                        g1.update(this, share, s.g1);
                        g2.update(this, share, s.g2);
                }
                /// Copying during cloning
                virtual Space*
                        copy(bool share) {
                                return new CPGraphSubgraph2vars(share,*this);
                        }
                /// Print the solution
                virtual void
                        print(std::ostream &os) {

                                    os << std::endl << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;
                                            os << "g1 = " << g1 << std::endl;
                                            os << "g2 = " << g2 << std::endl;
                                            os<< std::endl << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;


                               }
};


int main(int argc , char** argv) {
        /*En la siguiente linea se cambio de objeto Options a SizeOptions el cual es propio de Gecode 2.1.1,
         esto dado que Options de Gecode 1.0.0 incluia el atributo size, pero en Gecode 2.1.1 este atributo
         lo incluye es una nueva clase que hereda de Options que es SizeOptions
         además mientras que en Options de Gecode 1.0.0
         los atributos  icl, solutions y parse son asignados y obtenidos directamente, en SizeOptions esto se hace mediante
         metodos, asi que esto tambien se modifico.*/
        SizeOptions opt("CPGraphSubgraph");
        opt.icl(ICL_DOM);
        opt.solutions(0);
        opt.parse(argc, argv);
        if(opt.size() == 2) {
                Example::run<CPGraphSubgraph2vars,DFS>(opt);
        } else {

                Example::run<CPGraphSubgraph,DFS>(opt);
        }
        return 0;

        return 0;
}



