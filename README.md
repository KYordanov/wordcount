# Wordcount by Krasimir Yordanov

-------Development Notes--------

   The goal is to count how many times every word from a given dictionary is found in the input string.

   This is achieved by first counting the words in the dictionary and checking for errors. Then creating the dictionary in a dynamically allocated array. Sort the array with bubble sort, for simplicity. So it could be binary searched for every word in the input string. Every element of the array is structure consisting of the word, 25 chars, and the number of times the word was found so far in the input string, as unsigned char.
   
-------Improvements --------

   Change bubble sort with a better sorting algorithm.

