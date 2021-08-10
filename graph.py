'''
This program will "AVERAGE" the result from the experiment result (from AntSimData.csv) and put it into
a simple line graph. Please change axis name according to your experiments. 
'''
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

data = pd.read_csv("./AntSimData.csv", header=None).T
# print(data)
# data['mean'] = data.mean(axis=1)
#test_list = data[0].tolist()[:-1]

#res = all(i <= j for i, j in zip(test_list, test_list[1:]))
  
# printing result
#print ("Is list strictly increasing ? : " + str(res))

data = np.array(data)
plt.plot(data)
plt.xlabel('iterations')
plt.ylabel('cumulative food count/# of benevolent ants')
plt.savefig("graph.png")

