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
    //std::cout << "Process line : " << l << std::endl;
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
    
    //std::cout << "  slv:" << slv << std::endl;
    //std::cout << "  bench:" << bench << std::endl;
    //std::cout << "  rest:" << rest << std::endl;
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
    std::cout << ",Solved,Fastest,Smallest";
  }
  std::cout << std::endl;
  for( std::map< std::string, std::map< std::string, std::string > >::iterator it = table.begin(); it != table.end(); ++it )
  {
    std::cout << it->first;
    std::map< std::string, std::string >& bres = it->second;
    std::map< std::string, std::string >::iterator itbr;
    std::vector< int > slvStat;
    std::vector< int > timeStat;
    std::vector< int > exprStat;
    int minTime = -1;
    int minExprs = -1;
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
      // ------------- compute solved stats, best solved
      std::string sStatus;
      std::string sTime;
      std::string sExprs;
      size_t pos = 0;
      size_t pos_prev = 0;
      pos = itbr->second.find(",", pos);
      if( pos==std::string::npos )
      {
        continue;
      }
      sStatus = itbr->second.substr( pos_prev, pos-pos_prev );
      pos++;
      pos_prev = pos;
      pos = itbr->second.find(",", pos);
      if( pos==std::string::npos )
      {
        continue;
      }
      sTime = itbr->second.substr( pos_prev, pos-pos_prev );
      pos++;
      sExprs = itbr->second.substr(pos,itbr->second.size()-pos);
      if( sStatus=="SUCCESS" )
      {
        slvStat.push_back(1);
        // consider 2 decimal points
        int valTime = static_cast<int>(atof( sTime.c_str() )*100.0);
        //std::cout << "ATOF " << sTime.c_str() << " -> " << valTime << std::endl;
        if( minTime<0 || valTime<=minTime )
        {
          minTime = valTime;
        }
        timeStat.push_back(valTime);
        int valExprs = atoi( sExprs.c_str() );
        if( minExprs<0 || valExprs<=minExprs )
        {
          minExprs = valExprs;
        }
        exprStat.push_back(valExprs);
      }
      else
      {
        slvStat.push_back(0);
        timeStat.push_back(-1);
        exprStat.push_back(-1);
      }
      // ------------- 
    }
    for( unsigned i=0; i<allSlv.size(); i++ )
    {
      itbr = bres.find(allSlv[i]);
      std::cout << "," << itbr->second;
      std::cout << "," << slvStat[i];
      int valTime = timeStat[i];
      int valTimeResult = valTime>=0 && valTime-minTime<=100 ? 1 : 0;
      std::cout << "," << valTimeResult;
      int valExprs = exprStat[i];
      int valExprsResult = valExprs>=0 && valExprs<=minExprs ? 1 : 0;
      std::cout << "," << valExprsResult;
    }
    std::cout << std::endl;
  }
}
