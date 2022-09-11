/*
 * Abigail Solomon
 * CS 4375.003
 * Assignment_1
 *  
 *  This program will do data exploration with statistical  functions 
    on boston.csv file with C++. 
 *    
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdio.h>
#include <numeric>
#include <algorithm>

using namespace std;


// Prototypes- Declaration of functions  

double sum(vector <double>);
double mean(vector <double> );
double median(vector <double> );
double range(vector <double> );
double covariance(vector<double>, vector<double>);
double correlation(vector<double>, vector<double>);


// Main Function

int main(int argc, char** argv) {

	ifstream inFS;
	string line;
	string rm_in, medv_in;
	const int MAX_LEN = 1000;
	vector<double> rm(MAX_LEN);
	vector<double> medv(MAX_LEN);


	//Try to open file
	cout << " Opening the file Boston.csv." << endl;

	inFS.open("Boston.csv");
	if (!inFS.is_open()) {
		cout << "Could not open file Boston.csv." << endl;
		return 1;
	}

	//can now use inFS stream like cin stream
	// Boston.csv should contain two doubles

	cout << " Reading line l" << endl;
	getline(inFS, line);

	//echo heading
	cout << " heading: " << line << endl;

	int numObservations = 0;
	while (inFS.good()) {

		getline(inFS, rm_in, ',');
		getline(inFS, medv_in, '\n');

		rm.at(numObservations) = stof(rm_in);
		medv.at(numObservations) = stof(medv_in);

		numObservations++;
	}

	rm.resize(numObservations);
	medv.resize(numObservations);

	cout << " The new length is " << rm.size() << endl;

	cout << " Closing file Boston.csv." << endl;
	//Done with file,so close it
	inFS.close();

	cout << " Number of records: " << numObservations << endl;
	

	// Calling the statistical  functions for rm

	cout << "\n	 Stats for rm:" << endl << endl;
	cout << " The sum of the rm is: " << sum(rm) << endl;
	cout << " The mean of the rm is: " << mean(rm) << endl;
	cout << " The median of the rm is: " << median(rm) << endl;
	cout << " The range of the rm is: " << range(rm) << endl;

	// Calling the statistical  functions for rm

	cout << "\n	 Stats for medv:" << endl << endl;
	cout << "The sum of the medv is: " << sum(medv) << endl;
	cout << "The mean of the medv is: " << mean(medv) << endl;
	cout << "The median of the medv is: " << median(medv) << endl;
	cout << "The range of the medv is: " << range(medv) << endl << endl;

	// Calling covariance for both rm and medv

	cout << "The covariance of the rm and medv is: " << covariance(rm, medv) << endl;

	// Calling correlation for both rm and medv

	cout << "The correlation of the rm and medv is: " << correlation(rm, medv) << endl;

	cout << " \n Program terminated." << endl;

	return 0;

}

// Definition of functions

//sum function
double sum(vector <double> myVector) {
	double sum = accumulate(myVector.begin(), myVector.end(), 0.00);
	return sum;
}


//mean function
double mean(vector <double> myVector) {
	double sum = accumulate(myVector.begin(), myVector.end(), 0.00);
	double mean = (sum / myVector.size());
	return mean;
}

//median function
double median(vector <double> myVector) {
	sort(myVector.begin(), myVector.end());
	if ((myVector.size() % 2) == 0) {
		return (myVector[myVector.size() / 2] + myVector[(myVector.size() - 1) / 2]) / (double)2.0;
	}
	else {
		return myVector[myVector.size() / 2];
	}
}

//range function
double range(vector<double> myVector) {
	sort(myVector.begin(), myVector.end());
	double range = myVector.back() - myVector.front();
	return range;
}

//covariance function
double covariance(vector<double> myVector1, vector<double> myVector2) {
	double sum = 0.0;
	for (int i = 0; i < myVector1.size(); i++) {
		sum = sum + (myVector1[i] - mean(myVector1)) * (myVector2[i] - mean(myVector2));

	}
	return sum / (myVector1.size() - 1);
}

//correlation function
double correlation(vector<double> myVector1, vector<double> myVector2) {
	double sumVector1 = 0.0, sumVector2 = 0.0, sum_Vector1Vector2 = 0.0;
	double squareSumVector1 = 0.0, squareSumVector2 = 0.0;

	for (int i = 0; i < myVector1.size(); i++)
	{
		// sum of vector 1 numbers.
		sumVector1 = sumVector1 + myVector1[i];

		// sum of vector 2 numbers.
		sumVector2 = sumVector2 + myVector2[i];

		// sum of vector1[i] * vector2[i].
		sum_Vector1Vector2 = sum_Vector1Vector2 + myVector1[i] * myVector2[i];

		// sum of square of each vector.
		squareSumVector1 = squareSumVector1 + myVector1[i] * myVector1[i];
		squareSumVector2 = squareSumVector2 + myVector2[i] * myVector2[i];
	}

	// formula for correlation coefficient.
	float corr = (float)(myVector1.size() * sum_Vector1Vector2 - sumVector1 * sumVector2) / sqrt((myVector1.size() * squareSumVector1 - sumVector1 * sumVector1) * (myVector1.size() * squareSumVector2 - sumVector2 * sumVector2));

	return corr;
}