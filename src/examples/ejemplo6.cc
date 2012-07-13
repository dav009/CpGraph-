
#include "examples/support.hh"

#include "graphutils.icc"
#include "graph.hh"
#include "binarysimple.hh"
using namespace Gecode::Graph;
/** \brief Example to test the Symmetric propagator with OutAdjSetsGraphView distributing in a naive way
 * \ingroup Examples
 * */
class CPGraphSymmetric: public Example {
        private:
                OutAdjSetsGraphView g1;
                OutAdjSetsGraphView g2;
        public:
                /// Constructor with unused options
                CPGraphSymmetric(const SizeOptions& opt): g1(this,2){
                	Gecode::Graph::symmetric(this, g1);
                        g1.distrib(this);
                                }
                /// Constructor for cloning \a s
                CPGraphSymmetric(bool share, CPGraphSymmetric& s) : Example(share,s){
                        g1.update(this, share, s.g1);



                }
                /// Copying during cloning
                virtual Space*
                        copy(bool share) {
                                return new CPGraphSymmetric(share,*this);
                        }
                /// Print the solution
                virtual void
                        print(std::ostream &os) {

                                            os << std::endl << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;
                                           os << "g1 = " << g1 << std::endl;

                                            os<< std::endl << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;

 }
};
/** \brief Example to test the Symmetric propagator with NodeArcSetsGraphView distributing in a naive way 
 * \ingroup Examples
 * */
class CPGraphSymmetric2vars: public Example {
        private:
                ArcNode *an ; //used for member init
                NodeArcSetsGraphView g1;

        public:
                /// Constructor with unused options
                CPGraphSymmetric2vars(const Options& opt): an(new ArcNode(2)), g1(this,2) {

                        Gecode::Graph::symmetric(this,g1);
                        g1.distrib(this);
                }
                /// Constructor for cloning \a s
                CPGraphSymmetric2vars(bool share, CPGraphSymmetric2vars& s) : Example(share,s){
                        g1.update(this, share, s.g1);

                }
                /// Copying during cloning
                virtual Space*
                        copy(bool share) {
                                return new CPGraphSymmetric2vars(share,*this);
                        }
                /// Print the solution
                virtual void
                        print(std::ostream &os) {

                                    os << std::endl << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;
                                            os << "g1 = " << g1 << std::endl;

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
        SizeOptions opt("CPGraphSymmetric");
        opt.icl(ICL_DOM);
        opt.solutions(0);
        opt.parse(argc, argv);
        if(opt.size() == 2) {
                Example::run<CPGraphSymmetric2vars,DFS>(opt);
        } else {
                Example::run<CPGraphSymmetric,DFS>(opt);
        }
        return 0;
}

