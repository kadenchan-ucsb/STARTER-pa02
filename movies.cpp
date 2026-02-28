#include "movies.h"

Movie::Movie(std::string title, double rating) : title(title), rating(rating) {}

void Movie::printMovie() const {
    std::cout << title << ", " << rating << std::endl;
}