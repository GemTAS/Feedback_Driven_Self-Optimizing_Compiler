#include <stdio.h>
#include <string.h>
#include "profiler_runtime.h"

// We'll just use a brutally simple fixed-size array to track label hits.
// For a tiny compiler project like this, a hash map in C is overkill anyway!
static struct {
    char label[32];
    int count;
} hits[256];

static int n = 0;

void prof_hit(const char* l) {
    // Linear search is fine since we aren't expecting thousands of unique labels in our test programs.
    for(int i = 0; i < n; i++) {
        if(strcmp(hits[i].label, l) == 0) {
            hits[i].count++;
            return;
        }
    }

    // Oh hey, a brand new label! Let's record it.
    strcpy(hits[n].label, l);
    hits[n].count = 1;
    n++;
}

void prof_dump() {
    // At the end of the program, we dump out everything to text so the C++ engine can read it later.
    FILE* f = fopen("profile.txt", "w");

    for(int i = 0; i < n; i++)
        fprintf(f, "%s %d\n", hits[i].label, hits[i].count);

    fclose(f);
}
