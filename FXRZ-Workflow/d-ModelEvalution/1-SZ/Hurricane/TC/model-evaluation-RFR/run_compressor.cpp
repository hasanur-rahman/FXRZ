#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <math.h>



std::vector<std::string> tokenize(std::string s, std::string del = " ")
{
    int start = 0;
    int end = s.find(del);
    std::vector<std::string>elements;

    while (end != -1) {
        //std::cout << s.substr(start, end - start) << std::endl;
        std::string str = s.substr(start, end - start);
        if(str.size() > 0) elements.push_back(str);
        start = end + del.size();
        end = s.find(del, start);
    }
    std::string str = s.substr(start);
    elements.push_back(str);
    //std::cout << s.substr(start) << std::endl;

    return elements;
}

void scan(const char *in_file1_path, 
		const char* out_file_path, const char* file_name) {
	
	char *buffer = (char* )malloc(500 * sizeof(char)) ;
	
	sprintf(buffer, "rm -f %s", out_file_path);
	system((const char*) buffer);

	
	char *command_buffer = (char* )malloc(300 * sizeof(char));
	std::string cur_command;	
	std::fstream csv_fout;
	csv_fout.open(out_file_path, std::fstream::out | std::fstream::app);


	std::fstream csv_fin1, csv_fin2;
	csv_fin1.open(in_file1_path, std::fstream::in);
	
	std::string csv_buffer;
	std::vector<std::string> row;
	std::string val;

	int total_lines = 0;

	//int filtered_count[] = {0, 0};

	char *output_buffer = (char* )malloc(300 * sizeof(char));       
        sprintf(output_buffer, "./sz-compression-info-ori.txt");
        std::string output_string1(output_buffer);
	
	float avg_ratio_dif = 0.0, max_ratio_dif = 0.0;
	float avg_err_dif = 0.0, max_err_dif = 0.0;

	float avgEstimationError = 0.0;
	int numRows = 0;
		
	std::fstream cur_file;
	if (csv_fin1.is_open() && csv_fin1.good()) {
		while(std::getline(csv_fin1, csv_buffer)) {
			row = tokenize(csv_buffer, ",");

//			while(ss >> val) {
//				row.push_back(val);
//				if(ss.peek() == ',' || ss.peek() == ' ') ss.ignore();
//			}
			
			float ori_err = std::stof(row[0]);
			float pred_err = std::stof(row[1]);
			float ori_ratio = std::stof(row[2]);

			float cr1 = ori_ratio, cr2 = 0.0, cr3 = 0.0;	
			printf("ori ratio %.4f, original_error %.4f, pred error %.4f\n", ori_ratio, ori_err, pred_err);
		
			
			// rerun with ori error bound to verify ratio	
			sprintf(command_buffer, "sz -z -f -M REL -R %.10f -i %s -3 500 500 100", ori_err, file_name);
			cur_command = ""; cur_command += command_buffer;
			sprintf(command_buffer, " > %s 2>&1", output_string1.c_str());
			cur_command += command_buffer;
			printf("command1: %s\n", cur_command.c_str());
			system(cur_command.c_str());

			sprintf(command_buffer, "sz -x -f -M REL -R %.10f -s %s.sz -i %s -3 500 500 100 -a", ori_err, file_name, file_name);
			cur_command = ""; cur_command += command_buffer;
			sprintf(command_buffer, " >> %s 2>&1", output_string1.c_str());
			cur_command += command_buffer;
			printf("command2: %s\n", cur_command.c_str());
                        system(cur_command.c_str());

//			sprintf(command_buffer, "zfp -s -i %s -z %s.zfp -o %s.zfp.out -f -a %.10f -3 500 500 100 > %s 2>&1", file_name, file_name, file_name, ori_err, output_string1.c_str());
			cur_command = ""; cur_command += command_buffer;
			system(cur_command.c_str());
			std::fstream cur_file;
                        cur_file.open(output_string1.c_str(), std::ios::in);	
			if(cur_file.is_open()) {
				std::string line;
                                while(getline(cur_file, line)) {
				
					if(line.find("compressionRatio") != std::string::npos){
						int pos = line.find("compressionRatio");
                                                std::string value = "";

                                                while(pos < line.size()){
                                                        if( !((line[pos] >= '0' && line[pos] <= '9') || (line[pos] == '.') )) pos++;
                                                        else break;
                                                }

                                                while(pos < line.size()){
                                                        if( ((line[pos] >= '0' && line[pos] <= '9') || (line[pos] == '.') )) value += line[pos];
                                                        else break;
                                                        pos++;
                                                }

                                                float ret = 0.0;
                                                try{
                                                        ret = std::stof(value);
                                                }
                                                catch(const std::exception& e){

                                                        return ;
                                                }
						cr3 = ret;
							
					}
				}
			}
			cur_file.close();
			

			sprintf(command_buffer, "sz -z -f -M REL -R %.10f -i %s -3 500 500 100", pred_err, file_name);
			cur_command = ""; cur_command += command_buffer;
			sprintf(command_buffer, " > %s 2>&1", output_string1.c_str());
			cur_command += command_buffer;
			printf("command1: %s\n", cur_command.c_str());
			system(cur_command.c_str());

			 sprintf(command_buffer, "sz -x -f -M REL -R %.10f -s %s.sz -i %s -3 500 500 100 -a", pred_err, file_name, file_name);
			cur_command = ""; cur_command += command_buffer;
			sprintf(command_buffer, " >> %s 2>&1", output_string1.c_str());
			cur_command += command_buffer;
			printf("command2: %s\n", cur_command.c_str());
                        system(cur_command.c_str());

//                        sprintf(command_buffer, "zfp -s -i %s -z %s.zfp -o %s.zfp.out -f -a %.10f -3 500 500 100 > %s 2>&1", file_name, file_name, file_name, pred_err, output_string1.c_str());
                        cur_command = ""; cur_command += command_buffer;
                        system(cur_command.c_str());
			cur_file.open(output_string1.c_str(), std::ios::in);	
			if(cur_file.is_open()) {
				std::string line;
                                while(getline(cur_file, line)) {
				
					if(line.find("compressionRatio") != std::string::npos){
						int pos = line.find("compressionRatio");
                                                std::string value = "";

                                                while(pos < line.size()){
                                                        if( !((line[pos] >= '0' && line[pos] <= '9') || (line[pos] == '.') )) pos++;
                                                        else break;
                                                }

                                                while(pos < line.size()){
                                                        if( ((line[pos] >= '0' && line[pos] <= '9') || (line[pos] == '.') )) value += line[pos];
                                                        else break;
                                                        pos++;
                                                }

                                                float ret = 0.0;
                                                try{
                                                        ret = std::stof(value);
                                                }
                                                catch(const std::exception& e){

                                                        return ;
                                                }
						cr2 = ret;
							
					}
				}
			}
			cur_file.close();
		
			float estimationError = (100.0 * fabs(cr1 - cr2)) / cr1;
			avgEstimationError += estimationError;
			numRows++;			
	
			printf("Target compression ratio: %.4f, Real compression ratio from predicted error bound : %.4f\n", cr1, cr2);
			csv_fout << ori_err << ", " << pred_err << ", "  << cr1 << ", " << cr2 << ", " << estimationError <<  "\n";
			 
		}
	}
	
	//printf("SZ count: %d, ZFP count %d\n", filtered_count[0], filtered_count[1]);
	
	printf("Average Estimation Error: %.4f\n", avgEstimationError / numRows);
	system("rm sz-compression-info-ori.txt");
	printf(" total lines: %d\n", total_lines);
	csv_fout.close();
	csv_fin1.close();
	free(buffer);

}

int main(int argc, char *argv[])
{
	srand (time(NULL));
	scan((const char* )argv[1], (const char* )argv[2], (const char* )argv[3]);	
	return 0;

}	
