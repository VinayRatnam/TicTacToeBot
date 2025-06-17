/**
 * @file main.cpp
 * @brief Main entry point for the Tic-Tac-Toe Game
 *
 * Through this program, you can train an AI to play
 * Tic-Tac-Toe through Monte Carlo Simulations or
 * Q-Learning Methods. Additionally, you can play against
 * this bot as either Xs or Os.
 *
 * @author Vinay Reddy Ratnam
 * @date_created 05/29/25
*/


#include "Players.h" // Defines the Player Agent
#include "Board.h" // Defines Board that Agent & User can interact with
#include <iostream>
#include <vector>
#include <utility>
#include <limits> // Required for numeric_limits

using namespace std;

// Declare Monte Carlo training function
void trainingMC(Players &Bots);
void trainingQLearning(Players &Bots);

/**
 * @brief Entry point for program. Presents a menu that gives options to either
 * train AI, play against it, or exit program.
 *
 * @return 0 upon successful execution
 */
int main() {
    // Initializes AI bots. These bots persist throughout the main function
    // execution unless program is restarted.
    Players Bots;

    while (true) {
        int choice;
        cout << "--- Tic-Tac-Toe AI ---" << endl;
        cout << "1. Train bots using Monte Carlo Simulations" << endl;
        cout << "2. Train bots using Q-Learning" << endl;
        cout << "3. Play against bot as Xs (Player 1)" << endl;
        cout << "4. Play against bot as Os (Player 2)" << endl;
        cout << "5. End Program" << endl;
        cout << "Please choose an option: ";

        cin >> choice;

        // Input validation
        if (cin.fail()) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 1) {
            // train bots using Monte Carlo Simulations
            trainingMC(Bots);
        }

        else if (choice == 2) {
            trainingQLearning(Bots);
        }

        else if (choice == 3) {
            // Creates Tic-Tac-Toe game with User being Player 1
            Board currBoard;

            // set up which type of bot user is playing against
            bool bot_type; // false for MC, true for Q Learning
            string str_bot_type;
            cout << "Please select which type of bot to play against (1. Monte Carlo 2. Q-Learning): ";
            cin >> str_bot_type;
            if (stoi(str_bot_type) == 1) {
                bot_type = false;
            }
            else {
                bot_type = true;
            }

            // loop runs as long as game has not ended
            while (currBoard.checkGameEnded() == 0) {

                // display board so user can make a move
                currBoard.displayBoard();

                int row = -1, col = -1;

                cout << "Please select which row you'd like to place your X in (1-3): ";
                cin >> row;
                cout << "Please select which column you'd like to place your X in (1-3): ";
                cin >> col;

                // input player move into Board instance
                currBoard.playerAction({row-1, col-1});

                // Check if the human's move ended the game
                if (currBoard.checkGameEnded() != 0) {
                    break; // Exit loop before the bot gets to move
                }

                cout << "\nBot is making a move..." << endl;

                string curr_state = currBoard.getBoardState();
                float eps = 0.0f; // Epsilon = 0 for deterministic play
                pair<int,int> bots_action;
                if (bot_type) {
                    bots_action = Bots.makeMoveQLearning(curr_state, eps);
                }
                else {
                    bots_action = Bots.makeMoveMC(curr_state, eps);
                }


                // Check if the bot returned a valid move
                if (bots_action.first == -1) {
                    cout << "Bot doesn't know how to respond to this state and forfeits." << endl;
                    break;
                }

                currBoard.playerAction(bots_action);
            }


            // Display final board and result
            cout << "\n--- Game Over ---" << endl;
            currBoard.displayBoard();
            int result = currBoard.checkGameEnded();
            if (result == 1) cout << "You win!" << endl;
            else if (result == 2) cout << "Bot wins!" << endl;
            else cout << "It's a draw!" << endl;
        }

        else if (choice == 4) {
            // Create Tic-Tac-Toe game with User as Player 2
            Board currBoard;
            bool game_on = true;

            // set up which type of bot user is playing against
            bool bot_type; // false for MC, true for Q Learning
            string str_bot_type;
            cout << "Please select which type of bot to play against (1. Monte Carlo 2. Q-Learning): ";
            cin >> str_bot_type;
            if (stoi(str_bot_type) == 1) {
                bot_type = false;
            }
            else {
                bot_type = true;
            }

            while (game_on) {
                // --- BOT'S TURN ---
                cout << "\nBot is making a move..." << endl;
                string curr_state = currBoard.getBoardState();
                float eps = 0.0f; // Epsilon = 0 for deterministic play
                pair<int, int> bots_action;
                if (bot_type) {
                    bots_action = Bots.makeMoveQLearning(curr_state, eps);
                }
                else {
                    bots_action = Bots.makeMoveMC(curr_state, eps);
                }

                if (bots_action.first == -1) {
                    cout << "Bot doesn't know how to respond to this state and forfeits." << endl;
                    break;
                }
                currBoard.playerAction(bots_action);
                currBoard.displayBoard();

                if (currBoard.checkGameEnded() != 0) {
                    game_on = false; // Game over, exit loop
                    continue;
                }

                // --- HUMAN'S TURN ---
                int row = -1, col = -1;

                cout << "Please select which row you'd like to place your O in (1-3): ";
                cin >> row;
                cout << "Please select which column you'd like to place your O in (1-3): ";
                cin >> col;

                currBoard.playerAction({row-1, col-1});


                if (currBoard.checkGameEnded() != 0) {
                    game_on = false; // Game over, exit loop
                }
            }

            // Display final board and result
            cout << "\n--- Game Over ---" << endl;
            currBoard.displayBoard();
            int result = currBoard.checkGameEnded();
            if (result == 1) cout << "Bot wins!" << endl;
            else if (result == 2) cout << "You win!" << endl;
            else if (result == 3) cout << "It's a draw!" << endl;
            else cout << "Game ended unexpectedly." << endl;
        }

        else if (choice == 5) {
            cout << "Exiting program." << endl;
            return 0;
        }

        else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
    return 0;
};


/**
 * @brief Performs a number of trials chosen by User for bot to train
 * against itself using Monte Carlo Simulations. It employs a epsilon
 * -greedy strategy where the bot slowly sticks more to its policies
 * after more iterations to balance exploration vs exploitation. After
 * each game, it updates its action-value functions and policy.
 *
 * @param Bots A reference to the Players object will be trained. This
 * object's action-values and policies will be refined.
 */
void trainingMC(Players &Bots) {
    // Training_length is chosen by the User
    int training_length;

    cout << "For how many games do you want to train the bot?" << endl;
    cin >> training_length;


    // input validation for invalid number/input for training_length
    if (cin.fail() || training_length <= 0) {
        cout << "Invalid number of games." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    cout << "Starting training for " << training_length << " games..." << endl;

    for (int curr_traj = 0; curr_traj < training_length; curr_traj++) {
        // Epsilon decays from 1.0 down to nearly 0.0
        float epsilon = 1.0f - (static_cast<float>(curr_traj) / training_length);

        // initialize board for each training step
        Board currBoard;

        bool game_goes_on = true;
        while (game_goes_on) {
            string curr_state = currBoard.getBoardState();

            pair<int,int> bots_action = Bots.makeMoveMC(curr_state, epsilon);
            currBoard.playerAction(bots_action);

            int victory = currBoard.checkGameEnded();
            if (victory != 0) {
                game_goes_on = false;

                // change action-values & policy if game has ended
                string end_board = currBoard.getBoardState();
                vector<pair<int,int>> hist = currBoard.getEpisodeHistory(); // Assumes this function exists
                int num_moves = currBoard.getMove(); // Assumes this function exists
                Bots.changeStates(hist, end_board, victory, num_moves);
            }
        }
    }
    cout << "Training completed." << endl;

}

/**
 * @brief Trains bots using Q-Learning.
 * Asks user for a number of iterations to train the bot. Uses an epsilon-greedy method
 * where as bot gets more experience (has gone through more episodes), it chooses the
 * action with the highest action-value function. Temporal differences are used to train
 * the bot, so it will change action-value function for the action it chooses at its
 * previous state while the game continues. Policy is not used in Q-Learning.
 *
 * @param Bots: Action-Value Functions are stored within Bots; they will be updated during
 * training
 */
void trainingQLearning(Players &Bots) {
    int episodes = 0;
    cout << "For how many games do you want to train the bot?" << endl;
    cin >> episodes;

    for (int episode = 0; episode < episodes; episode++) {
        // initialize a board for each trajectory
        Board currBoard;
        float epsilon = 1 - (static_cast<float>(episode) / episodes);
        int game_ended = 0;


        // have both bots make one move each to start training
        string curr_state = currBoard.getBoardState();
        pair<int, int> curr_move = Bots.makeMoveQLearning(curr_state, epsilon);
        currBoard.playerAction(curr_move);
        curr_state = currBoard.getBoardState();
        curr_move = Bots.makeMoveQLearning(curr_state, epsilon);
        currBoard.playerAction(curr_move);

        // game runs as long as no one has won (or no draw)
        while(game_ended == 0) {
            // only calculate new action-values if game has not ended
            vector<pair<int, int>> episode_hist = currBoard.getEpisodeHistory();
            Bots.adjustActionValue(curr_state, episode_hist);

            curr_move = Bots.makeMoveQLearning(curr_state, epsilon);
            currBoard.playerAction(curr_move);
            curr_state = currBoard.getBoardState();
            game_ended = currBoard.checkGameEnded();
        }

        // calculate action-values when game has ended
        Bots.adjustTerminalActionVals(curr_state, currBoard.getEpisodeHistory(), game_ended);
    }
}