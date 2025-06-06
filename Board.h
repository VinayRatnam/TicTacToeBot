//
// Created by Vinay Reddy Ratnam on 6/3/25.
//
#pragma once
#include <vector>
#include <utility>
#include <iostream>
using namespace std;


class Board {
public:
    vector<vector<int>> grid; // 3x3 grid
    vector<pair<int, int>> episode_history; //contains data from game; moves that were made
    int move;

    Board() : grid(3, vector<int>(3,0)), move(0) {};

    //checks if game has ended; 0 = draw, 1 = player 1 has won, 2 = player 2 has won
    int checkGameEnded() {
        if (grid[0][0] != 0) {
            if (grid[0][0] == grid[0][1] && grid[0][1] == grid[0][2]) {
                return grid[0][0];
            }
            if (grid[0][0] == grid[1][1] && grid[1][1] == grid[2][2]) {
                return grid[0][0];
            }
            if (grid[0][0] == grid[1][0] && grid[1][0] == grid[2][0]) {
                return grid[0][0];
            }
        }
        if (grid[0][1] != 0 && grid[0][1] == grid[1][1] && grid[2][1]) {
            return grid[0][1];
        }
        if (grid[0][2] != 0) {
            if (grid[0][2] == grid[1][1] && grid[1][1] == grid[2][0]) {
                return grid[0][2];
            }
            if (grid[0][2] == grid[1][2] && grid[1][2] == grid[2][2]) {
                return grid[0][2];
            }
        }
        if (grid[1][0] != 0 && grid[1][0] == grid[1][1] && grid[1][1] == grid[1][2]) {
            return grid[1][0];
        }
        if (grid[2][0] != 0 && grid[2][0] == grid[2][1] && grid[2][1] == grid[2][2]) {
            return grid[2][0];
        }

        //check for draw
        if (episode_history.size() == 9) {
            return 0;
        }
    }

    string getBoardState() {
        string curr_board;
        for (int j = 0; j < 3; j++) {
            for (int i = 0; i < 3; i++) {
                curr_board.push_back('a' + grid[j][i]);
            }
        }

        return curr_board;
    }

    // Changes board based on player action
    void playerAction(pair<int,int> pos) {
        if (move % 2 == 0) {
            grid[pos.first][pos.second] = 1;
        }

        else {
            grid[pos.first][pos.second] = 2;
        }

        // update episode history
        episode_history.push_back({pos.first, pos.second});
        move++;
    }




};

