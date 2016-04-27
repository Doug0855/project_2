#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stack>
#include <climits>

using namespace std;
class Realm {  //Data structure to hold realm info, contains methods to calculate certain properties of the realm
public:
    string charm;
    int size;	//Size of magi Array
    int* magi; 	//Holds the values and sequence of the magi
    int power; 	//Max number of incantations possible for the given relam
    
    Realm(string charm, int nummagi) {
        
        size = nummagi;	//Easy for referencing later
        magi = new int[size];
        power = 1;		//Need to set this to increment later
        
        setMagi();
        setPower();
        setCharm(charm);
    }
    
    Realm() {
        //Default constructer//
    }
    
    //Calculates the max number of incantations possible for a given realm,
    void setPower() {
        
        //Setting array for LIS
        
        //Used to store indexes of magi to do lis
        //Numbers stored are INDEXES of magi array
        //i.e. lis[3] = 4. Used to reference magi[4]
        int lis[size+1];
        
        int low = 0;
        int high;
        int middle;
        
        //Initializing both
        for(int i = 0; i < size+1; i++) {
            lis[i] = -1;
        }
        
        // Beginning LSI
        lis[0] = 0;			//Always evaluating magi[0] first
        int lp = 1;			//LIS array increments differently
        
        for (int i = 1; i < size; ++i)
        {
            high = lp;
            
            //If next magi is larger, we put it in next
            if(magi[i] > magi[lis[lp-1]]) {
                lis[lp] = i;
                power += 1;
                lp++;
            }
            
            //If next magi smaller, we scan to see where it goes
            else if(magi[i] < magi[lis[lp-1]]) {
                
                if (magi[i] < magi[lis[0]])
                    lis[0] = i;
                
                //BINARY SEARCH
                else {
                    while (low <= high) {
                        middle = (low+ high) / 2;
                        
                        if (magi[lis[middle]] > magi[i]) {
                            high = middle - 1;
                        }
                        
                        else low = middle + 1;
                        
                    }
                    
                    lis[low] = i;
                }
            }
        }
    }
    
    void setMagi() {
        for (int i = 0; i < size; ++i)
        {
            cin >> magi[i];
            
        }
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
        return gems;
    }
    /********************************************************************/
    //Is this needed? Can't save power between realms
    void spendPower(int amount) {
        power -= amount;
    }
    /********************************************************************/
    
};

unsigned int levDis(string a, string b) { //Finds the minimum distance between 2 strings, including inserts, deletes, swaps, Implements the Levenshtein Distance algorithm
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

void dijkstra(vector< vector<int> > &adjMatrix,int source,int size, vector<int> &predecessor, vector<int> &distance)
{
    
    vector<bool> sptSet;    //shortest path tree set

    //**for loop causes core dump**
    for(int i = 0; i < size; i++)
    {
        /*
        distance[i] = INT_MAX;
        predecessor[i] = -1;
        sptSet[i] = false;
        */
        distance.push_back(INT_MAX);
        predecessor.push_back(-1);
        sptSet.push_back(false);
    }
    distance[source] = 0;

    for(int i = 0; i < size-1; i++)
    {
        //pick minimum distance vertex
        int min = INT_MAX;
        int min_index;
        for(int j = 0; j < size; j++)
        {
            if(distance[j] <= min && sptSet[j] == false)
            {
                min = distance[j];
                min_index = j;
            }
        }
        //terminate if min_index = end
        sptSet[min_index] = true;

        for(int j = 0; j < size; j++)
        {
            if(distance[min_index] != INT_MAX && !sptSet[j] && distance[min_index] + adjMatrix[min_index][j] < distance[j])
            {
                distance[j] = distance[min_index] + adjMatrix[min_index][j];
                predecessor[j] = min_index;
            }
        }
    }

    
}

int main() {
    /*
    int shit;
    cout << "input a size\n";
    cin >> shit;
    cout << "input magis\n";
    Realm test("A", shit);
    std::cout << "Power is: " << test.power << "\n";
    */
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
     
     //Construct Adjacency Matrix for Graph
     vector<vector<int> > adjMatrix(size, vector<int>(size));
     for (int i = 0; i < size; ++i)
     {
     for (int j = 0; j < size; ++j)
     {
     adjMatrix[i][j] = levDis(verse[i].getCharm(),verse[j].getCharm());
     }
     }
     //find source realm number
     int source;
     for(int i = 0; i < size; i++)
     {
        if(verse[i].getCharm() == start)
        {
            source = i;
        }
     }
     vector<int> predecessor(size);
     vector<int> distance(size);
     dijkstra(adjMatrix,source,size,predecessor,distance);
     for(int i = 0; i < size; i++)
     {
        cout << " " << distance[i];
     }
     /*
     //Prints Adjancency Matrix for debugging
     for (int i = 0; i < size; ++i)
     {
     for (int j = 0; j < size; ++j)
     {
     cout << adjMatrix[i][j] << " ";
     }
     
     cout << endl;
     }
     */
     return 0;
}