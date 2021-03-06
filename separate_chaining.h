#ifndef SEPARATE_CHAINING_H
#define SEPARATE_CHAINING_H

#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <functional>
using namespace std;

#include "is_prime.h"

// SeparateChaining Hash table class
//
// CONSTRUCTION: an approximate initial size or default of 101
//
// ******************PUBLIC OPERATIONS*********************
// bool insert( x )       --> Insert x

// bool remove( x )       --> Remove x
// bool contains( x )     --> Return true if x is present
// void makeEmpty( )      --> Remove all items
template <typename Keytype, typename HashedObj>
class HashSeparateChaining {
 public:
  explicit HashSeparateChaining(int size = 101) : current_size_{0} { 
    the_lists_.resize( 101 ); }

  bool Contains( const Keytype & k ) 
  {
	HashEntry new_;
	new_.key_=k;
    auto & which_list = the_lists_[ MyHash( k ) ];
        return find( begin( which_list ), end( which_list ), new_ ) != end( which_list );
  }
  
  void MakeEmpty( ) 
  {
    for( auto & this_list : the_lists_ )
      this_list.clear( );
  }
  
  bool Insert( const Keytype & key , const HashedObj & x ) 
  {
	HashEntry new_(x,key);
    auto & which_list = the_lists_[ MyHash( key ) ];
    if( find( begin( which_list ), end( which_list ), new_) != end( which_list) )
      return false;

    which_list.push_back( new_ );

    // Rehash; see Section 5.5
    if( ++current_size_ > the_lists_.size( ) )
      Rehash( );
  
    return true;
  }
    
  bool Insert( Keytype && key , HashedObj && x ) 
  {
	
	HashEntry new_(x,key);
	
    auto & which_list = the_lists_[ MyHash( key ) ];      
    if( find( begin( which_list ), end( which_list ), new_ ) != end( which_list ) )
      return false;
		

    which_list.push_back( std::move( new_ ) );

    // Rehash; see Section 5.5
    if( ++current_size_ > the_lists_.size( ) )
      Rehash( );

    return true;
  }


  bool Remove( const Keytype & k ) 
 {
	HashEntry new_;
	new_.key_=k;
    auto & which_list = the_lists_[ MyHash(k) ];
    auto itr = find( begin( which_list ), end( which_list ), new_);
	

    if( itr == end( which_list ) )
      return false;
    
    which_list.erase( itr );
    --current_size_;
    return true;

  }

  void getkeys(std::vector<Keytype> & k)
  {
		for(auto & i : the_lists_)
		{
			 auto & which_list=i;
			 for(auto & j : which_list)
			 {
							k.push_back(j.key_);
			 }
		}
  }

  HashedObj & operator []( const Keytype & key)
  {
	HashEntry new_;
	new_.key_=key;
    auto & which_list = the_lists_[ MyHash(key) ];
    auto itr = find( begin( which_list ), end( which_list ), new_);
	
	if(itr==end(which_list))
	{
		Insert(new_.key_,new_.element_);
		auto & which_list = the_lists_[ MyHash(key) ];
   	    auto itr = find( begin( which_list ), end( which_list ), new_);
		return (*itr).element_;		
	}
		return (*itr).element_;
  }

  size_t size()
  {
		return the_lists_.size();
  }

 private:
  struct HashEntry 
  {
    HashedObj element_;
	Keytype key_;
    
    HashEntry(const HashedObj& e = HashedObj{}, const Keytype& k = Keytype{})
    :element_{e}, key_{k} { }
    
    HashEntry(HashedObj && e, Keytype && k)
    :element_{std::move(e)},key_{std::move(k) } {}

	bool operator == (HashEntry a )
	{
		return (key_==a.key_);
	}
 
  };
  vector<list<HashEntry>> the_lists_; 
  size_t  current_size_;
  
  void Rehash( ) 
{
	
    vector<list<HashEntry>> old_lists = the_lists_;
    // Create new double-sized, empty table
    the_lists_.resize( PrimeProject::NextPrime( 2 * the_lists_.size( ) ) );
    for( auto & this_list : the_lists_ )
      this_list.clear( );
    
    // Copy table over
    current_size_ = 0;
    for( auto & this_list : old_lists )
	{
      for( auto & x : this_list )
	  {
		Insert( std::move(x.key_) , std::move( x.element_ ) );
	  }
	}
}
  
  size_t MyHash( const Keytype & x ) const 
  {
    static hash<Keytype> hf;
    return hf( x ) % the_lists_.size( );
  }


};

#endif
