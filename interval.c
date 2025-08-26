#include "interval.h"

const Interval INTERVAL_EMPTY = {DBL_MAX, -DBL_MAX};
const Interval INTERVAL_UNIVERSE = {-DBL_MAX, DBL_MAX};

Interval interval_empty(void) {
    return (Interval){DBL_MAX, -DBL_MAX};
}

Interval interval_create(double min, double max) {
    return (Interval){min, max};
}

double interval_size(const Interval *interval) {
    return interval->max - interval->min;
}

bool interval_contains(const Interval *interval, double x) {
    return interval->min <= x && x <= interval->max;
}

bool interval_surrounds(const Interval *interval, double x) {
    return interval->min < x && x < interval->max;
}