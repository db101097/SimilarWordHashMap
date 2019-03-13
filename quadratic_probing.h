#ifndef QUADRATIC_PROBING_H
#define QUADRATIC_PROBING_H

#include <vector>
#include <algorithm>
#include <functional>
#include "is_prime.h"
#include <iostream>
// Quadratic probing implementation.
template <typename Keytype , typename HashedObj>

class HashQuadraticProbing  {
 public:

  enum EntryType {ACTIVE, EMPTY, DELETED};

  explicit HashQuadraticProbing(size_t size = 101) : array_(PrimeProject::NextPrime(size))
    { MakeEmpty(); }
  
  bool Contains(const HashedObj & x) const 
  {
    return IsActive(FindPos(x));
  }
  
  void MakeEmpty() 
  {
    current_size_ = 0;
    for (auto &entry : array_)
      entry.info_ = EMPTY;
  }

  bool Insert(const Keytype & key , const HashedObj & x) 
  {
    // Insert x as active
    size_t current_pos = FindPos(key);
    if (IsActive(current_pos))
      return false;
    
    array_[current_pos].element_ = x;
	array_[current_pos].key_ = key;
    array_[current_pos].info_ = ACTIVE;

    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();    

    return true;
  }
    
  bool Insert(const Keytype && key , const HashedObj && x) 
 {
    // Insert x as active
    size_t current_pos = FindPos(key);
    if (IsActive(current_pos))
      return false;

    array_[current_pos].element_ = std::move(x);
	array_[current_pos].key_ = std::move (key); 
    array_[current_pos].info_ = ACTIVE;
    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();

    return true;
  }

  bool Remove(const Keytype & key) 
  {
    size_t current_pos = InternalHash(key);
    if (!IsActive(current_pos))
      return false;

    array_[current_pos].info_ = DELETED;
    return true;
  }

  HashedObj & operator [] (const Keytype & key )
  {
	 size_t current_pos=FindPos(key);
	 if(array_[current_pos].info_==EMPTY || array_[current_pos].info_==DELETED)
	 {	
			HashEntry t;
			t.key_=key;
			Insert(t.key_,t.element_);	
	 }
	
	 return array_[current_pos].element_;

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

  size_t size()
  {
		return current_size_;
  }

 private:        
  struct HashEntry {
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

  bool IsActive(size_t current_pos) const
  { 
	return array_[current_pos].info_ == ACTIVE; 
  }

  size_t FindPos(const Keytype & k) const 
  {
    size_t offset = 1;
    size_t current_pos = InternalHash(k);
      
    while (array_[current_pos].info_ == ACTIVE) 
  {
	  if(array_[current_pos].key_==k)
	  {
			return current_pos;
	  }
      current_pos += offset;  // Compute ith probe.
      offset += 2;
      if (current_pos >= array_.size())
	  current_pos -= array_.size();
    }
    return current_pos;
  }

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
  
  size_t InternalHash(const Keytype & x) const 
  {
    static std::hash<Keytype> hf;
    return hf(x) % array_.size( );
  }
};

#endif  // QUADRATIC_PROBING_H
