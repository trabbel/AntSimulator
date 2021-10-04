import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

mal_frac = str(("%.6f" % (2**-10)))
hell_evpr_multi = str(1000)
filename = "AntSimData_SIM_STEPS-50000_SIM_ITERS-1_mal_frac-"+mal_frac+"_mal_delay-100_mal_ants_focus-1_ant_tracing-1_ctr_pherm-0_hell_phermn_intens-1.000000_hell_phermn_evpr-"+hell_evpr_multi+".000000_iter-0.csv"
foldername = "./data_exp-mal_frac-mal_evpr_rate-one_iter/"
data = pd.read_csv(foldername+filename,header=None).T
data = np.array(data)
data = data.T
# print(data)
plt.plot(data[:,0], 'b', linestyle='-', label='Food bits per ant \ncollected from source')
plt.plot(data[:,1], 'm', linestyle='-.', label='Food bits per ant \ndelivered to the nest​')
plt.plot(data[:,2], 'r', linestyle='--', label='Fraction of ants that \ncollected food from source​')
plt.plot(data[:,3], 'k', linestyle=':', label='Fraction of ants that \ndelivered food to the nest​')
# plt.legend(fontsize=15)
# plt.xlabel('iterations')
# plt.ylabel('cumulative food count/# of benevolent ants')
plt.savefig(foldername+"graphs/"+filename+".png")

