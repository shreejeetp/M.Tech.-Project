import numpy as np
import matplotlib.pyplot as plt

#change these variables 
root_path="D:\\Projects\\Mtech Project\\"
minFlag=0 #0:avg,1:min


cnt_sizes=[200,500,1000,1500]
vm_types=['Synthentic','Real World']
def readData(folder):
    data=[]
    with open(root_path+folder+'\\res\\output'+'\\avg.txt','r') as avgFile:
        text=avgFile.read()
        avgData=text.split('\n')
        avgData=np.array(avgData).astype(float)
        avgData=np.around(avgData,decimals=2)
        data.append(avgData)
    with open(root_path+folder+'\\res\\output'+'\\min.txt','r') as minFile:
        text=minFile.read()
        minData=text.split('\n')
        minData=np.array(minData).astype(float)
        minData=np.around(minData,decimals=2)
        data.append(minData)
    with open(root_path+folder+'\\res\\output'+'\\stdDev.txt','r') as stdDevFile:
        text=stdDevFile.read()
        stdDevData=text.split('\n')
        stdDevData=np.array(stdDevData).astype(float)
        stdDevData=np.around(stdDevData,decimals=2)
        data.append(stdDevData)

    return data

if __name__=='__main__':
    data1=readData('my')
    data2=readData('tan')
    fig=plt.figure(figsize=(10,5))
    temp1=[x-10-30 for x in cnt_sizes]
    temp2=[x+10-30 for x in cnt_sizes]
    temp3=[x-10+30 for x in cnt_sizes]
    temp4=[x+10+30 for x in cnt_sizes]

    plt.bar(temp1,data1[minFlag][:4],color='#c62828',width=20,label="Proposed GGA: Synthetic VM")
    plt.bar(temp2,data2[minFlag][:4],color='#6a1b9a',width=20,label="Tan et al. GGA: Synthetic VM")
    plt.bar(temp3,data1[minFlag][4:],color='#ff5f52',width=20,label="Proposed GGA: Real VM")
    plt.bar(temp4,data2[minFlag][4:],color='#9c4dcc',width=20,label="Tan et al. GGA: Real VM")
    plt.ylabel('Energy Consumption(in kWh)')
    plt.xlabel('Container data-set size')
    plt.legend()
    plt.show()
    combined=[]
    for i in range(len(data1[0])):
        combined.append(str(data1[1][i])+' & '+str(data1[0][i])+' & '+str(data1[2][i])+' & '+str(data2[1][i])+' & '+str(data2[0][i])+' & '+str(data2[2][i]))
    vm_type=vm_types[0]
    final_str=''
    j=0
    for com in combined:
        if(j==len(cnt_sizes)):
            j=0
            vm_type=vm_types[1]
        final_str=final_str+vm_type+' & '+str(cnt_sizes[j])+' & '+com+'\\\\\n'
        j+=1
    minDiff=0
    for i in range(8):
        diff=-((data1[1][i]-data2[1][i])/data2[1][i])
        minDiff=minDiff+diff
    minDiff=minDiff/8
    print('The best case energy consumption is on average '+str(minDiff*100)+' higher')    
    avgDiff=0
    for i in range(8):
        diff=-((data1[0][i]-data2[0][i])/data2[0][i])
        avgDiff=avgDiff+diff
    avgDiff=avgDiff/8
    print('The average case energy consumption is on average '+str(avgDiff*100)+' higher')   
    print(final_str)

