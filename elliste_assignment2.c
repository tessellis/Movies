// Author: Tess Ellis
// OSU Email: elliste@oregonstate.edu
// Assignment: Programming Assignment 2 - Movies
// More information about program details and notes in README.md file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// defining constants
#define MAX_LANG 5
#define MAX_LANG_LEN 21
#define MIN_YEAR 1900
#define MAX_YEAR 2021
#define RANGE_YEAR (MAX_YEAR - MIN_YEAR + 1)

// defining Movie node structure as per intruct
// storing as singly linked list
typedef struct Movie {
    char *title;
    int year;
    char langs[MAX_LANG][MAX_LANG_LEN];
    int language_count;
    double rating;
    struct Movie *next;
} Movie;

// <---- Movile Building Function ----->
// allocates memoty and initializes a new movie node
Movie *build_movie(char *title, int year, char langs[][MAX_LANG_LEN], int lang_count, double rating) {
    // allocating memory with 'malloc'
    Movie *new_movie = malloc(sizeof(Movie));
    // dynamically allocating title (including null terminator)
    new_movie->title = malloc(strlen(title) + 1);
    // copying title into memory
    strcpy(new_movie->title, title);
    // assigning year to movie node
    new_movie->year = year;
    // assigning languages
    new_movie->language_count = lang_count;
    // iterating trough langs array
    for (int i = 0; i < lang_count; i++) {
        // copy and assign languages
        strncpy(new_movie->langs[i], langs[i], MAX_LANG_LEN);
    }
    // assign rating
    new_movie->rating = rating;
    // pointing to next movie (moving through linked list)
    new_movie->next = NULL;
    return new_movie;
}

// <------- CSV Reading Function ----->
// Reads CSV file and builds linked list of movies
Movie *read_movie_csv(const char *filename, int *movie_count) {
    // opening file
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("File error, try again.");
        return NULL;
    }

    char line[1024];
    // skipping header line
    fgets(line, sizeof(line), fp);

    // resetting head and tail
    Movie *head = NULL;
    Movie *tail = NULL;
    *movie_count = 0;

    // reading though csv of format: Title,Year,Languages,Rating Value
    while(fgets(line, sizeof(line), fp)) {
        char *title = strtok(line, ",");
        char *str_year = strtok(NULL, ",");
        char *str_lang = strtok(NULL, ",");
        char *str_rating = strtok(NULL, "\n");

        if (!title || !str_year || !str_lang || !str_rating)
            continue;

        // using atoi for converting c-style string 
        // to int (as per instruct)
        int year = atoi(str_year);
        // using atof to convert c-style to double precision
        double rating = atof(str_rating);

        // removing newline
        char lang_buff[512];
        // coyping languages and avoiding buffer overflows
        strncpy(lang_buff, str_lang, sizeof(lang_buff));
        // ensuring buffer is null terminated
        lang_buff[strcspn(lang_buff, "\n")] = '\0';

        // init 2D array
        char langs[MAX_LANG][MAX_LANG_LEN];
        int count_lang = 0;

        // initializing token with ""
        char *token = strtok(lang_buff, "[]");
        // until null or max
        while (token && count_lang < MAX_LANG) {
            strncpy(langs[count_lang++], token, MAX_LANG_LEN);
            token = strtok(NULL, "[];");
        }

        // building movie
        Movie *movie = build_movie(title, year, langs, count_lang, rating);

        // check if linked list is empty
        // if so, add and point to new movie
        if (!head) head = tail = movie;
        // else set current movie as tail's next
        else {
            tail->next = movie;
            tail = movie;
        }
        (*movie_count)++;
    }
    // closing file
    fclose(fp);
    return head;
}

// <------ Menu Option 1 ------>
// Show movies release in the specified year
void menu_option_1(Movie *head, int year) {
    int found = 0;
    while (head) {
        // if movie(s) with given year is/are found
        if (head->year == year) {
            // print title(s)
            printf("%s\n", head->title);
            found = 1;
        }
        // keep iterating
        head = head->next;
    }
    // if no movies with given year exist
    if (!found) {
        printf("No data about movies released in the year %d\n", year);
    }
}

// <------ Menu Option 2 ------>
// Show highest rated movie for each year
void menu_option_2(Movie *head) {
    // valid year range is 1900 to 2021
    int valid_years[RANGE_YEAR] = {0};
    for (Movie *b = head; b != NULL; b = b->next) {
        if (b->year < MIN_YEAR || b->year > MAX_YEAR) continue;
        if (valid_years[b->year - MIN_YEAR]) continue;

        // init max var
        Movie *max = b;
        // iterating & determining max
        for (Movie *c = b->next; c != NULL; c = c->next) {
            if (c->year == b->year && c->rating > max->rating) {
                max = c;
            }
        }
        // printing highest rating for each year to match
        // project inctruction example output (no commas)
        printf("%d %.1f %s\n", max->year, max->rating, max->title);
        valid_years[max->year - MIN_YEAR] = 1;
    }
}

// <------ Menu Option 3 ------>
// Show movies and their year of release for a specific language
void menu_option_3(Movie *head, const char *single_lang) {
    // starting from the beginning of the linked list
    Movie *current = head;
    int found = 0;
    
    while (current) {
        // init 2D array to hold langs
        char copy_langs[MAX_LANG * MAX_LANG_LEN];
        // copying langs into array
        strncpy(copy_langs, current->langs, sizeof(copy_langs));
        copy_langs[sizeof(copy_langs) - 1] = '\0';

        // init token with ";" delimeter
        char *token = strtok(copy_langs, ";");
        // until null
        while (token) {
            // tokenizing in ordere
            while (*token == ' ') token++;
            // assigning end to last token
            char *end = token + strlen(token) - 1;
            // removing whitespace
            while (end > token && *end == ' ') *end-- = '\0';

            // ignoring lang case
            if (strcasecmp(token, single_lang) == 0) {
                // if found print year and title of movies 
                // in given lang
                printf("%d %s\n", current->year, current->title);
                found = 1;
                break;
            }
            token = strtok(NULL, ";");
        }
    // keep iterating
    current = current->next;
    }

    // if no movies were found in given lang
    if (!found) {
        printf("No data about movies released in %s\n", single_lang);
    }
}

// <------ Memory Freeing Function ----->
// Gets rid of used allocated memory to open up more space for linked list
void free_memory(Movie *head) {
    while (head) {
        Movie *temp = head;
        head = head->next;
        free(temp->title);
        free(temp);
    }
}

// <-------- Main Function -------->
// calls all print statements in order to match given project example in the
// instruction screenshots, makes calls to functions, and end program
int main() {
    const char *filename = "movies_sample_1.csv";
    int movie_count = 0;

    Movie *movies = read_movie_csv(filename, &movie_count);
    if (!movies) {
        return 1;
    }

    // first line of output
    printf("Processed %s. and parsed data from %d movies\n\n", filename, movie_count);

    // options menu
    int choice;
    do {
        printf("1. Show movies released in the specified year\n");
        printf("2. Show highest rated movie for each year\n");
        printf("3. Show the title and year of release of all movies in a specific language\n");
        printf("4. Exit from the program\n\n");
        printf("Enter a choice from 1 to 4: ");

        if (scanf("%d", &choice) != 1) {
            printf("You entered an incorrect choice. Try again.\n");
            while (getchar() != '\n');
            continue;
        }
        
        getchar();

        // 1. Show movies release in the specified year
        if (choice == 1) {
            int year;
            printf("Enter the year for which you want to see movies: ");
            if (scanf("%d", &year) != 1) {
                while (getchar() != '\n');
                printf("No data about movies released in the year %d.\n", year);
                continue;
            }
            menu_option_1(movies, year);
            printf("\n");
        // 2. Show highest rated movie for each year
        } else if (choice == 2) {
            menu_option_2(movies);
            printf("\n");
        // 3. Show movies and their year of release for a specific language
        } else if (choice == 3) {
            char lang[MAX_LANG_LEN];
            printf("Enter the language for which you want to see movies: ");
            fgets(lang,sizeof(lang), stdin);
            lang[strcspn(lang, "\n")] = 0;
            menu_option_3(movies, lang);
            printf("\n");
        } else if (choice != 4) {
        printf("You entered an incorrect choice. Try again.\n\n");
        }
    } while (choice != 4);

    free_memory(movies);
    return 0;
}  
