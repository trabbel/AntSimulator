from mpl_toolkits.axes_grid1 import host_subplot
from mpl_toolkits import axisartist
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
#initialize paramenter here
plt.rcParams["figure.figsize"] = (12,10)
plt.rcParams["axes.labelsize"] = 25
plt.rcParams['xtick.labelsize'] = 18
plt.rcParams['ytick.labelsize'] = 18
plt.rcParams['legend.fontsize'] = 18
mal_frac = str(("%.6f" % (2**-8)))
hell_evpr_multi = str(1)
filename = "AntSimData_SIM_STEPS-50000_SIM_ITERS-4_mal_frac-"+mal_frac+"_mal_delay-100_mal_ants_focus-1_ant_tracing-1_ctr_pherm-0_hell_phermn_intens-"+hell_evpr_multi+"_hell_phermn_evpr-5.000000_dil_max-500_dil_incr-500_iter-20.csv"
foldername = "./data_exp-counters-multi-iteration/"
data = pd.read_csv(foldername+filename,header=None).T
data = np.array(data)
host = host_subplot(111, axes_class=axisartist.Axes)
plt.subplots_adjust(right=0.75)
par1 = host.twinx()
par1.axis["right"].toggle(all=True)
#define which one will get plot
l1,l2 = host.plot(data[0:2].T) #this is the left axis data
data[2:4] *= 100
l3,l4 = par1.plot(data[2:4].T) #this is the right axis data
#change the style of line here
plt.setp(l2,linestyle='--', linewidth=4) 
plt.setp(l3, linestyle=':', linewidth=4)
plt.setp(l4, linestyle='-.', linewidth=4)  
# host.set_ylim(-0.0005, 101) 
# par1.set_ylim(-0.05, 11)
par1.set_ylim(-5, 100) #set y range for fraction to be between 0 - 1
plt.xlabel('Iterations') #x axis
plt.ylabel('Food bits per ants', fontsize=20) #left axis
par1.set_ylabel("Percentage of cooperators", fontsize=20) #right axis
# par1.legend()
# host.legend([l1,l2],['Food bits per ant collected from source','Food bits per ant delivered food to nest']) #change legends here for left data
# par1.legend([l3,l4],['Successful collectors', 'Successful deliverers'])  #change legends here for right data
plt.savefig("graphs/5-oct_morning/"+filename+".png",dpi = 400, facecolor=host.get_facecolor())
#plt.savefig(foldername+"graphs/1.png",dpi = 400, facecolor=host.get_facecolor()) #modified for windows, need to change back
