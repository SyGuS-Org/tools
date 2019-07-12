//g++ mk_table.cpp -o mk_table
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <cstring>
#include <iomanip>
#include <unordered_set>


int main( int argc, char* argv[] )
{

  std::map< std::string, std::map< std::string, std::string > > table;

  std::fstream fs(argv[1], std::ios::in);

  std::string l;
  std::vector< std::string > lines;
  while( !fs.eof() ){
    getline( fs, l );
    lines.push_back(l);
  }
  std::string perSolver;
  
  for( unsigned i=0; i<lines.size(); i++ )
  {
    std::cout << "Process line : " << l << std::endl;
    l = lines[i];
    size_t pos = 0;
    size_t pos_prev = 0;
    pos = l.find(",", pos);
    if( pos==std::string::npos )
    {
      continue;
    }
    std::string slv = l.substr( pos_prev, pos-pos_prev );
    pos++;
    pos_prev = pos;
    pos = l.find(",", pos);
    if( pos==std::string::npos )
    {
      continue;
    }
    std::string bench = l.substr( pos_prev, pos-pos_prev );
    pos++;
    std::string rest = l.substr(pos,l.size()-pos);
    
    std::cout << "  slv:" << slv << std::endl;
    std::cout << "  bench:" << bench << std::endl;
    std::cout << "  rest:" << rest << std::endl;
    if( i==0 )
    {
      perSolver = rest;
    }
    else
    {
      table[bench][slv] = rest;
    }
  }

  int nsolvers = -1;
  std::vector< std::string > allSlv;
  std::unordered_set< std::string > setSlv;
  for( std::map< std::string, std::map< std::string, std::string > >::iterator it = table.begin(); it != table.end(); ++it )
  {
    for( std::map< std::string, std::string >::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
    {
      if( setSlv.find( it2->first )==setSlv.end() )
      {
        setSlv.insert( it2->first );
        allSlv.push_back(it2->first);
      }
    }
  }
  std::cout << "Benchmark";
  for( unsigned i=0; i<allSlv.size(); i++ )
  {
    std::cout << "," << allSlv[i] << "-" << perSolver;
  }
    std::cout << std::endl;
  for( std::map< std::string, std::map< std::string, std::string > >::iterator it = table.begin(); it != table.end(); ++it )
  {
    std::cout << it->first;
    std::map< std::string, std::string >& bres = it->second;
    std::map< std::string, std::string >::iterator itbr;
    for( unsigned i=0; i<allSlv.size(); i++ )
    {
      itbr = bres.find(allSlv[i]);
      if( itbr==bres.end() )
      {
        std::cout << "ERROR: no entry for (solver, benchmark) pair:" << std::endl;
        std::cout << "  Solver : " << allSlv[i] << std::endl;
        std::cout << "  Benchmark : " << it->first << std::endl;
        exit(1);
      }
      std::cout << "," << itbr->second;
    }
    std::cout << std::endl;
  }
}
