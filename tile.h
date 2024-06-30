#include <vector>
using namespace std;

class Tile{
private:
    vector<Tile*> adjacentTiles;
    int adjacentMineCount;
    int yPos;
    int xPos;
    bool visibility = false;
    bool flagged = false;
    bool mine = false;
    bool edge = false;
public:
    //Getters
    vector<Tile*> getAdjTiles(){
        return adjacentTiles;
    }
    int getAdjMineCount(){
        int buffer = 0;
        for(auto iter : adjacentTiles){
            if(iter->mine){
                buffer++;
            }
        }
        return buffer;
    }
    int getYPos(){
        return yPos;
    }
    int getXPos(){
        return xPos;
    }
    bool isVisible(){
        return visibility;
    }
    bool isFlagged(){
        return flagged;
    }
    bool isMine(){
        return mine;
    }
    bool isEdge(){
        return edge;
    }
    //Setters
    void pushAdjTile(Tile* d){
        adjacentTiles.push_back(d);
    }
    void setCoord(int y, int x){
        yPos = y;
        xPos = x;
    }
    void makeVisible(){
        visibility = true;
    }
    void toggleFlag(){
        if (flagged == true){
            flagged = false;
        }
        else if (flagged == false){
            flagged = true;
        }
    }
    void makeMine(){
        mine = true;
    }
    void makeEdge(){
        edge = true;
    }
};