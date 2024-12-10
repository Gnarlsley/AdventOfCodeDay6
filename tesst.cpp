#include <fstream>
#include <string>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <cmath>
#include <cstring>

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
std::unordered_set<std::streamoff> validObstaclePositions;


bool inBounds(int direction, std::fstream &file){
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

bool canMove(std::streamoff direction, std::fstream &file){
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

int solvePartOne(){
    std::fstream file("puzzle.txt", std::ios::in | std::ios::binary);
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

        if(file.fail()){
            std::cout << "Error details: " << strerror(errno);
        }
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
int main() {
     std::fstream file("puzzle.txt", std::ios::in | std::ios::out | std::ios::binary);

     std::streamoff direction = up;
     int count = 0;
     const char obstacle = '#';
     if(!file.is_open()){
         return 1;
     }

     //move pointer to character position
     file.seekg(startingPos, std::ios::beg);
     //add the start position to the list of vistied positions
     visitedPositions.insert(file.tellg());

     while(true){

        if(file.fail()){
            std::cout << "Error details: " << strerror(errno);
        }
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
    std::cout<<visitedPositions.size();
    std::cout<<"\n";
    for(const auto& pos : visitedPositions){
        if(file.fail()){
            std::cout << "Error details: " << strerror(errno);
        }
        //start at beginning tile
        file.seekg(startingPos, std::ios::beg);
        file.seekp(pos);
        // std::cout<<"Replaced character at: ";
        // std::cout<<file.tellp();
        // std::cout<<" with a #";
        // std::cout<<"\n";
        file.put(obstacle);
        std::unordered_map<std::streamoff, int> positionVisitCount;
        //do the movement test, check if looping
        while(inBounds(direction, file)){
            //add position to list
            positionVisitCount[file.tellg()]++;

            //then move
            //the canMove function both determines if movement is allowed AND moves the pointer
            if (!canMove(direction,file)){
                //if unable to move, "rotate" the pointer
                direction = rotate(direction);
            }

            //check if location has been visited more than 5 times
            if(positionVisitCount[file.tellg()] > 10){
                std::cout << "Found a position visited more than 50 times, loop found: ";
                std::cout << file.tellg();
                std::cout << "\n";
                validObstaclePositions.insert(pos);
                break;
            }
        }
        if(!inBounds(direction, file)){
            // std::cout<<"No loop found";
            // std::cout<<"\n";
        }
        
        file.seekp(pos);
        // std::cout<<"Replaced character at: ";
        // std::cout<<file.tellp();
        // std::cout<<" with a .";
        // std::cout<<"\n";
        file.put('.');
    }
    std::cout<<validObstaclePositions.size();
    file.close();
    return 0;
}