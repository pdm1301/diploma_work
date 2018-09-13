import random
import subprocess
import sys
import traceback
import logging
import json

random.seed(0)

paramValues = [1,2,4,8,16,32,64,128,256,512,1024]

runCount = 2
compilersCount=3
compiling={'Intel':["icpc", "-Ofast", "-xHost", "my_tr.cpp", "-o", "my"],
           'GCC':["g++", "-Ofast", "-march=native", "my_tr.cpp", "-o", "my"], # -mfma - для Haswell
           'Clang':["clang++", "-Ofast", "-fvectorize", "-march=native", "-ffp-contract=fast", "-ffast-math","-mfma", "my_tr.cpp", "-o", "my"]} # -mfma - Haswell
	}
matrSz = 1024
cmd=["timeout", "0.5s", "./my", str(matrSz)]


currentCompiler = ''

for currentCompiler in compiling:
    err_file=open(str(currentCompiler)+" errors.txt",'w')
    listOfResults=[]
    
    for i in range(len(paramValues)):
        for j in range(len(paramValues)):
            for k in range(len(paramValues)):
                for cycle in range(12):

                    currentParams=[cycle+1,i,j,k]
                    with open('my_tr.cpp', 'r') as f: code=f.read()

                    code=code.replace('%param{0}%'.format(1),str(cycle+1))
                    for r in range(3):
                        code = code.replace('%param{0}%'.format(r+2), str(paramValues[currentParams[r+1]]))

                    with open("tmp/my_tr.cpp", "w") as f: f.write(code)

                    procInfo = subprocess.run(compiling[currentCompiler], cwd='tmp')   #Компилируем программу разными компиляторами
                    procInfo.check_returncode()

                    minTime=sys.float_info.max
                    for l in range(runCount):
                        procInfo = subprocess.run(cmd, cwd='tmp', stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                        if procInfo.returncode == 124: tempResult = 10
                        elif procInfo.returncode == 0:
                            try:
                                tempResult = float(procInfo.stdout)
                            except Exception as e:
                                logging.error(traceback.format_exc())
                                err_file.write(str(traceback.format_exc())+"\n\n")
                                tempResult = -1
                        else:
                            err_file.write("current:"+str(currentParams[0])+' '+str(paramValues[currentParams[1]])+' '+ str(paramValues[currentParams[2]])+' '+str(paramValues[currentParams[3]])+"\n")
                            err_file.write('Error while running program: '+procInfo.stdout.decode("utf-8")+procInfo.stderr.decode("utf-8")+"\n\n")
                            tempResult = -1
                        if tempResult<minTime:
                            minTime=tempResult
                    listOfResults.append({'time':minTime, 'params':[cycle+1,paramValues[i],paramValues[j],paramValues[k]]})
                    print("current:", currentParams[0], paramValues[currentParams[1]], paramValues[currentParams[2]], paramValues[currentParams[3]],"Performance=",matrSz**3*2/tempResult/1e9)

    listOfResults.sort(key = lambda res: res['time'])
    with open(str(currentCompiler)+"_paramTime.txt", 'w') as f: json.dump(listOfResults, f)
    f.close()
