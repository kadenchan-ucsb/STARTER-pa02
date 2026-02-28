// Winter'24
// Instructor: Diba Mirza
// Student name: Kaden Chan
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <cstring>
#include <algorithm>
#include <limits.h>
#include <iomanip>
#include <set>
#include <queue>
#include <sstream>
using namespace std;

#include "utilities.h"
#include "movies.h"

bool parseLine(string &line, string &movieName, double &movieRating);

int main(int argc, char** argv){
    if (argc < 2){
        cerr << "Not enough arguments provided (need at least 1 argument)." << endl;
        cerr << "Usage: " << argv[ 0 ] << " moviesFilename prefixFilename " << endl;
        exit(1);
    }

    ifstream movieFile (argv[1]);
 
    if (movieFile.fail()){
        cerr << "Could not open file " << argv[1];
        exit(1);
    }
  
    // Create an object of a STL data-structure to store all the movies

    string line, movieName;
    double movieRating;
    // Read each file and store the name and rating
    std::priority_queue<Movie, std::vector<Movie>, decltype(cmp)> pq(cmp);
    vector<Movie> movieList;

    while (getline (movieFile, line) && parseLine(line, movieName, movieRating)){
            // Use std::string movieName and double movieRating
            // to construct your Movie objects
            // cout << movieName << " has rating " << movieRating << endl;
            // insert elements into your data structure
            
            pq.push(Movie(movieName, movieRating));
            movieList.push_back(Movie(movieName, movieRating));
    }

    movieFile.close();

    if (argc == 2){
            //print all the movies in ascending alphabetical order of movie names
            while (!pq.empty()) {
                pq.top().printMovie();
                pq.pop();
            }
            return 0;
    }

    ifstream prefixFile (argv[2]);

    if (prefixFile.fail()) {
        cerr << "Could not open file " << argv[2];
        exit(1);
    }

    vector<string> prefixes;
    while (getline (prefixFile, line)) {
        if (!line.empty()) {
            prefixes.push_back(line);
        }
    }
    
    sort(movieList.begin(), movieList.end(),[](const Movie &a, const Movie &b) {
        return a.title < b.title;
    });

    vector<pair<string, Movie>> highestMovies;
    for (const string &pref : prefixes) {
        auto it = lower_bound(movieList.begin(), movieList.end(), pref,[](const Movie &m, const string &value) {
            return m.title < value;
        });
        vector<Movie> matches;
        double bestRating = -1.0;
        string bestName;
        bool found = false;
        while (it!=movieList.end() && it->title.compare(0, pref.length(), pref) == 0) {
            matches.push_back(*it);
                if (!found ||
                    it->rating > bestRating ||(it->rating == bestRating && it->title < bestName)) {
                    bestRating = it->rating;
                    bestName = it->title;
                    found = true;
                }
            ++it;
        }
        if (matches.empty()) {
            cout << "No movies found with prefix " << pref << endl;
        } else {
            sort(matches.begin(), matches.end(), [](const Movie &a, const Movie &b) {
                if (a.rating != b.rating)
                    return a.rating > b.rating;     
                return a.title < b.title;
            });
            for (const Movie &m : matches)
                m.printMovie();
            cout<<endl;
        }
        if (found) {
            highestMovies.push_back(make_pair(pref, Movie(bestName, bestRating)));
        }
    }
    for(pair<string, Movie> &p : highestMovies) {
        cout << "Best movie with prefix " << p.first << " is " << p.second.title << " with rating " << p.second.rating << endl;
    }
    
    return 0;
}

/*input_20_random.csv: 0.015s
input_100_random.csv: 0.013s
input_1000_random.csv: 0.015s
input_10000_random.csv: 0.140s

all n, m, k, and l should affect runtime

Runtime of algorithm after the movies are stored in a vector:
Starts with an alphabetic sort of the movies(l*n*log n)
In a loop that runs once per prefix(m):
    Binary search for the first movie with the prefix(l log n)
    Loop through movies with the prefix and find the best rated movie(l*k)
    Sort the movies with the prefix by rating and name(l*k*log k)
Loop through list of best movies and print(at most l*k)

l*n*log n + m*(l log n + l*k + l*k*log k) + l*k
= l(n*log n + m(log n + k + k*log k) + k)

I designed this program for low time complexity, and I was able to achieve it
by using binary search, which allowed a time complexity of m*log n rather than n*m.
There was a cost of having to sort the movies by alphabetical order beforehand,
but the higher m is, the more impactful the binary search is, therefore making it 
the better option overall.
*/

bool parseLine(string &line, string &movieName, double &movieRating) {
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex+1));
    if (movieName[0] == '\"') {
        movieName = movieName.substr(1, movieName.length() - 2);
    }
    return true;
}