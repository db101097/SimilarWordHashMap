To compile :
When in the current directory of the files type:
make

To execute enter the following in the command line :
./test_hash_map words.txt <hash_type>
where hash_type can be double , quadratic or chaining

The code will generate a map with all words that can become another word with a change of a single letter 
The words that generate the map are the ones provided in words.txt

The program will then prompt the user to enter a word and then it will print all the words it can become with one letter change
The program will then again prompt the user to enter another word and do the same thing

If the word you input cannot be changed to another word by one letter replacement or the word is not in the file provided the program
will print "Sorry , no words were found".

Once the user enters both the words and the result is printed the program is terminated
