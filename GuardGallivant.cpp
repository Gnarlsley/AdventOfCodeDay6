#include <fstream>
#include <string>
#include <iostream>
#include <unordered_set>
#include <cmath>

#define right 1
#define left -1
#define up -132
#define down 132
#define startingPos 5508
#define lineLength 132
#define newLine 130

//Example Data
// #define ending 117
// #define lineLength 12
// #define lineCount 10
// #define right 1
// #define left -1
// #define up -12
// #define down 12
// #define startingPos 76

std::unordered_set<std::streamoff> visitedPositions;


bool inBounds(int direction, std::ifstream &file){
    int currPos = file.tellg();

    //check if pointer currently on \r\n characters
    if(direction == right || direction == left){
        int horizontalBound = newLine;
        while(horizontalBound <= currPos){
            if(currPos == horizontalBound || currPos == horizontalBound + 1){
                return false;
            }
            horizontalBound += lineLength;
        }
    }

    //check if pointer past beginning/end
    if(currPos == -1){
        return false;
    }
    
    return true;
}

int rotate(int direction){
        switch(direction){
            case up:
                return right;
                break;
            case right:
                return down;
                break;
            case down:
                return left;
                break;
            case left:
                return up;
                break;
            default:
                return direction;
                break;
        }
}

bool canMove(std::streamoff direction, std::ifstream &file){
    std::streampos currPos = file.tellg();
    std::streampos newPos = currPos + direction;
    file.seekg(newPos, std::ios::beg);
    char ch = file.peek();
    if(ch == '#'){
        //found obstacle, move back to original position
        file.seekg(currPos, std::ios::beg);
        return false;
    }
    //no obstacle, move character
    return true;
}

int main() {
    std::ifstream file("puzzle.txt", std::ios::binary);
    std::streamoff direction = up;
    int count = 0;
    if(!file.is_open()){
        return 1;
    }

    //move pointer to character position
    file.seekg(startingPos, std::ios::beg);
    //add the start position to the list of vistied positions
    visitedPositions.insert(file.tellg());

    //commence movement
    while(true){

        //first check if maze is complete
        if(!inBounds(direction, file)){
            break;
        }

        //then check if you've been on this square before
        if (visitedPositions.find(file.tellg()) == visitedPositions.end()) {
            visitedPositions.insert(file.tellg());
        }

        //then move
        //the canMove function both determines if movement is allowed AND moves the pointer
        if (!canMove(direction,file)){
            //if unable to move, "rotate" the pointer
            direction = rotate(direction);
        }
    }
    std::cout<<"Number of distinct positions: ";
    std::cout<<visitedPositions.size();

    file.close();
    return 0;
}