#ifndef TYPES_H
#define TYPES_H

/* Shared types used across the project */

typedef struct location {
    int id;
    int x;
    int y;
} location;

typedef struct Bin {
    int id;
    int amount;
    int locationId; /* index of location where this bin sits */
} bin;

typedef struct truck {
    int id;
    int capacity;
} truck;

#endif /* TYPES_H */
