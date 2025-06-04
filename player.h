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

class Players() {
public:
    map<string, map<char, float>> action_values;
    vector<char> all_actions {a, b, c, d, e, f, g, h, i};
    int move = 0;

    Player() {
        vector<string> states;
        states.push_back("");

        states.insert(states.end(), makeStatesAndActions("")); // should give all the permutations of states

        // let's extract all the combos; order is not important (to an extent)
        for (auto state : states) {
            vector<char> player_1_moves;
            vector<char> player_2_moves;
            for (int i = 0; i < state.size(); i+=2) {

                player_1_moves.push_back(state[i]);

                if (i+1 < state.size()) {
                    player_2_moves.push_back(state[i+1]);
                }
            }

            sort(player_1_moves.begin(), player_1_moves.end());
            sort(player_2_moves.begin(), player_2_moves.end());

            string final_form;
            int index = 0;
            while (index < player_2_moves.size()) {
                //size of player_1_moves will always be greater than or equal to size of player_2_moves
                final_form.push_back(player_1_moves[index]);
                final_form.push_back(player_2_moves[index++]);
            }
            if (index + 1 == player_1_moves.size() - 1) {
                final_form.push_back(player_1_moves[index+1]);
            }

            //final_form now has the desired convention to describe the state
            vector<char> extra_states
        }

    }

    vector<string> makeStatesAndActions(string state) {
        if (state.size() == 9) {
            return {};
        }

        set<char> actions_taken;
        for (int i = 0; i < state.size(); i++) {
            actions_taken.insert(state[i]);
        }

        vector<string> new_states;
        for (char i = 97; i < 97+9; i++) {
            if (actions_taken.find(i) == actions_taken.end()) {
                //character was not found in previous actions; it can be taken
                new_states.push_back(state + i);
            }
        }

        for (auto state : new_states) {
            new_states.insert(new_states.end(), makeStatesAndActions(state));
        }

        return new_states;
    }


}
