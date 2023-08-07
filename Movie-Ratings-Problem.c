#include <stdio.h>

#include <stdlib.h>

#include <time.h>

#include <pthread.h>

#include <semaphore.h>

#include <string.h>

#define FILENAME "movie-database.txt"

sem_t mutex;

int count = 0;

typedef struct movie
{
    char move_name[100], director[100];

    float rating;

    int year;

} movie;

void *DoSomeThing(void *arg)
{
    char keysword[100];

    strcpy(keysword, (char *)arg);

    sem_wait(&mutex);

    sleep(3);

    movie moviesArr[100];

    int currentIndex = -1;

    int total = 0;

    char move_name[100], director[100];

    float rating;

    int year;

    FILE *fptr;

    if ((fptr = fopen(FILENAME, "r")) == NULL)
    {
        printf("Error! opening file");

        exit(1);
    }

    while (fscanf(fptr, "%s %s %f %d", move_name, director, &rating, &year) != EOF)
    {
        if (strstr(move_name, keysword) != NULL)
        {
            movie tmovie;

            strcpy(tmovie.director, director);

            strcpy(tmovie.move_name, move_name);

            tmovie.rating = rating;

            tmovie.year = year;

            moviesArr[++currentIndex] = tmovie;

            total++;
        }
    }

    for (int i = 0; i < total; i++)
    {
        for (int j = 0; j < total; j++)
        {
            if (moviesArr[i].rating > moviesArr[j].rating)
            {
                movie tmovie = moviesArr[i];

                moviesArr[i].rating = moviesArr[j].rating;

                moviesArr[i].year = moviesArr[j].year;

                strcpy(moviesArr[i].move_name, moviesArr[j].move_name);

                strcpy(moviesArr[i].director, moviesArr[j].director);

                moviesArr[j].rating = tmovie.rating;

                moviesArr[j].year = tmovie.year;

                strcpy(moviesArr[j].move_name, tmovie.move_name);

                strcpy(moviesArr[j].director, tmovie.director);
            }
        }
    }

    count++;

    printf("\nResult of %d\n", count);

    for (int i = 0; i < total; i++)
    {
        printf("%s %s %.1f %d \n", moviesArr[i].move_name, moviesArr[i].director, moviesArr[i].rating, moviesArr[i].year);
    }

    sem_post(&mutex);
}

int main()
{
    int n;

    n = 7;

    pthread_t pid[n];

    if (sem_init(&mutex, 0, 5) == -1)
    {
        perror("Could not initialize mylock semaphore");

        exit(2);
    }

    char k[100];

    printf("Input keyword: ");
    
    scanf("%s", k);

    for (int i = 0; i < n; i++)
    {
        pthread_create(&pid[i], NULL, DoSomeThing, (void *)k);
    }

    for (int i = 0; i < n; i++)
    {
        pthread_join(pid[i], NULL);
    }
}
