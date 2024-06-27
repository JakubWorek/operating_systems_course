#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

int digit = 0;
int pid = 0;
bool is_answer = false;

void handleSignal() {
  is_answer = true;
  printf("received answer from catcher \n");
};

int main(int argc, char* args[]) {
  digit = atoi(args[2]);
  pid = atoi(args[1]);
  printf("Sender PID: %d\n", getpid());
  signal(SIGUSR1, handleSignal);
  kill(pid, SIGUSR1);
  if (handleSignal == false) {
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigsuspend(&mask);
  }
  sleep(1000);
  union sigval value;
  value.sival_int = digit; 
  sigqueue(pid, SIGUSR1, value);
  return 0;
}