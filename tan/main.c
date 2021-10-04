#include "macros.h"
#include "structures.h"
#include "data_read.h"
#include "population.h"
#include "evaluation.h"
#include "crossover.h"

int main()
{
    srand(time(0));
    readCsv();
    printf("\nread done");
    int remove_count = removeUnfitDataContainers();
    printf("\n%d no. of unfit containers removed, final no. of containers:%d",remove_count,CNT_SIZE);

    createPop();

    for(int i=0;i<NO_CROSSOVER;i++){
        elitism(head_population);
        printBestGene(head_population);
        crossMut(head_population);
    }
    geneptr best_gene=searchBestGeneUtlz(head_population);
    printStats(best_gene);
    printBestGene(head_population);
    writeOutput(best_gene->energy_consumption);
    printf("\nend\n");

}
