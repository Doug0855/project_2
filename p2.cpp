#include <iostream>
#include <string>
#include <vector> 
#include <algorithm>
#include <stack>

using namespace std;

class Realm {  //Data structure to hold realm info, contains methods to calculate certain properties of the realm
private:
	string charm;
	int magi[10000]; //Holds the powers and order of the magi
	int power; //Calculated value that ignores magi of lesser power to determine the max number of incantations possible for the given relam
public:
	Realm(string charm, int magi) {
		setMagi(magi);
		setPower(magi); 
		setCharm(charm); 
	}

	Realm() {
		//Default constructer//
	}

	int getPower() {
		return power;
	}

	//Calculates the max number of incantations possible for a given realm, 
	void setPower(int size) {
		int nPow = 1;
		int maxMagi = magi[0]; 
		for (int i = 1; i < size; ++i)
		{
			if (magi[i] > maxMagi)		
			{
				nPow++;
				maxMagi = magi[i]; 
			}
		}

		power = nPow; 
	}

	void setMagi(int size) {
		int nMagi[size]; 
		for (int i = 0; i < size; ++i)
		{
			cin >> nMagi[i];

		}
		magi = nMagi; 
	}

	string getCharm() {
		return charm; 
	}

	void setCharm(string ch) {
		charm = ch; 
	}


	//Returns the gem cost of moving to the next realm, given the levenshtein distance between the 2 strings
	int getGems(int dist) {
		int gems = magi[0]; 
		int maxMagi = magi[0];
		for (int i = 1; i < dist; ++i)
		{
			if (magi[i] > maxMagi)
			{
				gems += magi[i]; 
				maxMagi = magi[i];
			}
			magi[i] = 0;
		}
	}

	void spendPower(int amount) {
		power -= amount; 
	}


};

unsigned int levDis(string &a, string &b) { //Finds the minimum distance between 2 strings, including inserts, deletes, swaps, Implements the Levenshtein Distance algorithm
	unsigned int sA = a.size();
	unsigned int sB = b.size();   //Uses unsigned ints to prevent negative edge weights
	unsigned int min1, min2, offset; 

	vector<vector<unsigned int> > mat(sA + 1, vector<unsigned int>(sB+1));

	mat[0][0] = 0;
	for (unsigned int i = 1; i <= sA; ++i) //Fills the first row with 1->n for inserts/deletes 
	{
		mat[i][0] = i;
	}

	for (unsigned int i = 1; i <= sB; ++i) //fills the first column with 1->n for inserts/deletes
	{
		mat[0][i] = i;
	}

	for (unsigned int i = 1; i <= sA; ++i) //Fills the matrix with data, finds shortest path
	{
		for (unsigned int j = 1; j <= sB; ++j)
		{
			min1 = min(mat[i-1][j]+1, mat[i][j-1]+1);

			if (a[i-1] == b[j-1])
			{
				offset = 0;
			} else {
				offset = 1;
			}

			min2 = min(min1, mat[i-1][j-1] + offset); 

			mat[i][j] = min2; 
		}
	}
	return mat[sA][sB]; 
}



int main() {

	int size, magi;
	string charm, start, end;
	cin >> size;
	Realm verse[size]; 

	for (int i = 0; i < size; ++i)
	{
		
		cin >> charm; 
		cin >> magi; 

		verse[i] =  Realm(charm, magi);  	


	}

	cin >> start >> end; 



	return 0; 
}