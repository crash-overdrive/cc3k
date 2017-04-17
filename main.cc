#include <memory>
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <termios.h>
#include <unistd.h>
#include "board.h"
#include "status_code.h"                             

using namespace std;

bool fileExists(string filename) {
	return true;
}

void greetings() {
        cout << "Welcome to Chamber Crawler 3000. Greetings Player. Hope you have Fun playing this! Good Luck" <<  endl;
        cout << "Created with <3 By Kai Koponen, Monique Sidebottom and Shashwat Pratap" << endl;
        cout << "Starting the Game...." << endl;

}


char raceSelection() {
	while(true) {
		cout << "Enter the race you want to play as: Shade(s) , Drow(d) , Vampire(v) , Goblin(g) , Troll(t) or enter q for quitting : ";
		char c;
		cin >> c;
		if (cin.eof()) {
			cin.clear();
			return 'q';
		}
		else if (c == 's' || c == 'd' || c == 'v' || c == 'g' || c == 't' || c == 'q' || c == 'S' || c == 'D' || c == 'V' || c== 'G' || c == 'T' || c == 'Q') return c;
		else {
			cout << "Invalid Race Selection. Please try again." << endl;
		}
	}	
}


void successfulRaceSelection(string race) {
	cout << "Race Selection Successful. You have chosen to play as " << race << " " << endl;
}

string getProperDirection(string move) {
	while(true) {		
		if (move == "NO" || move == "no") {
			move = "no";
			return move;
		}	
		else if (move == "SO" || move == "so") {
			move = "so";
			return move;
		}
		else if (move == "EA" || move == "ea") {
			move = "ea";
			return move;
		}
		else if (move == "WE" || move == "we") {
			move = "we";
			return move;
		}
		else if (move == "NE" || move == "ne") {
			move = "ne";
			return move;
		}
		else if (move == "NW" || move == "nw") {
			move = "nw";
			return move;
		}
		else if (move == "SE" || move == "se") {
			move = "se";
			return move;
		}
		else if (move == "SW" || move == "sw") {
			move = "sw";
			return move;
		}
		else {
			cout << "Invalid Direction!! Proper Directions are North(no), South(so), East(ea), West(we), NorthEast(ne), NorthWest(nw), SouthEast(se), SouthWest(sw)... " << endl;
			cout << "Enter Direction again : " ;
			cin >> move;
			if (cin.eof()) {
				cin.clear();								
			}
		}
	}
}

string getProperDirectionNCurses(string move) {
	while(true) {		
		if (move == "i" || move == "I") {
			move = "nw";
			return move;
		}	
		else if (move == "/" || move == "?") {
			move = "se";
			return move;
		}
		else if (move == ";" || move == ":") {
			move = "ea";
			return move;
		}
		else if (move == "k" || move == "K") {
			move = "we";
			return move;
		}
		else if (move == "p" || move == "P") {
			move = "ne";
			return move;
		}
		else if (move == "O" || move == "o") {
			move = "no";
			return move;
		}
		else if (move == "." || move == ">") {
			move = "so";
			return move;
		}
		else if (move == "," || move == "<") {
			move = "sw";
			return move;
		}
		else {
			cout << "Invalid Direction!! Proper Directions are North( p ), South( , ), East( ; ), West( k ), NorthEast( o ), NorthWest( [ ), SouthEast( . ), SouthWest( m )... " << endl;
			cout << "Enter Direction again : " ;
			struct termios old_tio, new_tio;		

			/* get the terminal settings for stdin */
			tcgetattr(STDIN_FILENO,&old_tio);

			/* we want to keep the old setting to restore them a the end */
			new_tio=old_tio;

			/* disable canonical mode (buffered i/o) and local echo */
			new_tio.c_lflag &=(~ICANON & ~ECHO);

			/* set the new settings immediately */
			tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);
			char c; 
			c = getchar();
			
			//cin >> move;
			stringstream ss;		
			ss << c;
			ss >> move;
			cout << move << endl;
			tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);
			
		}
	}
}

int PCTurnNCurses(Board &B) {
	struct termios old_tio, new_tio;
	char c;

	/* get the terminal settings for stdin */
	tcgetattr(STDIN_FILENO,&old_tio);

	/* we want to keep the old setting to restore them a the end */
	new_tio=old_tio;

	/* disable canonical mode (buffered i/o) and local echo */
	new_tio.c_lflag &=(~ICANON & ~ECHO);

	/* set the new settings immediately */
	tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);
		
		
	string move;
	bool endgame = false;
	while(true) {
		//cout << "Entered while loop of n cursor!!" << endl;
		cout << "Player's Turn. Enter your move: " ;		
		c = getchar();
		//cin >> move;
		stringstream ss;		
		ss << c;
		ss >> move;
		cout << move << endl;

		if (move == "o" || move == "p" || move == "i" || move == "k" || move == ";" || move == "," || move == "?"  || move == "." ||
			move == "O" || move == "P" || move == "I" || move == "K" || move == ":" || move == "<" || move == "/"  || move == ">") {
			move = getProperDirectionNCurses(move);
			endgame = B.move(move);


			/* restore the former settings */
			tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);

			if (endgame) {
				//cout << "Returning CG from while loop of ncurses : " << endl;
				return COMPLETED_GAME;
			}
			//cout << "Returning PCT from while loop of ncurses : " << endl;
			return PC_TURN;
		}
		else if (move == "f" || move == "F") {

			/* restore the former settings */
			tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);

			//cout << "Returning TEM from while loop of ncurses : " << endl;
			return TOGGLE_ENEMY_MODE;
		}
		else if (move == "r" || move == "R") {
			//cout << "Returning RG from while loop of ncurses : " << endl;

			/* restore the former settings */
			tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);

			return RESTART_GAME;
		}
		else if (move == "q" || move == "Q") {
			//cout << "Returning UT from while loop of ncurses : " << endl;

			/* restore the former settings */
			tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);

			return USER_TERMINATION;
		}
		else if (move == "u" || move == "U") {
			char dir;	
			string direction;
			cout << "Enter direction of the potion which you want to use: " ;		
			dir = getchar();
			stringstream ss;
			ss << dir;
			ss >> direction;
			cout << direction << endl;
			direction = getProperDirectionNCurses(direction);
			endgame = B.usePotion(direction);

			/* restore the former settings */
			tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);
			
			if (endgame) {
				//cout << "Returning CG from while loop of ncurses : " << endl;
				return COMPLETED_GAME;
			}		
			//cout << "Returning PCT from while loop of ncurses : " << endl;
			return PC_TURN;
		}
		else if (move == "a" || move == "A") {
			char dir;
			string direction;			
			cout << "Enter direction in which you want to attack: " ;
			dir = getchar();
			stringstream ss;
			ss << dir;
			ss >> direction;
			cout << direction << endl;
			direction = getProperDirectionNCurses(direction);
			endgame = B.attack(direction);		

			/* restore the former settings */
			tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);

			if (endgame) {
				//cout << "Returning CG from while loop of ncurses : " << endl;
				return COMPLETED_GAME;
			}	
			//cout << "Returning PCT from while loop of ncurses : " << endl;
			return PC_TURN;
		}
		else if (move == "h" || move == "H") {
			cout << "Enter i / o / p  / ; / (/) / . / , / k to move your character in that direction" << endl;
			cout << "Enter f to stop enemies from moving" << endl;
			cout << "Enter r to restart game" << endl;
			cout << "Enter q to end game" << endl;
			cout << "Enter u <direction(new)> to use a potion in that particular direction" << endl;
			cout << "Enter a <direction(new)> to initiate an attack on an enemy in that direction" << endl;
			cout << "Enter n to exit WASD Mode..." << endl;
 		}
 		else if (move == "N" || move == "n") {

 			/* restore the former settings */
 			tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);

 			//cout << "Returning NCM from while loop of ncurses : " << endl;
 			return NCURSES_MODE;
 			
 		}
		else {
			cout << "Dude stop giving me Garbage Command and try again(WASD MODE).. Press (h) for help" << endl;
		}
	}
}

int PCTurn(Board &B) {	
	string move;
	bool endgame = false;
	while(true) {
		cout << "Player's Turn. Enter your move: " ;
		cin >> move;
		if (move == "no" || move == "so" || move == "ea" || move == "we" || move == "ne" || move == "nw" || move == "se"  || move == "sw" ||
			move == "NO" || move == "SO" || move == "EA" || move == "WE" || move == "NE" || move == "NW" || move == "SE"  || move == "SW") {
			move = getProperDirection(move);
			endgame = B.move(move);
			if (endgame) {
				return COMPLETED_GAME;
			}
			return PC_TURN;
		}
		else if (move == "f" || move == "F") {
			return TOGGLE_ENEMY_MODE;
		}
		else if (move == "r" || move == "R") {
			return RESTART_GAME;
		}
		else if (move == "q" || move == "Q") {
			return USER_TERMINATION;
		}
		else if (move == "u" || move == "U") {
			string direction;			
			cin >> direction;
			direction = getProperDirection(direction);
			endgame = B.usePotion(direction);	
			if (endgame) {
				return COMPLETED_GAME;
			}		
			return PC_TURN;
		}
		else if (move == "a" || move == "A") {
			string direction;			
			cin >> direction;
			direction = getProperDirection(direction);
			endgame = B.attack(direction);		
			if (endgame) {
				return COMPLETED_GAME;
			}	
			return PC_TURN;
		}
		else if (move == "h" || move == "H") {
			cout << "Enter no / so / ea / we/ ne / nw / se / sw to move your character in that direction" << endl;
			cout << "Enter f to stop enemies from moving" << endl;
			cout << "Enter r to restart game" << endl;
			cout << "Enter q to end game" << endl;
			cout << "Enter u <direction> to use a potion in that particular direction" << endl;
			cout << "Enter a <direction> to initiate an attack on an enemy in that direction" << endl;
			cout << "Enter n to activate WASD mode(DLC)!!! " << endl;
 		}


 		else if (move == "N" || move == "n") {
 			cout << "WASD Mode Activated!! " << endl;
 			cout << "Press (h) for help..." << endl;
 			//give all other info out here
 			
 			cin.ignore();
 			return NCURSES_MODE;


 		}
		else {
			cout << "Dude stop giving me Garbage Command and try again.. Press (h) for help" << endl;
		}
	}
}



int startGame(string presetfilename) {
	int status_code = START_GAME;
	int temp_status_code = status_code;
	string race;
	char c;
	#ifdef DEBUG
		cout << "Running command with status code: " << status_code << endl;
	#endif
	c = raceSelection();
	switch (c) {
		case 'Q':{						
			temp_status_code = USER_TERMINATION;
			#ifdef DEBUG
				cout << "Completed command with status code " << status_code << endl;
				cout << "Recieved status code " << temp_status_code << endl;
			#endif
			status_code = temp_status_code;
			return USER_TERMINATION;
		}
		case 'q':{
			temp_status_code = USER_TERMINATION;
			#ifdef DEBUG
				cout << "Completed command with status code " << status_code << endl;
				cout << "Recieved status code " << temp_status_code << endl;
			#endif
			status_code = temp_status_code;
			return USER_TERMINATION;
		}
		case 'S':{
			race = "Shade";
    		break;
		}			
		case 's':{
			race = "Shade";
    		break;
		}			
    	case 'D' :{
			race = "Drow";
        	break;
		}				
		case 'd':{
			race = "Drow";
       	   	break;
		}			
       	case 'V':{
			race = "Vampire";
        	break;
		}			
		case 'v':{
			race = "Vampire";
        	break;
		}			
        case 'T':{
			race = "Troll";
        	break ;
		}			
		case 't': {
			race = "Troll";
        	break;
		}			
        case 'G':{
			race = "Goblin";
        	break;
		}			
		case 'g': {
			race = "Goblin";
        	break;
		}	
	}
	successfulRaceSelection(race);
	Board B{presetfilename , race};
	temp_status_code = PC_TURN;
	#ifdef DEBUG
		cout << "Completed command with status code " << status_code << endl;
		cout << "Recieved status code " << temp_status_code << endl;
	#endif
	status_code = temp_status_code;		
	

	while (true) {
		#ifdef DEBUG
			cout << "Entered into while loop of startGame " << endl;
			cout << "Running command with status code: " << status_code << endl;
		#endif
		switch (status_code) {
			
			case PC_TURN: {
				temp_status_code = PCTurn(B); //PCTurn returns STOP_ENEMY_TURN, USER_TERMINATION, PC_TURN, COMPLETED_GAME, NCURSES_MODE or RESTART_GAME
				#ifdef DEBUG
					cout << "Completed command with status code " << status_code << endl;
					cout << "Recieved status code " << temp_status_code << endl;
				#endif
				status_code = temp_status_code;
				break;

			}	

			//have to exit out of this loop and main if user terminates the program by typing q and admitting defeat(What a Noob!)
			case USER_TERMINATION: {	
				temp_status_code = status_code;
				#ifdef DEBUG
					cout << "Completed command with status code " << status_code << endl;
					cout << "status code unchanged with value " << temp_status_code << endl;
				#endif
				return status_code;
			}		
				

			//if status code is RESTART_GAME then we have to send status_code to main and restart the game by calling the method again
			case RESTART_GAME: {
				temp_status_code = status_code;
				#ifdef DEBUG
					cout << "Completed command with status code " << status_code << endl;
					cout << "status code unchanged with value " << temp_status_code << endl;
				#endif
				return status_code;
			}

			
			case TOGGLE_ENEMY_MODE: { 
				B.toggleEnemyMode();
				temp_status_code = PC_TURN; 

				#ifdef DEBUG
					cout << "Completed command with status code " << status_code << endl;	
					cout << "Recieved status code " << temp_status_code << endl;
				#endif

				status_code = temp_status_code;
				break;
			}	

			case NCURSES_MODE: {

				temp_status_code = PCTurnNCurses(B); // PCTurnNCurses returns USER_TERMINATION, RESTART_GAME, COMPLETED_GAME, PC_TURN
				/*
				#ifdef DEBUG
					cout << "Completed command with status code " << status_code << endl;	
					cout << "Recieved status code " << temp_status_code << endl;
				#endif
				*/
				//cout << "Status Code after exiting from NCURSE iteration" << temp_status_code << endl;
				status_code = temp_status_code;
				if (status_code == NCURSES_MODE) {
					cout << "Exited out of WASD Mode..." << endl;
					cout << "Hope you liked this new mode!!" << endl;
					status_code = PC_TURN;					
				}
				else if (status_code == TOGGLE_ENEMY_MODE) {
					B.toggleEnemyMode();
					status_code = NCURSES_MODE;
				}
				else if (status_code == PC_TURN) {
					status_code = NCURSES_MODE;
				}
				temp_status_code = status_code;
				break;
			}	


			case COMPLETED_GAME: {	
				char input;			
				cout << "Wanna have a rematch (Y/N)? ";
				cin >> input;
				if (input == 'Y' || input == 'y') {
					temp_status_code = RESTART_GAME;
					#ifdef DEBUG
						cout << "Completed command with status code " << status_code << endl;	
						cout << "Recieved status code " << temp_status_code << endl;
					#endif
					status_code = temp_status_code;
					return status_code;
				}
				else {
					temp_status_code = USER_TERMINATION;
					#ifdef DEBUG
						cout << "Completed command with status code " << status_code << endl;	
						cout << "Recieved status code " << temp_status_code << endl;
					#endif
					status_code = temp_status_code;
					return status_code;
				}

			}			

			default:{
				cout << "Ran into an impossible status code " << status_code << " in startGame." << endl;
				cout << "Returning to main..." << endl;
				#ifdef DEBUG
					cout << "Completed command with status code " << status_code << endl;
				#endif
				return status_code;
			}		

		}
	}
}




int main(int argc, char *argv[]) {
	int exit_code;
	string presetfilename = "";
	int seed = 9;
	string filename;


	//command line options passed
	if (argc == 2) {
		filename = argv[1];
		istringstream ss{filename};
		if (ss >> seed) {
			cout << "Random Generation Activated. Pray that the RNG God has mercy on you. " << endl;
			cout << "Using value for seed : " << seed << endl;
			srand(seed);
		}
		else if (fileExists(filename)) {
			cout << "Reading in file : " << filename << endl;
			srand(time(0));
			cout << "Using random value for seed... " << endl;
			presetfilename = filename;
		}
		else {
			cout << "File name does not exist in the current directory. Exiting..." << endl;
			cout << "Exiting out of main!! " << endl;
			return 0;
		}
	}
	//fancy way to make second command line argument acceptable and ignore any other argument other than first 2
	else if (argc > 2) {
		presetfilename = argv[1];
		string strSeed = argv[2];
		if (fileExists(presetfilename)) {
			while (true) {
				istringstream ss{strSeed};
				if (ss >> seed) break;
				else {
					cout << "Behave!! Gimme valid value for setting the seed please: ";
					cin >> strSeed;
				}
			}
			cout << "Reading in file : " << presetfilename << endl;
			srand(seed);
			cout << "Using value for seed : " << seed << endl;
		}
		else {
			cout << "File name does not exist in the current directory. Exiting..." << endl;
			cout << "Exiting out of main!! " << endl;
		}		
	}

	else {
		cout << "Random Generation Activated. Pray that the RNG God has mercy on you. " << endl;
		srand(time(0));
		cout << "Using random value for seed... " << endl;
	}

	//greet the player
	greetings();
	exit_code = startGame(presetfilename); //startGame only ever returns RESTART_GAME or USER_TERMINATION
	while (true) {
		
		#ifdef DEBUG
			cout << "Entered while loop in main.. " << endl;
			cout << "Got exit code in main: " << exit_code << endl;
			cout << "Doing appropriate stuff accordingly now.. " << endl;
		#endif
		switch (exit_code) {
			case USER_TERMINATION: {
				cout << "You choose to quit :( " << endl;
				cout << "Exiting out of main!! Ending Game...." << endl;
				return 0;
			}
				
			case RESTART_GAME:{
				cout << "You choose to restart the Game.. Sweet!! " << endl;
				exit_code = startGame(presetfilename);
				break;
			}
			default: {
				cout << "Well something went wrong, recieved " << exit_code << " exit code in main function..." << endl;
				cout << "Exiting out of main.. Fix your shit!!" << endl;
				return 0;
			}
				
		}
	}
}
//have to implement checking for file name existing in current directory
