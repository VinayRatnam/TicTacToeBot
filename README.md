# Creating a Tic-Tac-Toe Bot using Reinforcment Learning (RL)
In this project, a Player 1 Tic-Tac-Toe Bot and Player 2 Tic-Tac-Toe bot will be created using Monte Carlo (MC) Simulations and Q-Learning for both of them. <br>
The objective is to get a better understanding of why Q-Learning works much faster than MC during training and show this increase in sample efficiency through graphs at the end.

## A basic overview of RL
In supervised machine learning, models are trained using pre-existing notions of what is correct and using this to minimize the loss function. However, for a lot of problems in the world, we do not have a definite answer to what is "correct". To still trian models to solve these kind of problems, we introduce RL, where our agent can take many actions and trajectories to get a better understanding of what works and what doesn't when there isn't a clear answer of what is "correct".
![image](https://github.com/user-attachments/assets/1fb1119c-3bfe-46ab-bdba-b126c5ecc34e)
In this image, we get to see the basic format of RL; we have an agent which we are trying to train that can interact with the environment. It interacts through actions and discovers if these actions are good or bad from the rewards the environment throws back at it.
I won't get into the specifics of MC simulations and Q-Learning methods, but if you want to learn more of how this project was possible, and learn about action/state-value functions, returns, and more, a great intro to RL can be found in this video: https://www.youtube.com/watch?v=VnpRp7ZglfA&t=1271s

## Full RL Problem in Tic-Tac-Toe
Tic-Tac-Toe is a two player game where strategy differs depending on whether you place down Xs or Os. Hence, in this project, I decided to train two bots, one for Xs (Player 1) and one for Os (Player 2). The agent is quite similar for each player so let' s define the RL problem for these bots:

### States
I decided to define each state for each player as the board composition at the moment the agent makes a move. To do this, I used a 9 character string to express each different type of board:
Each character represents a spot on the board (ex. 1st character represents the top left spot on the board; ex. 3rd character represents top right position on the board).

### Actions
For each state, the agent's possible actions are essentially where the spot is empty and they can put their chip.

### Rewards for Monte Carlo Simulation
It is important for the rewards in RL training to be strictly adherent to what we are trying to achieve. I could have made the goal to be trying to win in the least amount of moves. However, in such a short game like 
Tic-Tac-Toe, winning in the least amount of moves is basically irrelevant so I defined my rewards to adhere to the goal of winning, and trying your best not to lose.
I did this by setting the reward to 1 if the player made a winning move, 0 if the player made a move that led to their immediate loss, and 0 elsewhere. I 'backpropagated' these rewards using a return function so that
the agent could learn what moves prior to its loss, victory, or draw gave it the best advantage.

### Rewards for Q-Learning
The rewards for winning and losing are the same, but in Q-learning, we adjust the action-value functions according to the maximum reward for the next state the agent went to. This way of learning allows the agent to learn
immediately after making a move rather than waiting for the game to end to calculate the returns of moves in the trajectory history.
