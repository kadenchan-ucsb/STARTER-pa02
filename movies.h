#include <iostream>
#include <queue>
#include <vector>
#include <string>

struct Movie {
    
        std::string title;
        double rating;
    
        Movie(std::string title, double rating);
        void printMovie() const;
};

inline auto cmp = [](const Movie& a, const Movie& b) {
    return a.title > b.title;
};