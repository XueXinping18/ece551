#include "cyoa.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>

// Default initalize the story with an empty vector
Story::Story() : pages() {
}
void Story::addPage(const Page & newPage) {
  pages.push_back(newPage);
}

// utility function to parse a variable from stringstream of form "string=long"
void getVariableFromStream(stringstream & ss, string & name, long & val) {
  getline(ss, name, '=');
  if (ss.fail() || ss.eof()) {
    throw runtime_error("No '=' sign is found while expected");
  }
  ss >> val;
  if (ss.fail()) {
    throw runtime_error("Fail to parse variable value.");
  }
}
// parse the entire story, assembles story, pages, choices accordingly
Story Story::parseStory(const string & directory) {
  string filename = directory + "/story.txt";
  // open the story.txt file
  ifstream fs(filename);
  if (!fs.good()) {
    throw runtime_error("Fail to open " + filename);
  }
  // create the story
  Story story;
  // parse each line
  string temp_str;
  while (getline(fs, temp_str)) {
    story.parseLine(temp_str, directory);
  }

  fs.close();
  return story;
}
// parse a line in story.txt and perform the appropriate action
void Story::parseLine(const string & line, const string & directoryPrefix) {
  // check if a string is blank
  bool isBlank = true;
  for (size_t i = 0; i < line.length(); i++) {
    if (line[i] != ' ') {
      isBlank = false;
      break;
    }
  }
  if (isBlank)
    return;
  /* parse the line to determine it is for a choice or page */
  stringstream ss;
  ss << line;
  // parse the first integer
  size_t pageNum;
  if (!(ss >> pageNum)) {
    throw runtime_error("Fail to read the first integer of a line");
  }
  // parse the first delimiter
  char delimiterA;
  if (!(ss.get(delimiterA))) {
    throw runtime_error("Fail to read the first delimiter of a line");
  }
  switch (delimiterA) {
    case '@': {
      // Check the page number is valid, parse the page and add the page into story
      if (pageNum != pages.size()) {
        throw runtime_error("The page numbers are not in order");
      }
      addPage(Page::parsePage(line, directoryPrefix));
      break;
    }
    case '[':
    case ':': {
      // create a choice to the corresponding page
      const Choice & tmp = Choice::parseChoice(line);
      /* check the validity of the choice */
      if (tmp.getSource() >= pages.size()) {
        throw runtime_error(
            "A choice is defined in a page that has yet to be initialized");
      }
      char sourceType = pages[tmp.getSource()].getType();
      if (sourceType == 'W' || sourceType == 'L') {
        throw runtime_error("A choice in a win or loss page is unexpectedly added");
      }
      // add the choice into the correct page
      pages[pageNum].addChoice(tmp);
      break;
    }
    case '$': {
      /* check the validity of the variable */
      if (pageNum >= pages.size()) {
        throw runtime_error("A variable is added to a uninitialzied page");
      }
      string variable_name;
      long variable_value;
      getVariableFromStream(ss, variable_name, variable_value);
      pages[pageNum].addVariable(variable_name, variable_value);
      // check if extra character appears after the number
      char extraChar;
      ss.get(extraChar);
      if (!ss.fail()) {
        throw std::runtime_error("Extra characters found after variable value");
      }
      ss.clear();
    }
    default: {
      throw runtime_error("First delimiter of a line is incorrect");
    }
  }
}

/* check if the story refers to nonexistant page, or without winning or losing page, or having redundant page */
void Story::postInstantiationCheck() {
  bool hasWinPage = false;
  bool hasLossPage = false;
  vector<bool> isReferred(pages.size(), false);

  if (!pages.empty()) {  // avoid empty story causes out-of-bound error
    isReferred[0] = true;
  }
  // mark the labels
  for (size_t i = 0; i < pages.size(); i++) {
    if (pages[i].getType() == 'W') {
      hasWinPage = true;
    }
    if (pages[i].getType() == 'L') {
      hasLossPage = true;
    }
    for (size_t j = 0; j < pages[i].getNumberOfChoices(); j++) {
      size_t dest = pages[i][j].getDest();
      if (dest >= pages.size()) {
        throw runtime_error("Nonexistant Page " + to_string(dest) +
                            " is referred by some other choice");
      }
      // mark as referred when referred by a different page
      if (dest != pages[i][j].getSource()) {
        isReferred[dest] = true;
      }
    }
  }
  // check the labels set
  if (!hasWinPage) {
    throw runtime_error("No winning page");
  }
  if (!hasLossPage) {
    throw runtime_error("No losing page");
  }
  for (size_t i = 0; i < pages.size(); i++) {
    if (!isReferred[i]) {
      throw runtime_error("Page " + to_string(i) + " is never referred by some page");
    }
  }
}
// Interactive reading with user
void Story::readInteractively() const {
  // the map contains all the user variables
  map<string, long> user_history;
  size_t currentIndex = 0;
  while (true) {
    // update the current page, user variables and display
    const Page & currentPage = pages[currentIndex];
    currentPage.attachVariableToUser(user_history);
    currentPage.displayPageToUser(user_history);
    // determine if finished and exit accordingly
    if (currentPage.getType() == 'W' || currentPage.getType() == 'L') {
      return;
    }
    // read a number from a user and check if it is a number
    while (true) {
      string userInput;
      char * endNumPointer;
      if (!(getline(cin, userInput))) {
        // if eof reached, report as an error.
        throw runtime_error("EOF is reached before the game is finished!");
      }
      long long user_input_ll = strtoll(userInput.c_str(), &endNumPointer, 10);
      // make sure the input is an appropriate number and a valid choice from the current page
      // long long is used to avoid user passing a negative value or an overflow value cast to the acceptable range
      if (user_input_ll > (long long)currentPage.getNumberOfChoices() ||
          user_input_ll <= 0 || *endNumPointer != '\0') {
        cout << "That is not a valid choice, please try again" << endl;
        // cout << "--------ERROR---------" << endl;
        // cout << "number read: " + to_string(user_input_ll) << endl;
        cin.clear();
        continue;
      }
      else {  // input is valid, fetch the choice and update the page if available
        const Choice & choice = currentPage[(size_t)(user_input_ll - 1)];
        if (choice.isAvailable(user_history)) {
          currentIndex = choice.getDest();
          break;
        }
        else {
          // case where the page is not available
          cout << "That choice is not available at this time, please try again" << endl;
          cin.clear();
        }
      }
    }
  }
}
// For a valid story, print all the winning paths without cycles
void Story::printWinningPaths() const {
  vector<vector<size_t> > container;
  // prefix for backtracking algorithm, recording all the index of choices being made
  vector<size_t> pre_decisions;
  // record whether each node is visited on the current path
  vector<bool> visited(pages.size(), false);
  visited[0] = true;
  // find all winning paths
  addWinningPathsGivenPrefix(pre_decisions, 0, visited, container);
  if (container.empty()) {
    cout << "This story is unwinnable!" << endl;
  }
  else {
    // for each winnning path print out the path
    for (size_t i = 0; i < container.size(); i++) {
      printPathToWin(container[i]);
    }
  }
}
// print out the path given the indexes of choices made
void Story::printPathToWin(vector<size_t> choices) const {
  size_t current = 0;
  size_t i = 0;
  while (i <= choices.size()) {
    cout << current;
    cout << '(';
    if (pages[current].getType() == 'W') {
      cout << "win)" << endl;
      return;
    }
    else if (pages[current].getType() == 'L') {
      throw runtime_error("Implementation Error: Lose page in winning path found!");
    }
    // case for regular page, print and jump to the next page
    cout << choices[i] + 1 << "),";
    current = pages[current][choices[i]].getDest();
    i++;
  }
}

// recursively adding all the path that starts with a vector of decisions
// backtracking algorithm is used (DFS with undos)
void Story::addWinningPathsGivenPrefix(vector<size_t> & pre_decisions,
                                       size_t currentPage,
                                       vector<bool> & visited,
                                       vector<vector<size_t> > & container) const {
  const Page & page = pages[currentPage];
  if (page.getType() == 'W') {
    vector<size_t> fullPath(pre_decisions);
    container.push_back(fullPath);
    return;
  }
  else if (page.getType() == 'L') {
    return;
  }
  // for a regular page, if a next-page is not in the path yet, find all winning paths with that next-page recursively
  for (size_t i = 0; i < page.getNumberOfChoices(); i++) {
    size_t nextPage = page[i].getDest();
    if (!visited[nextPage]) {
      visited[nextPage] = true;
      pre_decisions.push_back(i);
      addWinningPathsGivenPrefix(pre_decisions, nextPage, visited, container);
      pre_decisions.pop_back();
      visited[nextPage] = false;
    }
  }
}

ostream & operator<<(ostream & stream, const Story & story) {
  for (size_t i = 0; i < story.pages.size(); i++) {
    stream << "Page " << story.pages[i].getNum() << endl;
    stream << "==========" << endl;
    stream << story.pages[i];
  }
  return stream;
}
Page::Page() : choices() {
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
                        to_string(page.num));
  }
  if (delimiterA != '@') {
    throw runtime_error("The first delimiter of a page should be '@'");
  }
  // parse the page type
  char pageType;
  if (!(ss >> pageType)) {
    throw runtime_error("Fail to read the page type for page " + to_string(page.num));
  }
  if (pageType != 'N' && pageType != 'W' && pageType != 'L') {
    throw runtime_error("Type of page " + to_string(page.num) +
                        " in story.txt is not defined");
  }
  page.type = pageType;
  // parse the second delimiter
  char delimiterB;
  if (!(ss.get(delimiterB))) {
    throw runtime_error("Fail to read the second delimitor for page " +
                        to_string(page.num));
  }
  if (delimiterB != ':') {
    throw runtime_error("Second delimitor for page " + to_string(page.num) +
                        " is incorrect");
  }
  // parse the file name for page content
  string pageFileName;
  if (!(getline(ss, pageFileName))) {
    throw runtime_error("Fail to read the filename for the content file of page " +
                        to_string(page.num));
  }
  // parse the page content
  ifstream fs(directoryPrefix + "/" + pageFileName);
  if (!fs.good()) {
    throw runtime_error("Fail to open the page file for page " + to_string(page.num));
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
    throw runtime_error("Index for choice out of bound");
  }
  return choices[index];
}
const string & Choice::getMessage() const {
  return message;
}
size_t Choice::getSource() const {
  return source_num;
}
size_t Choice::getDest() const {
  return dest_num;
}
bool Choice::isAvailable(const map<string, long> user_history) const {
  // regular block, available to everyone
  if (!conditional) {
    return true;
  }
  // conditional block
  if (user_history.find(condition.first) == user_history.end() ||
      user_history.at(condition.first) != condition.second) {
    return false;
  }
  return true;
}
string Choice::getMessageToUser(const map<string, long> user_history) const {
  if (isAvailable(user_history)) {
    return message;
  }
  else {
    return "<UNAVAILABLE>";
  }
}
/* parse the line into a choice, without the validity of the choice being checked in the context of the story*/
Choice Choice::parseChoice(const string & line) {
  Choice choice;
  /* parse the line */
  stringstream ss;
  ss << line;
  // parse the source_num integer
  if (!(ss >> choice.source_num)) {
    throw runtime_error("Fail to read the source page number of a choice");
  };
  // parse the first delimiter
  char delimiterA;
  if (!(ss.get(delimiterA))) {
    throw runtime_error("Fail to read the first delimiter of a choice");
  }
  if (delimiterA != ':' && delimiterA != '[') {
    throw runtime_error("The first delimiter of a choice is parsed in wrong form");
  }
  // handle the conditional choice
  if (delimiterA == '[') {
    choice.conditional = true;
    string variable_name;
    long variable_value;
    getVariableFromStream(ss, variable_name, variable_value);
    choice.condition.first = variable_name;
    choice.condition.second = variable_value;
    char endBracket;
    if (!ss.get(endBracket) || endBracket != ']') {
      throw runtime_error("Failure to read ']' when parse choice");
    }
  }
  else {
    // choice is non-conditional
    choice.conditional = false;
  }
  // parse the page type
  if (!(ss >> choice.dest_num)) {
    throw runtime_error("Fail to read the destination page number of a choice");
  };

  // parse the second delimiter
  char delimiterB;
  if (!(ss.get(delimiterB))) {
    throw runtime_error("Fail to read the second delimitor for a choice");
  }
  if (delimiterB != ':') {
    throw runtime_error("Second delimitor for a choice is incorrect");
  }
  // parse the remaining part into message
  if (!getline(ss, choice.message)) {
    throw runtime_error("Fail to extract message of a choice");
  }
  return choice;
}
ostream & operator<<(ostream & stream, const Choice & choice) {
  stream << choice.message;
  return stream;
}
