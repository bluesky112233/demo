
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "dictionary.h"
#include <ctype.h>
//#include <arpa/nameser.h>




char * str_to_lower(char *str)
{
    char *pNew1 = str;
    char *pNew2 = str;

    if(str != NULL) //NULL
    {
        if(strlen(str) != 0) //"" ?
        {
            while(*pNew1)
            {
                *pNew2 = tolower(*pNew1);
                ++pNew2;
                ++pNew1;
            }
            *pNew2 = '\0';

            return str;// return changed string
        }              // and prevent returning null to caller
    }
    return "";//Will never get here for non-null input argurment
}



char *rm_punct(char *str) {
    if (str == NULL){
        return NULL;
    }
    char *p = str;
    char *t = str + strlen(str) - 1;
    while (ispunct(*p)) p++;
    while (ispunct(*t) && p < t) { *t = 0; t--; }
    /* also if you want to preserve the original address */
    { int i;
        for (i = 0; i <= t - p + 1; i++)
        {
            str[i] = p[i];
        }
        p = str;
    } /* --- */
    if (strlen(p) == 0){
        return NULL;
    }
    return p;
}



/**
 * Returns true if all words are spelled correcty, false otherwise. Array misspelled is populated with words that are misspelled.
 */
int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[])
{
    char *pos = 0;                  // position of newline character
    char * pch;
    int number_misspelled = 0;
    char string[80];

    if (fp == NULL){
        exit(5);
    }

    while ( fgets (string, 80, fp) != NULL)
    {

// strip newline
        if ((pos = strchr(string, '\n')) != NULL)
            *pos = '\0';




        //get first word in string
        pch = strtok(string," ");
        // no dead string
        if (pch != NULL && strlen(pch) > LENGTH)
            pch = NULL;
        // strip punctuation
        pch = rm_punct(pch);



        while (pch != NULL) {
            // check original word
            if (check_word(pch, hashtable)) {
                pch = strtok(NULL, " ");
                pch = rm_punct(pch);
            }
            // if not on list, check lowercase version
            else if (check_word(str_to_lower(pch), hashtable)) {
                pch = strtok(NULL, " ");
                pch = rm_punct(pch);
            }
            // else is not a word, add to misspelled and move onto next word
            else {
                if (number_misspelled < MAX_MISSPELLED){
                    misspelled[number_misspelled] = (char *)malloc((LENGTH+1)*sizeof(char));
                    strncpy(misspelled[number_misspelled],pch,(LENGTH+1));
                    number_misspelled++;
                    pch = strtok(NULL, " ");
                    pch = rm_punct(pch);
                }
                else{
                    return number_misspelled;
                }
            }
        }
        // Get next line in file and chomp newline
//        line_buff = NULL;
//        line_size = getline(&line_buff, &line_buff_size, fp);
//        if ((pos = strchr(line_buff, '\n')) != NULL)
//            *pos = '\0';
    }
    return number_misspelled;
}

            



/**
 * Returns true if word is in dictionary else false.
 */
bool check_word(const char* word, hashmap_t hashtable[])
{
    int hashed = hash_function(word);       // hash value of word being passed in
    node * current = hashtable[hashed];     // current link in the list

    while(current != NULL){
        if (strcmp(word, current->word) == 0)
            return true;
        else
            current=current->next;
    }
    return false;
}



/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[])
{
    FILE *dictionary_list;      // File to read dictionary
    char *line_buff = NULL;     // String from file
    char *pos = 0;                  // position of newline character
    int hashed = 0;                 // hash value of string
    size_t line_buff_size = 0;  // size of string buffer
    ssize_t line_size;          // chars in string from file


    // open file
    if ((dictionary_list = fopen(dictionary_file, "r")) == NULL)
    {
        fprintf(stderr, " Error opening file\n");
        exit(1);
    }

    // read line and chomp newline
    line_size = getline(&line_buff, &line_buff_size, dictionary_list);
    if ((pos = strchr(line_buff, '\n')) != NULL)
        *pos = '\0';

    while (line_size >= 0) {

        // set index

        if (line_size > LENGTH+1){

            line_size = getline(&line_buff, &line_buff_size, dictionary_list);
            if ((pos = strchr(line_buff, '\n')) != NULL)
                *pos = '\0';
            continue;
        }
        hashed = hash_function(line_buff);
        //init a node and allocate memory
        struct node *current = (struct node *) malloc(sizeof(struct node));
        // set data for node
        strncpy(current->word, line_buff, LENGTH+1);
        // preappend node to linked list. This elimiates the need to find the end every time
        current->next = hashtable[hashed];
        hashtable[hashed] = current;



        // Get next line in file and chomp newline
        line_size = getline(&line_buff, &line_buff_size, dictionary_list);
        if ((pos = strchr(line_buff, '\n')) != NULL)
            *pos = '\0';
    }
    free(line_buff);
    fclose(dictionary_list);
    return true;
}
