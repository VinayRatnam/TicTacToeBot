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

    // initialize csv file
    ofstream outputFile("training_log.csv");
    if (!outputFile) {
        std::cerr << "Error: Could not open the file for writing!" << std::endl;
        // Handle the error, maybe exit the function or program
        return 0;
    }

    // set up columns of csv file
    outputFile << "episode,learning_method,player,avg_trajectory_value\n";


    long episodes = 10000000;
    int sampling_freq = 1;
    int samples_at_freq = 0;

    for (int curr_traj = 0; curr_traj < episodes; curr_traj++) {

        // Epsilon decays from 1.0 down to nearly 0.0
        float epsilon = 1.0f - (static_cast<float>(curr_traj) / episodes);

        // initialize board for each training step
        Board currBoard;

        bool game_goes_on = true;
        while (game_goes_on) {
            string curr_state = currBoard.getBoardState();

            pair<int,int> bots_action = BotsMC.makeMoveMC(curr_state, epsilon);
            currBoard.playerAction(bots_action);

            int victory = currBoard.checkGameEnded();
            if (victory != 0) {
                game_goes_on = false;

                // change action-values & policy if game has ended
                string end_board = currBoard.getBoardState();
                vector<pair<int,int>> hist = currBoard.getEpisodeHistory(); // Assumes this function exists
                int num_moves = currBoard.getMove(); // Assumes this function exists
                BotsMC.changeStates(hist, end_board, victory, num_moves);
            }
        }

        if (curr_traj % sampling_freq == 0) {
            // implement logic for getting avg action value at this point
            BotsMC.generateData(outputFile, true);
        }
    }
    cout << "Training completed." << endl;



}
