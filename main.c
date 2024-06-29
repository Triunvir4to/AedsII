#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "HTFacilities.h"
#include "HashTable.h"
#include "HashTableIterator.h"

// Definição da estrutura para armazenar os dados do filme

typedef struct {
    char *movie_id;
    char *movie_name;
    int year;
    char *certificate;
    int run_time;
    char *genre;
    float rating;
    char *description;
    char *director;
    char *director_id;
    char *star;
    char *star_id;
    float gross;
    int votes;
} Movie;

// Função para remover aspas duplas de uma string
void remove_quotes(char* field) {
    int length = strlen(field);
    for (int i = 0; i < length; i++) {
        if (field[i] == '"') {
            field[i] = '\0';
        }
    }
}

char* get_csv_field(char* line, int* pos, FILE* file) {
    int start = *pos;
    int length = 0;
    int in_quotes = 0;

    if (line[*pos] == '"') {
        in_quotes = 1;
        (*pos)++;
        start++;
    }

    while (line[*pos] != '\0' && (line[*pos] != ',' || in_quotes)) {
        if (line[*pos] == '"') {
            if (line[*pos + 1] == '"') {
                // Escaped quote
                (*pos)++;
            } else {
                in_quotes = !in_quotes;
            }
        }
        (*pos)++;
        length++;
    }

    if (line[*pos] == ',') {
        (*pos)++;
    }

    char* field = (char*)malloc((length + 1) * sizeof(char));
    if (field == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strncpy(field, &line[start], length);
    field[length] = '\0';

    // Handle multi-line fields
    while (in_quotes) {
        if (field[strlen(field) - 1] == '"') {
            in_quotes = 0;
            field[strlen(field) - 1] = '\0';  // Remove the closing quote
        } else {
            if (fgets(line, 10000, file) == NULL) {
                break;
            }
            *pos = 0;
            size_t new_length = strlen(field) + strlen(line) + 1;
            field = (char*)realloc(field, new_length);
            if (field == NULL) {
                printf("Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
            strcat(field, line);

            while (line[*pos] != '\0' && (line[*pos] != ',' || in_quotes)) {
                if (line[*pos] == '"') {
                    in_quotes = !in_quotes;
                }
                (*pos)++;
            }

            if (line[*pos] == ',') {
                (*pos)++;
            }
        }
    }

    // Remove surrounding quotes if any
    if (field[0] == '"' && field[strlen(field) - 1] == '"') {
        field[strlen(field) - 1] = '\0';
        memmove(field, &field[1], strlen(field));
    }

    remove_quotes(field);

    return field;
}

void print_movie(Movie *movie) {
    printf("Movie ID: %s\n", movie->movie_id);
    printf("Movie Name: %s\n", movie->movie_name);
    printf("Year: %d\n", movie->year);
    printf("Certificate: %s\n", movie->certificate);
    printf("Run Time: %d\n", movie->run_time);
    printf("Genre: %s\n", movie->genre);
    printf("Rating: %.1f\n", movie->rating);
    printf("Description: %s\n", movie->description);
    printf("Director: %s\n", movie->director);
    printf("Director ID: %s\n", movie->director_id);
    printf("Star: %s\n", movie->star);
    printf("Star ID: %s\n", movie->star_id);
    printf("Gross: %.2f\n", movie->gross);
    printf("Votes: %d\n", movie->votes);
}

Movie* load_movies(const char* filename, int* size) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        return NULL;
    }

    int capacity = 10;
    *size = 0;
    Movie* movies = (Movie*)malloc(capacity * sizeof(Movie));
    if (!movies) {
        printf("Error allocating memory\n");
        fclose(file);
        return NULL;
    }

    char line[10000];
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("Error reading header\n");
        free(movies);
        fclose(file);
        return NULL;
    }

    while (fgets(line, sizeof(line), file)) {
        if (*size >= capacity) {
            capacity *= 2;
            movies = (Movie*)realloc(movies, capacity * sizeof(Movie));
            if (!movies) {
                printf("Error reallocating memory\n");
                fclose(file);
                return NULL;
            }
        }

        Movie movie;
        int pos = 0;

        // movie_id
        movie.movie_id = get_csv_field(line, &pos, file);

        // movie_name
        movie.movie_name = get_csv_field(line, &pos, file);

        // year
        movie.year = atoi(get_csv_field(line, &pos, file));

        // certificate
        movie.certificate = get_csv_field(line, &pos, file);

        // run_time
        char *run_time_str = get_csv_field(line, &pos, file);
        movie.run_time = atoi(run_time_str);
        free(run_time_str);

        // genre
        movie.genre = get_csv_field(line, &pos, file);

        // rating
        movie.rating = atof(get_csv_field(line, &pos, file));

        // description
        movie.description = get_csv_field(line, &pos, file);

        // director
        movie.director = get_csv_field(line, &pos, file);

        // director_id
        movie.director_id = get_csv_field(line, &pos, file);

        // star
        movie.star = get_csv_field(line, &pos, file);

        // star_id
        movie.star_id = get_csv_field(line, &pos, file);

        // gross
        movie.gross = atof(get_csv_field(line, &pos, file));

        // votes
        movie.votes = atoi(get_csv_field(line, &pos, file));

        movies[*size] = movie;
        (*size)++;
    }

    fclose(file);
    return movies;
}

// Função para realizar a pesquisa sequencial
Movie* sequential_search(Movie* movies, int size, const char* movie_id, int* access_count) {
    *access_count = 0;
    for (int i = 0; i < size; i++) {
        (*access_count)++;
        if (strcmp(movies[i].movie_id, movie_id) == 0) {
            return &movies[i];
        }
    }
    return NULL;
}

// Função para pausar o console
void pause_console() {
    printf("Pressione Enter para continuar...");
    getchar();
}

// Função para liberar a memória alocada
void free_movies(Movie* movies, int size) {
    for (int i = 0; i < size; i++) {
        free(movies[i].movie_id);
        free(movies[i].movie_name);
        free(movies[i].certificate);
        free(movies[i].genre);
        free(movies[i].description);
        free(movies[i].director);
        free(movies[i].director_id);
        free(movies[i].star);
        free(movies[i].star_id);
    }
    free(movies);
}

// Função para exibir a interface de console
bool console_interface() {
    char input[10];
    int option;
    char filename[150];
    const char* folder = "../categories/";

    printf("Escolha uma categoria:\n");
    printf("1. Ação\n");
    printf("2. Aventura\n");
    printf("3. Animação\n");
    printf("4. Biografia\n");
    printf("5. Crime\n");
    printf("6. Família\n");
    printf("7. Fantasia\n");
    printf("8. Film-Noir\n");
    printf("9. História\n");
    printf("10. Horror\n");
    printf("11. Mistério\n");
    printf("12. Romance\n");
    printf("13. Ficção Científica\n");
    printf("14. Esportes\n");
    printf("15. Thriller\n");
    printf("16. Guerra\n");
    printf("17. Sair\n");

    fgets(input, sizeof(input), stdin);
    char *endptr;
    option = strtol(input, &endptr, 10);

    if (endptr == input || *endptr != '\n' || errno == ERANGE) {
        printf("Opção inválida.\n");
        return true;
    }

    switch (option) {
        case 1:
            strcpy(filename, "action.csv");
            break;
        case 2:
            strcpy(filename, "adventure.csv");
            break;
        case 3:
            strcpy(filename, "animation.csv");
            break;
        case 4:
            strcpy(filename, "biography.csv");
            break;
        case 5:
            strcpy(filename, "crime.csv");
            break;
        case 6:
            strcpy(filename, "family.csv");
            break;
        case 7:
            strcpy(filename, "fantasy.csv");
            break;
        case 8:
            strcpy(filename, "film-noir.csv");
            break;
        case 9:
            strcpy(filename, "history.csv");
            break;
        case 10:
            strcpy(filename, "horror.csv");
            break;
        case 11:
            strcpy(filename, "mystery.csv");
            break;
        case 12:
            strcpy(filename, "romance.csv");
            break;
        case 13:
            strcpy(filename, "scifi.csv");
            break;
        case 14:
            strcpy(filename, "sports.csv");
            break;
        case 15:
            strcpy(filename, "thriller.csv");
            break;
        case 16:
            strcpy(filename, "war.csv");
            break;
        case 17:
            printf("Saindo...\n");
            return false;
        default:
            printf("Opção inválida.\n");
            return true;
    }

    char full_path[200];
    snprintf(full_path, sizeof(full_path), "%s%s", folder, filename);

    int size;
    Movie* movies = load_movies(full_path, &size);
    if (!movies) {
        printf("Erro ao carregar os filmes.\n");
        pause_console();
        return true;
    }
    HashTable *hashMovies = newHash(sizeof(char *), sizeof(Movie),
                                    hash_string, compare_string);

    for(int i = 0; i < size; ++i)
        HTSet(hashMovies, movies[i].movie_id, &movies[i]);

    //DEBUG
//    HashTableIterator iterator = newHTIterator(hashMovies);
//
//    while(nextHTI(&iterator)){
//        Movie *movie = (Movie *)iterator.value;
//        printf("%s ", movie->movie_id);
//    }


    char *movie_id;
    size_t buffer_size = 0;
    printf("\n\n\nDigite o ID do filme para buscar: ");
    getline(&movie_id, &buffer_size, stdin);
    movie_id[strcspn(movie_id, "\n")] = '\0';

    int linear_access_count, hash_access_count;
    Movie *result = (Movie *)HTget(hashMovies, movie_id, &hash_access_count);
    sequential_search(movies, size, movie_id, &linear_access_count);

    if (result) {
        print_movie(result);
        printf("Acessos Lineares: %d\n", linear_access_count);
        printf("Acessos Hashing: %d\n", hash_access_count);
    } else
        printf("Filme não encontrado.\n");


    free_movies(movies, size);
    free_table(hashMovies);
    free(movie_id);
    pause_console();
    return true;
}

int main() {
    bool isRunning = true;
    while (isRunning) {
        isRunning = console_interface();
    }
    return 0;
}
