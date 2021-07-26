"""
This program will automate the script to get the experiment result. Note that this will not 
create the graph but will merge all csv files into single csv file.

idea: run one experiment (each time) and store csv on memory (well, who cares when we have turing cluster), 
then will merge all the experiment.

Change this script so that you can run the experiment as you intended.
"""

import subprocess
import pandas as pd
import numpy as np
#import time

#change these initial condition
save_experiment = False
timestep = 10000
iteration = 30
mal_fraction = 0.5
mal_timer = 100
mal_focus = "F"
tracing_pattern = "F"
counter_pheromone = "F"
fake_intensity = 0
display_GUI = "F"
metric_mode = 3
ref = -1
#loop function
matrix = np.zeros((10,10))
for i in range(10):
    print("experiment "+str(i)+"/10\n")
    #set initialized param
    mal_fraction = 0.5
    for j in range(10):
        print("subexperiment " + str(j)+"/10")
        if(save_experiment):
            name = "exp"+str(i+1)+"_"+str(j+1)+".csv"
        else:
            name = "AntSimDataBuf.csv"
        f = open(name,"w")
        subprocess.run(['./build/AntSimulator', str(timestep), str(iteration), str(mal_fraction), str(mal_timer), str(mal_focus), str(tracing_pattern), str(counter_pheromone), str(fake_intensity), str(display_GUI), str(metric_mode),str(1)], stdout = f)
        #this subprocess will yield one csv file
        #what we want is something like this column = [exp1, exp2, exp3, exp4, ...], each row represents the time step
        #with this format, we can, ideally, get n csv files (where n is number of experiments required to be done by another variable) and merge it again.
        f.close()
        data = pd.read_csv(name, header=None).T #this will transpose the dataframe
        #create average column
        data['mean'] = data.mean(axis=1)
        #sample the data at t = timestep/2 (note that bin size is 10).
        matrix[i][j] = data.loc[int(timestep/20),'mean']
        #print(str(data.loc[int(timestep/20),'mean'])+'\n')
        #update the value
        mal_fraction = mal_fraction/2
        del data
        
    #update the value
    fake_intensity = fake_intensity + 0.2
matrix = np.fliplr(matrix)
np.save('./data.npy', matrix)