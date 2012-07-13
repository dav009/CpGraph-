/*Copyright (c) 2005, Grégoire Dooms ( Université catholique de Louvain )
All rights  */

#include "examples/support.hh"
#include "graphutils.icc"
#include "graph.hh"
using namespace Gecode::Graph;
/** \brief Example to test the intantiateUB method of OutAdjSetsGraphView 
 * \ingroup Examples
 * */
class CPGraphInstUB: public Example {
        private:
                OutAdjSetsGraphView g1;
                OutAdjSetsGraphView g2;
        public: 
                /// Constructor with unused options
                CPGraphInstUB(const Options& opt): g1(this,loadGraph("g1.txt")), g2(this,loadGraph("g1.txt")){
                        cout << g1 << endl;
                        cout << g2 << endl;
                        g2.instantiateUB(this);
                        g1.distrib(this);

                }
                /// Constructor for cloning \a s
                CPGraphInstUB(bool share, CPGraphInstUB& s) : Example(share,s){
                        g1.update(this, share, s.g1);
                        g2.update(this, share, s.g2);
                }
                /// Copying during cloning
                virtual Space*
                        copy(bool share) {
                                return new CPGraphInstUB(share,*this);
                        }
                /// Print the solution
                virtual void
                        print(std::ostream &os) {
                                 os<<  std::endl << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;
                                 os<<  "g1 = " << g1 << std::endl;
                                 os<<  "g2 = " << g2 << std::endl;
                                 os<<  std::endl << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;
                        }
};
/** \brief Example to test the intantiateUB method of NodeArcSetsGraphView 
 * \ingroup Examples
 * */
class CPGraphInstUB2vars: public Example {
        private:
                ArcNode *an ; //used for member init  
                NodeArcSetsGraphView g1;
                NodeArcSetsGraphView g2;
        public: 
                /// Constructor with unused options
                CPGraphInstUB2vars(const Options& opt): g1(this,loadGraph("g1.txt")), g2(this,loadGraph("g1.txt")) {
                        g2.instantiateUB(this);
                        g1.distrib(this);
                }
                /// Constructor for cloning \a s
                CPGraphInstUB2vars(bool share, CPGraphInstUB2vars& s) : Example(share,s){
                        g1.update(this, share, s.g1);
                        g2.update(this, share, s.g2);
                }
                /// Copying during cloning
                virtual Space*
                        copy(bool share) {
                                return new CPGraphInstUB2vars(share,*this);
                        }
                /// Print the solution
                virtual void
                        print(std::ostream &os) {
                                 os<<  std::endl << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;
                                 os<<  "g1 = " << g1 << std::endl;
                                 os<< "g2 = " << g2 << std::endl;
                                os<<  std::endl << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;
                        }
};
int
main(int argc, char** argv) {
        /*En la siguiente linea se cambio de objeto Options a SizeOptions el cual es propio de Gecode 2.1.1,
         esto dado que Options de Gecode 1.0.0 incluia el atributo size, pero en Gecode 2.1.1 este atributo
         lo incluye es una nueva clase que hereda de Options que es SizeOptions
         además mientras que en Options de Gecode 1.0.0
         los atributos  icl, solutions y parse son asignados y obtenidos directamente, en SizeOptions esto se hace mediante
         metodos, asi que esto tambien se modifico.*/

        SizeOptions opt("CPGraphInstUB");
        opt.icl ( ICL_DOM);
        opt.solutions ( 0);
        opt.parse(argc,argv);

        if (opt.size() == 2) {        
          Example::run<CPGraphInstUB2vars,DFS>(opt);
  }else{
          Example::run<CPGraphInstUB,DFS>(opt);
  }
  return 0;
}
