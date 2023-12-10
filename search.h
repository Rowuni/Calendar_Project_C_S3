#ifndef SEARCH_H
#define SEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

void search_d_list_simple(t_d_list *list, int value);
void search_d_list_levels(t_d_list *list, int value);

#endif 
