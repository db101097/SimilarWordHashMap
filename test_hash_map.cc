#include <chrono>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <string>
#include<cstdlib>
#include<iterator>
#include "quadratic_probing.h"
#include "separate_chaining.h"
#include "double_probing_table.h"
using namespace std;

template <typename HashTableType>
void TestComputeAdjacentWords(HashTableType &hash_table, const vector<string> &words) 
{
  // This is for computing timing.
  const auto begin = chrono::high_resolution_clock::now();

  hash_table.MakeEmpty();  
  //..Insert your own code using algorithm described in Figure 4.73
  HashTableType group_by_length;
  HashTableType same_sub_string;

  for(auto & w:words)
  {
		string length=to_string(w.length()); // convert int to string since template type is map<string,vectorstring>
		group_by_length[length].push_back(w);// By using the bracket operator the length will be inserted 
  }
  vector<string> k;
  group_by_length.getkeys(k);// did not implement iterators so using a get key function instead

  for(auto & key : k) 
  {
		same_sub_string.MakeEmpty(); 
		vector<string>gbw=group_by_length[key];
		int group_length=stoi(key);
		
		for( int i=0;i<group_length;i++)
		{		    
				for(auto & s : gbw)
				{
					string hold_string=s;
				    hold_string.erase(i,1);
					same_sub_string[hold_string].push_back(s);
				}	
								
		}

		vector<string> k2;
  		same_sub_string.getkeys(k2);
			
 		for (auto keys : k2 )
 	   {
			vector<string>clique=same_sub_string[keys];
		    
			if(clique.size()>=2)
			{			
				for( int i=0;i<clique.size();++i)
				{
					for(int j=i+1;j<clique.size();++j)
					{
						
						hash_table[clique[i]].push_back(clique[j]);
						hash_table[clique[j]].push_back(clique[i]);
					}
				}
			}		   
  	   }
   }
 
		

  const auto end = chrono::high_resolution_clock::now();

  cout << chrono::duration_cast<chrono::nanoseconds>(end-begin).count() << "ns" << endl;
  cout << chrono::duration_cast<chrono::milliseconds>(end-begin).count() << "ms" << endl;
}

template <typename HashTableType>
void Print_Words(HashTableType map)
{

	string word;
	int i=0;
	while(i!=2)
	{
		cout<<"Please enter a word"<<endl;
		cin>>word;

		if(map[word].size()==0)
		{
			cout<<"Sorry , no words were found"<<endl;
		}

		else
		{
			for(auto & x : map[word])
			{
				cout<<x<<endl;
			}
		}

		i++;
	}
}	

vector<string> ReadWordsFromFile(const string &words_filename) 
{
  vector<string> the_words;
  string hold;
  ifstream file(words_filename);
  while(getline(file,hold))
  {
		the_words.push_back(hold);
  }
  return the_words;
}

// Sample main for program CreateAndTestHash
int main(int argc, char **argv) {
  
	if (argc != 3) 
	{
   	 cout << "Usage: " << argv[0] << " words_file_name hash_type (chaining, quadratic or double)" << endl;
   	 return 0;
  	}
  
  	const string words_filename(argv[1]);
  	const vector<string> the_words = ReadWordsFromFile(words_filename);

    const string param_flag(argv[2]);

  	if (param_flag == "chaining") 
  	{	
    	HashSeparateChaining<string,vector<string>> separate_chaining_table;
    	TestComputeAdjacentWords(separate_chaining_table, the_words);
		Print_Words(separate_chaining_table);
  	} 
	
	else if (param_flag == "quadratic") 
	{
   		HashQuadraticProbing<string,vector<string>> quadratic_probing_table;    
   	    TestComputeAdjacentWords(quadratic_probing_table, the_words);
		Print_Words(quadratic_probing_table);
  	} 

	else if (param_flag == "double") 
	{
    	HashTableDouble<string,vector<string>> double_probing_table;
    	TestComputeAdjacentWords(double_probing_table, the_words);
		Print_Words(double_probing_table);    
  	} 

	else 
	{
    	cout << "Uknown tree type " << param_flag << " (User should provide linear, quadratic, or double)" << endl;
  	}
  
	return 0;
}
