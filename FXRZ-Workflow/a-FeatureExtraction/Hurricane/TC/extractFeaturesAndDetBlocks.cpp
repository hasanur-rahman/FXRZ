#include <fstream>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <ctime>
#include <cstdlib>
#include <random>
#include <math.h>
#include <algorithm>
#include <map>
#include <cstring>
#include <iomanip>
#include <sys/time.h> 


#define BLOCK_SIZE 4
#define SAMPLING_OFFSET 4
#define  NUM_CELL_THRESHOLD 0.75


float* readDataset(const char *data_path, const int num_dims, const int *dims, int *num_entries_read) {
	int num_data_entries = 1;
	for (int i = 0; i < num_dims; i++) num_data_entries *= dims[i];

	float *data_buffer = (float* ) malloc(sizeof(float) * (num_data_entries));
	
	FILE *fp = fopen(data_path, "rb");
	
	if (fp) {
		printf("%s file opened\n", data_path);
	}	
	else{
		printf("Error in opening the file");
		exit(0);
	}

	
	int ret = fread(data_buffer, sizeof(float), num_data_entries, fp);
		
	if (ret > 0) {
  	printf("Read data set composed of %d entries.\n", ret);
		*num_entries_read = ret;
		return data_buffer;
	}
	else {
		*num_entries_read = 0;
		return NULL;
	}

}




struct timeval cost_start; /*only used for recording the cost*/
double curExecutionTime = 0;
	

void costStart()
{
	curExecutionTime = 0;
	gettimeofday(&cost_start, NULL);
}

void costEnd()
{
	double elapsed;
	struct timeval cost_end;
	gettimeofday(&cost_end, NULL);
	elapsed = ((cost_end.tv_sec*1000000+cost_end.tv_usec)-(cost_start.tv_sec*1000000+cost_start.tv_usec))/1000000.0;
	curExecutionTime += elapsed;
}







double getSplinePrediction(double value1, double value2, double value3, double value4)
{
        return -(1.0 / 16.0) * value1 + (9.0 / 16.0) * value2 + (9.0 / 16.0) * value3 - (1.0 / 16.0) * value4;
}


double changeScale(double old_val, double old_min, double old_max, double new_min, double new_max) {
	double old_range = old_max - old_min;
	double new_range = new_max - new_min;
	double ratio = (old_val - old_min) / old_range;
	return (ratio * new_range + new_min);
}


int determineCurrentBlock(const float *data, const int* block_starts, const int num_dims, const int* dims, float closeness_threshold)
{
	
	if (num_dims != 3) {
		printf("not 3 dimensional!");
		exit(1);
	}
	
	
	int min_cell_req = (int)(NUM_CELL_THRESHOLD * (BLOCK_SIZE * BLOCK_SIZE * BLOCK_SIZE));
	int num_cells = 0; 	

	/// Start CHANGE3.0
	size_t XYZ = dims[0]*dims[1]*dims[2];
	size_t XY = dims[1]*dims[2];
	size_t X = dims[2];
	/// End CHANGE3.0
	
	//int *indices = (int* )malloc(sizeof(int) * num_dims);
	

	float min_val, max_val;
	int is_first = 1;

	//std::vector<int>flatten_indices;

	for (int i = block_starts[0]; i < dims[0] && i < block_starts[0] + BLOCK_SIZE; i++) {
		for (int j = block_starts[1]; j < dims[1] && j < block_starts[1] + BLOCK_SIZE; j++) {			
			for (int k = block_starts[2]; k < dims[2] && k < block_starts[2] + BLOCK_SIZE; k++) {
				//indices[0] = i, indices[1] = j, indices[2] = k;
				//int flatten_index = get_flatten_index((const int *)indices, num_dims, dims);
				int I = i*XY+j*X+k;
				float curVal = data[I];
				if (is_first) {min_val = curVal; max_val = curVal; is_first = 0;}
				if(min_val > curVal) min_val = curVal;
				if(max_val < curVal) max_val = curVal;
				num_cells++;	
				//flatten_indices.push_back(flatten_index);
			}
		}			
	}
	
	if((num_cells < min_cell_req) || (max_val - min_val > closeness_threshold)){
				
		//free(indices);
		//std::vector<int>().swap(flatten_indices);
		return 1;
	}
	
	//free(indices);	
	//std::vector<int>().swap(flatten_indices);
	return 0;
}



double computeNonconstantBlockPercentage(const float *data, const int num_entries, const int num_dims, const int* dims, 
		float closeness_threshold)
{
	
	if (num_dims != 3) {
		printf("not 3 dimensional!");
		exit(1);
	}	
	int* starts = (int* ) malloc(sizeof(int) * num_dims);	

	
	int num_constant_blocks = 0;
	int num_non_constant_blocks = 0;
	int num_blocks = 0;
	for (int i = 0; i < dims[0]; i += SAMPLING_OFFSET * BLOCK_SIZE) {
		for (int j = 0; j < dims[1]; j += SAMPLING_OFFSET * BLOCK_SIZE) {
			for (int k = 0; k < dims[2]; k += SAMPLING_OFFSET * BLOCK_SIZE) {
				starts[0] = i, starts[1] = j, starts[2] = k;
				num_blocks++;
				int status = determineCurrentBlock(data, starts, num_dims, dims, closeness_threshold);
				if (status == 0) num_constant_blocks++;
				if (status == 1) num_non_constant_blocks++;
			}
		}
	}

	free(starts);
	return (1.0 * num_non_constant_blocks) / num_blocks;	
}


// gradient features
std::vector <double> getExtraFeatures (const float *data, const int num_entries, const int num_dims, const int* dims)
{
	std::vector<double>features;	

	
	// gradient
	double sum_dif_gradients = 0.0;
	double max_gradient = 0.0;
	double min_gradient = 0.0;
	int is_calc_first = 1;
	int num_sampled_gradients = 0;

	
	// Start CHANGE3.0
	size_t XYZ = dims[0]*dims[1]*dims[2];
	size_t XY = dims[1]*dims[2];
	size_t X = dims[2];	
	// End CHANGE3.0 	

		
	for (int i = 0; i < dims[0]; i += SAMPLING_OFFSET) {
		for (int j = 0; j < dims[1]; j += SAMPLING_OFFSET ) {
			for (int k = 0; k < dims[2]; k += SAMPLING_OFFSET ) {
				
				size_t I = i*XY+j*X+k; // CHANGE3.0
				double cur_data_val = (double) data[I]; // CHANGE3.0
				
								
				
	
				// mean_dif_neighbors feature
				double sum_gradients = 0.0;
				int num_adj_neighbors = 0;
				
				if (num_dims != 3) {
					printf("Number of dimensions should be 3\n");
					exit(1);
				}

				if (I-1 >= 0 && I-1 < XYZ) {sum_gradients += (double)data[I-1]; num_adj_neighbors++;}
				if (I-X >= 0 && I-X < XYZ) {sum_gradients += (double)data[I-X]; num_adj_neighbors++;}
				if (I-XY >= 0 && I-XY < XYZ) {sum_gradients += (double)data[I-XY]; num_adj_neighbors++;}				

				

				// neighbors
				if(num_adj_neighbors > 0) sum_gradients /= num_adj_neighbors; 
				if(num_adj_neighbors > 0) { 
					sum_dif_gradients += (cur_data_val - sum_gradients); 
					num_sampled_gradients++; 
					if (is_calc_first) {max_gradient = (cur_data_val - sum_gradients); min_gradient = (cur_data_val - sum_gradients);} 
					if(max_gradient < (cur_data_val - sum_gradients)) max_gradient = (cur_data_val - sum_gradients);
					if(min_gradient > (cur_data_val - sum_gradients)) min_gradient = (cur_data_val - sum_gradients);
					is_calc_first = 0;
				}

			}
		}
	}

	features.push_back(sum_dif_gradients / (1.0 * num_sampled_gradients));
	features.push_back(min_gradient);
	features.push_back(max_gradient);

	return features;


}	
					





	
std::vector<double>sampled_data_values;
// extract 5 features we used for our Random Forest Model
std::vector <double> getFeatures (const float *data, const int num_entries, const int num_dims, const int* dims, double &data_min, double &data_max, int &sampled_data_points)
{
	std::vector<double>features;	

	// Start CHANGE3.0
	size_t XYZ = dims[0]*dims[1]*dims[2];
	size_t XY = dims[1]*dims[2];
	size_t X = dims[2];	
	// End CHANGE3.0 	

	
	int num_sampled_entries = 0;
	int num_sampled_lorenzo_entries = 0;
	double avg_value  = 0.0;
	double min_val = (double)data[0], max_val = (double)data[0];	
	
	// neighbor
	double sum_dif_neighbors = 0.0;
	int num_sampled_neighbors = 0;


	// sum dif spline
	double sum_dif_splines = 0.0;	
	int num_valid_splines = 0;

	// mean dif lorenzo
	double sum_dif_lorenzos = 0.0;
	int num_valid_lorenzos = 0;

	sampled_data_points = 0;
	
	
	for (int i = 0; i < dims[0]; i += SAMPLING_OFFSET) {
		for (int j = 0; j < dims[1]; j += SAMPLING_OFFSET ) {
			for (int k = 0; k < dims[2]; k += SAMPLING_OFFSET ) {
				sampled_data_points++;				
					
				size_t I = i*XY+j*X+k; // CHANGE3.0	
				double cur_data_val = (double) data[I]; // CHANGE3.0
				
				sampled_data_values.push_back(cur_data_val);
	
				// avg_val feature
				avg_value += cur_data_val;
				num_sampled_entries++;		

				// val_range feature
				if(min_val > cur_data_val) min_val = cur_data_val;
				if(max_val < cur_data_val) max_val = cur_data_val;

				// mean_dif_neighbors feature
				
				// lorenzo
								
				if (num_dims != 3) {
					printf("Number of dimensions should be 3\n");
					exit(1);
				}	
				
				//printf("hoiche 1\n");
				// MND
				double sum_neighbors = 0.0;
				int num_adj_neighbors = 0;
				
				// MLD
				int cur_lorenzo_neighbors = 0;	
				double cur_lorenzo_sum = 0.0;				


				if (I-1 >= 0 && I-1 < XYZ) {sum_neighbors += (double)data[I-1]; num_adj_neighbors++; cur_lorenzo_sum += (double)data[I-1]; cur_lorenzo_neighbors++;}
				if (I-X >= 0 && I-X < XYZ) {sum_neighbors += (double)data[I-X]; num_adj_neighbors++; cur_lorenzo_sum += (double)data[I-X]; cur_lorenzo_neighbors++;}
				if (I-XY >= 0 && I-XY < XYZ) {sum_neighbors += (double)data[I-XY]; num_adj_neighbors++; cur_lorenzo_sum += (double)data[I-XY]; cur_lorenzo_neighbors++;}
				if (I+1 < XYZ) {sum_neighbors += (double)data[I+1]; num_adj_neighbors++;}	
				if (I+X < XYZ) {sum_neighbors += (double)data[I+X]; num_adj_neighbors++;}
				if (I+XY < XYZ) {sum_neighbors += (double)data[I+XY]; num_adj_neighbors++;}
				if (I-XY-1 >= 0 && I-XY-1 < XYZ) {cur_lorenzo_sum -= (double)data[I-XY-1]; cur_lorenzo_neighbors++;}
				if (I-X-1 >= 0 && I-X-1 < XYZ) {cur_lorenzo_sum -= (double)data[I-X-1]; cur_lorenzo_neighbors++;}
				if (I-XY-X >= 0 && I-XY-X < XYZ) {cur_lorenzo_sum -= (double)data[I-XY-X]; cur_lorenzo_neighbors++;}
				if (I-XY-X-1 >= 0 && I-XY-X-1 < XYZ) {cur_lorenzo_sum += (double)data[I-XY-X-1]; cur_lorenzo_neighbors++;}
				
				//printf("hoiche 2\n");				
				// MSD
				if (I+3 < XYZ && I-3 >= 0 && I-3 < XYZ) {
					double spline_value1 = getSplinePrediction((double)data[I-3], (double)data[I-1], 
						(double)data[I+1], (double)data[I+3]);
					sum_dif_splines += fabs(cur_data_val - spline_value1);
                                        num_valid_splines ++;	
				}

				if (I+3*X < XYZ && I-3*X >= 0 && I-3*X < XYZ) {
					double spline_value2 = getSplinePrediction((double)data[I-3*X], (double)data[I-X], 
						(double)data[I+X], (double)data[I+3*X]);
					sum_dif_splines += fabs(cur_data_val - spline_value2);
                                        num_valid_splines ++;

				}

				if (I+3*XY < XYZ && I-3*XY >= 0 && I-3*XY < XYZ) {
					double spline_value3 = getSplinePrediction((double)data[I-3*XY], (double)data[I-XY], 
						(double)data[I+XY], (double)data[I+3*XY]);
					sum_dif_splines += fabs(cur_data_val - spline_value3);
					num_valid_splines ++;
				}
				
				

				if (cur_lorenzo_neighbors == (1<<num_dims) - 1) {
					cur_lorenzo_sum /= ((1<<num_dims) - 1);	
					sum_dif_lorenzos += fabs( cur_lorenzo_sum - cur_data_val);
					num_valid_lorenzos++;	 	
				}
			

				

				// neighbors
				if(num_adj_neighbors > 0) sum_neighbors /= num_adj_neighbors; 
				if(num_adj_neighbors > 0) { sum_dif_neighbors += fabs(sum_neighbors - cur_data_val); num_sampled_neighbors++; }
				

			
			}
		}
	}
	
	avg_value /= num_sampled_entries;
		

	features.push_back(max_val - min_val); 
	features.push_back(avg_value);
	features.push_back(sum_dif_neighbors / (1.0 * num_sampled_neighbors));
	features.push_back(sum_dif_lorenzos / (1.0 * num_valid_lorenzos));	
	features.push_back(sum_dif_splines / ( 1.0 * num_valid_splines) );	
	//costEnd();

	
	data_min = min_val;
	data_max = max_val; 

	
	return features;
}


	

std::vector<std::string> splitByDelimiters(char* str, const char* delimiters)
{
	char *ptr = std::strtok(str, delimiters);
	std::vector<std::string>words;

	while (ptr != NULL) {
		words.push_back(ptr);
 	        ptr = std::strtok(NULL, delimiters);		
	}	

	return words;
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    	

int extractFeaturesAndDetNonconstantBlocks(const char* app_name, const char* datafile_name, 
		const int num_dims, 
		const int *dims )

{
	
	printf("\n\n\n ================  %s start ================\n", __func__);
	printf("datafile name %s\n", datafile_name);

	printf("dimension-values: %d\n", num_dims);
	for (int i = 0; i < num_dims; i++) printf("%d ", dims[i]);
	printf("\n");
	
	int ret_num_entries =  0;
	
	float* data = readDataset( datafile_name, num_dims, dims, &ret_num_entries);

	printf("num of data entries %d\n", ret_num_entries);

	
	if(data == NULL){
		printf("ERROR in %s!! You should provide a dataset filepath\n", __func__);
		return 1;
	}
	
	double data_min = 0.0, data_max = 0.0;
	double value_sum = 0.0;

	
	double total_time = 0.0, feature_extract_time = 0.0, avg_val_recalc_time = 0.0, non_constant_calc_time = 0.0;
	int sampled_data_points = 0;
	sampled_data_values.clear();	

	costStart();
	// get the features 	
	std::vector<double> features = getFeatures ( (const float *)data, (const int) ret_num_entries, num_dims, dims, data_min, data_max, sampled_data_points);
	costEnd();
	total_time += curExecutionTime;
	feature_extract_time = curExecutionTime;

	

	printf("Sampled data percentage: %.4lf%\n", (100.0 * sampled_data_points) / ret_num_entries);
	
	double val_range = data_max - data_min;

	double new_min = 0.0, new_max = val_range ;	
	int sampled_entries = 0;
	
	costStart();
	for (double val: sampled_data_values) {
		double new_val = changeScale(val, data_min, data_max, new_min, new_max);
		value_sum += new_val;
		sampled_entries++;
	}
	costEnd();
	total_time += curExecutionTime;
	avg_val_recalc_time = curExecutionTime;	

	//double val_ratio = (value_sum / (new_max * sampled_entries));
	
	double avg_val = value_sum / (1.0 * sampled_entries); 	
	features[1] = avg_val;	



	
	// get gradient-based features we omitted in the paper		
//	std::vector<double> extra_features = getExtraFeatures ( (const float *)data, (const int) ret_num_entries, num_dims, dims);
//	extra_features.push_back(val_ratio);

	double closeness_threshold = avg_val * 0.15; 
	
	costStart();
	double non_constant_fraction = computeNonconstantBlockPercentage((const float *)data, ret_num_entries, num_dims, dims, (float)closeness_threshold);
	costEnd();
	total_time += curExecutionTime;
	non_constant_calc_time = curExecutionTime;
	non_constant_fraction = changeScale(non_constant_fraction, 0.0, 1.0, 0.5, 1.0 );

	printf("sampled value range %.8lf\n", new_max - new_min);
	printf("closeness threshold: %.12lf, non constant block fraction: %.12lf\n", 
			closeness_threshold, non_constant_fraction);

	printf("Feature-extraction time: %.8lf\n", feature_extract_time);
	printf("Average value recalculation time %.8lf\n", avg_val_recalc_time);	
	printf("Non-constant block calculation time: %.8lf\n", non_constant_calc_time);
	printf("Total time cost: %8lf\n", total_time);


	/////////////////////
	system("mkdir Output");
	char *filepath = (char* )malloc(500 * sizeof(char));
	sprintf(filepath, "Output/feature_extraction_result.csv");
	std::string csv_cmd = "rm  "; csv_cmd += filepath;
	system(csv_cmd.c_str());

	std::fstream fout_result;
	fout_result.open((const char* )filepath, std::fstream::out | std::fstream::app);
	

	char* name_buffer = (char* )malloc(500 * sizeof(char));
        sprintf(name_buffer, "%s-%s", app_name, datafile_name);
        std::string name_buf(name_buffer);

	fout_result << name_buf << ", ";	
	for (double cur_feature: features) {
		fout_result << cur_feature << ", ";	
	}
	fout_result << non_constant_fraction << "\n";
	fout_result.close();
	
	free(name_buffer);
	free(filepath);
	/////////////////////

	
	

	// free
	free(data);
	
	std::string().swap(name_buf);
	std::vector<double>().swap(features);
	return 0;


}

// ------ TEST START -----

int main(int argc, char *argv[]) {

	// input
	if(argc != 3 ){
		printf("Please provide application-name and dataset-path\n");
		return 1;
	}
	
	
	
	const int num_dims = 3; // change this according to input dataset
	const int dims[] = {500, 500, 100}; // change this according to input dataset

	

	const char* application_name = argv[1];
	const char* datafile_name = argv[2];
	
	
	int ret = extractFeaturesAndDetNonconstantBlocks(application_name, datafile_name, num_dims, dims);
	
	return ret;
}

