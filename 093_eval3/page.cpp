#include <cstdio>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>

#include "cyoa.hpp"

Page::Page() : choices() {
}
Page::~Page() {
}
Page::Page(const Page & rhs) :
    num(rhs.num),
    type(rhs.type),
    choices(rhs.choices),
    content(rhs.content),
    variables(rhs.variables) {
}
Page & Page::operator=(const Page & rhs) {
  if (this == &rhs) {
    return *this;
  }
  num = rhs.num;
  type = rhs.type;
  choices = rhs.choices;
  content = rhs.content;
  variables = rhs.variables;
  return *this;
}
size_t Page::getNum() const {
  return num;
}
char Page::getType() const {
  return type;
}
size_t Page::getNumberOfChoices() const {
  return choices.size();
}
void Page::addChoice(const Choice & choice) {
  if (type == 'W' || type == 'L') {
    throw runtime_error("Adding choices to winning or losing page is wrong!");
  }
  choices.push_back(choice);
}
bool Page::containsDest(size_t dest) const {
  for (size_t i = 0; i < choices.size(); i++) {
    if (choices[i].getDest() == dest) {
      return true;
    }
  }
  return false;
}
void Page::addVariable(const string & s, long val) {
  variables[s] = val;
}
/* construct a page object from a line */
Page Page::parsePage(const string & line, const string & directoryPrefix) {
  Page page;
  /* parse the line */
  stringstream ss;
  ss << line;
  // parse the first integer
  if (!(ss >> page.num)) {
    throw runtime_error("Fail to read the first integer of a line");
  };
  // parse the first delimiter
  char delimiterA;
  if (!(ss.get(delimiterA))) {
    throw runtime_error("Fail to read the first delimiter for page " +
                        toString(page.num));
  }
  if (delimiterA != '@') {
    throw runtime_error("The first delimiter of a page should be '@'");
  }
  // parse the page type
  char pageType;
  if (!(ss >> pageType)) {
    throw runtime_error("Fail to read the page type for page " + toString(page.num));
  }
  if (pageType != 'N' && pageType != 'W' && pageType != 'L') {
    throw runtime_error("Type of page " + toString(page.num) +
                        " in story.txt is not defined");
  }
  page.type = pageType;
  // parse the second delimiter
  char delimiterB;
  if (!(ss.get(delimiterB))) {
    throw runtime_error("Fail to read the second delimitor for page " +
                        toString(page.num));
  }
  if (delimiterB != ':') {
    throw runtime_error("Second delimitor for page " + toString(page.num) +
                        " is incorrect");
  }
  // parse the file name for page content
  string pageFileName;
  if (!(getline(ss, pageFileName))) {
    throw runtime_error("Fail to read the filename for the content file of page " +
                        toString(page.num));
  }
  // parse the page content
  ifstream fs((directoryPrefix + "/" + pageFileName).c_str());
  if (!fs.good()) {
    throw runtime_error("Fail to open the page file for page " + toString(page.num));
  }
  // rdbuf is used to read the entire content of a stream efficiently
  stringstream content_ss;
  content_ss << fs.rdbuf();
  fs.close();
  page.content = content_ss.str();

  return page;
}

void Page::attachVariableToUser(map<string, long> & user_history) const {
  map<string, long>::const_iterator it = variables.begin();
  for (; it != variables.end(); ++it) {
    user_history[it->first] = it->second;
  }
}

void Page::displayPageToUser(const map<string, long> & user_history) const {
  cout << content;
  cout << endl;
  if (type == 'W') {
    cout << "Congratulations! You have won. Hooray!" << endl;
  }
  else if (type == 'L') {
    cout << "Sorry, you have lost. Better luck next time!" << endl;
  }
  else {
    cout << "What would you like to do?" << endl;
    cout << endl;
    for (size_t i = 0; i < choices.size(); i++) {
      cout << " " << i + 1 << ". " << choices[i].getMessageToUser(user_history) << endl;
    }
  }
}
ostream & operator<<(ostream & stream, const Page & page) {
  stream << page.content;
  stream << endl;
  if (page.type == 'W') {
    stream << "Congratulations! You have won. Hooray!" << endl;
  }
  else if (page.type == 'L') {
    stream << "Sorry, you have lost. Better luck next time!" << endl;
  }
  else {
    stream << "What would you like to do?" << endl;
    stream << endl;
    for (size_t i = 0; i < page.choices.size(); i++) {
      stream << " " << i + 1 << ". " << page.choices[i] << endl;
    }
  }
  return stream;
}
const Choice & Page::operator[](size_t index) const {
  if (index >= choices.size()) {
    throw runtime_error("Index to page out of bound!");
  }
  return choices[index];
}
