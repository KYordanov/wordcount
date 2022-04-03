# Wordcount by Krasimir Yordanov

-------Development Notes--------

   The goal is to count how many times evry word from an given dictionary, is found in input string.

   This is achieved by first counting the words in the dictionary and checking for errors. Then creating the dictionary in dynamically allocated array. Sort the array with bubble sort, for simplicity. So it could be binary searched for evry word in the input string. Evry element of the array is structure consiting of the word, 25 chars, and the number of times the word was found so far in the input sting, as unsigned char.
   
-------Improvements --------

   Change buble sort with better sorting alghorithm.

