#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <map>
#include <algorithm>
#include <random>
#include <chrono>
#include <numeric>

using namespace std;

// ------------------------------------
// Class and Structure Declarations
// ------------------------------------
class Node
{
public:
    int attribute;
    map<string, Node *> branches;
    string value; // Majority class at this node
};

// ------------------------------------
// Function Prototypes
// ------------------------------------

// Utility Functions
string pluralityValue(const vector<vector<string>> &examples);
string sameClass(const vector<vector<string>> &examples);
double calcGiniImpurity(int attribute, const vector<vector<string>> &examples);
double calcInformationGain(int attribute, const vector<vector<string>> &examples);

// Decision Tree Functions
Node *decisionTreeLearning(vector<vector<string>> &examples, vector<int> &attributes,
                           const vector<vector<string>> &parentExamples, bool useGini, bool selectTopThree, mt19937 &gen);
string classify(Node *root, const vector<string> &instance);

// Data Loading Function
vector<vector<string>> loadData(const string &filename);

// Print Function

void printResults(const string &header, double trainPercent, const vector<double> &infoGainAccuracies, const vector<double> &giniAccuracies,
                  const vector<double> &infoGainTopThreeAccuracies, const vector<double> &giniTopThreeAccuracies);

// Main Function
int main()
{
    // Load data
    vector<vector<string>> data = loadData("car.data");

    // Seed once for entire run
    random_device rd;
    auto seed = chrono::high_resolution_clock::now().time_since_epoch().count() + rd();
    mt19937 gen((unsigned long)seed);

    // For training data 80%, testing data 20%
    vector<double> infoGainAccuracies;
    vector<double> giniAccuracies;
    vector<double> infoGainTopThreeAccuracies;
    vector<double> giniTopThreeAccuracies;

    for (int i = 0; i < 20; ++i)
    {
        // Shuffle and split
        shuffle(data.begin(), data.end(), gen);
        size_t trainSize = static_cast<size_t>(0.8 * data.size());
        vector<vector<string>> trainingData(data.begin(), data.begin() + trainSize);
        vector<vector<string>> testingData(data.begin() + trainSize, data.end());

        // Always select best attribute (Info Gain)
        {
            vector<int> attributes = {0, 1, 2, 3, 4, 5};
            Node *rootInfoGain = decisionTreeLearning(trainingData, attributes, trainingData, false, false, gen);
            int correctInfoGain = 0;
            for (const auto &inst : testingData)
            {
                if (classify(rootInfoGain, inst) == inst.back())
                    correctInfoGain++;
            }
            infoGainAccuracies.push_back((double)correctInfoGain / testingData.size());
        }

        // Always select best attribute (Gini)
        {
            vector<int> attributes = {0, 1, 2, 3, 4, 5};
            Node *rootGini = decisionTreeLearning(trainingData, attributes, trainingData, true, false, gen);
            int correctGini = 0;
            for (const auto &inst : testingData)
            {
                if (classify(rootGini, inst) == inst.back())
                    correctGini++;
            }
            giniAccuracies.push_back((double)correctGini / testingData.size());
        }

        // Select one from top three attributes (Info Gain)
        {
            vector<int> attributes = {0, 1, 2, 3, 4, 5};
            Node *rootInfoGainTopThree = decisionTreeLearning(trainingData, attributes, trainingData, false, true, gen);
            int correctInfoGainTopThree = 0;
            for (const auto &inst : testingData)
            {
                if (classify(rootInfoGainTopThree, inst) == inst.back())
                    correctInfoGainTopThree++;
            }
            infoGainTopThreeAccuracies.push_back((double)correctInfoGainTopThree / testingData.size());
        }

        // Select one from top three attributes (Gini)
        {
            vector<int> attributes = {0, 1, 2, 3, 4, 5};
            Node *rootGiniTopThree = decisionTreeLearning(trainingData, attributes, trainingData, true, true, gen);
            int correctGiniTopThree = 0;
            for (const auto &inst : testingData)
            {
                if (classify(rootGiniTopThree, inst) == inst.back())
                    correctGiniTopThree++;
            }
            giniTopThreeAccuracies.push_back((double)correctGiniTopThree / testingData.size());
        }
    }

    printResults("Training Data 80%, Testing Data 20%", 80.0, infoGainAccuracies, giniAccuracies, infoGainTopThreeAccuracies, giniTopThreeAccuracies);

    // For training data 60%, testing data 40%
    vector<double> infoGainAccuracies2;
    vector<double> giniAccuracies2;
    vector<double> infoGainTopThreeAccuracies2;
    vector<double> giniTopThreeAccuracies2;

    for (int i = 0; i < 20; ++i)
    {
        shuffle(data.begin(), data.end(), gen);
        size_t trainSize2 = static_cast<size_t>(0.6 * data.size());
        vector<vector<string>> trainingData(data.begin(), data.begin() + trainSize2);
        vector<vector<string>> testingData(data.begin() + trainSize2, data.end());

        // Always select best attribute (Info Gain)
        {
            vector<int> attributes = {0, 1, 2, 3, 4, 5};
            Node *rootInfoGain = decisionTreeLearning(trainingData, attributes, trainingData, false, false, gen);
            int correctInfoGain = 0;
            for (const auto &inst : testingData)
            {
                if (classify(rootInfoGain, inst) == inst.back())
                    correctInfoGain++;
            }
            infoGainAccuracies2.push_back((double)correctInfoGain / testingData.size());
        }

        // Always select best attribute (Gini)
        {
            vector<int> attributes = {0, 1, 2, 3, 4, 5};
            Node *rootGini = decisionTreeLearning(trainingData, attributes, trainingData, true, false, gen);
            int correctGini = 0;
            for (const auto &inst : testingData)
            {
                if (classify(rootGini, inst) == inst.back())
                    correctGini++;
            }
            giniAccuracies2.push_back((double)correctGini / testingData.size());
        }

        // Select one from top three attributes (Info Gain)
        {
            vector<int> attributes = {0, 1, 2, 3, 4, 5};
            Node *rootInfoGainTopThree = decisionTreeLearning(trainingData, attributes, trainingData, false, true, gen);
            int correctInfoGainTopThree = 0;
            for (const auto &inst : testingData)
            {
                if (classify(rootInfoGainTopThree, inst) == inst.back())
                    correctInfoGainTopThree++;
            }
            infoGainTopThreeAccuracies2.push_back((double)correctInfoGainTopThree / testingData.size());
        }

        // Select one from top three attributes (Gini)
        {
            vector<int> attributes = {0, 1, 2, 3, 4, 5};
            Node *rootGiniTopThree = decisionTreeLearning(trainingData, attributes, trainingData, true, true, gen);
            int correctGiniTopThree = 0;
            for (const auto &inst : testingData)
            {
                if (classify(rootGiniTopThree, inst) == inst.back())
                    correctGiniTopThree++;
            }
            giniTopThreeAccuracies2.push_back((double)correctGiniTopThree / testingData.size());
        }
    }

    printResults("Training Data 60%, testing data 40%", 60.0, infoGainAccuracies2, giniAccuracies2, infoGainTopThreeAccuracies2, giniTopThreeAccuracies2);

    return 0;
}

// ------------------------------------
// Function Definitions
// ------------------------------------
string pluralityValue(const vector<vector<string>> &examples)
{
    map<string, int> counts;
    for (const auto &example : examples)
    {
        counts[example.back()]++;
    }
    string majorityClass = "";
    int maxCount = 0;
    for (const auto &pair : counts)
    {
        if (pair.second > maxCount)
        {
            maxCount = pair.second;
            majorityClass = pair.first;
        }
    }
    return majorityClass;
}

string sameClass(const vector<vector<string>> &examples)
{
    string firstClass = examples[0].back();
    for (const auto &example : examples)
    {
        if (example.back() != firstClass)
        {
            return "";
        }
    }
    return firstClass;
}

double calcGiniImpurity(int attribute, const vector<vector<string>> &examples)
{
    map<string, map<string, int>> valueCounts;
    for (const auto &example : examples)
    {
        string attributeValue = example[attribute];
        string className = example.back();
        valueCounts[attributeValue][className]++;
    }

    double gini = 0.0;
    int totalExamples = (int)examples.size();
    for (const auto &vPair : valueCounts)
    {
        int total = 0;
        for (const auto &cPair : vPair.second)
        {
            total += cPair.second;
        }
        double valueGini = 1.0;
        for (const auto &cPair : vPair.second)
        {
            double prob = (double)cPair.second / total;
            valueGini -= prob * prob;
        }
        gini += ((double)total / totalExamples) * valueGini;
    }
    return gini;
}

double calcInformationGain(int attribute, const vector<vector<string>> &examples)
{
    map<string, map<string, int>> valueCounts;
    map<string, int> classCounts;
    int totalExamples = (int)examples.size();

    for (const auto &example : examples)
    {
        string attributeValue = example[attribute];
        string className = example.back();
        valueCounts[attributeValue][className]++;
        classCounts[className]++;
    }

    double entropyBefore = 0.0;
    for (const auto &cPair : classCounts)
    {
        double prob = (double)cPair.second / totalExamples;
        entropyBefore -= prob * log2(prob);
    }

    double entropyAfter = 0.0;
    for (const auto &vPair : valueCounts)
    {
        int total = 0;
        for (const auto &cPair : vPair.second)
        {
            total += cPair.second;
        }
        double valueEntropy = 0.0;
        for (const auto &cPair : vPair.second)
        {
            double prob = (double)cPair.second / total;
            valueEntropy -= prob * log2(prob);
        }
        entropyAfter += ((double)total / totalExamples) * valueEntropy;
    }

    return entropyBefore - entropyAfter;
}

Node *decisionTreeLearning(vector<vector<string>> &examples, vector<int> &attributes,
                           const vector<vector<string>> &parentExamples, bool useGini, bool selectTopThree, mt19937 &gen)
{
    if (examples.empty())
    {
        Node *leaf = new Node;
        leaf->attribute = -1;
        leaf->value = pluralityValue(parentExamples);
        return leaf;
    }

    string common = sameClass(examples);
    if (!common.empty())
    {
        Node *leaf = new Node;
        leaf->attribute = -1;
        leaf->value = common;
        return leaf;
    }

    if (attributes.empty())
    {
        Node *leaf = new Node;
        leaf->attribute = -1;
        leaf->value = pluralityValue(examples);
        return leaf;
    }

    vector<pair<double, int>> measures;
    for (int attr : attributes)
    {
        double measure = useGini ? calcGiniImpurity(attr, examples) : calcInformationGain(attr, examples);
        measures.push_back({measure, attr});
    }

    if (useGini)
    {
        stable_sort(measures.begin(), measures.end(), [](auto &a, auto &b)
                    { return a.first < b.first; });
    }
    else
    {
        stable_sort(measures.begin(), measures.end(), [](auto &a, auto &b)
                    { return a.first > b.first; });
    }

    int bestAttribute;
    if (selectTopThree && measures.size() >= 3)
    {
        uniform_int_distribution<> distr(0, 2);
        bestAttribute = measures[distr(gen)].second;
    }
    else
    {
        bestAttribute = measures[0].second;
    }

    Node *root = new Node;
    root->attribute = bestAttribute;
    root->value = pluralityValue(examples);

    map<string, vector<vector<string>>> partitions;
    for (auto &ex : examples)
    {
        partitions[ex[bestAttribute]].push_back(ex);
    }

    vector<int> newAttributes = attributes;
    newAttributes.erase(remove(newAttributes.begin(), newAttributes.end(), bestAttribute), newAttributes.end());

    for (auto &p : partitions)
    {
        if (p.second.empty())
        {
            Node *leaf = new Node;
            leaf->attribute = -1;
            leaf->value = pluralityValue(examples);
            root->branches[p.first] = leaf;
        }
        else
        {
            Node *child = decisionTreeLearning(p.second, newAttributes, examples, useGini, selectTopThree, gen);
            root->branches[p.first] = child;
        }
    }

    return root;
}

string classify(Node *root, const vector<string> &instance)
{
    if (root->attribute == -1)
    {
        return root->value;
    }
    string attributeValue = instance[root->attribute];
    if (root->branches.find(attributeValue) != root->branches.end())
    {
        return classify(root->branches[attributeValue], instance);
    }
    return root->value;
}

vector<vector<string>> loadData(const string &filename)
{
    ifstream infile(filename);
    vector<vector<string>> data;
    string line;
    while (getline(infile, line))
    {
        istringstream ss(line);
        vector<string> instance;
        string value;
        while (getline(ss, value, ','))
        {
            instance.push_back(value);
        }
        data.push_back(instance);
    }
    infile.close();
    return data;
}

void printResults(const string &header, double trainPercent, const vector<double> &infoGainAccuracies,
                  const vector<double> &giniAccuracies, const vector<double> &infoGainTopThreeAccuracies,
                  const vector<double> &giniTopThreeAccuracies)
{

    double avgInfoGainAccuracy = accumulate(infoGainAccuracies.begin(), infoGainAccuracies.end(), 0.0) / infoGainAccuracies.size();
    double avgGiniAccuracy = accumulate(giniAccuracies.begin(), giniAccuracies.end(), 0.0) / giniAccuracies.size();
    double avgInfoGainTopThreeAccuracy = accumulate(infoGainTopThreeAccuracies.begin(), infoGainTopThreeAccuracies.end(), 0.0) / infoGainTopThreeAccuracies.size();
    double avgGiniTopThreeAccuracy = accumulate(giniTopThreeAccuracies.begin(), giniTopThreeAccuracies.end(), 0.0) / giniTopThreeAccuracies.size();

    cout << "|------------------------------------------------------------------------------------------|" << endl;
    cout << "|                                                  |    Average accuracy over 20 runs      |" << endl;
    cout << "|                                                  | " << header << "  |" << endl;
    cout << "|------------------------------------------------------------------------------------------|" << endl;
    cout << "|Attribute Selection strategy                      |  Information gain    | Gini Impurity  |" << endl;
    cout << "|------------------------------------------------------------------------------------------|" << endl;
    cout << "| Always select best attribute    \t           |  " << avgInfoGainAccuracy * 100 << "\t          |     " << avgGiniAccuracy * 100 << "    |" << endl;
    cout << "|------------------------------------------------------------------------------------------|" << endl;
    cout << "| Select one randomly from top three attributes    |  " << avgInfoGainTopThreeAccuracy * 100 << "\t         |  " << avgGiniTopThreeAccuracy * 100 << "        |" << endl;
    cout << "|------------------------------------------------------------------------------------------|" << endl;
}
