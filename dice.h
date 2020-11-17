#pragma once

#include <iostream>
#include <vector>
using namespace std;

#define DICE_FACES_NUMBER 6
#define TOTAL_DICE 3
#define TOTAL_ROUND 2
const int targetN = TOTAL_DICE*(DICE_FACES_NUMBER-1) + 1;

class State {
public:
    int remaining_round;
    int target;
    vector<int> dice = vector<int>(TOTAL_DICE);
    vector<int> flip = vector<int>(TOTAL_DICE);
    double chance;
    friend ostream &operator<<( ostream &output, const State &s ) { 
        output << s.remaining_round << " " << s.target;
        for(int d : s.dice)
            output << " " << d;
        for(int d : s.flip)
            output << " " << d;
        output << " " << s.chance;
        return output;            
     }
     int get_id() {
         int id = 0;
         int temp = 1;
         for(int i=0; i<TOTAL_DICE; i++) {
             id += (dice[i] - 1) * temp;
             temp *= DICE_FACES_NUMBER;
         }
         id += (target-TOTAL_DICE) * temp;
         temp *= targetN;
         id += remaining_round * temp;
         return id;
     }
     int traceback(int choice_weight, int choice, int case_i) {
        // return the id of the corresponding state
        State tempstate;
        tempstate.target = target;
        tempstate.remaining_round = remaining_round-1;
        vector<int> pos = vector<int>(choice_weight);
        int j = 0;
        for(int i=0; i<TOTAL_DICE; i++) {
            tempstate.dice[i] = dice[i];
            if(choice % 2) {
                pos[j++] = i;
            }
            choice /= 2;
        }
        for(int i=0; i<choice_weight; i++) {
            tempstate.dice[pos[i]] = case_i % DICE_FACES_NUMBER + 1;
            case_i /= DICE_FACES_NUMBER;
        }
        return tempstate.get_id();
     }
     void setFlip(int choice) {
        for(int i=0; i<TOTAL_DICE; i++) {
            flip[i] = choice % 2;
            choice /= 2;
        }
     }
     int successQ() {
         int sum=0;
         for(int d : dice)
            sum += d;
         return sum >= target;
     }
};

State decode(int id) {
    State state;
    for(int i=0; i<TOTAL_DICE; i++) {
        state.dice[i] = id % DICE_FACES_NUMBER + 1;
        id /= DICE_FACES_NUMBER;
    }
    state.target = id % targetN + TOTAL_DICE;
    id /= targetN;
    state.remaining_round = id;
    return state;
}

