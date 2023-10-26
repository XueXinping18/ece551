#ifndef __RAND_STORY_H__
#define __RAND_STORY_H__

#include <stdio.h>

#include "provided.h"
//any functions you want your main to use
catarray_t parseWords(char * fileName);
char ** parseTemplate(int * numLinesPr,
                      char * fileName,
                      catarray_t * cats,
                      int isReusable);
void printStory(FILE * fp, char ** story, int numLines);
void freeStory(char ** story, int numLines);
void freeCatarray(catarray_t * cats);
#endif
