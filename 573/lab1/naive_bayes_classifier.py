#%%
import numpy as np
from sklearn import datasets
# from utils import train_test_split, normalize, accuracy_score, Plot
import os
#%%
data_dir = os.path.join(os.getcwd(), '20newsgroups')
path_maps = os.path.join(data_dir, 'map.csv')
path_test_data = os.path.join(data_dir, 'test_data.csv')
path_test_label = os.path.join(data_dir, 'test_label.csv')
path_train_data = os.path.join(data_dir, 'train_data.csv')
path_train_label = os.path.join(data_dir, 'train_label.csv')
# %%
test_data = np.genfromtxt(path_test_data, delimiter=',')

# %%
