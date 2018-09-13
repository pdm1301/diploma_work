import subprocess
import sys
import traceback
import logging
import json

programs={'Intel' : "./intel_prog",
          'GCC' : "./gcc_prog",
          'Clang' : "./clang_prog",
          'Intel_avg' : "./intel_avg_prog",
          'GCC_avg' : "./gcc_avg_prog",
          'Clang_avg' : "./clang_avg_prog",
          'MKL' : "./mkl.out"}

matrSz=1024

for currentProgram in programs:
    err_file=open(str(currentProgram)+" errors.txt",'w')
    res_file=open(str(currentProgram)+"_data.txt", 'w')

    for i in range(1000):
        cmd=[programs[currentProgram] , str(matrSz)]
                
        procInfo = subprocess.run(cmd, stdout=subprocess.PIPE,stderr=subprocess.PIPE)
        
        if procInfo.returncode == 0:
            try:
                if currentProgram == 'MKL':
                    string=str(procInfo.stdout)
                    index=string.find("performance")
                    time=str(string[len("Time:  "): index-2])
                else:    
                    time=float(procInfo.stdout)
                    
                res_file.write(str(time)+"\n")
                
            except Exception as e:
                logging.error(traceback.format_exc())
                err_file.write(str(traceback.format_exc())+"\n\n")
                tempResult = -1
        else:
            err_file.write("current size:"+str(size)+"\n")
            err_file.write('Error while running program: '+procInfo.stdout.decode("utf-8")+procInfo.stderr.decode("utf-8")+"\n\n")
    
    err_file.close()
    res_file.close()