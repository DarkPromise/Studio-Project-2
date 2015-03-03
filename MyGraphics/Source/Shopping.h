#ifndef SHOPPING_H
#define SHOPPING_H_H

#include<string>
#include<vector>
#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 
#include "Item.h"
using std::string;
using std::vector;

class Shopping
{
public:
	vector<int> allItemType;
	vector<int> listChecker;
	vector<string> list;

	vector<int> ifGotLiao;
	bool getDiff;

	Shopping(void);
	~Shopping(void);
	
	void set(vector<int> allItemType);
	vector<string> randomList(int numofItems);
	bool check(vector<Item*> itemVector, vector<int> holding);
};

#endif