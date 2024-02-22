#include <catch2/catch_test_macros.hpp>

#include "./simulation/simulation.h"

#include "./elements/neural_field.h"
#include "elements/gauss_kernel.h"
#include "elements/mexican_hat_kernel.h"
#include "elements/normal_noise.h"
#include "elements/field_coupling.h"
#include "elements/gauss_stimulus.h"
#include "elements/lateral_interactions.h"