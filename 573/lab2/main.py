from __future__ import absolute_import, division, print_function, unicode_literals
import tensorflow as tf

import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np
import os
import pandas as pd
from tensorflow.keras.utils import to_categorical
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Conv1D, Flatten, Dropout#create model
import config as cfg
from tensorflow.keras.utils import plot_model
from tensorflow.keras.optimizers import RMSprop, Adam, Adagrad
import talos as ta
from tensorflow.keras.callbacks import EarlyStopping



def load_data():
    train_set = np.genfromtxt(cfg.path_tra,delimiter=",")
    test_set = np.genfromtxt(cfg.path_tes,delimiter=",")
    x_train, y_train = train_set[:,:-1], train_set[:,-1]
    x_test, y_test = test_set[:,:-1], test_set[:,-1]
    y_train = to_categorical(y_train)
    y_test = to_categorical(y_test)
    
    return x_train, y_train, x_test, y_test

def print_data_info():
    x_train, y_train, x_test, y_test = load_data()
    print("x_train shape: ", x_train.shape)
    print("y_train shape: ", y_train.shape)
    print("x_test shape: ", x_test.shape)
    print("y_test shape: ", y_test.shape)

def make_CNN():
    model = Sequential()#add model layers
    model.add(Conv1D(filters=64, kernel_size=3, activation='relu', input_shape=(64,1)))
    model.add(Conv1D(filters=64, kernel_size=3, activation='relu'))
    model.add(Flatten())
    model.add(Dense(10, activation=cfg.activation))
    return model

def make_NNC():
    model = Sequential()#add model layers
    model.add(Conv1D(filters=64, kernel_size=3, activation='relu', input_shape=(64,1)))
    model.add(Conv1D(filters=64, kernel_size=3, activation='relu'))
    model.add(Flatten())
    model.add(Dense(10, activation=cfg.activation))
    return model


def compile_model(model):
    model.compile(loss=cfg.loss,
                  optimizer=cfg.optimizer,
                  metrics=cfg.metrics)
    return model

def run_model(model, x_train, y_train):
    history = model.fit(x_train, y_train, validation_split=0.2, epochs=30)

def plot_history(history):
    """Plot acc, mne and loss of history
1
    Params
    ------
    history: History
        return of model.fit
    test_file_name: str
        test file name of current CV

    Returns
    -------
    save plot
    """

    # summarize history for accuracy
    plt.plot(history.history['acc'])
    plt.plot(history.history['val_acc'])
    plt.title('model accuracy')
    plt.ylabel('accuracy')
    plt.xlabel('epoch')
    plt.legend(['train', 'val'], loc='upper left')
    plt.savefig('accuracy' + '.png')
    plt.clf()

    # summarize history for loss
    plt.plot(history.history['loss'])
    plt.plot(history.history['val_loss'])
    plt.title('model loss')
    plt.ylabel('loss')
    plt.xlabel('epoch')
    plt.legend(['train', 'val'], loc='upper left')
    plt.savefig('loss' + '.png')
    plt.clf()

def eval_model(model, x_test, y_test):
    print('\n# Evaluate on test data')
    results = model.evaluate(x_test, y_test, batch_size=128)
    print('test loss, test acc:', results)

def search_cnn(x_train, y_train, x_val, y_val, params):
    model = Sequential()#add model layers
    kernel_size = params['kernel']
    act = params['activation']
    model.add(Conv1D(filters=64, kernel_size=kernel_size, activation=act, input_shape=(64,1)))
    model.add(Conv1D(filters=64, kernel_size=kernel_size, activation=act))
    model.add(Dropout(params['dropout']))
    model.add(Flatten())
    model.add(Dense(10, activation='softmax'))
    model.compile(loss=params['losses'],
                  # here we add a regulizer normalization function from Talos
                  optimizer=params['optimizer'](lr=params['lr']),
                  metrics=['accuracy'])
    es = EarlyStopping(monitor='val_loss', mode='min')
    history = model.fit(x_train, y_train, 
                        validation_split=0.2,
                        batch_size=params['batch_size'],
                        epochs=params['epochs'],
                        callbacks=[es],
                        verbose=0)
    
    # finally we have to make sure that history object and model are returned
    return history, model

def search_relu(x_train, y_train, x_val, y_val, params):
    model = Sequential()
    model.add(Dense(64, activation='relu'))
    model.add(Dropout(params['dropout']))
    model.add(Dense(params['first_neuron'], activation='relu'))
    model.add(Dropout(params['dropout']))
    model.add(Dense(10, activation=params['last_activation']))
    
    model.compile(loss=params['losses'],
                  # here we add a regulizer normalization function from Talos
                  optimizer=params['optimizer'](lr=params['lr']),
                  metrics=['accuracy'])
    
    history = model.fit(x_train, y_train, 
                        validation_split=0.2,
                        batch_size=params['batch_size'],
                        epochs=params['epochs'],
                        verbose=0)
    
    # finally we have to make sure that history object and model are returned
    return history, model

def search_tanh(x_train, y_train, x_val, y_val, params):
    model = Sequential()
    model.add(Dense(64, activation='tanh'))
    model.add(Dropout(params['dropout']))
    model.add(Dense(params['first_neuron'], activation='tanh'))
    model.add(Dropout(params['dropout']))
    model.add(Dense(10, activation=params['last_activation']))
    
    model.compile(loss=params['losses'],
                  # here we add a regulizer normalization function from Talos
                  optimizer=params['optimizer'](lr=params['lr']),
                  metrics=['accuracy'])
    
    history = model.fit(x_train, y_train, 
                        validation_split=0.2,
                        batch_size=params['batch_size'],
                        epochs=params['epochs'],
                        verbose=0)
    
    # finally we have to make sure that history object and model are returned
    return history, model

def run_search_relu(model=search_relu):
    x_train, y_train, x_test, y_test = load_data()
    p = {'lr': [0.5, 5],
     'first_neuron':[32, 64],
     'batch_size': [32, 16],
     'epochs': [100],
     'dropout': [0, 0.5],
     'weight_regulizer':[None],
     'emb_output_dims': [None],
     'shape':['brick','long_funnel'],
     'optimizer': [Adam, Adagrad, RMSprop],
     'losses': ['mse', 'categorical_crossentropy'],
     'activation':['relu'],
     'last_activation': ['softmax']}
    t = ta.Scan(x=x_train,
            y=y_train,
            model=model,
            experiment_name="relu", 
            params=p)
    return t

def run_search_tanh(model=search_tanh):
    x_train, y_train, x_test, y_test = load_data()
    p = {'lr': [0.5, 5],
     'first_neuron':[32, 64],
     'batch_size': [32, 16],
     'epochs': [100],
     'dropout': [0, 0.5],
     'weight_regulizer':[None],
     'emb_output_dims': [None],
     'shape':['brick','long_funnel'],
     'optimizer': [Adam, Adagrad, RMSprop],
     'losses': ['mse', 'categorical_crossentropy'],
     'activation':['relu'],
     'last_activation': ['softmax']}
    t = ta.Scan(x=x_train,
            y=y_train,
            model=model,
            experiment_name="tanh", 
            params=p)
    return t


def run_search_cnn(model=search_cnn):
    x_train, y_train, x_test, y_test = load_data()
    x_train = np.reshape(x_train, (3823, 64, 1))
    x_test = np.reshape(x_test, (1797, 64, 1))
    p = {'lr': [0.5, 5],
     'kernel':[3, 5],
     'batch_size': [32, 16],
     'epochs': [100],
     'dropout': [0, 0.5],
     'activation':['relu','tanh'],
     'optimizer': [Adam, Adagrad, RMSprop],
     'losses': ['mse', 'categorical_crossentropy'],
     'last_activation': ['softmax']}
    # p = {'lr': [0.5, 5],
    #  'kernel':[3, 5],
    #  'batch_size': [32, 16],
    #  'epochs': [100],
    #  'dropout': [0, 0.5],
    #  'weight_regulizer':[None],
    #  'emb_output_dims': [None],
    #  'shape':['brick','long_funnel'],
    #  'optimizer': [Adam, Adagrad, RMSprop],
    #  'losses': ['mse', 'categorical_crossentropy'],
    #  'activation':['relu'],
    #  'last_activation': ['softmax']}
    t = ta.Scan(x=x_train,
            y=y_train,
            model=model,
            experiment_name="cnn", 
            params=p)
    return t

def search_analysis(search_result):
    analyze_object = ta.Analyze(search_result)
    valacc = analyze_object['val_acc'].data
    best_index = valacc.idxmax(axis = 1)
    print(analyze_object.loc[best_index, :])

# def cnn_check():
#     train_set = np.genfromtxt(cfg.path_tra,delimiter=",")
#     test_set = np.genfromtxt(cfg.path_tes,delimiter=",")
#     x_train, y_train = train_set[:,:-1], train_set[:,-1]
#     x_test, y_test = test_set[:,:-1], test_set[:,-1]
#     x_train = np.reshape(x_train, (3823, 64, 1))
#     x_test = np.reshape(x_test, (1797, 64, 1))
#     y_train = to_categorical(y_train)
#     y_test = to_categorical(y_test)

#     #create model
#     model = Sequential()#add model layers
#     model.add(Conv1D(filters=64, kernel_size=3, activation='relu', input_shape=(64,1)))
#     model.add(Conv1D(filters=64, kernel_size=3, activation='relu'))
#     model.add(Flatten())
#     model.add(Dense(10, activation='softmax'))
#     #compile model using accuracy to measure model performance
#     model.compile(optimizer='adam', loss='categorical_crossentropy', metrics=['accuracy'])
#     model.fit(x_train, y_train, validation_split=0.2, epochs=100)
#     results = model.evaluate(x_test, y_test, batch_size=128)
#     print('test loss, test acc:', results)

if __name__ == "__main__":
    # 1. test neural network classifier with relu hidden layer
    search_result_relu = run_search_relu()
    search_analysis(search_result_relu)

    # 2. test neural network classifier with tanh hidden layer
    search_result_tanh = run_search_tanh()
    search_analysis(search_result_tanh)

    # 3. test CNN network
    # I am doing it manuly
    search_result_cnn = run_search_cnn()
    search_analysis(search_result_cnn)


