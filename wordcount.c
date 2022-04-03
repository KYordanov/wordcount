#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORD_MAX_SIZE	25

// Word_and_its_count data structure
typedef struct {
   char word[WORD_MAX_SIZE];  // the characters of the word
   unsigned char count;  // How many times was the word found, in the input string, so far
}tWord_and_its_count;

int count_words_in_file_check_for_error(FILE * dict_file);
int create_dict(tWord_and_its_count * * dict,FILE * dict_file, int word_count);
void sort_words(tWord_and_its_count * dict, int word_count);
void inc_when_word_is_found(tWord_and_its_count * dict, int word_count, char * word);

int main(int argc, char *argv[])
{
   int word_count;
   tWord_and_its_count * dict;
   FILE * text_file;
   
   // Open dictfile
   FILE * dict_file = fopen(argv[1], "r");
   if(dict_file == NULL)
   {
      printf("Fail to open the file %s\n", argv[1]);
      return 1;
   }
   
   do // do while used, so it can break in case of error, to free memory and close files
   {
   // Count the words (n) and check for error.
   word_count = count_words_in_file_check_for_error(dict_file);
   if(word_count < 0) break;
   
   // Create dynamically allocated array
   if(create_dict(&dict, dict_file, word_count) < 0)  break;
   
   // Sort
   sort_words(dict, word_count);
   
   // Go through all words in the input string and search for them in the dictionary, increment the count of the word when it is find
   text_file = fopen(argv[2], "r");
   char temp[25];
   if(text_file == NULL)
   {
      printf("Fail to open the file %s\n", argv[2]);
      break;
   }
   while(fscanf(text_file, "%s", temp) == 1)
   {
      inc_when_word_is_found(dict, word_count, temp);
   }
  
   }while(0);

   // Show
   for(int i = 0; i < word_count; i++)
   {
      if(dict[i].count != 0)
         printf("%d\t%s\n", dict[i].count, dict[i].word);
   }

   free(dict);
   fclose(text_file);
   fclose(dict_file);
}

int find_word_pos(tWord_and_its_count * dict, int left_index, int right_index, char * word)
{
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

void inc_when_word_is_found(tWord_and_its_count * dict, int word_count, char * word)
{
   int pos = find_word_pos(dict, 0, word_count - 1, word);
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

void sort_words(tWord_and_its_count * dict, int word_count)
{  // Buble sort
   for(int i = 0; i < word_count; i++)
   {
      for(int j = 0; j < word_count - 1 - i; j++)
      {
         if(strcmp(dict[j].word, dict[j+1].word) > 0)
         {
            swap_two_str(dict[j].word, dict[j+1].word);
         }
      }
   }
}

int create_dict(tWord_and_its_count * * dict,FILE * dict_file, int word_count)
{
   *dict = malloc(word_count * sizeof(tWord_and_its_count));
   if(dict == NULL)
   {
      printf("Failed to allocate memory\n");
      return -1;
   }
   rewind(dict_file);
   for(int i = 0; i < word_count; i++)
   {
	fscanf(dict_file, "%s", (*dict)[i].word);
   }
}

int count_words_in_file_check_for_error(FILE * dict_file)
{
   char ch;
   unsigned char in_word = 0, word_on_line_found = 0;
   int word_count= 0;
   
   while((ch = fgetc(dict_file)) != EOF)
   {
      if(ch == ' ' || ch == '\t' || ch == '\0' || ch == '\n')
      {
         if(in_word)
         {
            in_word = 0;
            word_count++;
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
         in_word = 1;
      }
   }
   
   return word_count;
}

