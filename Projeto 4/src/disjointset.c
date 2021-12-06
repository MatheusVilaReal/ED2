#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "disjointset.h"

typedef struct Set{

    unsigned int *rank, 
                 *parent, 
                  n;

} Set_t;

DisjointSet newDisjointSet(unsigned int n){

    Set_t* set = (Set_t*) malloc(sizeof(Set_t));
    
    set->parent = (unsigned int*) malloc(sizeof(unsigned int) * n);
    set->rank = (unsigned int*) malloc(sizeof(unsigned int) * n);
    
    set->n = n;

    for(unsigned int i = 0; i < n; i++){

        set->parent[i] = i;
        set->rank[i] = 0;
    }

    return set;
}

unsigned int set_find(DisjointSet set, unsigned int index){

    if(set == NULL)
        return -1;

    Set_t* s = (Set_t*) set;

    if(index > s->n)
        return -1;

    if(s->parent[index] != index)
        s->parent[index] = set_find(s, s->parent[index]);

    return s->parent[index];
}

void set_union(DisjointSet set, unsigned int a, unsigned int b){

    if(set == NULL)
        return;

    Set_t* s = (Set_t*) set;

    unsigned int aset = set_find(s, a);
    unsigned int bset = set_find(s, b);

    if(aset == bset)
        return;

    // Put smaller ranked item under
    // bigger ranked item if ranks are
    // different
    if(s->rank[aset] < s->rank[bset]){
        s->parent[aset] = bset;
    
    } else if(s->rank[aset] > s->rank[bset]){
        s->parent[bset] = aset;
    
    } else{

        s->parent[bset] = aset;
        s->rank[aset] = s->rank[aset] + 1;
    }

    // If ranks are same, then increment
    // rank.
}

void set_free(DisjointSet set){

    if(set == NULL)
        return;

    Set_t* s = (Set_t*) set;

    free(s->parent);
    free(s->rank);

    free(s);
}