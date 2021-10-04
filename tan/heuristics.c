#include "heuristics.h"

//heuristics that allocated container acc to tan paper
void heuristics2(geneptr t1,int cn_absent[],int absent_count){
    sortContainersByRes(t1);

    for(int i=0;(i<t1->npm)&&(absent_count>0);i++){
        pmptr pm=t1->pm_list[i];
        for(int j=0;(j<pm->nvm)&&(absent_count>0);j++){
            vmptr vm=pm->vm_list[j];
            //vmSizeIntegrityCheck(vm);
            for(int k=0;k<absent_count;k++){
                if(vm->ncn<2){
                    break;
                }
                float cpu_req=data1.CNT_TYPE[cn_absent[k]][0];
                float mem_req=data1.CNT_TYPE[cn_absent[k]][1];

                cnptr cn1=vm->cn_list[0];
                cnptr cn2=vm->cn_list[1];

                float combined_cpu=cn1->cpu+cn2->cpu;
                float combined_mem=cn1->mem+cn2->mem;

                if((combined_cpu>=cpu_req)&&(combined_mem>=mem_req)){
                    cn1=createCn(cn1->ogi);
                    cn2=createCn(cn2->ogi);

                    //printf("\nremove 1:%f",vm->mem);
                    removeContainer(cn1->ogi,t1);
                    //printf("\nremove 2:%f",vm->mem);
                    removeContainer(cn2->ogi,t1);

                    //printf("\nremove 3:%f",vm->mem);
                    cnptr new_cn=createCn(cn_absent[k]);
                    //printf("\nafter remove");
                    addCnToVmAndPm(new_cn,vm,pm);

                    ff_rc_heuristics(cn1,t1);
                    ff_rc_heuristics(cn2,t1);

                    sortContainersByRes(t1);

                    absent_count--;

                    if(k!=absent_count){
                        cn_absent[k]=cn_absent[absent_count];
                    }
                    //break;
                }
            }
        }
    }

    if(absent_count>0){
        allocate_list_ff_rc(t1,cn_absent,absent_count);
    }
}

//first fit/random create first fit heuristics to add a container to the gene
void ff_rc_heuristics(cnptr cn,geneptr t1){
    vmptr selected_vm=findVmForCn(t1,cn);

    //if theres any vm that can contain both
    if(selected_vm!=NULL){
        addCnToVmAndPm(cn,selected_vm,findPmByVm(selected_vm,t1));
        return;
    }

    //create new vm
    int vm_ogi=selectRandomVmForContainer(cn);
    vmptr new_vm=createVm(vm_ogi);
    addCnToVm(cn,new_vm);

    //find pm which has space for this vm
    pmptr selected_pm=findPmForVm(t1,new_vm);

    //if there is an existing pm that can have it
    if(selected_pm!=NULL){
        addVmToPm(new_vm,selected_pm);
        return;
    }
    //if there's not
    pmptr new_pm=createPm(t1);
    addVmToPm(new_vm,new_pm);
    addPmToGene(new_pm,t1);
    fitness1(t1);
}

//allocates list of container's index to gene by ff&rc/ff
void allocate_list_ff_rc(geneptr t1,int cn_list[],int list_length){
    for(int i=0;i<list_length;i++){
        cnptr newcn=createCn(cn_list[i]);
        ff_rc_heuristics(newcn,t1);
    }
}

