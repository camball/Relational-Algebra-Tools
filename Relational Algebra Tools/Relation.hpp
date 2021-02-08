//
//  Relation.hpp
//  Relational Algebra Tools
//
//  Created by Cameron Ball on 2/6/21.
//

#ifndef Relation_hpp
#define Relation_hpp

#include <iostream>
#include <string>
#include <unordered_set>
#include <set>

struct FD; // forward declare

typedef std::unordered_set<FD> FD_Set;

const std::unordered_set<std::string> types = {
    "INT",
    "INTEGER",
    "CHAR",
    "VARCHAR",
    "BIT",
    "BIT VARYING",
    "BOOLEAN",
    "SHORTINT",
    "FLOAT",
    "REAL",
    "DOUBLE PRECISION",
    "DECIMAL",
    "DATE",
    "TIME"
};

class Attribute {
public:
    Attribute() : name(""), type("") {} // leave name and type blank
    
    Attribute( std::string n ) : name(n), type("") {} // just initialise name
    
    Attribute( std::string n, std::string t ) : name(n) { // initialise name to whatever
        if ( isValidType(t) )
            type = t;
        else type = ""; // if type entered isn't in the pre-defined list, initialise to none
    }
    
    std::string getName() const { return name; }
    std::string getType() const { return type; }
    
    // less than op must be overloaded to have std::set of Attributes
    friend bool operator<( const Attribute &a, const Attribute &b ) { return a.name < b.name; }
    // equivalent op must be overloaded to have std::unordered_set of Attributes
    friend bool operator==( const Attribute& a, const Attribute& b ) { return a.name == b.name; }
    
    friend std::ostream& operator<<(std::ostream& os, const Attribute& a) {
        if ( a.type != "" )
            os << a.name << ":" << a.type;
        else os << a.name;
        return os;
    }
    
private:
    std::string name; // the name of the attribute
    std::string type; // the domain/data type of the attribute // this is not necessary data
    
    bool isValidType( std::string type ) {
        std::unordered_set<std::string>::const_iterator itr = types.find( type );
        
        if ( itr == types.end() )
            return false;
        else return true;
    }
};

struct AttributeHash {
    size_t operator() (const Attribute& a) const {
        return std::hash<std::string>()(a.getName());
    }
    // received help from this example:
    // https://stackoverflow.com/questions/15869066/inserting-into-an-unordered-set-with-custom-hash-function
};

typedef std::unordered_set<Attribute, AttributeHash> Attribute_Set;

// non-member function to compute the closure of a set of attributes
Attribute_Set computeClosure( const Attribute_Set a, const FD_Set );


struct FD { Attribute_Set left, right;
public: FD(Attribute_Set l, Attribute_Set r) : left(l), right(r) {}; };
// A functional dependency simply has a set of attributes on the left and right.


typedef std::set<Attribute> Schema; // ordered set of attributes, used in Tuple and Relation


class Tuple {
public:
    Tuple( Schema sch ) : s( sch ) {}
    
    Schema getSchema() const { return s;}
    
    // allows for random access
    Attribute operator[]( int i ) {
        Attribute a; // new Attribute with default constructor
        if ( s.empty() )
            return a;
        else {
            // Make a new iterator pointing to the start of the schema. Loop through, each checking if the current loop iteration is equal to the input value. Each iteration, the iterator is incremented as well, traversing the schema. If the input value is equal to the iteration number, return the attribute at the position of the iterator. If fails, return blank attribute.
            std::set<Attribute>::iterator itr = s.begin();
            for ( int component = 0; component < s.size(); component++, itr++ )
                if ( component == i ) return *itr;
            return a; // if we never found it, just return the default attribute
        }
    }
    /*
    Attribute operator[]( int i ) const {
        Attribute a; // new Attribute with default constructor
        if ( s.empty() )
            return a;
        else {
            // Make a new iterator pointing to the start of the schema. Loop through, each checking if the current loop iteration is equal to the input value. Each iteration, the iterator is incremented as well, traversing the schema. If the input value is equal to the iteration number, return the attribute at the position of the iterator. If fails, return blank attribute.
            std::set<const Attribute>::const_iterator itr = s.begin();
            for ( int component = 0; component < s.size(); component++, itr++ )
                if ( component == i ) return *itr;
            return a; // if we never found it, just return the default attribute
        }
    }
    */
private:
    const Schema s;
    // Remember what this class is: a tuple; an ordered set of attributes.
    // Having a tuple with no schema makes no sense,
    // so we only provide a constructor that takes in a Schema.
};
/*
struct TupleHash {
    size_t operator() (const Tuple& t) const {
        return std::hash<typeid(t[0]).name()>()( t[] );
    }
};
 
 
 Consider using std::tuple
 
*/


class Relation; // forward declare class Relation for use in Key methods

class Key {
public:
    bool isMinimal( const Relation& );  // checks if a key is minimal
    bool isSuperKey( const Relation& ); // takes in a Relation, checks it's minimal key(s),
                                        // and sees if this key is a superset of them.
private:
    Attribute_Set k; // a key is an unordered set of attributes
};


struct RelationHash; // forward declare

class Relation {
public:
    
    Relation( Schema s ); // at minimum, a relation must have a schema
    
    // TODO: copy constructor & copy assignment op overload
    
    // takes an input relation & set of FD's, returns a set of relations that are all in BCNF
    std::unordered_set<Relation, RelationHash> decomposeBCNF( Relation, FD_Set );
     
    std::string getName() const { return name; }
    std::set<Attribute> getSchema() const;
    
    bool addTuple( Tuple ); // inserts a new tuple into the relation; returns true upon success
    bool deleteTuple( Tuple ); // deletes a tuple from the relation; returns false if tuple was not found
    bool findTuple( Tuple ); // returns true if requested tuple is found
    
    
    FD_Set computeClosureOfFD_Set(); // uses fds member data.
                                    // Declared here so we don't have to have an input Relation parameter
    
    bool isInBNCF( Relation );
    bool isIn3NF( Relation );
    
    // input: this relation
    FD_Set projectFD_Set();
    
private:
    std::string name;
                                    // a relation has...
    Schema schema;                      // a schema
    std::unordered_set<Tuple> tuples;   // a set of tuples, containing the data in the relation
    std::unordered_set<Key> keys;       // a set of keys
    FD_Set fds;                         // optionally, a set of functional dependencies
    
    // adds to fds member data all FD's from non-trivial applications of the combining rule
    FD_Set useCombiningRule(); // used in computeClosureOfFD_Set()
};

struct RelationHash {
    size_t operator() (const Relation& r) const {
        return std::hash<std::string>()(r.getName());
    }
};

#endif /* Relation_hpp */
