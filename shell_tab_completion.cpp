/**
 * This file contains implementations of the functions that provide
 * tab-completion for the shell.
 *
 * You will need to finish the implementations of the completion functions,
 * though you're spared from implementing the high-level readline callbacks with
 * their weird static variables...
 */

#include "shell.h"
#include <cstdlib>
#include <iostream>
#include <readline/readline.h>
#include <readline/history.h>

using namespace std;

/**
   * Populates the given matches vector with all the ENV and local variables
   * that match the given text.
   *
   * @param text The text against which to match
   * @param matches The vector to fill with matching variables
   */
void Shell::get_env_completions(const char* text, vector<string>& matches) {
  // Gets environment variable
  for (char **en=environ; *en; en++){
      string tmp(*en);
      size_t index = tmp.find_first_of("=");
      tmp = "$" + tmp.substr(0, index);
      if(tmp.find(text) == 0){
        matches.push_back(tmp);
      }
  }
  // Gets local variables
  for (map<string,string>::iterator it=localvars.begin(); it!=localvars.end(); ++it){
    string tmp = text;
    if (it->first.find(tmp.substr(1)) == 0)
      matches.push_back("$" + it->first);
  }
  // Gets aliases
  for (map<string,string>::iterator it=aliases.begin(); it!=aliases.end(); ++it){
    string tmp = text;
    if (it->first.find(tmp.substr(1)) == 0)
      matches.push_back("$" + it->first);
  }

}

/**
   * Populates the given matches vector with all the 1) builtin functions,
   * 2) aliases, and 3) executable files on the user's $PATH that match the
   * given text.
   *
   * @param text The text against which to match
   * @param matches The vector to fill with matching variables
   */
void Shell::get_command_completions(const char* text, vector<string>& matches) {
  for (map<string, builtin_t>::iterator it=builtins.begin(); it!=builtins.end(); ++it){
    if (it->first.find(text) == 0)
      matches.push_back(it->first);
  }
  // TODO: Make an iterator through $PATH object maybe use opendir to get list of
  // contents then check if !directory and complete if so

}


char** Shell::word_completion(const char* text, int start, int end) {
  char** matches = NULL;

  if (text[0] == '$') {
    matches = rl_completion_matches(text, env_completion_generator);
  } else if (start == 0) {
    matches = rl_completion_matches(text, command_completion_generator);
  } else {
    // We get directory matches for free (thanks, readline!).
  }

  return matches;
}


char* Shell::env_completion_generator(const char* text, int state) {
  // A list of all the matches.
  // Must be static because this function is called repeatedly.
  static vector<string> matches;

  // If this is the first time called, construct the matches list with
  // all possible matches.
  if (state == 0) {
    getInstance().get_env_completions(text, matches);
  }

  // Return a single match (one for each time the function is called).
  return pop_match(matches);
}


char* Shell::command_completion_generator(const char* text, int state) {
  // A list of all the matches.
  // Must be static because this function is called repeatedly.
  static vector<string> matches;

  // If this is the first time called, construct the matches list with
  // all possible matches.
  if (state == 0) {
    getInstance().get_command_completions(text, matches);
  }

  // Return a single match (one for each time the function is called).
  return pop_match(matches);
}


char* Shell::pop_match(vector<string>& matches) {
  if (matches.size() > 0) {
    const char* match = matches.back().c_str();

    // Delete the last element.


    // We need to return a copy, because readline deallocates when done.
    char* copy = (char*) malloc(strlen(match) + 1);
    strcpy(copy, match);
    matches.pop_back();
    return copy;
  }

  // No more matches.
  return NULL;
}
