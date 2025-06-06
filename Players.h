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
using namespace std;

class Players {
public:
    map<string, map<int, float>> action_values;
    map<string, map<int, float>> policies;
    default_random_engine generator;
    uniform_real_distribution<float> distribution;

    vector<int> rewards {-1,0,-1};

    Players() : generator(12345), distribution(0.0f,1.0f) { //default constructor should initialize states for training
        // create states
        pair<string, pair<int, int>> initial_state {"", {0,0}};
        createStates(initial_state);
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

    // takes in board position as input and spits out move
    pair<int,int> makeMoveMC(string curr_state) {
        // access policy for specific state
        map<int, float> policy = policies[curr_state];
        float decision = distribution(generator);

        float curr_prob = 0.0;
        for (auto iter = policy.begin(); iter != policy.end(); iter++) {
            curr_prob += iter->second;
            if (decision <= curr_prob) {
                return convertToBoardSpot(iter->first);
            }
        }
        auto iter2 = policy.crbegin();
        return convertToBoardSpot(iter2->first);

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

};

