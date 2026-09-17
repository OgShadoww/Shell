#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef enum {
  INFO_CODE,
  TIME_CODE,
  CLEAR_CODE
} parse_codes;

char *parse_commands[] = {
  [INFO_CODE] = "information",
  [TIME_CODE] = "time",
  [CLEAR_CODE] = "clear",
  NULL
};

int read_command(char *buff) {
  int n;

  n = read(STDIN_FILENO, buff, 127);
  if(n < 0) return -1;
  if(n > 128) return -1;

  if(buff[n-1] == '\n') {
    buff[n-1] = '\0';
  }
  else {
    buff[n] = '\0';
  }

  return 1;
}

int lookup_command(char *command) {
  for(int i = 0; parse_commands[i] != NULL; i++) {
    if(strcmp(command, parse_commands[i]) == 0) {
      return i;
    }
  }

  return -1;
}

int parse_command(char *buff, char *command, char *argv[]) {
  for(int i = 0; i < strlen(buff); i++) {
    if(buff[i] == ' ') {
      strncpy(command, buff, i-1);
    }
  }

  strncpy(command, buff, strlen(buff));

  return 0;
}

int handle_command(int parse_code, char* argv[]) {
  switch (parse_code) {
    case INFO_CODE: {
      printf("Information: \n");
      fflush(stdout);
      break;
    }
    case TIME_CODE: {
      printf("");
      fflush(stdout);
      break;
    }
    case CLEAR_CODE: {
      printf("\033[2J");
      fflush(stdout);
      printf("\033[1;1H");
      fflush(stdout);
    }
    default:
      break;
  }

  return 0;
}

int main() {
  printf("> ");
  fflush(stdout);
  char buff[256];
  char command[128];
  char *argv[4];
  int parse_code;

  while(1) {
    read_command(buff);
    parse_command(buff, command, argv);
    parse_code = lookup_command(command);
    handle_command(parse_code, argv);
    
    printf("> ");
    fflush(stdout);
    memcpy(buff, "", 0);
    memcpy(command, "", 0);
    for(int i = 0; argv[i] != NULL; i++) {
      memcpy(argv[i], "", 0);
    }
    parse_code = -1;
  }

  return 0;
}
