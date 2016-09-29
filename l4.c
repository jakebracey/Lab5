/*
 ============================================================================
 Name        : L4.c
 Author      : Jacob Bracey
 Description : ECE 3220: Lab 4
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
int *load_array(char* file, int* length, int* max_val);
double *do_offset(int* array, int* length, char*file);
double *do_scale(int* array, int* length, char*file);
double getmean(int[], int*);
int getmax(int[], int*);
void write_stats(char[], int[], int*);
void do_center(int[], int*, char*);
void do_normal(int[], int*, char*);

int main(void) {
	int file_sel;
	int* length = malloc(sizeof(int));
	int* max_val = malloc(sizeof(int));
	
	//defines all of our file name strings
	char file_name[16];
	char stat_file[23];
	char offset_file[19];
	char scaled_file[19];
	char center_file[22];
	char normal_file[23];
	
	//defines strings for our data 
	int* array;
	double* array_changed;
	
	//variable to choose which operation to do on the input data
	int off_or_scale;

	//gets file choice from user
	printf("Which file would you like to open:\n");
	scanf("%d", &file_sel);
	if (file_sel < 1 || file_sel > 11) {
		while (file_sel < 1 || file_sel > 11) {
			printf("Available files are files 1-11\nPlease enter a valid file number:\n");
			scanf("%d", &file_sel);
		}
	}

	//actually creates the file names to be used
	if (file_sel < 10) {
		sprintf(file_name, "Raw_data_0%d.txt", file_sel);
		sprintf(stat_file, "Statistics_data_0%d.txt", file_sel);
		sprintf(offset_file, "Offset_data_0%d.txt", file_sel);
		sprintf(scaled_file, "Scaled_data_0%d.txt", file_sel);
		sprintf(center_file, "Centered_data_0%d.txt", file_sel);
		sprintf(normal_file, "Normalized_data_0%d.txt", file_sel);
	} else {
		sprintf(file_name, "Raw_data_%d.txt", file_sel);
		sprintf(stat_file, "Statistics_data_%d.txt", file_sel);
		sprintf(offset_file, "Offset_data_%d.txt", file_sel);
		sprintf(scaled_file, "Scaled_data_%d.txt", file_sel);
		sprintf(center_file, "Centered_data_%d.txt", file_sel);
		sprintf(normal_file, "Normalized_data_%d.txt", file_sel);
	}

	//loads the array from the input file
	array = load_array(file_name, length, max_val);

	//part of code to either offset or scale
	printf("Enter if you would like to\n(1) Offset the original signal\n(2) Scale the original signal\n");
	scanf("%d", &off_or_scale);
	if (off_or_scale < 1 || off_or_scale > 2) {
		while (off_or_scale < 1 || off_or_scale > 2) {
			printf("Enter if you would like to\n(1) Offset the original signal\n(2) Scale the original signal\n");
			scanf("%d", &off_or_scale);
		}
	}
	if (off_or_scale == 1) {//if they select 1 then the offset function is called
		array_changed = do_offset(array, length, offset_file);
	} else {//if they select 2 then the scale function is called
		array_changed = do_scale(array, length, scaled_file);
	}

	write_stats(stat_file, array, length); //writes stats to file, inside this function the average and max functions are called
	do_center(array, length, center_file); //writes centered values to file
	do_normal(array, length, normal_file); //writes normalized values to file

	//frees the memory we allocated for our strings. 
	free(array);
	free(array_changed);

} //end of main

double *do_offset(int* array, int* length, char* file) {
	//function to offset all of the data points
	double val;
	double* array_changed = malloc(sizeof(double) * *length);//mallocs a string to put our altered values in
	printf("Enter the factor that you would like to offset the data samples by:\n");
	scanf("%lf", &val);
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
	return array_changed;
}

double *do_scale(int* array, int* length, char* file) {
	//funciton to scale all of the input data points
	double val;
	double* array_changed = malloc(sizeof(double) * *length);//mallocs a string to put our altered values in
	printf("Enter the factor that you would like to scale the data samples by:\n");
	scanf("%lf", &val);
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
	return array_changed;
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
