#include "unpack_n_merge.h"

void unpackPm(geneptr t1){
    printf("\nIn UnpackPm:Start");
    fitness1(t1);
    float prob[t1->npm];
    int no_of_unpacks=1;
    for(int i=0;i<t1->npm;i++){
        prob[i]=calculateProbOfUnpack(t1->pm_list[i],t1);
        if(i!=0){
            prob[i]+=prob[i-1];
        }
        //printf("\nran:%f/%f\n",t1->pm_list[i]->utlz,prob[i]);
    }
    for(int i=0;i<no_of_unpacks;i++){
        float ran=(float)rand()/RAND_MAX;
        for(int j=0;j<t1->npm;j++){
            if(ran <= prob[j] ){
                removePm(t1->pm_list[j],t1);
                break;
            }
        }
    }
    int cn_absent[CNT_SIZE];
    int absent_count=findAbsentContainers(t1,cn_absent);
    printf("\nIn UnpackPm: %d Containers Removed from %d pms",absent_count,no_of_unpacks);
    heuristics2(t1,cn_absent,absent_count);
    printf("\nIn UnpackPm:End");
}

float calculateProbOfUnpack(pmptr pm,geneptr t1){
    float prob_of_unpack=0;
    float sum_of_free_cpu=0;
    for(int i=0;i<t1->npm;i++){
        sum_of_free_cpu+=(1-t1->pm_list[i]->utlz);
    }
    prob_of_unpack=(1-pm->utlz)/sum_of_free_cpu;
    return(prob_of_unpack);
}

void mergeVm(geneptr t1){
    //printf("\nIn MergeVm:Start");
    //sort all vm asc
    sortVmbyRes(t1);
    //printStats(t1);
    //for each pm, either replace 2 smallest with a larger random vm or just replace 1 if possible
    for(int i=0;i<t1->npm;i++){
        pmptr pm=t1->pm_list[i];
        if(pm->nvm<1){
            removePm(pm,t1);
            i=i-1;//so that the pm replaced will be evaluated.
            continue;
        }
        if(pm->nvm>=2){
            vmptr first_vm=pm->vm_list[0];
            vmptr second_vm=pm->vm_list[1];

            if(can2VmsBeReplacedBy1(first_vm,second_vm,pm)==1){
                //printf("\nIn MergeVm:1");
                replace2VmsByRandom(first_vm,second_vm,pm);
                //printf("\nIn MergeVm:2");
                continue;
            }
        }

        vmptr vm=pm->vm_list[0];

        if(canThisVmBeReplacedByLargerVm(vm,pm)==1){
            //printf("\nIn MergeVm:3\t%f",pm->mem);
            replaceThisVmByLargerVm(vm,pm);
            //printf("\nIn MergeVm:4");
        }
    }
    printf("\nIn MergeVm:End");
}
