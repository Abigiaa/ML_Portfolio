/*
 * Abigail Solomon
 * CS 4375.003
 * Portfolio: ML from Scratch
 *
 *  This program will implement Naive Bayes on titanic_project.csv file 
    to predict survived based predictors age, pclass and sex.
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
vector<vector<double>> transposeMatrix(const vector<vector<double>> v);
double  getSum(const vector<double>& );
double  getMean(const vector<double>& );
double  getVar(const vector<double>& );
vector<double>  getApriori(const vector<double>& );
vector<vector<double>>  getLikelihood(const vector<double>&, const vector<double>&, const vector<double>&, const int&, const int&);
double  calculateLH(const double&, const double&, const double&);
vector<vector<double>>  getLikelihood(const vector<double>&, const vector<double>&, const vector<double>&, const int&, const int&);
vector<vector<double>>  findCLikelihood(const vector<double>&, const vector<double>, const vector<double>&, const int&);


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

        string      input;
        vector<double>  survived, pclass, sex, age;

        
        getline(fIn, input);

        // Import all the data while the file is open
        while (!fIn.eof())
        {
            // unused data point
            getline(fIn, input, ',');

            //used data
            getline(fIn, input, ',');
            pclass.push_back(stod(input) - 1);
            getline(fIn, input, ',');
            survived.push_back(stod(input));
            getline(fIn, input, ',');
            sex.push_back(stod(input));
            getline(fIn, input);
            age.push_back(stod(input));
        }

        vector<vector<double>>      lh_pclass, lh_sex, age_mean_var;

        //Divide the gathered data into 'train' and 'test' set
        vector<vector<double>>      train, test;
        for (int i = 0; i < 800; i++)
            train.push_back({ pclass[i], survived[i], sex[i], age[i] });
        for (int i = 800; i < survived.size(); i++)
            test.push_back({ pclass[i], survived[i], sex[i], age[i] });

        //Transpose data
        train = transposeMatrix(train);
        test = transposeMatrix(test);

        //Begin the timing process of the algorithm
        auto start = chrono::high_resolution_clock::now();

        //Applying Naive Bayes algorithm to find the necessary likelihood using 'train' set
        vector<double>  aprior(getApriori(train[1]));
        vector<double>  count_survived(2);
        for (int i = 0; i < train[1].size(); i++)
            train[1][i] == 0 ? count_survived[0]++ : count_survived[1]++;
        lh_pclass = getLikelihood(train[1], train[0], count_survived, 2, 3);
        lh_sex = getLikelihood(train[1], train[2], count_survived, 2, 2);
        age_mean_var = findCLikelihood(train[1], train[3], count_survived, 2);

        //Stop the timing process and assign the elapsed time
        auto stop = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed_time = stop - start;

        double      num_s(0), num_p(0), denominator(0);

        vector<vector<double>>  result;

        //Calculating probabilities on the 'test' data set 
        for (int i = 0; i < test[1].size(); i++)
        {
            num_s = lh_pclass[1][test[0][i]] * lh_sex[1][test[2][i]] * aprior[1] * calculateLH(test[3][i], age_mean_var[0][1], age_mean_var[1][1]);
            num_p = lh_pclass[0][test[0][i]] * lh_sex[0][test[2][i]] * aprior[0] * calculateLH(test[3][i], age_mean_var[0][0], age_mean_var[1][0]);
            denominator = (num_s + num_p);

            result.push_back({ {num_p / denominator},{num_s / denominator} });
        }
        //Change  probability results into a binary factor
        vector<vector<double>>      prediction;
        for (int i = 0; i < test[1].size(); i++)
            prediction.push_back({ result[i][1] > 0.5 ? (double)1 : (double)0 });

        //Evaluating the prediction and calculate the metrics
        double     TP(0), TN(0), FP(0), FN(0);
        for (int i = 800; i < pclass.size(); i++)
        {
            if (prediction[i - 800][0] == 0 && survived[i] == 0)    TP++;
            if (prediction[i - 800][0] == 0 && survived[i] == 1)    FP++;
            if (prediction[i - 800][0] == 1 && survived[i] == 1)    TN++;
            if (prediction[i - 800][0] == 1 && survived[i] == 0)    FN++;
        }
        cout << endl << "  TP: " << TP << "  " << "   FP: " << FP << endl << "  FN: " << FN << "  " << "    TN: " << TN << endl << endl;

        //Outpitting the calculated metrics
        cout << " Accuracy: " << ((TP + TN) / (TP + TN + FP + FN)) << endl;
        cout << " Specificity: " << TP / (TP + FN) << endl;
        cout << " Sensitivity: " << TN / (TN + FP) << endl;
        cout << endl << " Run time for the algorithm: " << elapsed_time.count() << " sec" << endl;
    
    return 0;
}

// Definition of functions

// Get the tranpose of matrix 'v'
vector<vector<double>> transposeMatrix(const vector<vector<double>> v)
{
    vector<vector<double>> transposeMatrix;

    //Check size and reSize
    if (v.size() > 0)
    {
        if (v[0].size() > 0)
        {
            transposeMatrix.resize(v[0].size());
            for (int i = 0; i < transposeMatrix.size(); i++)
                transposeMatrix[i].resize(v.size());
        }
    }

    //Transpose: changing rows to columns
    for (int row = 0; row < v.size(); row++)
        for (int col = 0; col < v[row].size(); col++)
            transposeMatrix[col][row] = v[row][col];

    return transposeMatrix;
}

// Calculate the sum of a given matrix 'v'
double  getSum(const vector<double>& v)
{
    double  sum(0);

    //Add up all values in the vector
    for (int i = 0; i < v.size(); i++)
        sum += v[i];

    return sum;
}

//Calculate the mean of a given matrix 'v'
double  getMean(const vector<double>& v)
{
    return (getSum(v) / v.size());
}

// Calculate the variance of a given matrix 'v'
double  getVar(const vector<double>& v)
{
    double  var(0), mean(getMean(v));

    // add all the variance
    for (int i = 0; i < v.size(); i++)
        var += (v[i] - mean) * (v[i] - mean);

    //average variance
    return var / v.size();
}

//Calculate the A-Priority of a given matrix 'v'
vector<double>  getApriori(const vector<double>& v)
{
    vector<double>  Apriori(2);

    //Looping through the vector and count the occurence of the instances
    for (int i = 0; i < v.size(); i++)
    {
        if (v[i] == 0)  Apriori[0]++;
        if (v[i] == 1)  Apriori[1]++;
    }

    //Divide by to total size to get the aprior
    Apriori[0] /= v.size();
    Apriori[1] /= v.size();

    return Apriori;
}

//Calculate discrete likelihood 
vector<vector<double>>  getLikelihood(const vector<double>& cls, const vector<double>& lvl, const vector<double>& cls_count, const int& numCls, const int& numLvl)
{
    vector<vector<double>>  result;

    //reSize
    result.resize(numCls);
    for (int i = 0; i < result.size(); i++)
        result[i].resize(numLvl);

    //likelihood of each specific class and level
    for (int c = 0; c < numCls; c++)
    {
        for (int l = 0; l < numLvl; l++)
        {
            double     count(0);
            for (int i = 0; i < cls.size(); i++)
                if (cls[i] == c && lvl[i] == l)   count++;
            result[c][l] = count / cls_count[c];
        }
    }

    return result;
}

//Calculate the likelihood using the mean and variance
double  calculateLH(const double& value, const double& meanValue, const double& varValue)
{
    return (1 / (sqrt(2 * (2 * acos(0.0)) * varValue) * exp(-(pow((value - meanValue), 2)) / (2 * varValue))));
}

//Calculate continuous likelihood 
vector<vector<double>>  findCLikelihood(const vector<double>& cls, const vector<double> lvl, const vector<double>& cls_count, const int& numCls)
{
    vector<vector<double>>  result;
    vector<double>  temp, mean, var;

    //reSize
    result.resize(numCls);
    for (int i = 0; i < result.size(); i++)
        result[i].resize(2);

    //Gather all the data of each class
    for (int c = 0; c < numCls; c++)
    {
        for (int i = 0; i < cls.size(); i++)
            if (cls[i] == c)
                temp.push_back(lvl[i]);
        //Calculate and store mean and variance in temp vectors
        mean.push_back(getMean(temp));
        var.push_back(getVar(temp));
        temp.clear();
    }

    //Put in the mean and variance into each class
    for (int i = 0; i < mean.size(); i++)
    {
        result[0][i] = mean[i];
        result[1][i] = var[i];
    }
    return result;
}

