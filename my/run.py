import re
import os
import statistics

#change these variables
gccPath = "C:\\Program Files\\CodeBlocks\\MinGW\\bin\\"
currentPath = "D:\\Projects\\Mtech Project\\my\\"
trials=30

reps = ["CNT_TYPE", "CNT_SIZE_INIT", "VM_TYPE", "VM_SIZE"]

files = ['crossover', 'crud_gene', 'crud_vm', 'data_read', 'evaluation',
         'first_fit', 'heuristics_bpp', 'integrity', 'main', 'population', 'unpack_n_merge']


cn_types = ["cont_syn", "cont_rea"]
cn_sizes = ["200", "500", "1000", "1500"]


if __name__ == "__main__":
    configs = []
    for cn in cn_types:
        for size in cn_sizes:
            vm_type = "vm_syn"
            vm_size = "10"
            if cn == "cont_rea":
                vm_type = "vm_rea"
                vm_size = "20"
            configs.append([cn+size, size, vm_type, vm_size])

    op = []
    os.chdir(currentPath)
    with open("macros.h", "r") as macroFile:
        macro = macroFile.read()
        for config in configs:
            text = macro
            for i, rep in enumerate(reps):
                line = "(#define "+rep+" )\S*( )"
                replacement = "#define "+rep+" "+config[i]+" "
                text = re.sub(line, replacement, text)
            op.append(text)

    os.chdir(gccPath)
    buildObjs = ''
    for filename in files:
        buildObjs = buildObjs+' "'+currentPath+'res\\obj\\'+filename+'.o"'
    buildObjs = buildObjs.strip()

    for i,macro in enumerate(op):
        with open(currentPath+'macros.h', "w") as macroFile:
            macroFile.seek(0)
            macroFile.write(macro)
            macroFile.truncate()

        for filename in files:
            command = '.\\gcc -Wall -O2 -c "'+currentPath+filename + \
                '.c" -o "'+currentPath+'res\\obj\\'+filename+'.o'
            print(command)
            os.system(command)

        buildCmd = '.\\gcc -o "'+currentPath+'res\\final'+str(i)+'.exe" '+buildObjs
        print(buildCmd)
        os.system(buildCmd)

    outputs=[]
    for i in range(len(cn_types)*len(cn_sizes)):
        outputList=[]
        for j in range(trials):
            os.chdir(currentPath)
            os.system('.\\res\\final'+str(i)+'.exe')

            with open(currentPath+'output\\op.csv','r') as opFile:
                outputList.append(float(opFile.read()))

        outputs.append(outputList)

    opStDev=[]
    opMin=[]
    opAvg=[]
    for output in outputs:
        opStDev.append(statistics.stdev(output))
        opMin.append(min(output))
        opAvg.append(statistics.mean(output))
    
    with open(currentPath+'res\\output\\stdDev.txt','w') as stdFile:
        stdFile.seek(0)
        stdFile.write(' \n'.join(map(str, opStDev)))
        stdFile.truncate()
    with open(currentPath+'res\\output\\min.txt','w') as minFile:
        minFile.seek(0)
        minFile.write(' \n'.join(map(str, opMin)))
        minFile.truncate()
    with open(currentPath+'res\\output\\avg.txt','w') as avgFile:
        avgFile.seek(0)
        avgFile.write(' \n'.join(map(str, opAvg)))
        avgFile.truncate()