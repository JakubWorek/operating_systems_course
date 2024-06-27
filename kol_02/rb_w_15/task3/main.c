#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <stdint.h>
#include <sys/types.h>
#include <pthread.h>


volatile int thread_continue = 1;


void* hello(void* arg){
    sleep(1);
    while(thread_continue){
        printf("Hello world from thread number %d\n", *(int*)arg);        
		printf("Hello again world from thread number %d\n", *(int*)arg);
		sleep(2);
    }
	printf("Thread number %d is terminating\n", *(int*)arg);
	free(arg);
    return NULL;
}


int main(int argc, char** args){
	if(argc !=3){
		printf("Not a suitable number of program parameters\n");
		return(1);
	}

	int n = atoi(args[1]);

	/**************************************************
	Utworz n watkow realizujacych funkcje hello
	**************************************************/
	pthread_t threads[n]; // Tablica identyfikatorów wątków

    // Tworzenie n wątków realizujących funkcję hello
    for(int i = 0; i < n; i++){
        int* arg = malloc(sizeof(int)); // Alokacja pamięci na argument
        if(arg == NULL){
            perror("Failed to allocate memory for thread arg");
            exit(EXIT_FAILURE);
        }
        *arg = i;
        if(pthread_create(&threads[i], NULL, hello, arg) != 0){
            perror("Failed to create the thread");
            free(arg); // W przypadku błędu zwolnienie pamięci
        }
    }
	
    int i = 0;
    while(i++ < atoi(args[2])) {
		printf("Hello from main %d\n",i);
		sleep(2);
    }
	i = 0;

	/*******************************************
	"Skasuj" wszystke uruchomione watki i poczekaj na ich zakonczenie
	*******************************************/
	thread_continue = 0; // Ustawienie zmiennej na 0 spowoduje zakończenie pętli w wątkach

    // Oczekiwanie na zakończenie wszystkich wątków
    for(int i = 0; i < n; i++){
        pthread_join(threads[i], NULL);
    }

    printf("DONE");
    return 0;
}

