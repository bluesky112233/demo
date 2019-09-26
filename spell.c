
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "dictionary.h"
#include <ctype.h>




bool check_word(const char* word, hashmap_t hashtable[])
{
    int bucket = hash_function(word);       
    node * new_node = hashtable[bucket];     

    while(new_node != NULL){
        if (strcmp(word, new_node->word) == 0)
            return true;
        else
            new_node=new_node->next;
    }
    return false;
}



char * lowerString(char *str)
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

            return str;
        }              
    }
    return "";
}



char *remove_punctuation(char *str) {
    if (str == NULL){
        return NULL;
    }
    char *p = str;
    char *t = str + strlen(str) - 1;
    while (ispunct(*p)) p++;
    while (ispunct(*t) && p < t) { *t = 0; t--; }
    
    { int i;
        for (i = 0; i <= t - p + 1; i++)
        {
            str[i] = p[i];
        }
        p = str;
    } 
    if (strlen(p) == 0){
        return NULL;
    }
    return p;
}




int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[])
{
    char *position = 0;                  
    char * pos_char;
    int number_misspelled = 0;
    char string[80];

    if (fp == NULL){
        exit(5);
    }

    while ( fgets (string, 80, fp) != NULL)
    {

        if ((position = strchr(string, '\n')) != NULL)
            *position = '\0';




        
        pos_char = strtok(string," ");
        
        if (pos_char != NULL && strlen(pos_char) > LENGTH)
            pos_char = NULL;
    
        pos_char = remove_punctuation(pos_char);



        while (pos_char != NULL) {
            
            if (check_word(pos_char, hashtable)) {
                pos_char = strtok(NULL, " ");
                pos_char = remove_punctuation(pos_char);
            }
            
            else if (check_word(lowerString(pos_char), hashtable)) {
                pos_char = strtok(NULL, " ");
                pos_char = remove_punctuation(pos_char);
            }
        
            else {
                if (number_misspelled < MAX_MISSPELLED){
                    misspelled[number_misspelled] = (char *)malloc((LENGTH+1)*sizeof(char));
                    strncpy(misspelled[number_misspelled],pos_char,(LENGTH+1));
                    number_misspelled++;
                    pos_char = strtok(NULL, " ");
                    pos_char = remove_punctuation(pos_char);
                }
                else{
                    return number_misspelled;
                }
            }
        }
        
    }
    return number_misspelled;
}

            





bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[])
{
    FILE *the_dictionary;      
    char *buffer = NULL;     
    char *position = 0;                  
    int bucket = 0;                 
    size_t buffer_size = 0;  
    ssize_t size_of_line;          


    
    if ((the_dictionary = fopen(dictionary_file, "r")) == NULL)
    {
        fprintf(stderr, " Error opening file\n");
        exit(1);
    }

    
    size_of_line = getline(&buffer, &buffer_size, the_dictionary);
    if ((position = strchr(buffer, '\n')) != NULL)
        *position = '\0';

    while (size_of_line >= 0) {

        

        if (size_of_line > LENGTH+1){

            size_of_line = getline(&buffer, &buffer_size, the_dictionary);
            if ((position = strchr(buffer, '\n')) != NULL)
                *position = '\0';
            continue;
        }
        bucket = hash_function(buffer);
    
        struct node *new_node = (struct node *) malloc(sizeof(struct node));
        
        strncpy(new_node->word, buffer, LENGTH+1);
    
        new_node->next = hashtable[bucket];
        hashtable[bucket] = new_node;



        size_of_line = getline(&buffer, &buffer_size, the_dictionary);
        if ((position = strchr(buffer, '\n')) != NULL)
            *position = '\0';
    }
    free(buffer);
    fclose(the_dictionary);
    return true;
}
