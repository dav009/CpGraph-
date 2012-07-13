/*Copyright (c) 2005, Grégoire Dooms ( Université catholique de Louvain )
All rights reserved.*/

#include "examples/support.hh"
#include "graphutils.icc"
#include "graph.hh"
//
using namespace Gecode::Graph;
/** \brief Most basic example possible for OutAdjSetsGraphView
 *
 * instantiate the View and distribute in a naive way.
 * \ingroup Examples
 */
class CPGraphBasic: public Example {
        private:
                OutAdjSetsGraphView g1;
        public:
                /// Constructor with unused options
                CPGraphBasic(const Options& opt):  g1(this,loadGraph("g1.txt")){
                        cout << g1 << endl;
                          g1.distrib(this);
                }
                /// Constructor for cloning \a s
                CPGraphBasic(bool share, CPGraphBasic& s) : Example(share,s){
                        g1.update(this, share, s.g1);
                }
                /// Copying during cloning
                virtual Space*
                        copy(bool share) {
                                return new CPGraphBasic(share,*this);
                        }
                /// Print the solution
                virtual void
                        print(std::ostream &os) {

                                os << "\tg1 = " << g1 << std::endl;
                        }
};
/** \brief Most basic example possible for NodeArcSetsGraphView
 *
 * instantiate the View and distribute in a naive way.
 * \ingroup Examples
 */
class CPGraphBasic2vars: public Example {
        private:
                NodeArcSetsGraphView g1;
        public:
                /// Constructor with unused options
                CPGraphBasic2vars(const Options& opt): g1(this,loadGraph("g1.txt")){
                        cout << g1 << endl;
                          g1.distrib(this);
                }
                ///  Constructor for cloning       \a s
                CPGraphBasic2vars(bool share, CPGraphBasic2vars& s) : Example(share,s){
                        g1.update(this, share, s.g1);
                }
                ///  Copying during cloning
                virtual Space*
                        copy(bool share) {
                                return new CPGraphBasic2vars(share,*this);
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

        SizeOptions opt("CPGraphBasic");
        opt.icl(ICL_DOM);
        opt.solutions(0);
        opt.parse(argc, argv);
        //cout<<opt.size();
        if(opt.size() == 2) {
                Example::run<CPGraphBasic2vars,DFS>(opt);
        } else {

                Example::run<CPGraphBasic,DFS>(opt);
        }
        return 0;
}
