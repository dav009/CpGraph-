//
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

//using namespace std;
#include <utility> //for pair
#include <vector>
#include <boost/dynamic_property_map.hpp>
#include <boost/tuple/tuple.hpp>


/**
 * Returns a word from decriptor file in. Used in loadAmalfi(...).
 */
unsigned readWord(istream& in);
/**
 * 
 * Local function to load an amalfi untyped graph format file.
 * - f is the file name
 * - returns a pair of vectors <L1,L2> :
 *   - L1 is the vector of n node ids ranging from [0,...,n-1]
 *   - L2 is the vector of edges reprented by pair of node ids 
 *
 */
pair<vector<int>,vector<pair<int,int> > > loadAmalfi(string f);
vector<int> loadList(string filename);
pair<vector<int>,vector<pair<int,int> > > loadGraph(const string filename);
pair<vector<int>,vector<boost::tuple<int,int,int> > > loadGraphWeights(const string filename);
pair<vector<int>,vector<pair<int,int> > > loadDotGraph(const string filename);
boost::tuple<vector<int>,vector<pair<int,int> >,vector<int>,vector<pair<int,int> > > loadDotGraphOptForb(const string filename);
