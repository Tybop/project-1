/**
* This file contains implementations of the functions that are responsible for
* executing commands that are not builtins.
*
* Much of the code you write will probably be in this file. Add helper methods
* as needed to keep your code clean. You'll lose points if you write a single
* monolithic function!
*/

#include "shell.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>

using namespace std;


int Shell::execute_external_command(vector<string>& tokens) {

  int status;
  pid_t pid = fork();
  pid_t waited;

  switch(pid){
    case -1:
      cout << "External call failed (fork failure).\n";
      return 1;
    case 0:
      execlp(("/bin/"+tokens[0]).c_str(), tokens[0].c_str(),NULL);
      waited = waitpid(pid, &status,WIFEXITED);
      break;
    default:
      break;
  }
}
