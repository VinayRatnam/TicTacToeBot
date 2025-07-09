#pragma once
#include "Players.h"
#include "Board.h"
#include <vector>
#include <fstream>
#include <map>
using namespace std;


/**
 * @brief Creates csv file with data on Monte Carlo Training vs Q-Learning Training.
 *
 * @format Creates csv files with long tidy format (Optimal for seaborn analysis in future).
 * Each line contains episode,learning_method,player,avg_action_value
 *
 */
int create_csv() {

    // initialize bots for Monte Carlo training
    Players BotsMC;
    Players BotsQLearning;

    // initialize csv file
    ofstream outputFile("training_log.csv");
    if (!outputFile) {
        std::cerr << "Error: Could not open the file for writing!" << std::endl;
        // Handle the error, maybe exit the function or program
        return 0;
    }

    // set up columns of csv file
    outputFile << "episodes_trained,learning_method,player,avg_trajectory_value\n";


    long episodes = 10000000;
    int sampling_freq = 10;
    int samples_at_freq = 0;

    for (int curr_traj = 1; curr_traj < episodes+1; curr_traj++) {

        // Epsilon decays from 1.0 down to nearly 0.0
        float epsilon = 1.0f - (static_cast<float>(curr_traj-1) / episodes-1);

        // initialize board for each training step
        Board currBoardMC;
        Board currBoardQlearning;

        // run trajectory for Monte Carlo Simulation
        bool game_goes_on = true;
        while (game_goes_on) {
            string curr_state = currBoardMC.getBoardState();

            pair<int,int> bots_action = BotsMC.makeMoveMC(curr_state, epsilon);
            currBoardMC.playerAction(bots_action);

            int victory = currBoardMC.checkGameEnded();
            if (victory != 0) {
                game_goes_on = false;

                // change action-values & policy if game has ended
                string end_board = currBoardMC.getBoardState();
                vector<pair<int,int>> hist = currBoardMC.getEpisodeHistory(); // Assumes this function exists
                int num_moves = currBoardMC.getMove(); // Assumes this function exists
                BotsMC.changeStates(hist, end_board, victory, num_moves);
            }
        }

        // run trajectory for Q-Learning simulation
        int game_ended = 0;

        string curr_state = currBoardQlearning.getBoardState();
        pair<int, int> curr_move = BotsQLearning.makeMoveQLearning(curr_state, epsilon);
        currBoardQlearning.playerAction(curr_move);
        curr_state = currBoardQlearning.getBoardState();
        curr_move = BotsQLearning.makeMoveQLearning(curr_state, epsilon);
        currBoardQlearning.playerAction(curr_move);

        // game runs as long as no one has won (or no draw)
        while(game_ended == 0) {
            // only calculate new action-values if game has not ended
            vector<pair<int, int>> episode_hist = currBoardQlearning.getEpisodeHistory();
            BotsQLearning.adjustActionValue(curr_state, episode_hist);

            curr_move = BotsQLearning.makeMoveQLearning(curr_state, epsilon);
            currBoardQlearning.playerAction(curr_move);
            curr_state = currBoardQlearning.getBoardState();
            game_ended = currBoardQlearning.checkGameEnded();
        }

        // calculate action-values when game has ended
        BotsQLearning.adjustTerminalActionVals(curr_state, currBoardQlearning.getEpisodeHistory(), game_ended);

        if (curr_traj % sampling_freq == 0) {
            // implement logic for getting avg action value at this point
            BotsMC.generateData(outputFile, true, curr_traj);
            BotsQLearning.generateData(outputFile, false, curr_traj);
            samples_at_freq++;

            if (samples_at_freq == 10) {
                sampling_freq *= 10;
                samples_at_freq = 0;
            }
        }
    }
    cout << "Training Completed." << endl;

}

int main() {
    // Call the function that contains all your logic
    create_csv();

    // Return 0 to indicate the program finished successfully
    return 0;
}
