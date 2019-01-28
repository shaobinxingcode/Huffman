#include <iostream>
#include <string>
#include <map>
#include "encode.h"
#include "decode.h"
#include "search.h"
using namespace std;


int main(int argc, char **argv){
  if (argc != 4){ 
    cout << "Wrong input" << endl;
    return 0;
  }
  map<string, int> command_map;
  command_map["-e"] = 0; command_map["-d"] = 1; command_map["-s"] = 2;
  char *command = argv[1];
  
  switch(command_map[command]){
    case 0:{
      char *input_src_path = argv[2];
      char *output_src_path = argv[3];
      encode(input_src_path, output_src_path);
      break;
    }
    case 1:{
      char *input_src_path = argv[2];
      char *output_src_path = argv[3];
      decode(input_src_path, output_src_path);
      break;
    }
    case 2:{
      char *query_string = argv[2];
      char *file_path = argv[3];
      int num = search(query_string, file_path);
      cout << num << endl;
      break;
    }
  } 
   
  return 0;
}
