/*Copyright (c) 2005, Grégoire Dooms ( Université catholique de Louvain )
All rights reserved.*/
#include "examples/support.hh"
#include "graphutils.icc"
#include "graph.hh"

using namespace Gecode::Graph;

/** \brief Example to test the Complement propagator with OutAdjSetsGraphView   distributing in a naive way
 * \ingroup Examples
 * */
class CPGraphComplement: public Example {
private:

        OutAdjSetsGraphView g1;
        OutAdjSetsGraphView g2;
public:
        /// Constructor with unused options
        CPGraphComplement(const Options& opt): g1(this,loadGraph("g1.txt"))  , g2(this,loadGraph("g1.txt").first.size()){

                Gecode::Graph::complement(this,g1,g2);
                        g1.distrib(this);
                        g2.distrib(this);

        }
        /// Constructor for cloning \a s
        CPGraphComplement(bool share, CPGraphComplement& s) : Example(share,s){
                g1.update(this, share, s.g1);

                g2.update(this, share, s.g2);

        }
        /// Copying during cloning
        virtual Space*
        copy(bool share) {
                return new CPGraphComplement(share,*this);
        }
        /// Print the solution
        virtual void
        print(std::ostream &os) {

                   os << "g1 = " << g1 << std::endl;
                   os<< "g2 = " << g1 << std::endl;
                   os<< std::endl << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;
        }
};
/** \brief Example to test the Complement constraint with NodeArcSetsGraphView distributing in a naive way
 * \ingroup Examples
 * */

class CPGraphComplement2vars: public Example {

private:


        NodeArcSetsGraphView g1;

        NodeArcSetsGraphView g2;

public:

        /// Constructor with unused options

        CPGraphComplement2vars(const Options& opt): g1(this,loadGraph("g1.txt"))  , g2(this,loadGraph("g1.txt").first.size()){
                Gecode::Graph::complement(this,g1,g2);

                                g1.distrib(this);
                                g2.distrib(this);
        }


        /// Constructor for cloning \a s
        CPGraphComplement2vars(bool share, CPGraphComplement2vars& s) : Example(share,s){
                g1.update(this, share, s.g1);
                g2.update(this, share, s.g2);
        }
        /// Copying during cloning
        virtual Space*
        copy(bool share) {
                return new CPGraphComplement2vars(share,*this);
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

        SizeOptions opt("CPGraphComplement");
        opt.icl(ICL_DOM);
        opt.solutions(0);
        opt.parse(argc, argv);
        if(opt.size() == 2) {
                Example::run<CPGraphComplement2vars,DFS>(opt);
        } else {
                Example::run<CPGraphComplement,DFS>(opt);
        }
        return 0;
}

