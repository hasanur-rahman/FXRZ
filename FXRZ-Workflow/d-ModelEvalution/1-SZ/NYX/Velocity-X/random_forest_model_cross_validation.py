from sklearn.model_selection import train_test_split
import pandas as pd
import numpy as np
import os, sys, csv
from sklearn.preprocessing import StandardScaler
from sklearn.ensemble import RandomForestRegressor
from sklearn.model_selection import RandomizedSearchCV
from sklearn.model_selection import RepeatedKFold
from sklearn.model_selection import cross_val_score
from sklearn.feature_selection import RFE
from sklearn.metrics import make_scorer
import math
from math import sqrt
from datetime import datetime


EPSILON=0.000000001

inp_train_file = str(sys.argv[1])
inp_test_file = str(sys.argv[2])

out_train_file = "temp_train_with_header_rfr.csv"
out_test_file = "temp_test_with_header_rfr.csv"
csv_train_out = open(out_train_file, 'w')
csvwriter = csv.writer(csv_train_out)


totalTrainingTime = 0.0
totalTestingTime = 0.0

columns = ["name", "val-range", "avg-val",  "mean-neighbors",  "mean-lorenzo", "mean-spline", "ori-cratio", "cratio", "label"]
dropped_columns = ["name", "ori-cratio"]


csvwriter.writerow(columns)
with open(inp_train_file, 'r') as csv_file:
    csv_reader = csv.reader(csv_file)
    for row in csv_reader:
        csvwriter.writerow(row)


csv_train_out.close()

df = pd.read_csv(out_train_file)

df = df.drop(columns=dropped_columns).copy()

X_train = df.drop(columns=["label"]).copy()
y_train = df["label"]  


csv_test_out = open(out_test_file, 'w')
csvwriter = csv.writer(csv_test_out)


csvwriter.writerow(columns)
#
with open(inp_test_file, 'r') as csv_file:
    csv_reader = csv.reader(csv_file)
    for row in csv_reader:
        csvwriter.writerow(row)


csv_test_out.close()


df = pd.read_csv(out_test_file)


ori_ratio = df["ori-cratio"]
ori_ratio = ori_ratio.values.flatten()


df = df.drop(columns=dropped_columns).copy()
X_test = df.drop(columns=["label"]).copy()
y_test = df["label"]


prev_time = datetime.now()
scaler = StandardScaler()
train_scaled = scaler.fit_transform(X_train)
cur_time = datetime.now()
totalTrainingTime = totalTrainingTime + (cur_time - prev_time).total_seconds()

prev_time = datetime.now()
test_scaled = scaler.transform(X_test)
cur_time = datetime.now()
totalTestingTime = totalTestingTime + (cur_time - prev_time).total_seconds()



prev_time = datetime.now()
##########
# Number of trees in random forest
n_estimators = [int(x) for x in np.linspace(start = 90, stop = 1200, num = 30)]
# Number of features to consider at every split
#max_features = ['auto', 'sqrt']
max_features = ['auto']
# Maximum number of levels in tree
max_depth = [int(x) for x in np.linspace(10, 110, num = 11)]
max_depth.append(None)
# Minimum number of samples required to split a node
min_samples_split = [2, 5, 10]
# Minimum number of samples required at each leaf node
min_samples_leaf = [1, 2, 4]
# Method of selecting samples for training each tree
bootstrap = [True, False]

# Create the random grid
random_grid = {'n_estimators': n_estimators,
               'max_features': max_features,
               'max_depth': max_depth,
               'min_samples_split': min_samples_split,
               'min_samples_leaf': min_samples_leaf,
               'bootstrap': bootstrap}





def myCustomLossFunc(ground_truth, predictions):
	g_t = ground_truth.values.flatten()
	lossVal = 0.0
	for i in range(0, len(g_t)):
		mnVal = np.abs(g_t[i])
		if np.abs(predictions[i]) < mnVal:
			mnVal = np.abs(predictions[i])
			
		lossVal += np.abs((100.0 * np.abs(g_t[i] - predictions[i])) / (1.0 * mnVal))
	return lossVal

custom_scorer = make_scorer(myCustomLossFunc, greater_is_better=False)


rf = RandomForestRegressor()
cv = RepeatedKFold(n_splits=20, n_repeats=3, random_state=1)
rf_model = RandomizedSearchCV(estimator=rf, param_distributions=random_grid, n_iter=10, cv=cv, refit=True, scoring=custom_scorer, verbose=3, random_state=42, n_jobs = -1)
rf_model.fit(train_scaled, y_train)


cur_time = datetime.now()

totalTrainingTime = totalTrainingTime + (cur_time - prev_time).total_seconds()


print("Best params: ", rf_model.best_params_)

prev_time = datetime.now()
y_pred = rf_model.predict(test_scaled)
cur_time = datetime.now()
predict_time = (cur_time - prev_time).total_seconds()
(row, col) = test_scaled.shape
totalTestingTime = totalTestingTime + predict_time

print("total training time: ", format(totalTrainingTime, '.8f'))
print("total testing time: ", format(totalTestingTime, '.8f'))


y_test_np = y_test.values.flatten()
y_pred = y_pred.flatten()


os.system("mkdir Output-RFR")
csv_pred_out = open("Output-RFR/test_data_with_prediction_rf.csv", 'w')
csvwriter = csv.writer(csv_pred_out)


it=0
VALUE_RANGE = 49312264.00
for test_val in y_test_np:
    csvwriter.writerow([(y_test_np[it]/VALUE_RANGE), (y_pred[it]/VALUE_RANGE), ori_ratio[it]])
    it = it + 1


csv_pred_out.close()



os.system("rm temp_train_with_header_rfr.csv temp_test_with_header_rfr.csv")

