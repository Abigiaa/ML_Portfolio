/*
 * Abigail Solomon
 * CS 4375.003
 * Portfolio: ML from Scratch
 *
 *  This program will perform logistic regression 
    on titanic_project.csv file to predict survived based on sex.
 *
 */
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <tgmath.h>
#include <algorithm>
#include <chrono>

using namespace std;

// Prototypes- Declaration of functions 
vector<vector<double>>  getSigmoid(const vector<vector<double>> & );
vector<vector<double>> multiplyMatrix(const vector<vector<double>>& v1, const vector<vector<double>>&);
vector<vector<double>> transposeMatrix(const vector<vector<double>>);



// Main function
int main()
{
    ifstream     fIn("titanic_project.csv");

    // Try to open file.

    if (!fIn.is_open()) {
        cout << "Could not open file titanic_project.csv." << endl;
        return 1;
    }

    // can now use fIn stream like cin stream
    // titanic_project.csv should contain two two vectors

    cout << " Reading data " << endl << endl;

    string                      input;
    vector<vector<double>>      sex;
    vector<double>              survived;

    getline(fIn, input);

    // Import all the data while the file is open
    while (!fIn.eof())
    {
        //unused data point
        getline(fIn, input, ',');
        //used data points
        getline(fIn, input, ',');
        sex.push_back({ 1, stod(input) });
        getline(fIn, input, ',');
        survived.push_back(stod(input));
        getline(fIn, input);
    }

    double                      learning_rate(0.001);
    const int                   iteration(500);
    vector<vector<double>>      probVector, tProbVector, error, weight({ {1},{1} });


    //Divide the gathered data into 'train' and 'test' set
    vector<vector<double>>      train, test;
    for (int i = 0; i < 800; i++)
        train.push_back(sex[i]);
    for (int i = 800; i < sex.size(); i++)
        test.push_back(sex[i]);

    //Start the timing process of the algorithm
    auto start = chrono::high_resolution_clock::now();

    // Find the optimal coefficient using 'train' set
    for (int count = 0; count < iteration; count++)
    {
        //Generate a vector of sigmoid to estimate the error
        probVector = getSigmoid(multiplyMatrix(train, weight));
        for (int i = 0; i < train.size(); i++)
            error.push_back({ survived[i] - probVector[0][i] });

        // Adjust the weight using the calculated error
        tProbVector = multiplyMatrix(transposeMatrix(train), error);
        for (int row = 0; row < tProbVector.size(); row++)
            for (int col = 0; col < tProbVector[row].size(); col++)
                weight[row][col] += (tProbVector[row][col] * learning_rate);
        error.clear();
    }
    //Stop the timing process and assign the elapsed time
    auto stop = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_time = stop - start;

    // Print the optimal coefficients w0 and w1
    cout << "  w0: " << weight[0][0] << "\tw1: " << weight[1][0] << endl;

    // Predictions on 'test' data set using the calculated coefficients
    vector<vector<double>>      prediction;
    for (int i = 0; i < test.size(); i++)
        prediction.push_back({ (1 / (1 + exp((-1) * (weight[0][0] * test[i][0] + weight[1][0] * test[i][1])))) > 0.5 ? (double)1 : (double)0 });

    //Evaluating the prediction and calculate the metrics
    double     TP(0), TN(0), FP(0), FN(0);
    for (int i = 800; i < sex.size(); i++)
    {
        if (prediction[i - 800][0] == 0 && survived[i] == 0)    TP++;
        if (prediction[i - 800][0] == 0 && survived[i] == 1)    FP++;
        if (prediction[i - 800][0] == 1 && survived[i] == 1)    TN++;
        if (prediction[i - 800][0] == 1 && survived[i] == 0)    FN++;
    }
    cout << endl << "  TP: " << TP << "  " << "   FP: " << FP << endl << "  FN: " << FN << "  " << "    TN: " <<TN << endl << endl;

    // Print the calculated metrics
    cout << "  Accuracy: " << ((TP + TN) / (TP + TN + FP + FN)) << endl;
    cout << "  Specificity: " << TP / (TP + FN) << endl;
    cout << "  Sensitivity: " << TN / (TN + FP) << endl;
    cout << endl << "  Run time of the algorothm:  " << elapsed_time.count() << " sec" << endl;

    return 0;
}


// Definition of functions

//Get Sigmoid vector function
vector<vector<double>>  getSigmoid (const vector<vector<double>> & v)
{
    vector<vector<double>>  sigmoidVector;


    //reSize
    if (v.size () > 0)
    {
        if (v[0].size () > 0)
        {
            sigmoidVector.resize (v[0].size ());
            for (int i = 0; i < sigmoidVector.size (); i++)
                sigmoidVector[i].resize (v.size ());
        }
    }

    // Calculate the sigmoid through out the vector.
    for (int row = 0; row < v.size (); row++)
        for (int col = 0; col < v[row].size (); col++)
            sigmoidVector[col][row] = (1 / (1 + exp (v[row][col] * (-1))));

    return sigmoidVector;
}

// Multiplication of the two vectors 'v1' and 'v2' function
vector<vector<double>> multiplyMatrix (const vector<vector<double>> & v1, const vector<vector<double>> & v2)
{
    vector<vector<double>> multipliedVector;

    //Check size and reSize
    if (v1.size() > 0)
    {
        multipliedVector.resize (v1.size ());
        if (v2.size() > 0)
            if (v2[0].size() > 0)
                for (int i = 0; i < multipliedVector.size(); i++)
                    multipliedVector[i].resize(v2[0].size());

        // First is number of column in v1 is equal to number of rows in v2
        if (v1[0].size () == v2.size ())
        {
            //Mutliplying v1's row  by v2's column 
            for (int row = 0; row < v1.size (); row++)
                for (int col2 = 0; col2 < v2[0].size (); col2++)
                    for (int col1 = 0; col1 < v1[row].size (); col1++)
                        multipliedVector[row][col2] += v1[row][col1] * v2[col1][col2];
        }
    }

    return multipliedVector;
}

//Get the transpose matrix 'v' function
vector<vector<double>> transposeMatrix (const vector<vector<double>> v)
{
    vector<vector<double>> transposeMatrix;
    
    //Check size and reSize
    if (v.size () > 0)
    {
        if (v[0].size () > 0)
        {
            transposeMatrix.resize(v[0].size());
            for (int i = 0; i < transposeMatrix.size(); i++)
                transposeMatrix[i].resize(v.size());
        }
    }

    //Transpose: changing rows to colomns
    for (int row = 0; row < v.size(); row++)
        for (int col = 0; col < v[row].size(); col++)
            transposeMatrix[col][row] = v[row][col];

    return transposeMatrix;
}


