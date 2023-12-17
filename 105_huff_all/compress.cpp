#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <functional>
#include <iostream>
#include <map>
#include <queue>

#include "node.h"
#include "readFreq.h"

void writeHeader(BitFileWriter * bfw, const std::map<unsigned, BitString> & theMap) {
  for (int i = 0; i < 257; i++) {
    std::map<unsigned, BitString>::const_iterator it = theMap.find(i);
    if (it != theMap.end()) {
      bfw->writeByte(it->second.size());
      bfw->writeBitString(it->second);
    }
    else {
      bfw->writeByte(0);
    }
  }
}

#include <fstream>

void writeCompressedOutput(const char * inFile,
                           const char * outFile,
                           const std::map<unsigned, BitString> & theMap) {
  BitFileWriter bfw(outFile);
  writeHeader(&bfw, theMap);

  std::ifstream input(inFile, std::ifstream::in);
  char c;
  while (input.get(c)) {
    std::map<unsigned, BitString>::const_iterator it = theMap.find((unsigned char)c);
    if (it != theMap.end()) {
      bfw.writeBitString(it->second);
    }
    else {
      exit(EXIT_FAILURE);
    }
  }

  // Write EOF marker
  std::map<unsigned, BitString>::const_iterator eof_it = theMap.find(256);
  if (eof_it != theMap.end()) {
    bfw.writeBitString(eof_it->second);
  }

  input.close();
}

int main(int argc, char ** argv) {
  if (argc != 3) {
    std::cerr << "Usage: compress input output\n";
    return EXIT_FAILURE;
  }

  const char * inFile = argv[1];
  const char * outFile = argv[2];

  uint64_t * freqs = readFrequencies(inFile);
  if (freqs == NULL) {
    std::cerr << "Error reading frequencies from input file\n";
    return EXIT_FAILURE;
  }

  // Build the Huffman tree
  Node * root = buildTree(freqs);
  delete[] freqs;
  if (root == NULL) {
    std::cerr << "Error building Huffman tree\n";
    return EXIT_FAILURE;
  }

  // Build the encoding map
  std::map<unsigned, BitString> theMap;
  BitString empty;
  root->buildMap(empty, theMap);

  writeCompressedOutput(inFile, outFile, theMap);

  delete root;

  return EXIT_SUCCESS;
}
