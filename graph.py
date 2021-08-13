'''
This program will "AVERAGE" the result from the experiment result (from AntSimData.csv) and put it into
a simple line graph. Please change axis name according to your experiments. 
'''
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

data = pd.read_csv("./data/AntSimData_SIM_STEPS-50000_SIM_ITERS-1_mal_frac-0.000000_mal_delay-100_mal_ants_focus-1_ant_tracing-1_ctr_pherm-0_hell_phermn_intens-1.000000_hell_phermn_evpr-1.000000_iter-0.csv", header=None).T
# print(data)
# data['mean'] = data.mean(axis=1)
#test_list = data[0].tolist()[:-1]

#res = all(i <= j for i, j in zip(test_list, test_list[1:]))
  
# printing result
#print ("Is list strictly increasing ? : " + str(res))

data = np.array(data)
plt.plot(data.T)
plt.xlabel('iterations')
plt.ylabel('cumulative food count/# of benevolent ants')
plt.savefig("graph.png")

