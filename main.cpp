#include "dice.h"
#include <fstream>
#include <string>
using namespace std;

int power_int(int a, int n) {
	int temp = 1;
	for(int i=0; i<n; i++)
		temp *= a;
	return temp;
}

int main() {
	int total_entries = targetN * (TOTAL_ROUND+1);
	for(int i=0; i<TOTAL_DICE; i++) {
		total_entries *= DICE_FACES_NUMBER;
	}
	cout << total_entries << " entries" << endl;
	vector<State> states  = vector<State>(total_entries);
	int choiceN = power_int(2, TOTAL_DICE);
	for(int id=0; id<total_entries; id++) {
		// dynamic programing
		// cout << "calculating id " << id << endl;
		states[id] = decode(id);
		if (states[id].successQ()) 
			states[id].chance = 1.0;
		else if (states[id].remaining_round == 0)
			states[id].chance = 0.0;
		else {
			int best_choice = 0;
			double best_chance = 0;
			for(int choice=0; choice<choiceN; choice++) {
				int choice_weight = 0;
				int temp_choice = choice;
				for(int i=0; i<TOTAL_DICE; i++) {
					choice_weight += temp_choice % 2;
					// cout << temp_choice % 2 << " ";
					temp_choice /= 2;
				}
				// cout << " : " << choice_weight << " weight" << endl;
				int cases = power_int(DICE_FACES_NUMBER, choice_weight);
				double acc_chance = 0;
				for(int i=0; i<cases; i++) {
					acc_chance += states[states[id].traceback(choice_weight, choice, i)].chance;
				}
				acc_chance /= cases;
				if (acc_chance >= best_chance) {
					best_choice = choice;
					best_chance = acc_chance;
					// cout << " (" << choice << ", " << acc_chance << ") ";
				}
			}
			states[id].setFlip(best_choice);
			states[id].chance = best_chance;
		}
	}

	string filename = "F"+to_string(DICE_FACES_NUMBER)+"D"+to_string(TOTAL_DICE)+"R"+to_string(TOTAL_ROUND)+".data";
	ofstream ofs;
	ofs.open(filename);
	for(int id=0; id<total_entries; id++) {
		if(states[id].successQ()==0 && states[id].remaining_round > 0) {
			for(int i=0; i<TOTAL_DICE; i++) {
				if (states[id].dice[i] *TOTAL_DICE >= states[id].target && states[id].flip[i] == 1) {
					ofs << "WEIRD1: ";
				}
				if (states[id].dice[i] *TOTAL_DICE < states[id].target && states[id].flip[i] == 0) {
					ofs << "WEIRD0: ";
				}
			}
		}
		ofs << states[id] << endl;
	}
	ofs.close();

	int cases = power_int(DICE_FACES_NUMBER, TOTAL_DICE);
	State top;
	top.remaining_round = TOTAL_ROUND+1;
	cout << "target" << "\tsuccess_chance" << endl;
	for(int target=TOTAL_DICE; target < TOTAL_DICE+targetN; target++) {
		double acc_chance = 0;
		top.target = target;
		for(int i=0; i<cases; i++) {
			acc_chance += states[top.traceback(TOTAL_DICE, cases-1, i)].chance;
		}
		acc_chance /= double(cases);
		// cout << target << "\t" << acc_chance << endl;
		cout  << acc_chance << ", " << endl;
	}
	return 0;
}
