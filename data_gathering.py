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
#change these initial condition
timestep = 10000
iteration = 1
mal_fraction = 0.5
mal_timer = 100
mal_focus = 'F'
tracing_pattern = 'F'
counter_pheromone = 'F'
fake_intensity = 0
display_GUI = 'F'
metric_mode = 1
#loop function
matrix = np.zeros((10,10))
for i in range(10):
    print("experiment "+str(i)+"/10\n")
    for j in range(10):
        print("subexperiment " + str(j)+"/10")
        subprocess.run(['./build/AntSimulator', str(timestep), str(iteration),
                    str(mal_fraction), str(mal_timer), mal_focus, tracing_pattern, counter_pheromone, str(fake_intensity), display_GUI, str(metric_mode)])
        #this subprocess will yield one csv file
        #what we want is something like this column = [exp1, exp2, exp3, exp4, ...], each row represents the time step
        #with this format, we can, ideally, get n csv files (where n is number of experiments required to be done by another variable) and merge it again.
        data = pd.read_csv('AntSimData.csv', header=None).T #this will transpose the dataframe
        matrix[i][j] = data[0].max()
        print(str(data[0].max())+'\n')
        #update the value
        mal_fraction = mal_fraction/2
    #update the value
    fake_intensity = fake_intensity + 0.2
np.save('./data.npy', matrix)