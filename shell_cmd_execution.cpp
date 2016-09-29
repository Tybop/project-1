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
  vector<int> file_status = check_file_redirect(tokens);
  int pipe_status = check_for_pipes(tokens);
  if (file_status[0] == -1 || pipe_status == 2) // Catches bad redirections and conflicting pipe calls
  return 1;

  int file_code = file_redirect(tokens, file_status);


  pid_t pid = fork();
  pid_t waited;

  switch(pid){
    case -1:
    cout << "External call failed (fork failure).\n";
    break;
    case 0:
    execlp(("/bin/"+tokens[0]).c_str(), tokens[0].c_str(),NULL);
    if (errno != 0){
      cout << "External call failed (command: " << tokens[0] << " not found)\n";
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

int Shell::check_for_pipes(vector<string> argv){
  int retVal = 0;
  for (int i = 0; i < argv.size(); i++){
    if (argv[i] == "|")
    retVal = 1;
    if (retVal == 1 && argv[i] == "<"){
    cout << "You cannot pipe and read in from a file at the same time.\n";
    retVal = 2;
    }
  }
  return retVal;
}

int Shell::handle_pipes(vector<string>& argv){

  int numCmds = 0;
  int pipeArr[2];

  for (int i = 0; i < argv.size(); i++){
    if (argv[i] == "|"){
			numCmds++;
		}
  }numCmds++;



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

vector<int> Shell::check_file_redirect(vector<string>& argv){
  vector<int> retVal;
  for (int i = 0; i < argv.size(); i++){
    if (argv[i] == "|"){
      retVal.push_back(-1);
      cout << "The out put from a piped command is inputed into the next command not an external file.\n";
      break;
    }else if (argv[i] == ">"){
      retVal.push_back(1);
      retVal.push_back(i+1);
      break;
    }else if (argv[i] == ">>"){
      retVal.push_back(2);
      retVal.push_back(i+1);
      break;
    }else if (argv[i] == "<"){
      retVal.push_back(3);
      retVal.push_back(i+1);
      break;
    }
  }
  if (targetFile == argv.size())
  {
    cout << "You must supply a file for the program to access.\n";
    retVal[0] = -1;
  }
  return retVal;
}

int Shell::file_redirect(vector<string>& argv, vector<int> typeTarget) {
  switch (typeTarget[0]){
    case 1: // > write or overwrite
    freopen(argv[typeTarget[1]].c_str(), "w", stdout);
    return 1;
    case 2: // >> append
    freopen(argv[typeTarget[1]].c_str(), "a", stdout);
    return 1;
    case 3: // < input
    freopen(argv[typeTarget[1]].c_str(), "r", stdin);
    return 2;
    default:
    return 0;
  }



}
