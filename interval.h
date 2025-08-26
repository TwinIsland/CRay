#pragma once

#include <stdbool.h>
#include <float.h>

typedef struct {
    double min;
    double max;
} Interval;

Interval interval_empty(void);
Interval interval_create(double min, double max);

double interval_size(const Interval *interval);
bool interval_contains(const Interval *interval, double x);
bool interval_surrounds(const Interval *interval, double x);

extern const Interval INTERVAL_EMPTY;
extern const Interval INTERVAL_UNIVERSE;