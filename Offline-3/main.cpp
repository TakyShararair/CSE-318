

#include<bits/stdc++.h>
#include <dirent.h>

using namespace std;

struct City
{
    int id;
    double x;
    double  y;
};

double euclideanDistance(const City &a, const City &b)
{
    double m = pow(a.x - b.x,2 );
    double n = pow(a.y - b.y, 2);
    return sqrt(m+n);
}

void readTSPFile(const string &filename, vector<City> &cities)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string eachLine;
    while (getline(file, eachLine))
    {
        if (eachLine == "NODE_COORD_SECTION")
        {
            break;
        }
    }

    while (getline(file, eachLine))
    {
        if (eachLine == "EOF")
        {
            break;
        }

        City city;
        int id;
        double x, y;
        if (sscanf(eachLine.c_str(), "%d %lf %lf", &id, &x, &y) == 3)
        {
            city.id = id - 1; // Adjust for zero-based indexing
            city.x = x;
            city.y = y;
            cities.push_back(city);
        }
    }
}

vector<vector<double>> computeDistanceMatrix(const vector<City> &cities)
{
    int n = cities.size();
    vector<vector<double>> distances(n, vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            distances[i][j] = euclideanDistance(cities[i], cities[j]);
        }
    }
    return distances;
}

vector<int> nearestNeighbourHeuristic(const vector<City> &cities)
{
    vector<int> tour;
    vector<bool> visited(cities.size(), false);
    int current = 0;
    tour.push_back(current);
    visited[current] = true;

    while (tour.size() < cities.size())
    {
        
        double minDistance = INT_MAX;
        int nextCity = -1;

        for (int i = 0; i < cities.size(); ++i)
        {
            if (!visited[i])
            {
                double distance = euclideanDistance(cities[current], cities[i]);
                if (distance < minDistance)
                {
                    minDistance = distance;
                    nextCity = i;
                }
            }
        }

        tour.push_back(nextCity);
        visited[nextCity] = true;
        current = nextCity;
    }
    return tour;
}

vector<int> cheapestInsertion(const vector<City> &cities)
{
    vector<int> tour = {0, 1};
    vector<bool> inTour(cities.size(), false);
    inTour[0] = true ;
    inTour[1] = true;

    while (tour.size() < cities.size())
    {
      
         double minIncrease = INT_MAX;
        int bestCity = -1;
        int bestPosition = -1;

        for (int i = 0; i < cities.size(); ++i)
        {
            if (inTour[i])
            {
                continue;
            }

            for (int j = 0; j < tour.size(); ++j)
            {
                int next = (j + 1) % tour.size();
                double increase = euclideanDistance(cities[tour[j]], cities[i]) +
                                  euclideanDistance(cities[i], cities[tour[next]]) -
                                  euclideanDistance(cities[tour[j]], cities[tour[next]]);
                if (increase < minIncrease)
                {
                    minIncrease = increase;
                    bestCity = i;
                    bestPosition = next;
                }
            }
        }

        tour.insert(tour.begin() + bestPosition, bestCity);
        inTour[bestCity] = true;
    }

    return tour;
}

vector<int> farthestInsertion(const vector<City> &cities)
{
    vector<int> tour = {0};
    vector<bool> inTour(cities.size(), false);
    inTour[0] = true;

    while (tour.size() < cities.size())
    {
        double maxDistance = -1;
        int nextCity = -1;
        for (int i = 0; i < cities.size(); ++i)
        {
            if (!inTour[i])
            {
       
                 double minDistance = INT_MAX;
                for (int j : tour)
                {
                    double distance = euclideanDistance(cities[i], cities[j]);
                    if (distance < minDistance)
                    {
                        minDistance = distance;
                    }
                }
                if (minDistance > maxDistance)
                {
                    maxDistance = minDistance;
                    nextCity = i;
                }
            }
        }

        int bestPosition = -1;

        double minIncrease = INT_MAX;
        for (int i = 0; i < tour.size(); ++i)
        {
            int next = (i + 1) % tour.size();
            double increase = euclideanDistance(cities[tour[i]], cities[nextCity]) +
                              euclideanDistance(cities[nextCity], cities[tour[next]]) -
                              euclideanDistance(cities[tour[i]], cities[tour[next]]);
            if (increase < minIncrease)
            {
                minIncrease = increase;
                bestPosition = next;
            }
        }

        tour.insert(tour.begin() + bestPosition, nextCity);
        inTour[nextCity] = true;
    }

    return tour;
}


vector<int> randomInsertion(const vector<City> &cities)
{
    vector<int> tour;
    vector<bool> visited(cities.size(), false);
    int current = rand() % cities.size();
    tour.push_back(current);
    visited[current] = true;

    while (tour.size() < cities.size())
    {
        vector<int> unvisitedNodes;
        for (int i = 0; i < cities.size(); ++i)
        {
            if (!visited[i])
            {
                unvisitedNodes.push_back(i);
            }
        }

        int next = unvisitedNodes[rand() % unvisitedNodes.size()];

        // Find the best position to insert the new node
        int bestPosition = 0;
        double bestIncrease = INT_MAX;
        for (int i = 0; i < tour.size(); ++i)
        {
            int prev = tour[i];
            int nextNode = (i + 1) % tour.size() < tour.size() ? tour[(i + 1) % tour.size()] : tour[0];
            double increase = euclideanDistance(cities[prev], cities[next]) + euclideanDistance(cities[next], cities[nextNode]) - euclideanDistance(cities[prev], cities[nextNode]);
            if (increase < bestIncrease)
            {
                bestIncrease = increase;
                bestPosition = i + 1;
            }
        }
        tour.insert(tour.begin() + bestPosition, next);
        visited[next] = true;
    }
    return tour;
}

double calculateTourCost(const vector<vector<double>> &distances, const vector<int> &tour)
{

    double totalCost = 0.0;
    for (int i = 0; i < tour.size(); ++i)
    {
        int initial = tour[i];
        int final = tour[(i + 1) % tour.size()];
        totalCost += distances[initial][final];
    }
    return totalCost;
}

void printTour(const vector<int> &tour, double cost)
{
    for (int city : tour)
    {
        //  cout << city + 1 << " ";  // Adjusting back to original city IDs
    }
    cout << "Total cost: " << cost << endl;
}

vector<int> twoOpt(const vector<vector<double>> &distances, vector<int> &tour)
{
    double bestCost = calculateTourCost(distances, tour);
    bool improved = true;
    int n = tour.size();

    while (improved)
    {
        improved = false;
        for (int i = 0; i < n - 1; ++i)
        {
            for (int k = i + 2; k < n; ++k)
            {
                if (k == n - 1 && i == 0)
                {
                    continue; // Avoid breaking the tour into two separate tours
                }
                    
                int a = tour[i];
                int b = tour[(i + 1) % n];
                int c = tour[k];
                int d = tour[(k + 1) % n];

                double delta = -distances[a][b] - distances[c][d] + distances[a][c] + distances[b][d];

                if (delta < -1e-6)
                {
                    reverse(tour.begin() + i + 1, tour.begin() + k + 1);
                    bestCost += delta;
                    improved = true;
                    break; // Improvement found, restart the search
                }
            }
            if (improved)
                break;
        }
    }
    return tour;
}

vector<int> nodeShift(const vector<vector<double>> &distances, vector<int> &tour)
{
    double bestCost = calculateTourCost(distances, tour);
    bool improved = true;
    int n = tour.size();

    while (improved)
    {
        improved = false;
        for (int i = 0; i < n; ++i)
        {
            int city_i = tour[i];
            int prev_i = tour[(i - 1 + n) % n];
            int next_i = tour[(i + 1) % n];
            double cost_remove = distances[prev_i][city_i] + distances[city_i][next_i] - distances[prev_i][next_i];

            for (int j = 0; j < n; ++j)
            {
                if (i == j || (j + 1) % n == i)
                {
                    continue;
                }
                    
                int prev_j = tour[j];
                int next_j = tour[(j + 1) % n];

                double cost_insert = distances[prev_j][city_i] + distances[city_i][next_j] - distances[prev_j][next_j];

                double delta = -cost_remove + cost_insert;

                if (delta < -1e-6)
                {
                    tour.erase(tour.begin() + i);
                    if (i < j)
                    {
                        tour.insert(tour.begin() + j, city_i);
                    }
                    else
                    {
                        tour.insert(tour.begin() + j + 1, city_i);
                    }
                    bestCost += delta;
                    improved = true;
                    break;
                }
            }
            if (improved)
                break;
        }
    }
    return tour;
}

vector<int> nodeSwap(const vector<vector<double>> &distances, vector<int> &tour)
{
    double bestCost = calculateTourCost(distances, tour);
    bool improved = true;
    int n = tour.size();

    while (improved)
    {
        improved = false;
        for (int i = 0; i < n - 1; ++i)
        {
            int city_i = tour[i];
            int prev_i = tour[(i - 1 + n) % n];
            int next_i = tour[(i + 1) % n];

            for (int j = i + 1; j < n; ++j)
            {
                int city_j = tour[j];
                int prev_j = tour[(j - 1 + n) % n];
                int next_j = tour[(j + 1) % n];

                if (abs(i - j) == 1 || abs(i - j) == n - 1)
                {
                    // Adjacent nodes
                    continue; // 2-Opt already handles this case
                }
                else
                {
                    double delta = -(distances[prev_i][city_i] + distances[city_i][next_i] + distances[prev_j][city_j] + distances[city_j][next_j]) + (distances[prev_i][city_j] + distances[city_j][next_i] + distances[prev_j][city_i] + distances[city_i][next_j]);

                    if (delta < -1e-6)
                    {
                        swap(tour[i], tour[j]);
                        bestCost += delta;
                        improved = true;
                        break;
                    }
                }
            }
            if (improved)
                break;
        }
    }
    return tour;
}

void processDirectory(const string &directoryPath)
{
    ofstream csvFile("2005098.csv");
    csvFile << "Filename,Constructive method, Initial Cost, Purterbative Method, Improved Cost, Reduced Cost , Percentage \n";

    DIR *dir;
    struct dirent *entry;
    if ((dir = opendir(directoryPath.c_str())) != NULL)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            string filename = entry->d_name;
            if (filename.size() > 4 && filename.substr(filename.size() - 4) == ".tsp")
            {
                vector<City> cities;
                string filePath = directoryPath + "/" + filename;
                readTSPFile(filePath, cities);

                if (cities.empty())
                    continue;

                // Precompute distance matrix
                vector<vector<double>> distances = computeDistanceMatrix(cities);


                string cons1 = "Nearest Neighbour";
                cout << "Nearest Neighbour Heuristic Tour:" << endl;
                vector<int> nnTour = nearestNeighbourHeuristic(cities);
                double nnCost = calculateTourCost(distances, nnTour);
                printTour(nnTour, nnCost);

                string purter1 = "2-opt";
                cout << "2-Opt Improvement on Nearest Neighbour Tour:" << endl;
                vector<int> optTour1 = nnTour; // Start with nnTour
                optTour1 = twoOpt(distances, optTour1);
                double optCost1 = calculateTourCost(distances, optTour1);
                printTour(optTour1, optCost1);

                double reduc1 = nnCost - optCost1;
                double per1 = (reduc1/nnCost)*100;

                string node1 = "Node Shift";
                cout << "Node Shift Improvement on Nearest Neighbour Tour:" << endl;
                vector<int> shiftTour1 = nnTour; // Start with nnTour
                shiftTour1 = nodeShift(distances, shiftTour1);
                double shiftCost1 = calculateTourCost(distances, shiftTour1);
                printTour(shiftTour1, shiftCost1);

                double reduc2 = nnCost - shiftCost1;
                double per2 = (reduc2/nnCost)*100;

                string node2 = "Node Swap";
                cout << "Node Swap Improvement on Nearest Neighbour Tour:" << endl;
                vector<int> swapTour1 = nnTour; // Start with nnTour
                swapTour1 = nodeSwap(distances, swapTour1);
                double swapCost1 = calculateTourCost(distances, swapTour1);
                printTour(swapTour1, swapCost1);

                double reduc3 = nnCost - swapCost1;
                double per3 = (reduc3/nnCost)*100;

                csvFile << filename << "," << cons1 << "," << nnCost << ", " << purter1 << "," << optCost1 << "," << reduc1 << "," << per1 <<  "\n";
                csvFile << filename << "," << cons1 << "," << nnCost << ", " << node1 << "," << shiftCost1 << "," << reduc2 << "," << per2 << "\n";
                csvFile << filename << "," << cons1 << "," << nnCost << ", " << node2 << "," << swapCost1 << "," << reduc3 << "," << per3 << "\n";

                string cons2 = "Cheapest Insertion";
                cout << "Cheapest Insertion Heuristic Tour:" << endl;
                vector<int> ciTour = cheapestInsertion(cities);
                double ciCost = calculateTourCost(distances, ciTour);
                printTour(ciTour, ciCost);

                cout << "2-Opt Improvement on Nearest Insertion Tour:" << endl;
                vector<int> optTour2 = ciTour; // Start with ciTour
                optTour2 = twoOpt(distances, optTour2);
                double optCost2 = calculateTourCost(distances, optTour2);
                printTour(optTour2, optCost2);

                double reduc4 = ciCost - optCost2;
                double per4 = (reduc4/ciCost)*100;

                cout << "Node Shift Improvement on Nearest Insertion Tour:" << endl;
                vector<int> shiftTour2 = ciTour; // Start with ciTour
                shiftTour2 = nodeShift(distances, shiftTour2);
                double shiftCost2 = calculateTourCost(distances, shiftTour2);
                printTour(shiftTour2, shiftCost2);

                double reduc5 = ciCost - shiftCost2;
                double per5 = (reduc5/ciCost)*100;


                cout << "Node Swap Improvement on Nearest Insertion Tour:" << endl;
                vector<int> swapTour2 = ciTour; // Start with ciTour
                swapTour2 = nodeSwap(distances, swapTour2);
                double swapCost2 = calculateTourCost(distances, swapTour2);
                printTour(swapTour2, swapCost2);

                double reduc6 = ciCost - swapCost2;
                double per6 = (reduc6/ciCost)*100;

                csvFile << filename << "," << cons2 << "," << ciCost << ", " << purter1 << "," << optCost2 << "," << reduc4 << "," << per4 << "\n";
                csvFile << filename << "," << cons2 << "," << ciCost << ", " << node1 << "," << shiftCost2 << "," << reduc5 << "," << per5 << "\n";
                csvFile << filename << "," << cons2 << "," << ciCost << ", " << node2 << "," << swapCost2 << "," << reduc6 << "," << per6 << "\n";

                string cons3 = "Farthest Insertion";
                cout << "Farthest Insertion Heuristic Tour:" << endl;
                vector<int> fiTour = farthestInsertion(cities);
                double fiCost = calculateTourCost(distances, fiTour);
                printTour(fiTour, fiCost);

                cout << "2-Opt Improvement on Farthest Insertion Tour:" << endl;
                vector<int> optTour3 = fiTour; // Start with fiTour
                optTour3 = twoOpt(distances, optTour3);
                double optCost3 = calculateTourCost(distances, optTour3);
                printTour(optTour3, optCost3);

                double reduc7 = fiCost - optCost3;
                double per7 = (reduc7/fiCost)*100;

                cout << "Node Shift Improvement on Farthest Insertion Tour:" << endl;
                vector<int> shiftTour3 = fiTour; // Start with fiTour
                shiftTour3 = nodeShift(distances, shiftTour3);
                double shiftCost3 = calculateTourCost(distances, shiftTour3);
                printTour(shiftTour3, shiftCost3);

                double reduc8 = fiCost - shiftCost3;
                double per8 = (reduc8/fiCost)*100;

                cout << "Node Swap Improvement on Farthest Insertion Tour:" << endl;
                vector<int> swapTour3 = fiTour; // Start with fiTour
                swapTour3 = nodeSwap(distances, swapTour3);
                double swapCost3 = calculateTourCost(distances, swapTour3);
                printTour(swapTour3, swapCost3);

                double reduc9 = fiCost - swapCost3;
                double per9 = (reduc9/fiCost)*100;

                csvFile << filename << "," << cons3 << "," << fiCost << ", " << purter1 << "," << optCost3 << "," << reduc7 << ", " << per7 << "\n";
                csvFile << filename << "," << cons3 << "," << fiCost << ", " << node1 << "," << shiftCost3 << "," << reduc8 << ", " << per8 << "\n";
                csvFile << filename << "," << cons3 << "," << fiCost << ", " << node2 << "," << swapCost3 << "," << reduc9 << ", " << per9 <<"\n";
            
                string cons4 = "Random Insertion";
                cout << "Random Insertion Heuristic Tour:" << endl;
                vector<int> riTour = randomInsertion(cities);
                double riCost = calculateTourCost(distances, riTour);
                printTour(riTour, riCost);

                cout << "2-Opt Improvement on Random Insertion Tour:" << endl;
                vector<int> optTour4 = riTour; // Start with riTour
                optTour4 = twoOpt(distances, optTour4);
                double optCost4 = calculateTourCost(distances, optTour4);
                printTour(optTour4, optCost4);

                double reduc10 = riCost - optCost4;
                double per10 = (reduc10/riCost)*100;

                cout << "Node Shift Improvement on Random Insertion Tour:" << endl;
                vector<int> shiftTour4 = riTour; // Start with riTour
                shiftTour4 = nodeShift(distances, shiftTour4);
                double shiftCost4 = calculateTourCost(distances, shiftTour4);
                printTour(shiftTour4, shiftCost4);

                double reduc11 = riCost - shiftCost4;
                double per11 = (reduc11/riCost)*100;

                cout << "Node Swap Improvement on Random Insertion Tour:" << endl;
                vector<int> swapTour4 = riTour; // Start with riTour
                swapTour4 = nodeSwap(distances, swapTour4);
                double swapCost4 = calculateTourCost(distances, swapTour4);
                printTour(swapTour4, swapCost4);

                double reduc12 = riCost - swapCost4;
                double per12 = (reduc12/riCost)*100;

                csvFile << filename << "," << cons4 << "," << riCost << ", " << purter1 << "," << optCost4 << "," << reduc10 << ", " << per10 << "\n";
                csvFile << filename << "," << cons4 << "," << riCost << ", " << node1 << "," << shiftCost4 << "," << reduc11 << ", " << per11 << "\n";
                csvFile << filename << "," << cons4 << "," << riCost << ", " << node2 << "," << swapCost4 << "," << reduc12 << ", " << per12 <<"\n";

            }

        }
        closedir(dir);
    }
    else
    {
        cerr << "Could not open directory: " << directoryPath << endl;
    }

    csvFile.close();
}

int main()
{
    string directoryPath = "E:/TSP_Check";
    processDirectory(directoryPath);
    cout << "CSV file '2005098.csv' generated successfully." << endl;
    return 0;
}
