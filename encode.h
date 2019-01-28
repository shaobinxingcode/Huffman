#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <iostream>
#include <fstream>
#include "global.h"
using namespace std;

void add_bit(char* bitarray, int k){
    int i = k / 8;
    int j = k % 8;
    bitarray[i] = ((0x80 >> j) | bitarray[i]);
}

void encode(char* input, char* output){
	ifstream inputfile;
	FILE *output_file = fopen(output,"wb");
	int length;
	int i;

	inputfile.open(input);
	inputfile.seekg(0,ios::end);
	length = inputfile.tellg();
	inputfile.seekg(0,ios::beg);

	char s[length] = {0};
	inputfile.read(s,length);
	inputfile.close();

	for(i = 0; i < length; i++){
		countArray[(unsigned char)s[i]] ++;
	}
	
	hQueueNode *hQueue;
	initQueue(&hQueue);

	for(i = 0; i < 256; i++){
		if(countArray[i]){
			hfmTreeNode *hNode = (hfmTreeNode*)malloc(sizeof(hfmTreeNode));

			hNode->symbol = i;
			hNode->left = NULL;
			hNode->right = NULL;

			addQueueNode(&hQueue, hNode, countArray[i]);
		}
	}
	queueNode* q = hQueue->first;

	hHfmTreeNode* tree = crtHfmTree(&hQueue);
	hdTableNode* table = crtTable(tree);

	tableNode* t = table->first;
	map<int,char*> encode_map;

	while(t != NULL){
		encode_map[t->symbol] = t->code;
		//cout << t->symbol << "\t" << t->code << endl;
		t = t->next;
	}

	string encode_file;
	output_file = fopen(output, "wb");
	fwrite(countArray,sizeof(int),256,output_file);
	free(countArray);
	int order;

	for(i = 0; i < length; i++){
		order = int((unsigned char)s[i]);
		encode_file += encode_map[order];
	}

	char* temp = (char*)malloc(int(encode_file.length())*sizeof(char));
	for(i = 0; i < int(encode_file.length()); i++){
		temp[i] = encode_file[i];
	}	

	int blocknum = encode_file.length()/8;
	if(encode_file.length()%8 != 0){
		blocknum += 1;
	}

	char *bitarray = (char*)malloc(int(encode_file.length())*sizeof(char));
	memset(bitarray,0,int(encode_file.length()));
	for(i = 0; i < encode_file.length(); i++){
		if(temp[i] == '1'){
			add_bit(bitarray,i);
		}
	}

	//cout << encode_file << endl;
	fwrite(bitarray,sizeof(char),blocknum,output_file);
	fclose(output_file);
}