from scipy.interpolate import interp1d
import numpy, math
import os, sys, csv
import random


# Macro
EPSILON=0.000000001
INC=0.000001

stationaryPointsFilepath = str(sys.argv[1])
targetCompRatiosFilepath = str(sys.argv[2])



sampledCompRatios = [] # these are the compression ratio we want to interpolate

with open(targetCompRatiosFilepath, 'r') as f:
	lines = f.readlines()
	
	for line in lines:
		sampledCompRatios.append(float(line.strip()))


stationaryTuples = []

with open(stationaryPointsFilepath, 'r') as f:
	csvreader = csv.reader(f, skipinitialspace=False, delimiter=',')
	
	for row in csvreader:
		#print(row)
		ratio = float(row[0])
		#print(ratio)
		eb = float(row[1])
		
		if (ratio > 0.0) and (eb > 0.0):
			stationaryTuples.append((ratio, eb))


#sort for interpolation
stationaryTuples.sort(key=lambda x: (x[0], x[1]))

# corner case check for more than one same compression ratio present in case
# python interpolation API needs to provide distinct x-axis (Compression Ratio)
prevCompRatio = 0.0
it = 0

for cur in stationaryTuples:
	(compRatio, eb) = cur
	
	if (compRatio - prevCompRatio < EPSILON) : # same compression ratio present
		compRatio = prevCompRatio + INC # give current compression ratio a very little increment so that they are distinct now
	
	stationaryTuples[it] = (compRatio, eb)
	prevCompRatio = compRatio
	it += 1

# fillup x-axis and corresponding y-axis values
xAxisValues = []
yAxisValues = []
for cur in stationaryTuples:
	(compRatio, eb) = cur
	xAxisValues.append(compRatio)
	yAxisValues.append(eb)
	

# check finally if the x-axis is strictly increasing
prevCompRatio = 0.0
for compRatio in xAxisValues:
	if (compRatio < prevCompRatio):
		sys.exit("x-axis should be strictly increasing")
	prevCompRatio = compRatio



# Now interpolate the required compression ratio to get estimated error bounds
interpolant = interp1d(xAxisValues, yAxisValues) # provide stationary points
interpolatedErrorBounds = interpolant(sampledCompRatios)


# write these interpolated results in a csv file for later usage
os.system("mkdir Output")
outputFilepath = "Output/interpolated_results.csv"

with open(outputFilepath, 'w') as f:
	csvwriter = csv.writer(f)
	it = 0
	
	for cRatio in sampledCompRatios:
		csvwriter.writerow([cRatio, interpolatedErrorBounds[it]])
		it += 1



print("===== done =====")
