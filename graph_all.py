import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import glob

path = r'./data_exp-counter-patience_max_incr (another copy)' # use your path
dilusion_max_set = [50, 100, 250, 500, 750, 1000]
dilusion_increment_power = [1, 2, 5, 10, 50, 100, 1000]
# evapor_set = evapor_set[::-1]
all_files = glob.glob(path + "/*_iter-0.csv")

spec_data = np.zeros((6,7,100,4))

for i, e in enumerate(dilusion_max_set):
    for j, m in enumerate(dilusion_increment_power):
        for file in all_files:
            if ("mal_frac-0.003906") in file:
                if ("dil_max-"+str(e)+"_") in file:
                    if ("dil_incr-"+str((int)(e/m))+"_") in file:
                        dt = np.array(pd.read_csv(file, header=None))
                        modified_data = dt
                        modified_data = (modified_data)/(1-(2**-m))
                        spec_data[i, j] = dt
                        break

fig,axn = plt.subplots(6, 7, sharex=True, sharey=True)

for i,ax in enumerate(axn.flat):
    e = int(i/7)
    m = i%7
    data = spec_data[e,m]
    ax.plot(data)
    # ax.set_ylim([0,0.5])

# plt.xlabel('it/erations')
# plt.ylabel('cumulative food count/# of benevolent ants')
# plt.savefig("graphs/graph_all_ants_fraction.png")
plt.savefig("graphs/graph_all_food_collected_per_ant.png")

