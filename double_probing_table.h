#ifndef DOUBLE_PROBING_H
#define DOUBLE_PROBING_H

#include <vector>
#include <algorithm>
#include <functional>
#include <functional>
#include "is_prime.h"
#include <iostream>

template < typename Keytype , typename HashedObj >
class HashTableDouble {
public :
	
  enum EntryType {ACTIVE, EMPTY, DELETED};

  explicit HashTableDouble(size_t size = 101) : array_(PrimeProject::NextPrime(size))
    { MakeEmpty(); }
  
  void MakeEmpty() 
  {
    current_size_ = 0;
    for (auto &entry : array_)
      entry.info_ = EMPTY;
  }

  bool Insert(const Keytype & k,const HashedObj & element)
  {
		size_t insert_pos=Find_Pos(k);
		if(array_[insert_pos].key_==k)
			return false;

		array_[insert_pos].element_=element;
		array_[insert_pos].key_=k;
		array_[insert_pos].info_=ACTIVE;
		if (++current_size_ > array_.size() / 2)
      	Rehash();

		return true;	
  }

  bool Insert (const Keytype && k , HashedObj && element)
  { 
		size_t insert_pos=Find_Pos(k);
		if(array_[insert_pos].key_==k)
			return false;

		array_[insert_pos].element_=std::move(element);
		array_[insert_pos].key_=std::move(k);
		array_[insert_pos].info_=ACTIVE;	
		
		if (++current_size_ > array_.size() / 2)
      	Rehash();

		return true;
  }

  void getkeys(std::vector<Keytype> & k)
  {
		for(auto & i : array_)
		{		  
			 if(i.info_==ACTIVE)
			{
				k.push_back(i.key_);
			}
		}
  }
		

  HashedObj & operator [] (const Keytype & key )
  {
	 size_t insert_pos=Find_Pos(key);
	 if(array_[insert_pos].info_==EMPTY || array_[insert_pos].info_==DELETED)
	 {	
		HashEntry t;
		t.key_=key;
		Insert(t.key_,t.element_);		
	 }
	
	 return array_[insert_pos].element_;
  }


  bool remove (const Keytype & k)
  {
		size_t insert_pos=Find_Pos(k);
		if(array_[insert_pos].key_==k && array_[insert_pos].info_==ACTIVE)
		{
			array_[insert_pos].info_=DELETED;	
			return true;
		}

		return false;	
  }

  size_t size()
  {
		return current_size_;
  }


private:
struct HashEntry 
{
    HashedObj element_;
	Keytype key_;
    EntryType info_;
    
    HashEntry(const HashedObj& e = HashedObj{},const Keytype& k= Keytype{} , EntryType i = EMPTY)
    :element_{e},key_{k}, info_{i} { }
    
    HashEntry(HashedObj && e,Keytype && k , EntryType i = EMPTY)
    :element_{std::move(e)},key_{std::move(k)} , info_{ i } {}
};

std::vector<HashEntry> array_;
size_t current_size_;


void Rehash() 
{
    std::vector<HashEntry> old_array = array_;

    // Create new double-sized, empty table.
    array_.resize(PrimeProject::NextPrime(2 * old_array.size()));
    for (auto & entry : array_)
      entry.info_ = EMPTY;
    
    // Copy table over.
    current_size_ = 0;
    for (auto & entry :old_array)
      if (entry.info_ == ACTIVE)
	Insert(std::move(entry.key_) , std::move(entry.element_));
}

size_t Find_Pos (Keytype k)
{
		if(array_[hash1(k)].info_==ACTIVE && array_[hash1(k)].key_!=k)
		{
				return hash2(k);
		}

		return hash1(k);
}

size_t hash2(Keytype k)
{
	size_t small_prime=array_.size();
	small_prime--;
	while(PrimeProject::IsPrime(small_prime)!=true)
	{
			small_prime--;
	}
	
	static std::hash<Keytype> hf;
    size_t pos= hf(k) % array_.size( );
	size_t increment = small_prime-(pos%small_prime);
	
	while(array_[pos].info_==ACTIVE )
	{
		if(array_[pos].key_==k)
		{
			return pos;
		}

		pos+=increment;
				
		if(pos>=array_.size())
		{
			pos-=(array_.size());
		}
	}

		return pos;
}

size_t hash1(Keytype k)
{
	static std::hash<Keytype> hf;
    return hf(k) % array_.size( );	
}



};

#endif //DOUBLE_PROBING_H
