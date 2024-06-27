#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int pid = 0;
int command = 0;
int count = 0;

void handleSignal(int sig, siginfo_t* info, void *context) {
  if (sig == 10) {
    if (info->si_value.sival_int != 0){
      printf("Signal %d received from process: %d with value: %d\n", sig, info->si_pid, info->si_value.sival_int);
      command = info->si_value.sival_int;
    } else {
      printf("Signal %d received from process: %d \n", sig, info->si_pid);
    }
    pid = info->si_pid;
  }
}

int main(int argc, char* argv[]) {
  struct sigaction action;
  action.sa_sigaction = handleSignal;
  action.sa_flags = SA_SIGINFO;
  sigaction(SIGUSR1, &action, NULL);
  printf("PID: %d\n", getpid());
  while(1) {
    if (pid != 0) { 
      kill(pid, SIGUSR1);
      if (command == 3) {
        return 0;
      } else if (command == 2) {
        count += 1;
        printf("%d \n", count);
      } else if(command == 1) {
        count += 1;
        for (int i = 1; i<= 100; i++) printf("%d ", i);
        printf("\n");
      }
      command = 0;
      pid = 0;
    }
    sleep(1000);
  }
  return 0;
}