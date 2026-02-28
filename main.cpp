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

    for (const string &pref : prefixes) {
        auto it = lower_bound(movieList.begin(), movieList.end(), pref,[](const Movie &m, const string &value) {
            return m.title < value;
        });
        vector<Movie> matches;
        double bestRating = -1.0;
        string bestName;
        bool found = false;
        for (const Movie &m : movieList) {
            if (m.title.rfind(pref, 0) == 0){
                matches.push_back(m);
                if (!found ||
                    m.rating > bestRating ||
                    (m.rating == bestRating && m.title < bestName)) {
                    bestRating = m.rating;
                    bestName = m.title;
                    found = true;
                }
            }
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
            cout << "Best movie with prefix " << pref << " is: "
                << bestName << " with rating " <<  bestRating << endl;
        }
    }
    
    return 0;
}

/* Add your run time analysis for part 3 of the assignment here as commented block*/

bool parseLine(string &line, string &movieName, double &movieRating) {
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex+1));
    if (movieName[0] == '\"') {
        movieName = movieName.substr(1, movieName.length() - 2);
    }
    return true;
}