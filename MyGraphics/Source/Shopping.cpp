/******************************************************************************/
/*!
\file	Shopping.cpp
\author Group 7
\brief
CPP file for Shopping
*/
/******************************************************************************/
#include "Shopping.h"


Shopping::Shopping(void)
{
}


Shopping::~Shopping(void)
{
}

void Shopping::set(vector<int> allItemType)
{
	this->allItemType = allItemType;
}

vector<string> Shopping::randomList(int numofItems)
{
	list.clear();
	listChecker.clear();
	ifGotLiao.clear();

	srand (time(NULL));

	for(int i = 1; i <= numofItems; ++i)
	{
		int a;

		getDiff = false;
		while(getDiff == false)
		{
			a = rand()%11;
			if(ifGotLiao.size() == 0)
				break;
			for(int b = 0; b < ifGotLiao.size(); ++ b)
			{
				if(ifGotLiao[b] != a)
				{
					getDiff = true;
				}
				else
				{
					getDiff = false;
					break;
				}
			}
		}
		ifGotLiao.push_back(a);

		if(a == 0)
		{
			list.push_back("CACTUS JUICE");
			listChecker.push_back(allItemType[a]);
		}
		else if(a == 1)
		{
			list.push_back("CADBURY CHOCALATE");
			listChecker.push_back(allItemType[a]);
		}
		else if(a == 2)
		{
			list.push_back("CAN OF VEGGIE");
			listChecker.push_back(allItemType[a]);
		}
		else if(a == 3)
		{
			list.push_back("CAT FOOD");
			listChecker.push_back(allItemType[a]);
		}
		else if(a == 4)
		{
			list.push_back("CEREAL");
			listChecker.push_back(allItemType[a]);
		}
		else if(a == 5)
		{
			list.push_back("DETERGENT");
			listChecker.push_back(allItemType[a]);
		}
		else if(a == 6)
		{
			list.push_back("FERERRO ROCHER");
			listChecker.push_back(allItemType[a]);
		}
		else if(a == 7)
		{
			list.push_back("MAC n CHEESE");
			listChecker.push_back(allItemType[a]);
		}
		else if(a == 8)
		{
			list.push_back("MILO");
			listChecker.push_back(allItemType[a]);
		}
		else if(a == 9)
		{
			list.push_back("POTATO CHIPS");
			listChecker.push_back(allItemType[a]);
		}
		else if(a == 10)
		{
			list.push_back("SARDINE");
			listChecker.push_back(allItemType[a]);
		}
	}
	
	return list;
}

bool Shopping::check(vector<Item*> itemVector, vector<int> holding)
{
	if(holding.size() != listChecker.size())
	{
		return false;
	}

	else
	{
		vector<int> temp;
		temp = listChecker;

		for(int a = 0; a < holding.size(); ++a)
		{
			for(int b = 0; b < holding.size(); ++b)
			{
				if(itemVector[holding[a]]->mesh == temp[b])
				{
					temp.erase(temp.begin()+b);
					break;
				}
			}
		}
		if(temp.size() == 0)
			return true;
		else
			return false;
	}
}