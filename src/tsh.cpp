#include <tsh.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <iostream>
#include <sstream>

using namespace std;

void simple_shell::parse_command(char* cmd, char** cmdTokens) {
    istringstream cmd_stream(cmd);
    string token;
    int cmd_index = 0;
    while (cmd_stream >> token) {
        cmdTokens[cmd_index] = strdup(token.c_str());
        cmd_index++;
    }
    cmdTokens[cmd_index] = NULL;
}

void simple_shell::exec_command(char** argv) {
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        execvp(argv[0], argv);
    } else {
        int wc = wait(NULL);
	    assert(wc >= 0);
    }
}

void simple_shell::exec_command_pipe (int in, int out, char **cmdTokens) {
  int rc = fork();

  if (rc < 0) {
      fprintf(stderr, "fork failed\n");
      exit(1);
  }
  
  if (rc == 0) {
      if (in != 0) {
          dup2 (in, 0);
          close (in);
      }
      if (out != 1) {
          dup2 (out, 1);
          close (out);
      }
    execvp(cmdTokens[0], cmdTokens);
  }else {
      int wc = wait(NULL);
      assert(wc >= 0);
  }

}

bool simple_shell::isQuit(char* cmd) {
    return strcmp("quit", cmd) == 0;
}
