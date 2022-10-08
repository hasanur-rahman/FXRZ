import os, sys, csv

logfile = str(sys.argv[1])

os.system("mkdir Output")

csvfile = open('Output/fraz_analysis.csv', 'w' )
csvwriter = csv.writer(csvfile)

with open(logfile, 'r') as f:
	lines = [line.strip() for line in f.readlines()]
	
	i = 0
	
	while i < len(lines):
		if "opt:target" in lines[i]:
			first_part = lines[i].split("=")[0] 
			target = float(lines[i].split("=")[1])
			
			print("target ", target)
			
			i = i + 1
			
			err_bound = 0.0
			result = 0.0
			runtime = 0.0


			while i < len(lines):
				if "opt:input <data>" in lines[i]:
					tmp = lines[i].split("[")[1]
					#print("tmp input: ", tmp)
					err_bound = float(tmp.split(",")[0])
					print("err bound ", err_bound)
				if "opt:output <data>" in lines[i]:
					tmp = lines[i].split("[")[1]
					#print("tmp output: ", tmp)
					result = float(tmp.split(",")[0])
					#print("next: ", result)
					print("result: ", result)

				if "time:compress <uint32>" in lines[i]:
					#print("line: ", lines[i])
					#print("sec: ", lines[i].split("=")[1])
					runtime = float(lines[i].split("=")[1])
					runtime = runtime / 1000.0					
					print("runtime ", runtime)					

				if ("opt:target" in lines[i]):
					csvwriter.writerow([target, result, 100.0 * (abs(target - result) / target ), runtime, err_bound])
					i = i - 1
					break
				elif i + 1 == len(lines):
					csvwriter.writerow([target, result, 100.0 * (abs(target - result) / target ), runtime, err_bound])
					
				i = i + 1

		i = i + 1


csvfile.close()
				
