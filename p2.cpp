#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stack>
#include <climits>
#include <queue>

using namespace std;
class Realm {  //Data structure to hold realm info, contains methods to calculate certain properties of the realm
public:
    string charm;
    int size;       //Size of magi Array
    int* magi;      //Holds the values and sequence of the magi
    int* order;     //LSI of magi to use. Can use as many as needed
    int power;      //Max number of incantations possible for the given relam
    int gems;       //Number of gems required to leave realm
    int dvalue;     //Value from the distance array for this node in Dijkstra
    int realm_num;  //Index for realm
    bool popped;    //Has the Node been popped in Dijkstra's?

    Realm(string charm, int nummagi, int num) {
        
        realm_num = num;
        size = nummagi; //Easy for referencing later
        magi = new int[size];
        power = 1;      //Need to set this to increment later
        gems = 0;       //Need to set this to increment later
        popped = false;
        
        setMagi();
        //cout << "set the magi\n";
        setPowerandGems();
        //cout << "set da powah\n";
        setCharm(charm);
    }
    
    Realm() {
        //Default constructer//
    }
    
    //Calculates the max number of incantations possible for a given realm,
    void setPowerandGems() {
        
        //Setting array for LIS
        
        //Used for tracing path through numbers
        //Each value stores magi index of previous value in lis
        //i.e. magi =  {0, 9, 2, 4} will have parent {-1, 0, 0, 2}
        /*
        Longest path will be magi[parent[3]] preceeded by 
        magi[parent[2]], preceeded by magi[parent[0]];
        */
        int parent[size];

        //Used to store indexes of magi to do lis
        //Numbers stored are INDEXES of magi array
        //i.e. lis[3] = 4. Used to reference magi[4]
        int lis[size+1];
        
        int low = 0;
        int high;
        int middle;
        
        //Initializing both
        for(int i = 0; i < size; i++) {
            parent[i] = -1;
            lis[i] = -1;
        }
        lis[size] = 0;
        
        // Beginning LSI
        lis[0] = 0;         //Always evaluating magi[0] first
        int lp = 1;         //LIS array increments differently
        
        for (int i = 1; i < size; ++i)
        {
            high = lp;
            
            //cout << magi[i] << " is next up\n";
            //If next magi is larger, we put it in next
            if(magi[i] > magi[lis[lp-1]]) {
                //cout <<"lis at " <<lp<< " is " << i << "\n";
                lis[lp] = i;
                parent[i] = lis[lp-1];
                power += 1;
                lp++;
            }
            
            //If next magi smaller, we scan to see where it goes
            else if(magi[i] < magi[lis[lp-1]]) {
                
                if (magi[i] <= magi[lis[0]])
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
                    if(low == magi[i]) {
                        continue;
                    }
                    lis[low] = i;
                    parent[i] = lis[low-1];
                }
            }
        }
        
        // cout << "parent array is: ";
        // for(int i = 0; i < size; i++) {
        //  cout << "[" << parent[i] << "]";
        // }
        // cout << "\n";
        

        // cout << "lis array is: ";
        // for(int i = 0; i < size; i++) {
        //  cout << "[" << lis[i] << "]";
        // }
        // cout << "\n";

        order = new int[power];
        int parentpos = lis[lp-1];

        for(int i = power -1; i > -1; i--) {
            order[i] = magi[parentpos];
            parentpos = parent[parentpos];
        }

        // cout << "order array is: ";
        // for(int i = 0; i < power; i++) {
        //  cout << "[" << order[i] << "]";
        // }
        // cout << "\n";
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
        // cout << "Flag 5" << endl;
        if(dist > power) {
            return 0;
        }
        else {
            // cout << "Flag 7" << endl;
            for(int i = 0; i < dist; i++) {
                //cout << order[i] << endl; 
                gems += order[i];
            }
            // cout << "Flag 8" << endl;
            return gems; 
        }

        // cout << "Flag 9"  << endl; 
    }
    
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
struct realm_dvalue_greater_than {
    bool operator()(const Realm& a, const Realm& b) const {
        return a.dvalue > b.dvalue;
    }
};

void dijkstra(vector<Realm> &verse, vector< vector<int> > &adjMatrix, int sourcenode, int endnode, int size, vector<int> &predecessor,vector<Realm> &distance, string startrealm,string endrealm)
{
    priority_queue<Realm,vector<Realm>,realm_dvalue_greater_than> minHeap;
    vector<Realm> vertexSet (size);
    int start_index;
    int end_index;
    
    for(int i = 0; i < size; i++)
    {
        verse[i].dvalue = INT_MAX;
        predecessor.push_back(INT_MAX);
        //vertexSet.push_back(i);
        if(verse[i].charm == startrealm)
        {
            //start_index = i;
            verse[i].dvalue = 0;
        }
        else if(verse[i].charm == endrealm)
        {
            end_index = i;
        }
        minHeap.push(verse[i]);
    }
        
    //make_heap(verse.begin(),verse.end(),realm_dvalue_greater_than());
    while(!minHeap.empty())   //!vertexSet.empty()
    {
        Realm current = minHeap.top();
        minHeap.pop();
        current.popped = true;
        current.realm_num;
        //cout  << current.charm << " -> ";

        //sets popped realm's popped bool to true
        for(int i= 0; i < size; i++)
        {
            if(current.realm_num == i)
            {
                verse[i].popped = true;
            }
        }
        //for each adjacent realm
        for(int i = 0; i < size; i++)
        {
            int index = current.realm_num;
            if(adjMatrix[index][i] <= verse[index].power)    //enough magi to make incantations
            {
                int length = current.dvalue + adjMatrix[index][i];
                if(length < verse[i].dvalue)
                {
                    //cout << verse[i].charm << endl;
                    //cout << "New length: " << length << "    " << "Old length: " << verse[i].dvalue << endl;
                    verse[i].dvalue = length;
                    predecessor[i] = index;
                }
            }
        }

        priority_queue<Realm,vector<Realm>,realm_dvalue_greater_than> tempHeap;
        for(int i = 0; i < size; i++)
        {
            if(verse[i].popped == false)
            {
                tempHeap.push(verse[i]);
            }
            minHeap = tempHeap;
        }
    }
    
//return verse[end_index].dvalue;
    
}

void backtrack(vector<int> &previous, vector<Realm> &verse, vector< vector<int> > &adjMatrix, int start, int end) {
    int gems = 0; 
    int p = 0; 
    int encants = 0;
    int distance = 0;
    int current = end;
    //cout << "Flag 1" << endl;
    while(1) {
        //cout << "Called " << p << endl; 
        p++;
        distance = adjMatrix[previous[current]][current];
        //cout << "Flag 2" << endl;
        if (verse[previous[current]].power < distance)
        {
            cout << "IMPOSSIBLE" << endl;
            break;
        } 
        //cout << "Flag 3" << endl;
        //cout << "Distance: " << distance << "Power: " << verse[previous[current]].power << endl; 
        gems += verse[previous[current]].getGems(distance); 
        //cout << "Flag 4" << endl;
        encants += distance; 

        if (previous[current] == start)
        {
            cout << encants << " " << gems << endl; 
            break; 
        } else {
            current = previous[current]; 
        }

        //cout << "Flag 5" << endl; 
    }
} 

int main() {
    
    int size, magi;
    string charm, start, end;
    cin >> size;
    vector<Realm> verse(size);

     
    for (int i = 0; i < size; ++i)
    {
     
    cin >> charm;
    cin >> magi;
     
    verse[i] =  Realm(charm, magi,i);

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
    int source, target;
    for(int i = 0; i < size; i++)
    {
        if(verse[i].getCharm() == start)
        {
            source = i;
        }
        if(verse[i].getCharm() == end)
        {
            target = i;
        }
    }
    vector<int> predecessor(size);
    vector<Realm> distance(size);

    vector<int> predecessor_gb(size); 
    vector<Realm> distance_gb(size); 
    
    dijkstra(verse,adjMatrix,source,target,size,predecessor,distance,start,end);
    backtrack(predecessor, verse, adjMatrix, source, target); 

    for (int i = 0; i < size; ++i)
      {
          verse[i].popped = false; 
      }  

    dijkstra(verse,adjMatrix,target,source,size,predecessor_gb,distance_gb,end,start);
    backtrack(predecessor_gb, verse, adjMatrix, target, source);     
    return 0;
    
}