import matplotlib
#matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
import os

def save(name='', fmt='png'):
    pwd = os.getcwd()
    iPath = './pictures/{}'.format(fmt)
    if not os.path.exists(iPath):
        os.mkdir(iPath)
    os.chdir(iPath)
    plt.savefig('{}.{}'.format(name, fmt), fmt='png')
    os.chdir(pwd)

compilers=["Intel_data.txt","GCC_data.txt","Clang_data.txt","Intel_avg_data.txt","GCC_avg_data.txt","Clang_avg_data.txt", "MKL_data.txt"]
for currentCompiler in compilers:
    data = np.genfromtxt(currentCompiler, skip_header=0)

    fig, ax = plt.subplots()
    ax.set_yscale('log')
    ax.hist(data, 30, normed=1, facecolor='green', alpha=0.75)
    #ax.axis([0, 25000, 0, 0.00015])

    from sklearn.neighbors.kde import KernelDensity
    import numpy as np
    m1 = np.min(data)
    m2 = np.max(data)
    dm = m2-m1
    kde0 = KernelDensity(kernel='gaussian', bandwidth=dm/30).fit(data.reshape(-1, 1))
    X_plot = np.linspace(m1-0.2*dm, m2+0.2*dm, 1000).reshape(-1, 1)
    Dens0 = np.exp(kde0.score_samples(X_plot))  #score_samples возвращает логарифм плотности
    fig, ax = plt.subplots()
    ax.plot(X_plot, Dens0, color='blue')
    ax.set_yscale('log')
    ax.set_ylim(0.01, np.max(Dens0)*1.1)
    #plt.show()

    save(currentCompiler, fmt='pdf')
    save(currentCompiler, fmt='png')  
