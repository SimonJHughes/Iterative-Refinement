/*
 * Name: Simon Hughes
 * Date Submitted: May 2, 2022
 * Lab Section: 001
 * Assignment Name: Iterative Refinement and the Traveling Salesman Problem
 */

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>
using namespace std;

const int N = 14; //Number of cities in cities.txt
typedef pair<double,double> Point; //<latitude,longitude> of a city
vector<Point> P(N), best; //P - current solution, best - best solution
map<Point, int> cities; //Point (latitude,longitude) -> index of city in cities.txt order
string cityNames[] = {"New York City",
                      "Chicago",
                      "Los Angeles",
                      "Toronto",
                      "Houston",
                      "Montreal",
                      "Mexico City",
                      "Vancouver",
                      "Atlanta",
                      "Denver",
                      "San Juan",
                      "New Orleans",
                      "Miami",
                      "Kansas City"}; //Index of a city -> city name

//Calculates "distance"
//Units are lat./long. "degrees" on an x-y plane
//to simplify calculations (not mi/km on globe)
double dist(int i, int j)
{
  double dx = P[(i+N)%N].first - P[(j+N)%N].first;
  double dy = P[(i+N)%N].second - P[(j+N)%N].second;
  return sqrt(dx*dx + dy*dy);
}

//Looks for a lower distance between two edges by testing swaps. If one is found, swap occurs
bool refine(double &len)
{
  for(int i = 0; i < P.size(); i++)
  {
    pair<Point, Point> currentEdge (P[i], P[(i + 1) % N]);
    double currentDistance = dist(i, (i + 1) % N);
    for(int j = i + 2; j < P.size() + i; j++)
    {
      if(i != j % N)
      {
        double edge2 = dist(j % N, (j + 1) % N);
        double newEdge1 = dist(i, j % N);
        double newEdge2 = dist(i + 1, (j + 1) % N);
        if(currentDistance + edge2 > newEdge1 + newEdge2)
        {
          //Swap
          int iterate = 0;
          for(int k = i + 1; k < j; k++)
          {
            Point tempPoint = P[k];
            P[k] = P[j - iterate];
            P[j - iterate] = tempPoint;
            iterate++;
          }
          //Calculate total distance
          double newTotal = 0;
          for(int k = 0; k < N - 1; k++)
          {
            newTotal += dist(k, k + 1);
          }
          newTotal += dist(0, N - 1);
          len = newTotal;
          return true;
        }
      }
    }
  }
  return false;
}

//Returns best length for test case
double tspRefine()
{
  double best_len = 999999999;
  ifstream fin("cities.txt");
  for (int i=0; i<N; i++)
  {
    fin >> P[i].first >> P[i].second;
    cities[P[i]] = i;
  }
  
  srand(time(NULL));
  for(int i = 0; i < 5; i++)
  {
    double current_len;
    //Generate random permutation
    for(int j = 0; j < N; j++)
    {
      int randIndex = rand() % N;
      Point temp = P[randIndex];
      P[randIndex] = P[j];
      P[j] = temp;
    }
    //Calculate total dist of permutation
    for(int j = 0; j < N - 1; j++)
    {
      current_len += dist(j, j + 1);
    }
    current_len += dist(0, N - 1);
    //Refine
    while(refine(current_len)) {};
    if(current_len < best_len)
    {
      best_len = current_len;
      best = P;
    }
  }

  for (auto p : best) cout << cityNames[cities[p]] << endl;
  cout << "\nTotal length: " << best_len << "\n";
  return best_len;
}

int main(void)
{
  double best_len = 999999999;
  best_len=tspRefine();
  return 0;
}
