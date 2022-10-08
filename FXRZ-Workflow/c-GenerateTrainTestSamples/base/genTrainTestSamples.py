import numpy, math
import os, sys, csv


featuresResultFilepath = str(sys.argv[1])
interpolatedResultFilepath = str(sys.argv[2])

featuresTuple = ("", 0.0, 0.0, 0.0, 0.0, 0.0)
nonConstantBlockRatio = 0.0


with open(featuresResultFilepath, 'r') as f:
	csvreader = csv.reader(f, skipinitialspace=False, delimiter=',')
	
	for row in csvreader:
		datasetName = str(row[0])
		valueRange = float(row[1])
		avgValue = float(row[2])
		mnd = float(row[3])
		mld = float(row[4])
		msd = float(row[5])
		nonConstantBlockRatio = float(row[6])

		featuresTuples = (datasetName, valueRange, avgValue, mnd, mld, msd)

		break


print("Features: " + str(featuresTuples))
print("Non-constant block ratio: " + str(nonConstantBlockRatio))

os.system("mkdir Output")
outputFilepath = "Output/model_data_samples.csv"

outputFile = open(outputFilepath, 'w')
csvwriter = csv.writer(outputFile)

with open(interpolatedResultFilepath, 'r') as f:
	csvreader = csv.reader(f, skipinitialspace=False, delimiter=',')
	
	for row in csvreader:
		compressionRatio = float(row[0])
		interpolatedErrBound = float(row[1])
		
		(datasetName, valueRange, avgValue, mnd, mld, msd) = featuresTuples
		convertedCompressionRatio = compressionRatio * nonConstantBlockRatio
	
		csvwriter.writerow([datasetName, valueRange, avgValue, mnd, mld, msd, convertedCompressionRatio, interpolatedErrBound])


outputFile.close()

print("<<<<<<< Generating Data Samples Done >>>>>>>")
	
