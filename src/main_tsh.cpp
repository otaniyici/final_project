#include <tsh.h>

using namespace std;

int main() {
  char cmd[81];
  simple_shell *shell = new simple_shell();
  cout << "tsh> ";
  while (fgets(cmd, sizeof(cmd), stdin)) {
    int fd[2];

    // get the position of the pipe character
    char* pipe_pos = strchr(cmd, '|');
    char* curr_cmd = cmd;

    int in = 0;
    while (pipe_pos) {
      char *tokens[25];
      // divide the command
      *pipe_pos = '\0';
      shell->parse_command(curr_cmd, tokens);
      // initialiaze the pipe
      pipe(fd);
      // get the input from stdin of the main file or pevious pipe's 0 and output to the next pipe's 1
      shell->exec_command_pipe(in, fd[1], tokens);
      // update the stdin of the next command
      in = fd[0];
      // update the command
      curr_cmd = pipe_pos+1;
      pipe_pos = strchr(curr_cmd, '|');
      // also close the 0 end of the pipe from parent
      close(fd[1]);
    }

  
    // save original stdout
    int d = dup(0);
    if (in != 0) {
      // if we exited the pipe loop copy and close the last pipe's 0 end
      dup2 (in, 0);
      close (in);
    }

    // execute the last command likewe normally do
    char *tokens[25];
    shell->parse_command(curr_cmd, tokens);
    if(cmd[0] != '\n') {
      if (shell->isQuit(*tokens)) {
        exit(0);
      } else {
        shell->exec_command(tokens);
      }
    }
    
    // change back to original stdout
    dup2 (d, 0);
    close(d);
    cout << "tsh> ";
  }
  cout << endl;
  exit(0);
}