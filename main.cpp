//
// Created by Vinay Reddy Ratnam on 6/3/25.
//
#include "Players.h"
#include "Board.h"
#include <iostream>
#include <random>
#include <vector>
#include <utility>
using namespace std;


int main() {
    while (true) {
        int choice;
        cout << "Please choose what you would like to do. Enter an integer value:" << endl;
        cout << "1. Train bots using Monte Carlo Simulations" << endl;
        cout << "2. Train bots using Q-Learning" << endl;
        cout << "3. Play against bot as Xs" << endl;
        cout << "4. Play against bot as Os" << endl;
        cout << "5. End Program" << endl;
        cout << "6. Display Board Test" << endl;

        cin >> choice;

        if (choice == 1) {

        }
        else if (choice == 2) {

        }
        else if (choice == 3) {

        }
        else if (choice == 4) {

        }
        else if (choice == 6) {
            Board currBoard;
            currBoard.displayBoard();
        }
        else {
            return 0;
        }
    }

};

void trainingMC() {
    Players Bots;
    int training_length;

    cout << "For how many games do you want to train the bot?" << endl;
    cin >> training_length;

    for (int curr_traj = 0; curr_traj < training_length; curr_traj++) {
        float epsilon = 1.f - (static_cast<float>(curr_traj) / training_length);
        Board currBoard;

        bool game_goes_on = true;
        while (game_goes_on) {
            string curr_state = currBoard.getBoardState();


            pair<int,int> bots_action = Bots.makeMoveMC(curr_state, epsilon);
            currBoard.playerAction(bots_action);


            int victory = currBoard.checkGameEnded();
            if (victory != 0) {
                game_goes_on = false;

                // change action-values if game has ended
                string end_board = currBoard.getBoardState();
                vector<pair<int,int>> hist = currBoard.episode_history;
                Bots.changeStates(hist, end_board, victory);
            }


        }
    }
}