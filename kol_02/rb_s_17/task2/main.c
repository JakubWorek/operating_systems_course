#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <sys/wait.h>

#define FILE_NAME "common.txt"
#define SEM_NAME "/kol_sem"


int main(int argc, char** args){

   if(argc !=4){
    printf("Not a suitable number of program parameters\n");
    return(1);
   }

   /**************************************************
   Stworz semafor systemu V
   Ustaw jego wartosc na 1
   ***************************************************/
   key_t sem_key = ftok(SEM_NAME, 1);
   int sem_id = semget(sem_key, 1, IPC_CREAT | 0644);
   if(sem_id == -1){
      perror("semget");
      return 1;
   }
   union semun {
      int val;
   } sem_arg;
   sem_arg.val = 1;
   if(semctl(sem_id, 0, SETVAL, sem_arg) == -1){
      perror("semctl");
      return 1;
   }
   
   
   int fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC , 0644);
   
   int parentLoopCounter = atoi(args[1]);
   int childLoopCounter = atoi(args[2]);
      
   char buf[35];
   pid_t childPid;
   int max_sleep_time = atoi(args[3]);
   


   if(childPid=fork()){
      int status = 0;
      srand((unsigned)time(0)); 

      while(parentLoopCounter--){
         int s = rand()%max_sleep_time+1;
         sleep(s);    
            
         /*****************************************
          sekcja krytyczna zabezpiecz dostep semaforem
         **********************************************/
         struct sembuf sem_op;
         sem_op.sem_num = 0;
         sem_op.sem_op = -1;
         sem_op.sem_flg = 0;
         if(semop(sem_id, &sem_op, 1) == -1){
               perror("semop");
               return 1;
         }
         
         sprintf(buf, "Wpis rodzica. Petla %d. Spalem %d\n", parentLoopCounter,s);
         write(fd, buf, strlen(buf));
         write(1, buf, strlen(buf));

         sem_op.sem_op = 1;
         if(semop(sem_id, &sem_op, 1) == -1){
               perror("semop");
               return 1;
         }
               
         /*********************************
          Koniec sekcji krytycznej
         **********************************/

      }
      waitpid(childPid,&status,0);
   }
   else{
      srand((unsigned)time(0)); 
      while(childLoopCounter--){

         int s = rand()%max_sleep_time+1;
         sleep(s);                
            

         /*****************************************
         sekcja krytyczna zabezpiecz dostep semaforem
         **********************************************/
         struct sembuf sem_op;
         sem_op.sem_num = 0;
         sem_op.sem_op = -1;
         sem_op.sem_flg = 0;
         if(semop(sem_id, &sem_op, 1) == -1){
               perror("semop");
               return 1;
         }
            
         sprintf(buf, "Wpis dziecka. Petla %d. Spalem %d\n", childLoopCounter,s);
         write(fd, buf, strlen(buf));
         write(1, buf, strlen(buf));

         sem_op.sem_op = 1;
         if(semop(sem_id, &sem_op, 1) == -1){
               perror("semop");
               return 1;
         }

         /*********************************
         Koniec sekcji krytycznej
         **********************************/
      }
      _exit(0);
   }
     
   /*****************************
   posprzataj semafor
   ******************************/
   if(semctl(sem_id, 0, IPC_RMID, sem_arg) == -1){
      perror("semctl");
      return 1;
   }

   close(fd);
   return 0;
}
     
        
