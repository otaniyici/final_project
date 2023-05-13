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
        if (strcmp(argv[0], "alias")==0) {
            simple_shell::alias(argv);
            exit(0);
        }
        int e = execvp(argv[0], argv);
        if (e == -1) {
            cout << "command not found" << endl;
            exit(-1);
        }
    } else {
        int wc = wait(NULL);
	    assert(wc >= 0);
    }
}

void simple_shell::alias(char** argv) {
    for(int i = 1; argv[i] != NULL && i < 25; i++) {
        char* key = argv[i];
        char* eq_pos = strchr(key, '=');
        *eq_pos = '\0';
        char* value = eq_pos + 1;
        cout << key << endl;
        cout << value << endl;
        alias_map[key] = value;
        cout << alias_map[key] << endl;
        if(!eq_pos) {
            cout << "enter valid key=value pair" << endl;
        }
    }
    
    cout << "alias out" << endl;
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
