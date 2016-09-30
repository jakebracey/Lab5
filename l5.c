/*
 ============================================================================
 Name        : L4.c
 Author      : Jacob Bracey
 Description : ECE 3220: Lab 4
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <io.h>
#include <conio.h>
#include <string.h>

int isNumeric (const char * s);
int *load_array(char* file, int* length, int* max_val);
void do_offset(int* array, int* length, char*file, double val);
void do_scale(int* array, int* length, char*file,double val);
double getmean(int[], int*);
int getmax(int[], int*);
void write_stats(char[], int[], int*);
void do_center(int[], int*, char*);
void do_normal(int[], int*, char*);

int main(int argc, char *argv[]) {
	int file_sel=0;
	int* length = malloc(sizeof(int));
	int* max_val = malloc(sizeof(int));
	
	//defines all of our file name strings
	char file_name[66];
	char stat_file[73];
	char offset_file[69];
	char scaled_file[69];
	char center_file[72];
	char normal_file[73];
	char temp_file_name[16];
	
	int i=1;
	int n_flag=0;
	int o_flag=0;
	int s_flag=0;
	int S_flag=0;
	int C_flag=0;
	int N_flag=0;
	int r_flag=0;
	int h_flag=0;
	double offset_val,scale_val;
	char* ptr_1;
	char* new_name;
	int scale_result=0;
	int offset_result=0;
	
	
	while(i<argc){
		if(argv[i][0]=='-' && argv[i][1]=='n'){
			if(isNumeric(argv[i+1])==0){
				n_flag=-1;
				goto print_outs;
			}
			else{
				file_sel=atoi(argv[i+1]);
				n_flag=1;
				i++;
			}
		}
		else if(argv[i][0]=='-' && argv[i][1]=='o'){
				if(isNumeric(argv[i+1])==0){
				o_flag=-1;
				}
				else{
				offset_val=strtod(argv[i+1],&ptr_1);
				i++;
				o_flag=1;
				}
			
		}
		else if(argv[i][0]=='-' && argv[i][1]=='s'){
				if(isNumeric(argv[i+1])==0){
				s_flag=-1;
				}
				else{
				scale_val=strtod(argv[i+1],&ptr_1);
				i++;
				s_flag=1;
				}
			
		}
		else if(argv[i][0]=='-' && argv[i][1]=='S'){
				S_flag=1;	
		}
		else if(argv[i][0]=='-' && argv[i][1]=='C'){
				C_flag=1;				
		}
		else if(argv[i][0]=='-' && argv[i][1]=='N'){
				N_flag=1;	
		}
		else if(argv[i][0]=='-' && argv[i][1]=='r'){
				i++;
				if(i<argc){
				
				new_name=malloc(sizeof(argv[i]+4));
				sprintf(new_name,"%s",argv[i]);
				r_flag=1;
				}
				else
					r_flag=-1;
		}
		else if(argv[i][0]=='-' && argv[i][1]=='h'){
				h_flag=1;
				printf("Thankyou for asking for help\n\nYou have the following options to put in your command line:\n-n <File Number>	Selects the file number which you would like to open\n-o <Offset Value>	Lets you instruct the program to offset the data and lets you offset it by the value you choose\n-s <Scale Factor>	Lets you instruct the program to scale the data and lets you scale it by the value you choose\n-S	Lets you instruct the program to get the statistics from the data\n-C	Lets you instruct the program to Center the data points\n-N	Lets you instruct the program to Normalize the data points\n-r <NewName>	Lets you create a copy of the input file you select and then formats the output files to this name\n-h	Lets you display the help menu\n\nThe following are example calls\n./a.exe -f 2 -o 43 -S\n./a.exe -f 2 -s -2.5 -C\n./a.exe -f 11 -r NewName1 -N\n\nSince you obviously needed help we are terminating the program with no output\n");
				return 0;
		}
	i++;
	}
	printf("file_sel%d\n",file_sel);
	printf("offset_val%lf\n",offset_val);
	//printf("new_file_name%s",new_file_name);
	printf("scale_val%lf\n",scale_val);
	printf("n:%d o:%d s:%d S:%d C:%d N:%d r:%d",n_flag,o_flag,s_flag,S_flag,C_flag,N_flag,r_flag);
	
	//defines strings for our data 
	int* array;
	double* array_changed;
	
	if(n_flag!=1){
		goto print_outs;
	}
	
	if(r_flag!=1){
	//this is run if no filename was selected to be renamed
	//actually creates the file names to be used
		if (file_sel < 10) {
		sprintf(file_name, "Raw_data_0%d.txt", file_sel);
		sprintf(stat_file, "Statistics_data_0%d.txt", file_sel);
		sprintf(offset_file, "Offset_data_0%d.txt", file_sel);
		sprintf(scaled_file, "Scaled_data_0%d.txt", file_sel);
		sprintf(center_file, "Centered_data_0%d.txt", file_sel);
		sprintf(normal_file, "Normalized_data_0%d.txt", file_sel);
		} 
		else {
		sprintf(file_name, "Raw_data_%d.txt", file_sel);
		sprintf(stat_file, "Statistics_data_%d.txt", file_sel);
		sprintf(offset_file, "Offset_data_%d.txt", file_sel);
		sprintf(scaled_file, "Scaled_data_%d.txt", file_sel);
		sprintf(center_file, "Centered_data_%d.txt", file_sel);
		sprintf(normal_file, "Normalized_data_%d.txt", file_sel);
		}
	}
	else{
	//this is run if a new filename was selected	
		if (file_sel < 10)
		sprintf(temp_file_name, "Raw_data_0%d.txt", file_sel);
		else
		sprintf(temp_file_name, "Raw_data_%d.txt", file_sel);
		
		sprintf(file_name, "%s.txt", new_name);
		sprintf(stat_file, "%s_Statistics.txt", new_name);
		sprintf(offset_file, "%s_Offset.txt", new_name);
		sprintf(scaled_file, "%s_Scaled.txt", new_name);
		sprintf(center_file, "%s_Centered.txt", new_name);
		sprintf(normal_file, "%s_Normal.txt", new_name);
	
	FILE* fpd = fopen(file_name, "w");
	//opens the given output file for writing
	if (fpd == NULL) //making sure the output file exists
		freopen(file_name, "w", fpd);
	
	FILE* fps = fopen(temp_file_name, "r");
	//opens the given input file for reading
	if (fps == NULL) //making sure the input file exists
		freopen(file_name, "w", fps);
	
	char ch;
	while((ch=getc(fps))!=EOF)
	putc(ch,fpd);
	fclose(fpd);
	fclose(fps);
	
	}//end of else for the rename part

	//loads the array from the input file
	array = load_array(file_name, length, max_val);

	
	if (o_flag == 1) {//offset function is called if they enter -o as an argument
		do_offset(array, length, offset_file,offset_val);
	} 
	if(s_flag == 1) {//scale function is called if they enter -s as an argument
		do_scale(array, length, scaled_file,scale_val);
	}

	if(S_flag==1){
	write_stats(stat_file, array, length); //writes stats to file, inside this function the average and max functions are called
	}
	
	if(C_flag==1){
	do_center(array, length, center_file); //writes centered values to file
	}
	
	if(N_flag==1){
	do_normal(array, length, normal_file); //writes normalized values to file
	}
print_outs:	
	//frees the memory we allocated for our strings. 
	free(array);
	if(n_flag!=1)
		printf("\nProgram did not receive a a file number to read from\n");
	if(o_flag==-1)
		printf("\nProgram did not receive a valid offset value from the user\nProgram was not able to perform this function\n");
	if(s_flag==-1)
		printf("\nProgram did not receive a valid scale value from the user\nProgram was not able to perform this function\n");
	if(r_flag==-1)
		printf("\nProgram could not find a new name to name the files\nAll other commands were executed using the default file name\n");
	if(r_flag==-1||s_flag==-1||o_flag==-1||n_flag!=1){
	printf("\n\n\nSince you had some trouble with the formatting we thought you might like some help\n\nYou have the following options to put in your command line:\n-n <File Number>	Selects the file number which you would like to open\n-o <Offset Value>	Lets you instruct the program to offset the data and lets you offset it by the value you choose\n-s <Scale Factor>	Lets you instruct the program to scale the data and lets you scale it by the value you choose\n-S	Lets you instruct the program to get the statistics from the data\n-C	Lets you instruct the program to Center the data points\n-N	Lets you instruct the program to Normalize the data points\n-r <NewName>	Lets you create a copy of the input file you select and then formats the output files to this name\n-h	Lets you display the help menu\n\nThe following are example calls\n./a.exe -f 2 -o 43 -S\n./a.exe -f 2 -s -2.5 -C\n./a.exe -f 11 -r NewName1 -N\n\nEven though you had some incorrect formatting we executed as many commands as possible\n");	
	}
	

} //end of main



void do_offset(int* array, int* length, char* file,double val) {
	//function to offset all of the data points
	double* array_changed = malloc(sizeof(double) * *length);//mallocs a string to put our altered values in
	int i;
	for (i = 0; i < *length; i++) {//goes through every term in the array
		*(array_changed + i) = (double) (*(array + i) + val);//adds the value to every term
	}

	//this part of the function writes the output to the given output file
	FILE* fp = fopen(file, "w");
	//opens the given input file for reading

	if (fp == NULL) //making sure the input file exists
	{
		freopen(file, "w", fp);
	}
	fprintf(fp, "%d %.4lf\n", *length, val);
	for (i = 0; i < *length; i++) {
		fprintf(fp, "%.4lf\n", *(array_changed + i));
	}
	fclose(fp);
	return;
}

 void do_scale(int* array, int* length, char* file,double val) {
	//funciton to scale all of the input data points
	double* array_changed = malloc(sizeof(double) * *length);//mallocs a string to put our altered values in
	int i;
	for (i = 0; i < *length; i++) {
		*(array_changed + i) = (double) (*(array + i) * val);
	}

	//this part of the function writes the output to the given output file
	FILE* fp = fopen(file, "w");
	//opens the given input file for write

	if (fp == NULL) //making sure the input file exists
	{
		freopen(file, "w", fp);
	}
	fprintf(fp, "%d %.4lf\n", *length, val);
	for (i = 0; i < *length; i++) {
		fprintf(fp, "%.4lf\n", *(array_changed + i));
	}
	fclose(fp);
	
	return;
}

void do_center(int array[], int* length, char* file)
//function to center all of the data points and then output those points to an output file
{
	int i;
	double val, mean;
	double* array_changed = malloc(sizeof(double)* *length); //mallocs space for array
	mean= getmean(array, length); //gets average value for array
	for(i=0; i<*length; i++) //shifts values in array to make mean zero, applying changes to seperate array
	{
		*(array_changed + i)=(double)(*(array+i) - mean);
	}

	FILE* fp = fopen(file, "w"); //opens file for write
	if (fp==NULL) //creates new file if one doesn't exist yet
	{
		freopen(file, "w", fp);
	}
	fprintf(fp, "%d %.4f\n", *length, mean);
	for(i = 0; i<*length; i++) //prints new array values to file
	{
		fprintf(fp, "%.4f\n", *(array_changed+i));
	}
	free(array_changed); //frees memory
	fclose(fp); //closes file
	return;
}

void do_normal(int array[], int* length, char* file){
	//funciton to normalize the array and then put those values in an output file
	int i, max;
	double val;
	double* array_changed = malloc(sizeof(double)* *length); //mallocs space for array
	max= getmax(array, length); //gets max value for array
	for(i=0; i<*length; i++) //scales values in array to make max 1, applying changes to seperate array
	{
		*(array_changed + i)=(double)(*(array+i) / (double)max);
	}

	FILE* fp = fopen(file, "w"); //opens file for write
	if (fp==NULL) //creates new file if one doesn't exist yet
	{
		freopen(file, "w", fp);
	}
	fprintf(fp, "%d %.4f\n", *length, (double)(1/(double)max));
	for(i = 0; i<*length; i++) //prints new array values to file
	{
		fprintf(fp, "%.4f\n", *(array_changed+i));
	}
	free(array_changed); //frees memory
	fclose(fp); //closes file
	return;
}

int *load_array(char* file, int* length, int* max_val) {
	//function to load the data from the input file in the array
	FILE* fp = fopen(file, "r");
	//opens the given input file for reading

	if (fp == NULL) { //making sure the input file opens correctly
		printf("Error opening input file");
		return 0;
		//terminates program
	}

	fscanf(fp, "%d %d", length, max_val);

	int* array = malloc(sizeof(int) * *length);
	//mallocs space for the array

	int i = 0;
	for (i = 0; i < *length; i++) {
		//for loop to put the values into the array
		fscanf(fp, "%d", (array + i));
	}
	fclose(fp);
	return array;
}

double getmean(int array[], int *length) {
	//function to find the average of all the data points in the input file
	int i = 0;
	int add = 0;
	double mean = 0;
	for (i = 0; i < *length; i++) //adds the values in array
			{
		add += array[i];
	}
	mean = (double) add / *length; //devides by # of values added
	return mean; //returns average
}

int getmax(int array[], int *length) {
	//funciton to find the maximum value in the data set
	int i = 0;
	int max = 0;
	for (i = 0; i < *length; i++) //finds max value in array
			{
		if (array[i] > max)
			max = array[i];
	}
	return max;
}

void write_stats(char* file, int array[], int* length) {
	//function to write the average and maximum value in a output file
	FILE* fp = fopen(file, "w");
	//opens the given input file for reading

	if (fp == NULL) //making sure the input file exists
	{
		freopen(file, "w", fp);
	}
	//calls the mean and max function all in the fprintf function
	fprintf(fp, "%.02f %d", getmean(array, length), getmax(array, length)); //writes to file
	fclose(fp);
	return;
}
int isNumeric (const char * s)
{
    if (s == NULL || *s == '\0' || isspace(*s))
      return 0;
    char * p;
    strtod (s, &p);
    return *p == '\0';
}