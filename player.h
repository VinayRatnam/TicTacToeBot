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
using namespace std;

class Players {
public:
    map<string, map<int, float>> action_values;

    int move = 0;
    vector<int> rewards {-1,0,-1};

    Player() { //default constructor should initialize states for training
        // create states
        pair<string, pair<int, int>> initial_state {"", {0,0}};
        createStates(initial_state);
    }

    trainWithMC

private:
    void createStates(pair<string, pair<int,int>> curr_state) {
        if (curr_state.first.size() == 9) {
            //this is a finalized state
            //calculate the possible actions for said state
            map<int, float> actions;
            for (int i = 0; i < 9; i++) {
                if (curr_state.first[i] == 'a') {
                    actions[i] = 0.0;
                }
            }
            action_values[curr_state.first] = actions;
        }

        else {
            if (curr_state->second.first < 5) {
                createStates({curr_state.first + 'b', {curr_state.second.first+1, curr_state.second.second}});
            }
            if (curr_state->second->second < 4) {
                createStates({curr_state.first + 'c', {curr_state.second.first, curr_state.second.second+1}});
            }
            createStates({curr_state.first + 'a', curr_state.second});
        }
    }
};
