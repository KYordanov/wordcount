#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORD_MAX_SIZE    25

// Word_and_its_count data structure
typedef struct {
   char word[WORD_MAX_SIZE];  // the characters of the word
   unsigned char count;  // How many times was the word found, in the input string, so far
}tWord_and_its_count;

int count_words_in_file_check_for_error(FILE * dict_file);
int create_dict(tWord_and_its_count * * dict,FILE * dict_file, int dict_word_count);
void sort_words(tWord_and_its_count * dict, int dict_word_count);
FILE * take_input(int argc, char *argv[]);
void inc_when_word_is_found(tWord_and_its_count * dict, int dict_word_count, char * word);

int main(int argc, char *argv[])
{
   int dict_word_count, i;
   tWord_and_its_count * dict;
   FILE * input_stream;

   // Open dictfile
   FILE * dict_file = fopen(argv[1], "r");
   if(dict_file == NULL)
   {
      printf("Fail to open the file %s\n", argv[1]);
      return 1;
   }
   
   do // do while used, so it can break in case of error, to free memory and close files
   {
   // Count the words and check for error.
   dict_word_count = count_words_in_file_check_for_error(dict_file);
   if(dict_word_count < 0) break;
   
   // Create dynamically allocated array
   if(create_dict(&dict, dict_file, dict_word_count) < 0) break;
   
   // Sort
   sort_words(dict, dict_word_count);
   
   // Check for duplicate words
   for(i = 0; i < dict_word_count - 2; i++)
   {
      if(strcmp(dict[i].word, dict[i+1].word) == 0)
      {
         printf("Error: Duplicate word in %s\n", argv[1]);
         break;
      }
   }

   // Take input according to text files given
   input_stream = take_input(argc, argv);
   if(input_stream == NULL) break;
   
   // Go through all words in the input string and search for them in the dictionary, increment the count of the word when it is find
   char temp[WORD_MAX_SIZE];
   int total_words_num = 0;
   while(fscanf(input_stream, "%s", temp) == 1)
   {
      if(strcmp(temp, "wordcount_stop") == 0)
         break;
      inc_when_word_is_found(dict, dict_word_count, temp);
	  total_words_num++;
   }
   

   // Show result
   for(i = 0; i < dict_word_count; i++)
      if(dict[i].count != 0)
         printf("%d\t%s\n", dict[i].count, dict[i].word);

   printf("%d\ttotal words\n", total_words_num);

   }while(0);

   free(dict);
   fclose(input_stream);
   fclose(dict_file);

   return 0;
}

FILE * take_input(int argc, char *argv[])
{
   FILE * txt_file;
   char ch;
   if(argc == 3)
   {
      txt_file = fopen(argv[2], "r");
      if(txt_file == NULL)
      {
         printf("Fail to open the file %s\n", argv[2]);
         return NULL;
      }
      return txt_file;
   }
   else if(argc > 3)
   {  // merge all text files into new one
      FILE * merged_txt_files = fopen("/tmp/wordcount_merged_txt_files", "w+");
      if(merged_txt_files == NULL)
      {
         printf("Fail to create file.\n");
         return NULL;
      }
      for(int i = 2; i < argc; i++)
      {
      	txt_file = fopen(argv[i], "r");
      	if(txt_file == NULL)
      	{
      	   printf("Fail to open the file %s\n", argv[i]);
      	   return NULL;
      	}
      	while((ch = fgetc(txt_file)) != EOF)
      	   fputc(ch, merged_txt_files);
      }
      rewind(merged_txt_files);
      return merged_txt_files;
   }
   else if(argc == 2)
   {  // no text files given, take standard input as input
      printf("Please enter text, wordcount_stop when ready:\n");
      return stdin;
   }
}

int find_word_pos(tWord_and_its_count * dict, int left_index, int right_index, char * word)
{  // Recursive binary search
   if(right_index >= left_index)
   {
      int mid = left_index + (right_index - left_index) / 2;

      if(strcmp(dict[mid].word, word) == 0)
         return mid;
    	 
      if(strcmp(dict[mid].word, word) > 0)
         return find_word_pos(dict, left_index, mid - 1, word);
    	
      return find_word_pos(dict, mid + 1, right_index, word);
   }
   return -1;
}

void inc_when_word_is_found(tWord_and_its_count * dict, int dict_word_count, char * word)
{
   int pos = find_word_pos(dict, 0, dict_word_count - 1, word);
   if(pos >= 0)
      dict[pos].count++;
}

void swap_two_str(char * str1, char * str2)
{
   char temp[25];
   strcpy(temp, str1);
   strcpy(str1, str2);
   strcpy(str2, temp);
}

void sort_words(tWord_and_its_count * dict, int dict_word_count)
{  // Bubble sort
   for(int i = 0; i < dict_word_count; i++)
   {
      for(int j = 0; j < dict_word_count - 1 - i; j++)
      {
         if(strcmp(dict[j].word, dict[j+1].word) > 0)
         {
            swap_two_str(dict[j].word, dict[j+1].word);
         }
      }
   }
}

int create_dict(tWord_and_its_count * * dict,FILE * dict_file, int dict_word_count)
{
   *dict = malloc(dict_word_count * sizeof(tWord_and_its_count));
   if(dict == NULL)
   {
      printf("Failed to allocate memory\n");
      return -1;
   }
   rewind(dict_file);
   for(int i = 0; i < dict_word_count; i++)
   {
      fscanf(dict_file, "%s", (*dict)[i].word);
      (*dict)[i].count = 0;
      if((*dict)[i].word[0] == '#')
         i--;
   }
   return 1;
}

int count_words_in_file_check_for_error(FILE * dict_file)
{
   char ch;
   unsigned char in_word = 0, word_on_line_found = 0;
   int dict_word_count= 0;

   while((ch = fgetc(dict_file)) != EOF)
   {
      if(ch == ' ' || ch == '\t' || ch == '\0' || ch == '\n')
      {
         if(in_word)
         {
            in_word = 0;
            dict_word_count++;
            word_on_line_found++;
         }
    	
     	 if(ch == '\0' || ch == '\n')
     	 {
            if(word_on_line_found > 1)
            {
               printf("Error: no more than one word per line.\n");
               return -1;
            }
            word_on_line_found = 0;
         }
      }
      else
      {
         if(ch == '#' && in_word == 0 && word_on_line_found == 0)
         {  // skip this line
            while((ch = fgetc(dict_file)) != '\n');
         }
         else
         {
            in_word = 1;
         }
      }
   }
   return dict_word_count;
}

