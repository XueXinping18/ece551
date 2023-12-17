#ifndef __CYOA_HPP__
#define __CYOA_HPP__

#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;
/* Choice class encapsulate a edge between to page*/
class Choice {
 private:
  string message;
  size_t source_num;
  size_t dest_num;
  // if displayed unconditionally, conditional is false;
  pair<string, long> condition;
  bool conditional;

 public:
  Choice();
  static Choice parseChoice(const string & line);
  friend ostream & operator<<(ostream & stream, const Choice & choice);
  size_t getSource() const;
  size_t getDest() const;
  const string & getMessage() const;
  // used for step4: display while considering the condition
  string getMessageToUser(const map<string, long> user_history) const;
  // decide the avaiability of a choice according to user history
  bool isAvailable(const map<string, long> user_history) const;
  Choice(const Choice & rhs);
  Choice & operator=(const Choice & rhs);
  virtual ~Choice();
};
// Page object encapsulate a node in a story
class Page {
 private:
  // The unique identifier of page in a story, pageNum
  size_t num;
  // The type of the page
  char type;
  vector<Choice> choices;
  string content;
  // The variables and their values attached to the page
  map<string, long> variables;

 public:
  Page();
  char getType() const;
  static Page parsePage(const string & line, const string & storyDirectory);
  friend ostream & operator<<(ostream & stream, const Page & page);
  void addChoice(const Choice & choice);
  size_t getNum() const;
  size_t getNumberOfChoices() const;
  bool containsDest(size_t dest) const;
  const Choice & operator[](size_t index) const;
  // used in step 4: print message while considering user history
  void displayPageToUser(const map<string, long> & user_history) const;
  // attach all variables of the page to user
  void attachVariableToUser(map<string, long> & user_history) const;
  // attach a variable to a page
  void addVariable(const string & s, long val);
  Page(const Page & rhs);
  Page & operator=(const Page & rhs);
  virtual ~Page();
};

class Story {
 private:
  vector<Page> pages;
  void addPage(const Page & newPage);
  void parseLine(const string & newLine, const string & storyDirectory);
  void addWinningPathsGivenPrefix(vector<size_t> & pre_decisions,
                                  size_t currentPage,
                                  vector<bool> & visited,
                                  vector<vector<size_t> > & container) const;
  void printPathToWin(vector<size_t> choicesVec) const;

 public:
  Story();
  static Story parseStory(const string & storyDirectory);
  void postInstantiationCheck();
  friend ostream & operator<<(ostream & stream, const Story & story);
  void readInteractively() const;
  void printWinningPaths() const;
  Story(const Story & rhs);
  Story & operator=(const Story & rhs);
  virtual ~Story();
};
// Utility functions
string toString(size_t x);
void getVariableFromStream(stringstream & ss, string & name, long & val);

#endif
