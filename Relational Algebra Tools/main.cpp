//
//  main.cpp
//  Relational Algebra Tools
//
//  Created by Cameron Ball on 2/5/21.
//

#include <iostream>
#include "Relation.hpp"

using std::string;
using std::cout;

int main()
{
    Attribute_Set aSet;
    
    aSet.insert( Attribute("A", "INT") );
    aSet.insert( Attribute("B") );
    aSet.insert( Attribute("C") );
    aSet.insert( Attribute("D") );
    
    
    Schema s;
    
    s.insert( Attribute("A") );
    
    /*
     // will print out all keys of aSet
     
     for ( auto x : aSet )
         cout << x << ' ';
    */
    
    
    
    
    /*
     // will test if an attribute is in the set
     
    std::unordered_set<Attribute>::const_iterator got = aSet.find( Attribute("B") );
    
    if ( got == aSet.end() )
        std::cout << "not found in aSet\n";
    else
        std::cout << *got << " is in aSet\n";
    */
     
    
    
    
    return 0;
}
