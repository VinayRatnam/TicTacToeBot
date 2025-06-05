//
// Created by Vinay Reddy Ratnam on 6/3/25.
//
#pragma once
#include <vector>
#include <utility>
using namespace std;


class Board {
public:
    vector<vector<int>> grid(3, vector<int>(3,0)); // 3x3 grid
    vector<pair<int, int>> episode_history; //contains data from game; moves that were made

    Board() {};
private:

    // Changes board based on player action
    void playerAction(int player, pair<int,int> pos) {
        if (player == 1) {
            grid[pos.first][pos.second] = 1;
        }

        if (player == 2) {
            grid[pos.first][pos.second] = 2;
        }

        // update episode history
        episode_history.push_back(<pos.first, pos.second>);
    }

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

    """Add deconstructor if loop is not used"""
};

