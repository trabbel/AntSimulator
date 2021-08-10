import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
#load data and preprocess the data
data = np.load('./data.npy')
#data = np.fliplr(data)
print(data.shape)
#heatmap axis
x = [0,0.2,0.4,0.6,0.8,1,1.2,1.4,1.6,1.8,2.0]
y = ['2^-'+str(n) for n in range(1,12)]
y = np.flip(y)
#x = np.flip(x)
plot = sns.heatmap(data,xticklabels = y, yticklabels = x)
plot.figure.savefig("heatmap.png")
