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
        if (simple_shell::alias_map.find(cmdTokens[cmd_index]) != simple_shell::alias_map.end()) {
            string alias = simple_shell::alias_map[cmdTokens[cmd_index]];
            istringstream alias_stream(alias);
            while (alias_stream >> token) {
                cmdTokens[cmd_index] = strdup(token.c_str());
                cmd_index++;
            }
        }else {
            cmd_index++;
        }
        
    }
    cmdTokens[cmd_index] = NULL;
}

void simple_shell::exec_command(char** argv) {
    if (strcmp(argv[0], "alias")==0) {
        simple_shell::alias(argv);
        return;
    }
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
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
    // if no arg or with -p flag print the key=value pairs
    if(argv[1] == NULL || strcmp(argv[1], "-p") == 0) {
        for (const auto entry : simple_shell::alias_map) {
            cout << "alias " << entry.first << "=" << entry.second << endl;
        }
        return;
    }
    // add new key=value pairs to the hashmap
    for(int i = 1; argv[i] != NULL && i < 25; i++) {
        char* key = argv[i];
        char* eq_pos = strchr(key, '=');
        *eq_pos = '\0';
        char* value = eq_pos + 1;
        simple_shell::alias_map[key] = value;
        if(!eq_pos) {
            cout << "enter valid key=value pair" << endl;
        }
    }
}

void simple_shell::exec_command_pipe (int in, int out, char **cmdTokens) {
  int rc = fork();

  if (rc < 0) {
      fprintf(stderr, "fork failed\n");
      exit(1);
  }
  // copy and close the pipes
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
