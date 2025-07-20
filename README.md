# Programming Assignment 2 : Movies
- Reads a CSV file with movie data that is provided to the program as an argument
- Processes the data in the file to create structs to hold data for each movie
- Creates a linked list of all these structs
- Gives user choices to ask questions about the movies in the data
- Prints out the data about the movies per user choice

## Structure:
Using a linked list structure to dynamically allocate data. Linked lists are of the form:

        head           node            tail
    [data:next] --> [data:next] --> [data:next] --> NULL  