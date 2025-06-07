//
// Created by Vinay Reddy Ratnam on 6/3/25.
//

#pragma once
#include <vector>
#include <map>
#include <utility>
#include <string>
#include <set>
#include <algorithm>
#include <random>
#include <iostream>
#include <cmath>
using namespace std;

class Players {
public:
    map<string, map<int, float>> action_values;
    map<string, map<int, float>> policies;
    map<string, map<int, float>> uniform_policy;
    default_random_engine generator;
    uniform_real_distribution<float> distribution;

    Players() : generator(12345), distribution(0.0f,1.0f) { //default constructor should initialize states for training
        // create states
        pair<string, pair<int, int>> initial_state {"", {0,0}};
        createStates(initial_state);

        uniform_policy = policies;
    }

    pair<int,int> convertToBoardSpot(int val) {
        // hard encode conversions
        switch (val) {
            case 0:
                return {0,0};
            case 1:
                return {0,1};
            case 2:
                return {0,2};
            case 3:
                return {1,0};
            case 4:
                return {1,1};
            case 5:
                return {1,2};
            case 6:
                return {2,0};
            case 7:
                return {2,1};
            case 8:
                return {2,2};
            default:
                return {-1,-1};
        }
    }

    // takes in board position as input and spits out move; takes epsilon into account
    pair<int,int> makeMoveMC(string curr_state, float epsilon) {
        // access policy for specific state
        map<int, float> policy;
        bool random_move = (distribution(generator) < epsilon);
        float curr_prob = 0.0;


        if (random_move) {
            policy = uniform_policy[curr_state];
        }
        else {
            policy = policies[curr_state];
        }

        float decision = distribution(generator);

        for (auto iter = policy.begin(); iter != policy.end(); iter++) {
            curr_prob += iter->second;
            if (decision <= curr_prob) {
                return convertToBoardSpot(iter->first);
            }
        }

        if (!policy.empty()) {
            // Return the last action in the policy as a safe fallback
            return convertToBoardSpot(policy.rbegin()->first);
        }

        // If the policy was empty, there are no moves to make from this state.
        // Return an invalid move to signal this.
        return {-1, -1};
    }


    void changeStates(vector<pair<int, int>> &episode_history, string &grid_string, int victory, int &num_moves) {
        int player1_return;
        int player2_return;

        // iterate backwards through moves and adjust action-values

        // player 1 has won
        if (victory == 1) {
            player1_return = 1;
            player2_return = -1;
        }
        else if (victory == 2) {
            player1_return = -1;
            player2_return = 1;
        }
        else { // if draw has occurred
            player1_return = 0;
            player2_return = 0;
        }

        string curr_grid = grid_string;
        float gamma = 0.9f; //standard for calculating return
        float alpha = 0.1f; //standard for adjusting action-value functions
        while (num_moves > 0) {
            curr_grid = goBackOneMove(curr_grid, episode_history[num_moves - 1]);
            int action = episode_history[num_moves-1].first * 3 + episode_history[num_moves-1].second;

            // change action-value function
            if (num_moves % 2 == 0) {
                action_values[curr_grid][action] += alpha * (player2_return - action_values[curr_grid][action]);
                player2_return *= gamma;
            }
            else {
                action_values[curr_grid][action] += alpha * (player1_return - action_values[curr_grid][action]);
                player1_return *= gamma;
            }

            //adjust policy using softmax
            softMax(curr_grid);

            num_moves--;
        }




    }

private:
    void createStates(pair<string, pair<int,int>> curr_state) {
        if (curr_state.first.size() == 9) {
            //this is a finalized state
            //calculate the possible actions for said state
            map<int, float> actions;
            map<int, float> policy;
            for (int i = 0; i < 9; i++) {
                if (curr_state.first[i] == 'a') {
                    actions[i] = 0.0; policy[i] = 0.0;
                }
            }

            // create the uniform distribution for policies in each state
            float uniform_val;
            if (policy.empty()) {
                // Handle the case of an empty map, e.g., set uniform_val to 0 or throw an error
                uniform_val = 0.0f; // Or some other appropriate handling
                std::cerr << "Warning: actions map is empty, cannot calculate uniform value." << std::endl;
            } else {
                uniform_val = 1.0f / policy.size(); // Use 1.0f to ensure floating-point division
            }

            for (auto iter = policy.begin(); iter != policy.end(); iter++) {
                iter->second = uniform_val;
            }
            action_values[curr_state.first] = actions;
            policies[curr_state.first] = policy;
        }

        else {
            if (curr_state.second.first < 5) {
                createStates({curr_state.first + 'b', {curr_state.second.first+1, curr_state.second.second}});
            }
            if (curr_state.second.second < 4) {
                createStates({curr_state.first + 'c', {curr_state.second.first, curr_state.second.second+1}});
            }
            createStates({curr_state.first + 'a', curr_state.second});
        }
    }

    string goBackOneMove(string state, pair<int, int> last_move) {
        // Calculate the index directly from the pair
        // last_move.first is the row, last_move.second is the column
        int index = last_move.first * 3 + last_move.second;

        // Optional: Check if the index is valid before modifying the string
        if (index >= 0 && index < state.length()) {
            state[index] = 'a';
        }
        else {
            cout << "error in state length or index" << endl;
        }

        return state;
    }

    /*string convertGridtoString(vector<vector<int>> grid) {
        string curr_board;
        for (int j = 0; j < 3; j++) {
            for (int i = 0; i < 3; i++) {
                curr_board.push_back('a' + grid[j][i]);
            }
        }

        return curr_board;
    }*/

    void softMax(string curr_grid) {
        // first find max action value
        const map<int, float> &curr_action_values = action_values[curr_grid];

        float max_val = -999.f; // set max_val initially to a very low number

        for (auto iter = curr_action_values.begin(); iter != curr_action_values.end(); iter++) {
            if (max_val < iter->second) {
                max_val = iter->second;
            }
        }

        float sum_exp = 0;
        for (auto iter2 = curr_action_values.begin(); iter2 != curr_action_values.end(); iter2++) {
            sum_exp += exp(iter2->second - max_val);
        }

        // calculate probabilities with softmax now and change policy
        for (auto iter2 = curr_action_values.begin(); iter2 != curr_action_values.end(); iter2++) {
            policies[curr_grid][iter2->first] = exp(iter2->second - max_val) / sum_exp;
        }
    }
};

