//REPLACE getGems() in REALM
int getGems(int dist) {
        cout << "Flag 5" << endl;
        if(dist > power) {
            return 0;
        }
        else {
            cout << "Flag 7" << endl;
            for(int i = 0; i < dist; i++) {
                cout << order[i] << endl; 
                gems += order[i];
            }
            cout << "Flag 8" << endl;
            return gems; 
        }

        cout << "Flag 9"  << endl; 
    }

//COPY PASTE THIS IN 
void backtrack(vector<int> &previous, Realm verse[], vector< vector<int> > &adjMatrix, int start, int end) {
    int gems = 0; 
    int encants = 0;
    int distance = 0;
    int current = end;
    cout << "Flag 1" << endl;
    while(1) {
        distance = adjMatrix[previous[current]][current];
        cout << "Flag 2" << endl;
        if (verse[previous[current]].power < distance)
        {
            cout << "IMPOSSIBLE" << endl;
            break;
        } 
        cout << "Flag 3" << endl;
        cout << "Distance: " << distance << "Power: " << verse[previous[current]].power << endl; 
        gems += verse[previous[current]].getGems(distance); 
        cout << "Flag 4" << endl;
        encants += distance; 

        if (previous[current] == start)
        {
            cout << encants << " " << gems << endl; 
            break; 
        } else {
            current = previous[current]; 
        }

        cout << "Flag 5" << endl; 
    }
} 