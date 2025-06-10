#include <vector>
#include <map>
#include <utility>
#include <string>
#include <set>
#include <algorithm>
#include <random>
#include <iostream>
#include <cmath>
#include <fstream>

// Forward declarations to avoid dependency issues if Board needs Players or vice-versa
class Players;
class Board; // Assuming you have a Board class defined elsewhere

using namespace std;

class Players {
private:
    map<string, map<int, float>> action_values;
    map<string, map<int, float>> policies;
    map<string, map<int, float>> uniform_policy;
    default_random_engine generator;
    uniform_real_distribution<float> distribution;

public:
    Players() : generator(12345), distribution(0.0f,1.0f) {
        pair<string, pair<int, int>> initial_state {"", {0,0}};
        createStates(initial_state);
        uniform_policy = policies;
    }

    pair<int,int> makeMoveMC(const string& curr_state, float epsilon) {
        map<int, float> policy;
        if (distribution(generator) < epsilon) {
            policy = uniform_policy[curr_state];
        } else {
            policy = policies[curr_state];
        }

        int action = selectMove(policy);
        return convertToBoardSpot(action);
        return {-1, -1};
    }

    void changeStates(const vector<pair<int, int>>& episode_history, const string& grid_string, int victory, int num_moves) {
        int player1_return, player2_return;
        if (victory == 1) {
            player1_return = 10;
            player2_return = -10;
        } else if (victory == 2) {
            player1_return = -10;
            player2_return = 10;
        } else {
            player1_return = 0;
            player2_return = 0;
        }

        string curr_grid = grid_string;
        float gamma = 0.9f;
        float alpha = 0.1f;
        set<string> visited_states; // Keep track of states to update their policy later

        while (num_moves > 0) {
            curr_grid = goBackOneMove(curr_grid, episode_history[num_moves - 1]);
            int action = episode_history[num_moves - 1].first * 3 + episode_history[num_moves - 1].second;

            // Add state to the set of states that need a policy update
            visited_states.insert(curr_grid);

            if (num_moves % 2 == 0) { // Player 2's turn
                action_values[curr_grid][action] += alpha * (player2_return - action_values[curr_grid][action]);
                player2_return *= gamma;
            } else { // Player 1's turn
                action_values[curr_grid][action] += alpha * (player1_return - action_values[curr_grid][action]);
                player1_return *= gamma;
            }
            num_moves--;
        }

        // --- POLICY IMPROVEMENT STEP ---
        // After all action-values for the episode are updated, THEN update the policies
        for (const string& state_to_update : visited_states) {
            updatePolicyForState(state_to_update);
        }
    }

    /**
     * @brief Bot decides on a move based on greatest action_value in current state
     * @param curr_state is the string holding the current state of the board
     * @return <row, col> of the move that the bot wants to make. If uniform dist.
     * generates number below epsilon, use a random move generated from uniform_policy.
     */
    pair<int, int> makeMoveQLearning(string curr_state, float epsilon) {
        if (distribution(generator) < epsilon) {
            // make random move based on uniform_policy
            map<int, float> curr_uniform_pol = uniform_policy[curr_state];
            int action = selectMove(curr_uniform_pol);
            return convertToBoardSpot(action);
        }
        else {
            // make move with highest action-value
            map<int, float> curr_action_vals = action_values[curr_state];
            auto max = curr_action_vals.begin();
            for (auto iter = ++curr_action_vals.begin(); iter != curr_action_vals.end(); iter++) {
                if (iter->second > max->second) {
                    max = iter;
                }
            }

            return convertToBoardSpot(max->first);
        }

    };

    /**
     * @brief Function creates a csv of bot's policies for all states
     *
     * @format (Long/Tidy Format) The csv's format for each line will be:
     * state,action,probability\n
     * This means each line will store a state along with one of its actions
     * and that action's corresponding probability
     *
     * @return true if csv was created successfully, false otherwise
     */
    bool createCSVPolicies() {

    }

    bool createCSVActionValues() {}


private:
    void createStates(pair<string, pair<int,int>> curr_state) {
        if (curr_state.first.size() == 9) {
            map<int, float> actions;
            map<int, float> policy;
            for (int i = 0; i < 9; i++) {
                if (curr_state.first[i] == 'a') {
                    actions[i] = 0.0;
                    policy[i] = 0.0;
                }
            }

            float uniform_val = 0.0f; // Initialize to a safe default
            if (!policy.empty()) {
                uniform_val = 1.0f / policy.size();
            }

            for (auto iter = policy.begin(); iter != policy.end(); ++iter) {
                iter->second = uniform_val;
            }
            action_values[curr_state.first] = actions;
            policies[curr_state.first] = policy;
        } else {
            if (curr_state.second.first < 5) {
                createStates({curr_state.first + 'b', {curr_state.second.first + 1, curr_state.second.second}});
            }
            if (curr_state.second.second < 4) {
                createStates({curr_state.first + 'c', {curr_state.second.first, curr_state.second.second + 1}});
            }
            createStates({curr_state.first + 'a', curr_state.second});
        }
    }

    pair<int,int> convertToBoardSpot(int val) {
        switch (val) {
            case 0: return {0,0};
            case 1: return {0,1};
            case 2: return {0,2};
            case 3: return {1,0};
            case 4: return {1,1};
            case 5: return {1,2};
            case 6: return {2,0};
            case 7: return {2,1};
            case 8: return {2,2};
            default: return {-1,-1};
        }
    }

    string goBackOneMove(string state, pair<int, int> last_move) {
        int index = last_move.first * 3 + last_move.second;
        if (index >= 0 && index < state.length()) {
            state[index] = 'a';
        }
        return state;
    }

    /**
     * @brief Selects an action based on a given probability distribution (policy).
     * @param pol is a reference to the given probability distribution
     * @return An integer representing the chosen action, or -1 if the policy is empty.
     */
    int selectMove(const map<int, float>& pol) {
        float choice = distribution(generator);
        float sum_probs = 0;
        for (auto iter = pol.begin(); iter != pol.end(); iter++) {
            sum_probs += iter->second;
            if (choice < sum_probs) {
                return iter->first;
            }
        }
        return pol.rbegin()->first; // return last action just in case nothing returns
    }

    // Using the "pure function" approach for calculating softmax is cleaner
    map<int, float> calculateSoftmax(const map<int, float>& values) {
        map<int, float> probabilities;
        if (values.empty()) {
            return probabilities;
        }

        float max_val = values.begin()->second;
        for (const auto& pair : values) {
            if (pair.second > max_val) max_val = pair.second;
        }

        float sum_exp = 0.0f;
        for (const auto& pair : values) {
            sum_exp += exp(pair.second - max_val);
        }

        if (sum_exp > 0) {
            for (const auto& pair : values) {
                probabilities[pair.first] = exp(pair.second - max_val) / sum_exp;
            }
        }
        return probabilities;
    }

    // Public-style function from the artifact, now made private
    void updatePolicyForState(const string& state) {
        if (action_values.count(state)) {
            const map<int, float>& values = action_values.at(state);
            policies[state] = calculateSoftmax(values);
        }
    }
};