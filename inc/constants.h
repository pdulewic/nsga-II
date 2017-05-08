#ifndef CONSTANTS_H
#define CONSTANTS_H

const int DEFAULT_POPULATION_SIZE = 100;
const int MIN_POPULATION_SIZE = 10;
const int MAX_POPULATION_SIZE = 10000;

const int MIN_PROBLEM_SIZE = 1;
const int MAX_PROBLEM_SIZE = 10;

const int DEFAULT_X_RANGE = 100;
const int MIN_X_RANGE = -10000;
const int MAX_X_RANGE = 10000;

const int DEFAULT_GENERATIONS = 100;
const int MIN_GENERATIONS = 10;
const int MAX_GENERATIONS = 1000;

const int MAIN_WINDOW_WIDTH = 1000;
const int MAIN_WINDOW_HEIGHT = 700;

// przewaga dominującego rodzica przy przekazywaniu genów
const double DOMINANT_PARENT_ADVANTAGE = 0.8;  // 80%
const double MUTATION_PROBABILITY = 0.3;
// maksymalny procent, o jaki mogą się zmienić elementy rozwiązania przy mutacji
const double MUTATION_STRENGTH = 0.4;
const double BIG_DOUBLE = 1000000.0;

#endif // CONSTANTS_H
