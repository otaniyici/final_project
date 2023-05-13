#include <tsh.h>

using namespace std;

int main() {
  char cmd[81];
  simple_shell *shell = new simple_shell();
  cout << "tsh> ";
  while (fgets(cmd, sizeof(cmd), stdin)) {
    int fd[2];
    char* pipe_pos = strchr(cmd, '|');
    char* curr_cmd = cmd;

    int in = 0;
    while (pipe_pos) {
      char *tokens[25];
      *pipe_pos = '\0';
      shell->parse_command(curr_cmd, tokens);
      pipe(fd);
      shell->exec_command_pipe(in, fd[1], tokens);
      in = fd[0];
      curr_cmd = pipe_pos+1;
      pipe_pos = strchr(curr_cmd, '|');
      close(fd[1]);
    }
  
    int d = dup(0);
    if (in != 0) {
      dup2 (in, 0);
      close (in);
    }

    char *tokens[25];
    shell->parse_command(curr_cmd, tokens);
    if(cmd[0] != '\n') {
      if (shell->isQuit(*tokens)) {
        exit(0);
      } else {
        shell->exec_command(tokens);
      }
    }
    
    dup2 (d, 0);
    close(d);
    cout << "tsh> ";
  }
  cout << endl;
  exit(0);
}