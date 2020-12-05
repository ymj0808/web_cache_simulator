#include "random_helper.h"
#include <random>

std::mt19937_64 globalGenerator;

void seedGenerator() { globalGenerator.seed(SEED); }
