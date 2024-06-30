#include <vector>
#include <map>
#include <set>
#include <random>
using namespace std;

class Board {
private:
    int cols;
    int rows;
    int mines;
public:
    //[y][x]
    vector<vector<Tile*>> boardPieces;
    vector<Tile*> mineLocations;
    Board(int &cols,int &rows,int &mines){
        this->cols = cols;
        this->rows = rows;
        this->mines = mines;
        for(int i = 0; i < rows + 2; i++){
            vector<Tile*> temp;

            if (i == 0 || i == rows + 1){
                for(int k = 0; k < cols + 2; k++){
                    Tile* newTile = new Tile;
                    newTile->setCoord(i,k);
                    newTile->makeEdge();
                    temp.push_back(newTile);
                }
            }
            else {
                for (int j = 0; j < cols + 2; j++) {
                    if(j == 0 || j == cols + 1){
                        Tile* newTile = new Tile;
                        newTile->setCoord(i,j);
                        newTile->makeEdge();
                        temp.push_back(newTile);
                        continue;
                    }
                    else {
                        Tile* newTile = new Tile;
                        newTile->setCoord(i, j);
                        temp.push_back(newTile);
                    }
                }
            }
            boardPieces.push_back(temp);
        }
        createMines();
        for(int i = 0; i < rows + 2; i++){
            for(int j = 0; j < cols + 2; j++){
                adjTiles(boardPieces[i][j]);
            }
        }
    }

    void createMines() {
        random_device seed;
        mt19937 gen(seed());
        uniform_int_distribution<> height(1, rows - 1);
        uniform_int_distribution<> width(1, cols - 1);
        int mineCount = 0;
        while (mineCount < mines) {
            unsigned int randY = height(gen);
            unsigned int randX = width(gen);
            if(!(boardPieces[randY][randX]->isMine())){
                boardPieces[randY][randX]->makeMine();
                mineLocations.push_back(boardPieces[randY][randX]);
                mineCount++;
            }
        }
    }

    void adjTiles(Tile* d){
        int y = d->getYPos();
        int x = d->getXPos();
        for(int i = y - 1; i <= y + 1; i++){
            for(int j = x - 1; j <= x + 1; j++){
                if (d->isEdge()){
                    continue;
                }
                if ((y == i) && (x == j)){
                    continue;
                }
                if(d->isMine()){
                    d->pushAdjTile(boardPieces[i][j]);
                }
                else{
                    d->pushAdjTile(boardPieces[i][j]);
                }
            }
        }
    }

        //Check if it is a mine, if it is a mine, end the game.
        //If it is not a mine, reveal the current tile. If it is a number, you end the recursive search.
        //If it is an empty tile, keep revealing its neighbor tiles.
        //Repeat step 2 and 3 until there's no available neighbor tile for you to reveal
    void checkTile(Tile* d){
            if((!d->isMine()) && (!d->isEdge()) && (d->getAdjMineCount() == 0)){
                d->makeVisible();
                for(auto iter : d->getAdjTiles()){
                if(!iter->isFlagged() && !iter->isVisible() && !iter->isEdge()){
                    checkTile(iter);
                }
            }
        }
    }
};