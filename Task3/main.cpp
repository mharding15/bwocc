#include <stdio.h>
#include <iostream>
#include <string.h>
#include "bit_vector.h"
#include "rank_support.h"
#include "select_support.h"
#include "utils.h"

using namespace std;

int main(int argc, char *argv[]){
  if (strncmp(argv[1], "build", 5) == 0){
    utils u;
    u.createOccTable(argv[2], argv[3]);
  } else if (strncmp(argv[1], "query", 5) == 0){
    utils u;
    u.useOccTable(argv[2], argv[3]);
  } else {
    cout << "It was not build or query??";
  }
  return 0;
}
