/* 
 * File:   pydaw_files.h
 * Author: JeffH
 *
 * Functions for opening pydaw files and reading/parsing their string content 
 */

#ifndef PYDAW_FILES_H
#define	PYDAW_FILES_H

#ifdef	__cplusplus
extern "C" {
#endif

/*Standard string sizes.  When in doubt, pick a really big one, it's better to 
 * waste memory than to SEGFAULT...*/
#define LMS_LARGE_STRING 65536
#define LMS_MEDIUM_STRING 8192
#define LMS_SMALL_STRING 512
#define LMS_TINY_STRING 32
    
#include <stdio.h>
    
char * get_string_from_file(char * a_file, int a_size)
{
    char * f_buffer = (char*)malloc(sizeof(char) * a_size);
    FILE * f_file;
	
    f_file = fopen(a_file, "r");
	
    while (!feof(f_file))
    {
        fgets(f_buffer,a_size,f_file);
        printf("%s",f_buffer);
    }
	
    fclose(f_file);
    
    return f_buffer;
}

typedef struct st_1d_char_array
{
    char ** array;
    int x_count;    
}t_1d_char_array;

typedef struct st_2d_char_array
{
    char *** array;
    int x_count;
    int y_count;
}t_2d_char_array;

void g_free_1d_char_array(t_1d_char_array * a_array)
{
    int f_i = 0;
    
    while(f_i < (a_array->x_count))
    {
        free(a_array->array[f_i]);
        f_i++;
    }
    
    free(a_array->array);    
    free(a_array);
}

void g_free_2d_char_array(t_2d_char_array * a_array)
{
    int f_i, f_i2 = 0;
    
    while(f_i < (a_array->x_count))
    {
        f_i2 = 0;
        
        while(f_i2 < (a_array->y_count))
        {
            free(a_array->array[f_i][f_i2]);
        }
        
        free(a_array->array[f_i]);
        f_i++;
    }
    
    free(a_array->array);    
    free(a_array);
}

/* A specialized split function.  Column count and string size will always be known in advance
 for all of the use cases in PyDAW*/
t_1d_char_array * c_split_str(char * a_input, char a_delim, int a_column_count, int a_string_size)
{
    int f_i = 0;
    int f_current_string_index = 0;
    int f_current_column = 0;
    
    t_1d_char_array * f_result = (t_1d_char_array*)malloc(sizeof(t_1d_char_array));
    f_result->array = (char**)malloc(sizeof(char) * a_column_count);
    f_result->x_count = a_column_count;
    
    while(f_i < a_column_count)
    {
        f_result->array[f_i] = (char*)malloc(sizeof(char) * a_string_size);
        f_i++;
    }
    
    f_i = 0;
    
    while(1)
    {
        if(a_input[f_i] == a_delim)
        {
            f_current_column++;
            if(f_current_column >= a_column_count)
            {
                break;
            }
            f_current_string_index = 0;
            
        }
        else if((a_input[f_i] == '\n') || (a_input[f_i] == '\0'))
        {
            break;
        }
        else
        {
            f_result->array[f_current_column][f_current_string_index] = a_input[f_i];
            f_current_string_index++;
        }
        
        f_i++;
    }
    
    return f_result;
}

/* Return a 2d array of strings from a file delimited by "|" and "\n" individual fields are 
 * limited to being the size of LMS_TINY_STRING */
t_2d_char_array * g_get_2d_array_from_file(char * a_file, int a_size)
{
    char * f_file_chars = get_string_from_file(a_file, a_size);
    
    t_2d_char_array * f_result = (t_2d_char_array*)malloc(sizeof(t_2d_char_array));
    
    f_result->x_count = 0;
    int f_i = 0;
    while(f_i < a_size)  //Count the width of the rows.  It's assumed that every row will have the same column count, if not, the file is malformed
    {
        if(f_file_chars[f_i] == '|')
        {
            f_result->x_count = (f_result->x_count) + 1;
        }
        else if((f_file_chars[f_i] == '\n') || (f_file_chars[f_i] == '\0'))
        {
            break;
        }
        
        f_i++;
    }
    
    f_result->y_count = 0;
    f_i = 0;
    while(f_i < a_size)  //Count the width of the rows.  It's assumed that every row will have the same column count, if not, the file is malformed
    {
        if(f_file_chars[f_i] == '\n')
        {
            f_result->y_count = (f_result->y_count) + 1;
        }
        else if(f_file_chars[f_i] == '\0')
        {
            break;
        }
        
        f_i++;
    }
    
    f_result->array = (char***)malloc(sizeof(char*) * (f_result->x_count));
    
    f_i = 0;
    int f_i2 = 0;
    
    while(f_i < (f_result->x_count))
    {
        f_result->array[f_i] = (char**)malloc(sizeof(char*) * (f_result->y_count));
        
        while(f_i2 < (f_result->y_count))
        {
            f_result->array[f_i][f_i2] = (char*)malloc(sizeof(char) * LMS_TINY_STRING);
                        
            f_i2++;
        }
        
        f_i++;
    }
    
    //TODO:  Populate the array here...
    
    return f_result;
    
}

#ifdef	__cplusplus
}
#endif

#endif	/* PYDAW_FILES_H */

