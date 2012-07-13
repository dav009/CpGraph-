/*Copyright (c) 2005, Grégoire Dooms ( Université catholique de Louvain )
All rights reserved.*/



#include "examples/support.hh"
#include "graphutils.icc"
#include "graph.hh"

using namespace Gecode::Graph;

/** \brief Example to test the Path propagator with OutAdjSetsGraphView  distributing in a naive way
 * \ingroup Examples
 * */
class CPGraphSimplePath: public Example {
        private:
                OutAdjSetsGraphView g1;
        public:
                /// Constructor  sith unused options
                CPGraphSimplePath(const Options& opt): g1(this,loadGraph("g2.txt")) {
                        Gecode::Graph::path(this,g1,0,5);
                                g1.distrib(this);
                }
                /// Constructor for cloning \a s
                CPGraphSimplePath(bool share, CPGraphSimplePath& s) : Example(share,s){
                        g1.update(this, share, s.g1);
                }
                /// Copying during cloning
                virtual Space*
                        copy(bool share) {
                                return new CPGraphSimplePath(share,*this);
                        }
                /// Print the solution
                virtual void
                        print(std::ostream &os) {

                        os<< "\tg1                =  " << g1 << std::endl;

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

        SizeOptions opt("CPGraphSimplePath");
        opt.icl ( ICL_DOM);
        opt.solutions ( 0);
        opt.parse(argc,argv);

        if (opt.size() == 2) {
                // Example::run<CPGraphSimplePath2vars,DFS>(opt);
                /*
                 * Observaciones v2(NodeArcSetsGraphView)
                 La restriccion de path  no funciona correctamente  sobre esta vista una posible explicación a esto
                 es que el propagador pathdegree el cual sirve de base para esta restriccion no esta completamente
                 implementado,además se descarto que fuera un problema propio de CP(Graph) actualizado dado que
                 esto sucede tambien en la version de CP(Graph) que trabaja en Gecode 1.0.0.
                 Para probar experimentalmente que path no funciona correctamente en Gecode 1.0.0
                 se instancio una vista con un grafo completo de 5 nodos y se  restringio que las salidas
                 tuvieran un camino entre 0 y 3, algunas salidas erroneas fueron :
                 Nodes: {0#3}#4
                Arcs: [
                (0,3), (1,2), (2,0), (3,1)]
                 Nota importante: path sobre la v1(OutAdjSetsGraphView) tambien no funciono correctamente en algunos casos, por ejemplo
                 sobre el anterior problema,sin embargo,se decidio evaluar esta vista  con path porque de todas las  soluciones que se obtenia,
                 una gran parte eran correctas, mientras que la v2  funcionaba mal la mayoria de veces que se evaluo con esta restriccion

                 De aqui en adelante las restricciónes basadas en path como path con distirbucion personalizado y pathcost,
                 se evaluaran unicamente sobre la vista 1 , es decir, OutAdjSetsGraphView.
                 */
        } else {

                Example::run<CPGraphSimplePath,DFS>(opt);

                return 0;
        }
}


