#ifndef _SIMPLE_SHELL_H
#define _SIMPLE_SHELL_H

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <unordered_map>
using namespace std;

class simple_shell {
 public:
  unordered_map<string, string> alias_map = {};
  void parse_command(char* cmd, char** cmdTokens);
  void exec_command(char** argv);
  bool isQuit(char* cmd);
  void exec_command_pipe (int in, int out, char** cmdTokens);
  void alias(char** argv);
};

#endif