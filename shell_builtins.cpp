/**
 * This file contains the implementations of the builtin functions provided by
 * the shell (or will, once you've finished implementing them all).
 */

#include "shell.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <readline/history.h>

using namespace std;

int Shell::com_ls(vector<string>& argv) {
  if (argv.size() < 2)
    system("dir");
  else if (argv.size() == 2){ //TODO: Implement ls <directory> functionality
    //char dir = argv[1];
    //chdir(dir);
    system("dir");
  }
  else{
    cout << "Too many arguments\n";
    return 1;
  }
  return 0;
}


int Shell::com_cd(vector<string>& argv) {
  // TODO: YOUR CODE GOES HERE
  cout << "cd called" << endl; // delete when implemented
  return 0;
}


int Shell::com_pwd(vector<string>& argv) {
  if (argv.size() > 1){
    cout << "pwd doesn't take any arguments\n";
    return 1;
  }
  char cwd[1024];
  cout << getcwd(cwd, sizeof(cwd)) << endl;
  return 0;
}


int Shell::com_alias(vector<string>& argv) {
  // TODO: YOUR CODE GOES HERE
  cout << "alias called" << endl; // delete when implemented
  return 0;
}


int Shell::com_unalias(vector<string>& argv) {
  // TODO: YOUR CODE GOES HERE
  cout << "unalias called" << endl; // delete when implemented
  return 0;
}


int Shell::com_echo(vector<string>& argv) {
  if (argv.size() < 2){
    cout << "Echo will print to the screen any arguments placed after it e.g. \necho words\nwords\n";
    return 0;
  }
  for (int i = 1; i < argv.size(); i++)
    cout << argv[i] << " "; 
  cout << endl;
  return 0;
}


int Shell::com_history(vector<string>& argv) {
  if (argv.size() > 1){
    cout << "history doesn't take any arguments\n";
    return 1;
  }
  using_history();
  int length = history_length;
  register HIST_ENTRY **the_list;
  the_list = history_list();
  for(int i = 0; i < length; i++){
    cout << i << " " << the_list[i]->line << endl;
  }
  return 0;
}


int Shell::com_exit(vector<string>& argv) {
  exit(0); //Exit Success
  return 0;
}
