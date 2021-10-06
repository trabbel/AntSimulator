import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import glob

path = r'./data_exp-counters-multi-iteration' # use your path
dilusion_max_set = [50, 100, 250, 500, 750, 1000]
dilusion_increment_power = [1, 2, 5, 10, 50, 100, 1000]
# evapor_set = evapor_set[::-1]
all_files = glob.glob(path + "/*ctr_pherm-0*iter-*.csv")

spec_data = np.zeros((6,7,4))
k = 0
# for i, e in enumerate(dilusion_max_set):
#     for j, m in enumerate(dilusion_increment_power):
for file in all_files:
            # if (("mal_frac-0.015625") in file) and (("hell_phermn_evpr-1.000000") in file):
  if (("AntSimData_SIM_STEPS-50000_SIM_ITERS-4_mal_frac-0.500000_mal_delay-100_mal_ants_focus-1_ant_tracing-1_ctr_pherm-0_hell_phermn_intens-1.000000_hell_phermn_evpr-0.000000_dil_max-500_dil_incr-500_iter-") in file):
                # if ("dil_max-"+str(e)+"_") in file:
                #     if ("dil_incr-"+str((int)(e/m))+"_") in file:
    dt = np.array(pd.read_csv(file, header=None))
    # modified_data = dt
    # modified_data = (modified_data)/(1-(2**-m))
    # spec_data[i, j] += dt[-1]/20
    # k +=1
    print(dt[-1], file[-5], file[-6])
    k += (dt[-1][0])/20
                        # print(file)
                        # continue

print(k)
# #heatmap axis
# x = np.array(dilusion_max_set)
# # y = [str(n) for n in range(1,11)]
# y = np.array(dilusion_increment_power)
# # x=x[::-1]
# # y=y[::-1]
# color_blue = sns.color_palette("coolwarm_r", as_cmap=True)
# color_green = sns.color_palette("coolwarm_r", as_cmap=True)

# fig,axn = plt.subplots(2, 2, sharex=True, sharey=True)

# df=[]
# for i in range(spec_data.shape[-1]):
#     df.append(pd.DataFrame(spec_data[:,:,i]))

# print(df)

# for i,ax in enumerate(axn.flat):
#     if(i%2==0):
#         df[i] = df[i].rename_axis("Rel Evaporation Multi")
#     if(i>1):
#         df[i] = df[i].rename_axis("Malicious Ants Fraction", axis=1)
#     color = color_blue if i<2 else color_green
#     plot = sns.heatmap(df[i], ax=ax, xticklabels = y, yticklabels = x, cmap=color, cbar=True, vmin=0, vmax=1 if i>1 else 8.5)
#     plt.xticks(rotation = 45)

# plot.figure.savefig("graphs/6-oct_morning/heatmap_015625_1.png")
