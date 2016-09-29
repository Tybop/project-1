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
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

using namespace std;


int Shell::execute_external_command(vector<string>& tokens) {
  int file_status = file_redirect(tokens);
  if (file_status == 3) // Catches bad redirections
    return 1;

  int status = 0;
  pid_t pid = fork();
  pid_t waited;

  switch(pid){
    case -1:
      cout << "External call failed (fork failure).\n";
      status = 1;
      break;
    case 0:
      execlp(("/bin/"+tokens[0]).c_str(), tokens[0].c_str(),NULL);
      if (errno != 0){
        cout << "External call failed (command: " << tokens[0] << " not found)\n";
        status = 1;
      }
      break;
    default:
      waited = waitpid(pid, NULL, 0);
      file_reset(file_status);
      if (errno != 0)
        exit(0);
      return errno;
  }
}

int Shell::file_reset(int to_close){
  switch (to_close){
    case 1:
      freopen ("/dev/tty", "w", stdout);
      return 0;
    case 2:
      freopen ("/dev/tty", "r", stdin);
      return 0;
    default:
      return 0;
  }
}


int Shell::file_redirect(vector<string>& argv) {
  int fileRedirect = 0;
  int targetFile = 0;
  for (int i = 0; i < argv.size(); i++){
    if (argv[i] == ">"){
      fileRedirect = 1;
      targetFile = i + 1;
      break;
    }else if (argv[i] == ">>"){
      fileRedirect = 2;
      targetFile = i + 1;
      break;
    }else if (argv[i] == "<"){
      fileRedirect = 3;
      targetFile = i + 1;
      break;
    }
  }
  if (targetFile == argv.size())
  {
    cout << "You must supply a file for the program to access.\n";
    return 3;
  }

  switch (fileRedirect){
    case 1: // > write or overwrite
      freopen(argv[targetFile].c_str(), "w", stdout);
      return 1;
    case 2: // >> append
      freopen(argv[targetFile].c_str(), "a", stdout);
      return 1;
    case 3: // < input
      freopen(argv[targetFile].c_str(), "r", stdin);
      return 2;
    default:
      return 0;
  }



}
