#include <signal.h>
#include <assert.h>
#include <stdio.h>

char running = 1;

static void catch_function(int signo)
{
  switch(signo)
  {
  case SIGTERM:
  case SIGINT:
    puts("caught signal");
    running = 0;
    break;
  default:
    raise(signo);
  }
}

void setup_signal(void)
{
  assert(signal(SIGINT, catch_function) != SIG_ERR);
}
