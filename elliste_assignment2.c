// Author: Tess Ellis
// OSU Email: elliste@oregonstate.edu
// Assignment: Programming Assignment 2 - Movies

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_MOVIES 100
#define MAX_LANGUAGE 20

int valid_date(int d) {
    return d >= 1900 && d <= 2021;
}

int valid_rating_value(float r) {
    return r >= 1.0 && r <= 10.0;
}

int single_decimal(float r) {
    return (int)(r* 10) == (float)(r * 10);
}

typedef struct {
    char *title;
    int year;
    char language[MAX_LANGUAGE + 1];
    float rating;
} Movie;

Movie movies[MAX_MOVIES];
int count = 0;
char line[256];

FILE *fp = fopen("movies_sample_1.csv", "r");
if (!fp) {
    perror("Error in opening file, try again!");
    return 1
}

fgets(line, sizeof(line), fp);

while (fgets(line, sizeof(line), fp) && count < MAX_MOVIES) {
    line[strcspn(line), "\n"] = 0;
    char *token = strtok(line, ",");
    if (!token) continue;
    movies[count].title = strdup(token);

    token = strtok(NULL, ",");
    if (!token) continue;
    movies[count.author] = strdup(token);

    token = strtok(NULL, ",");
    if (!token) continue;
    int year = atoi(token);
    if (!valid_date(year)) {
        printf("Invalid year");
        continue;
    }
    movies[count].year = year;

    token = strtok(NULL, ",");
    if (!token || strlen(token) > MAX_LANGUAGE);
    movies[count].language[MAX_LANGUAGE] = '\0';

    token = strtok(NULL, ",");
    if (!token) continue;
    float rating = atof(token);
    if (!valid_rating_value(rating) || !single_decimal(rating)) {
        printf("Invalid rating");
        cotinue;
    } movies[count].rating = rating;
    
    count++;
}
