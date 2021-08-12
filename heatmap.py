import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
#load data and preprocess the data
data = pd.read_csv("./AntSimData_avgOf10.csv", header=None)
#data = np.fliplr(data)
print(data.shape)
#heatmap axis
x = [0,0.5,1.0,2.0,5.0,10,50,100,500,1000]
y = ['2^-'+str(n) for n in range(1,11)]
# y = np.flip(y)
# x = np.flip(x)
color = sns.color_palette("Blues", as_cmap=True)
plot = sns.heatmap(data, annot=True,xticklabels = y, yticklabels = x, cmap=color)
plot = plot.pivot("Relative Evaporation Multiplier", "Malicious Ants Fraction", "Food Bits per Ant")
plot.figure.savefig("heatmap.png")
