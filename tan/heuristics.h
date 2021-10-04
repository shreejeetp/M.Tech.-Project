#ifndef HEURISTICS_H_INCLUDED
#define HEURISTICS_H_INCLUDED

#include "macros.h"
#include "structures.h"
#include "crud_gene.h"
#include "crud_vm.h"
#include "integrity.h"
#include "first_fit.h"

void heuristics2(geneptr t1,int cn_absent[],int absent_count);
void ff_rc_heuristics(cnptr cn,geneptr t1);
void allocate_list_ff_rc(geneptr t1,int cn_list[],int list_length);

#endif // HEURISTICS_H_INCLUDED
