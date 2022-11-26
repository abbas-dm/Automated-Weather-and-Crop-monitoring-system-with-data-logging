# Part of BTP Project
'''
Building a Model to take Temperature, Humidity and Rainfall data to predict PH of the soil for pomegranate fruit.
'''

import pandas as pd
from pandas.plotting import scatter_matrix
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm
import seaborn as sns
import os, pickle
from sklearn.preprocessing import StandardScaler
from sklearn.preprocessing import MinMaxScaler
from sklearn.neighbors import KNeighborsClassifier
from sklearn.metrics import confusion_matrix
from sklearn.model_selection import train_test_split

 
# Reading the dataset
df = pd.read_csv("Crop_recommendation.csv")

# PH value vary between 3.00 to 8.00, So to simplify we choose to classify
# these values into 4 classes they are - 
#       3 for ph less than 5 (low PH)
#       5 for ph suitable for pomegranate 5 - 6.5
#       6 for ph suitable for pomegranate 6.5 - 7.5
#       8 for ph greater than 7.5 (High PH)
for i in range(len(df['ph'])):
    if(df['ph'][i] < 5):
        df['ph'][i] = 3
    elif(5 <= df['ph'][i] < 6.5):
        df['ph'][i] = 5
    elif(6.5 <= df['ph'][i] < 7.5):
        df['ph'][i] = 6
    elif(7.5 <= df['ph'][i]):
        df['ph'][i] = 8

features = ['temperature', 'humidity', 'rainfall']

X = df[features]
y = df['ph']

# dividing X, y into train and test data
X_train, X_test, y_train, y_test = train_test_split(X, y, random_state = 0)

knn = KNeighborsClassifier(n_neighbors = 10).fit(X_train, y_train)
 
# accuracy on X_test
accuracy = knn.score(X_test, y_test)
print(accuracy)
 
# creating a confusion matrix
knn_predictions = knn.predict(X_test)
cm = confusion_matrix(y_test, knn_predictions)

print(cm)

# Save Model to a file
with open('knn.h', 'wb') as m:
    pickle.dump(knn, m)
