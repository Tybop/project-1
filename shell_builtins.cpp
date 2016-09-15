/**
 * This file contains the implementations of the builtin functions provided by
 * the shell (or will, once you've finished implementing them all).
 */

#include "shell.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <readline/history.h>
#include <dirent.h>

using namespace std;

int Shell::com_ls(vector<string>& argv) {
  if (argv.size() < 2)
    system("dir");
  else if (argv.size() == 2){
    DIR* dir = opendir(argv[1].c_str());
    if (dir){
      chdir(argv[1].c_str());
      system("dir");
    }else{
      cout << "Directory not listed\n";
      return 1;
    }
  }
  else{
    cout << "Too many arguments\n";
    return 1;
  }
  return 0;
}


int Shell::com_cd(vector<string>& argv) {
  if (argv.size() == 2){
    chdir(argv[1].c_str());
  }else if(argv.size() == 1){
    chdir(getenv("HOME"));
  }else{
    cout << "Improper number of arguments" <<endl;
    return 1;
  }
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
  //first find = sign then create two strings one before and one after. pass as key and value to alias map else fail the input and explain proper usage
  /*if (argv.size() > 1){
    cout << "Improper entry, alias takes no arguments.\nTo store a new alias use format: string=command\nWherestring is unique and command is a shell command." << endl;
  }*/
  if (argv.size() > 1 && argv[1].find("=",1) != string::npos && !aliases.count(argv[1].substr(0, argv[1].find("=",1)))){ 
    int n = argv[1].find("=",1);
    string key = argv[1].substr(0, n);
    string value = (argv[1].substr(n+1));
    for (int i=2; i < argv.size(); i++)
      value += (" " + argv[i]);
    aliases.insert(pair<string,string>(key,value));
    cout << "Alias stored\n";
    return 0;
  }else if(argv.size() == 1){
    if (aliases.empty())
      cout << "No aliases" << endl;
    else{
      for (map<string,string>::iterator it=aliases.begin(); it!=aliases.end(); ++it)
        cout << it->first << " => " << it->second << '\n';
    }
    return 0;
  }else{
    cout << "Improper entry, alias is either empty or takes one argument.\nTo store a new alias use format: string=command\nWhere string is unique and command is a shell command." << endl;
  return 1;
  }
}


int Shell::com_unalias(vector<string>& argv) {
  if (argv.size() > 1){
    map<string, string>::iterator it;
    if (argv[1] == "-a"){
      aliases.erase(aliases.begin(), aliases.end());
    }else if(aliases.count(argv[1])){
      it = aliases.find(argv[1]);
      aliases.erase(it);
    }else{
      cout << "alias not found\n";
      return 1;
    }
    return 0;
  }else{
    cout << "Not enough arguments" << endl;
    return 1;
  }
}


int Shell::com_echo(vector<string>& argv) {
  if (argv.size() < 2){
    cout << endl;
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
    cout << i + 1 << " " << the_list[i]->line << endl;
  }
  return 0;
}


int Shell::com_exit(vector<string>& argv) {
  exit(0); //Exit Success
  return 0;
}
