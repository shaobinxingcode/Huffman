#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <iostream>
#include <fstream>
using namespace std;

int search(char* query, char* input){
	int i;
	int counter = 0;
	string encodedcode = "";
	ifstream input_file;

	input_file.open(input);
	input_file.seekg(0,ios::end);
	int length = input_file.tellg();
	input_file.seekg(0,ios::beg);

	char file_buffer[length] = {0};
	input_file.read(file_buffer,length);

	int counterArray[256] = {0};

	hQueueNode *hQueue;
	initQueue(&hQueue);

	for(i = 0; i < 1024; i += 4){
		if(int(file_buffer[i]) != 0){

			int asci_num = i/4;
			counterArray[asci_num] = int(file_buffer[i]);
			//cout << char(asci_num) << "\t" << counterArray[asci_num] << endl;
			hfmencode[asci_num] = counterArray[asci_num];

			hfmTreeNode *hNode = (hfmTreeNode*)malloc(sizeof(hfmTreeNode));
			hNode->symbol = asci_num;
			hNode->left = NULL;
			hNode->right = NULL;
			addQueueNode(&hQueue, hNode, counterArray[asci_num]);
		}
	}

	for(i = 1024; i < length; i ++){
		int uc = int((unsigned char)(file_buffer[i]));
		string dechar = "";
		int trantobinary[8] = {128,64,32,16,8,4,2,1};
		for(int j = 0; j < 8; j++){
			if(uc >= trantobinary[j]){
				dechar += "1";
				uc -= trantobinary[j];
			}
			else
				dechar += "0";
		}
		encodedcode += dechar;

	}

	//cout << encodedcode << endl;

	queueNode *q = hQueue->first;

	hHfmTreeNode* tree = crtHfmTree(&hQueue);
	hdTableNode* table = crtTable(tree);

	tableNode* t = table->first;

	map<string,int> message_map;
	map<int,string> checklist;

	string order = "";

	while(t != NULL){
		order = t->code;
		message_map[order] = t->symbol+1;
		checklist[t->symbol] = order;
		//cout << order << "\t" << message_map[order] << endl;
		counter += hfmencode[t->symbol] * order.length();
		t = t->next;
	}

	int decodedmessage[counter] = {-1};
	int messagelen = 0;
	order = "";
	for(i = 0; i < counter; i ++){
		order += encodedcode[i];
		if(message_map[order]){
			decodedmessage[messagelen] = int(message_map[order] - 1);
			messagelen ++;
			order = "";
		}
	}

	int querylen = strlen(query);
	int queryint[querylen] = {0};
	for(i = 0; i < querylen; i++){
		queryint[i] = int((unsigned char)(query[i]));
		if(checklist[queryint[i]] == "")
			return 0;
	}

	int num = 0;
	for(i = 0; i < messagelen - querylen + 1; i ++){
		int j = 0;
		int b = 1;
		for(j = 0; j < querylen; j ++){
			if(queryint[j] != decodedmessage[i + j]){
				b = 0;
				break;
			}
		}
		if(b == 1)
			num += 1;
	}
	return num;
	
	
}