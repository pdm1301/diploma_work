import random
import subprocess
import sys
import traceback
import logging
import json

random.seed(0)

paramValues = [1,2,4,8,16,32,64,128,256,512,1024]

runCount = 50
compiling={'Intel':["icpc", "-Ofast", "-xHost", "my_tr.cpp", "-o", "my"],
           'GCC':["g++", "-Ofast", "-march=native",  "my_tr.cpp", "-o", "my"], # -mfma - для Haswell
           'Clang':["clang++", "-Ofast", "-fvectorize", "-march=native", "-ffp-contract=fast", "-ffast-math","-mfma", "my_tr.cpp", "-o", "my"]} # -mfma - для Haswell
	}
matrSz = 1024
cmd=["./my", str(matrSz)]

currentCompiler = ''

for currentCompiler in compiling:
    err_file=open(str(currentCompiler)+" errors.txt",'w')
    with open(str(currentCompiler)+"_paramTime.txt", 'r') as f: listOfResults = json.load(f)
        
    countOfParams=len(listOfResults)//10
    listOfResults = listOfResults[:countOfParams]
    
    bestParams=[]
    bestTime=sys.float_info.max

    for i in range(countOfParams):
        currentParams=listOfResults[i]['params']
        with open('my_tr.cpp', 'r') as f: code=f.read()

        for r in range(len(currentParams)):
            code = code.replace('%param{0}%'.format(r+1), str(currentParams[r]))

        with open("tmp/my_tr.cpp", "w") as f: f.write(code)

        procInfo = subprocess.run(compiling[currentCompiler], cwd='tmp')   #Компилируем программу разными компиляторами
        procInfo.check_returncode()

        minTime=sys.float_info.max
        for j in range(runCount):
            procInfo = subprocess.run(cmd, cwd='tmp', stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            if procInfo.returncode == 0:
                try:
                    tempResult = float(procInfo.stdout)
                except Exception as e:
                    logging.error(traceback.format_exc())
                    err_file.write(str(traceback.format_exc())+"\n\n")
                    tempResult = -1
            else:
                err_file.write("current:"+str(currentParams[0])+' '+str(currentParams[1])+' '+ str(currentParams[2])+' '+str(currentParams[3])+"\n")
                err_file.write('Error while running program: '+procInfo.stdout.decode("utf-8")+procInfo.stderr.decode("utf-8")+"\n\n")
                tempResult = -1
            if tempResult<minTime:
                minTime=tempResult

        if minTime<bestTime:
            bestTime=minTime
            bestParams=currentParams

        print("current:", currentParams[0], currentParams[1], currentParams[2], currentParams[3],"Performance=",matrSz**3*2/minTime/1e9)
        print("best:", bestParams[0], bestParams[1], bestParams[2], bestParams[3],"Performance=",matrSz**3*2/bestTime/1e9)

        listOfResults[i]['time'] = minTime

    listOfResults.sort(key = lambda res: res['time'])
    with open(str(currentCompiler)+"_paramTime_exact.txt", 'w') as f: json.dump(listOfResults, f)
    f.close()
