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
#define fileSize 17160

//Example Data
// #define ending 117
// #define lineLength 12
// #define lineCount 10
// #define right 1
// #define left -1
// #define up -12
// #define down 12
// #define startingPos 76
// #define fileSize 118
// #define newLine 10

std::unordered_map<std::streamoff, int> visitedPositions;
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
    if(currPos < 0 || currPos >= fileSize){
        file.clear();
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
    if(file.fail()){
        file.clear();
        return true;
    }
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
    visitedPositions[file.tellg()]++;

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
            visitedPositions[file.tellg()]++;
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

int getFileSize(){
    std::ifstream file("puzzle.txt", std::ios::binary | std::ios::ate);
    std::streamoff size = file.tellg();
    file.close();

    std::cout<<"File size: " << size << "bytes";
    return 0;
}
int main() {
    getFileSize();
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
     visitedPositions[file.tellg()]++;

     while(true){
  
        //first check if maze is complete
        if(!inBounds(direction, file)){
            break;
        }

        if(file.fail()){
            std::cout << "Error details: " << strerror(errno);
        }

        //check if you've been on this square before
        visitedPositions[file.tellg()]++;
        //move
        //the canMove function both determines if movement is allowed AND moves the pointer
        if (!canMove(direction,file)){
            //if unable to move, "rotate" the pointer
            direction = rotate(direction);
        }
    }
    std::cout<<visitedPositions.size();
    std::cout<<"\n";
   for (int pos = 0; pos < fileSize; pos++){
         direction = up;
         //std::streampos pos = obsPos.first;
        if(pos == startingPos){
             continue;
         }
            file.seekp(pos,std::ios::beg);
            file.seekg(pos,std::ios::beg);
            if(file.fail()){
                std::cerr << "Error moving write pointer to position: " << pos << "\n";
                file.clear();
            }
            char original = file.peek();
            file.put(obstacle);
            file.seekg(startingPos, std::ios::beg);
            std::unordered_map<std::streamoff, int> positionVisitCount;

            //do the movement test, check if looping
            while(true){
                std::streampos currPos = file.tellg();
                //add position to list
                positionVisitCount[currPos]++;

                //check if location has been visited more than 5 times
                if(positionVisitCount[currPos] > 500){
                    std::cout << "Found a position visited more than 5 times, loop found: ";
                    std::cout << pos;
                    std::cout << "\n";
                    validObstaclePositions.insert(pos);
                    break;
                }

                //move
                //the canMove function both determines if movement is allowed AND moves the pointer
                if (!canMove(direction,file)){
                    //if unable to move, "rotate" the pointer
                    direction = rotate(direction);
                }

                //if maze is escapable
                if(!inBounds(direction, file)){
                    if(file.fail()){
                        file.clear();
                    }   
                    std::cout<<"No loop: "<<pos<<"\n";
                    break;
                }
            }
            file.seekp(pos, std::ios::beg);
            file.put(original);
        // std::cout<<"Replaced character at: ";
        // std::cout<<file.tellp();
        // std::cout<<" with a .";
        // std::cout<<"\n";
   }
    std::cout<<validObstaclePositions.size();
    file.close();
    return 0;
}