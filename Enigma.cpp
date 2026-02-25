//
// Created by beasl on 2/25/2026.
//
#include <algorithm> //for std::shuffle
#include <numeric> //for std::shuffle
#include <array>
#include <random>
#include <iostream>
#include <unordered_map>
using namespace std;
/**
 * The goal of this class is to make an Enigma like machine.
 * Two things need to occur, one scrammbling the letters,
 * and we need a function to decrypt the letter.
 *
 */
class Enigma {
private:
    static constexpr int keyTotal = 94;
    //Representation of letter via integer 0-25
    array<int, keyTotal> letterInandOut;
    array<int, keyTotal> rotor1;
    array<int, keyTotal> rotor2;
    array<int, keyTotal> rotor3;
    array<int, keyTotal> rotor4;
    array<int, keyTotal> rotor5;


    //Want to make this a bit more interesting so I am going to make a list of notches on each rotor
    // Accompanied by if they are accessible or not
    int currentRotorOnePosition;
    int currentRotorTwoPosition;
    int currentRotorThreePosition;
    int currentRotorFourPosition;
    int currentRotorFivePosition;
    unordered_map<int, bool> rotorOneNotch;
    unordered_map<int, bool> rotorTwoNotch;
    unordered_map<int, bool> rotorThreeNotch;
    unordered_map<int, bool> rotorFourNotch;
    unordered_map<int, bool> rotorFiveNotch;
    // original engima has max up to 10 plugins, this will max out at 13
    array<int, keyTotal> reflector;
    // 1. Setup Randomness
    std::random_device rd;
    //Mersenne Twister Engine
    std::mt19937 g;
    array<char, keyTotal> indexToChar;
    unordered_map<char, int> charToIndex;

/**
 * So 0-25 are lowercase characters, 26-51 Upper case,
 */
public:

    Enigma() : g(rd()){
        std::uniform_int_distribution<int> notchDist(0, keyTotal-1);
        //Building rotors
        setUpRotors();
        //Setting Up Reflectors
        setUpReflector();
        //Setting up Notches at random
        setUpRotorNotches();
        currentRotorOnePosition = notchDist(g);
        currentRotorTwoPosition  = notchDist(g);
        currentRotorThreePosition  = notchDist(g);
        currentRotorFourPosition  = notchDist(g);
        currentRotorFivePosition  = notchDist(g);
    }

    void setUpCharMapping() {
        for (char cur = 'a'; cur <='z';cur++) {
            int idx = indexToChar.size();
            indexToChar
        }
    }
    /**
     * This sets up rotors in a nontraditional way.
     * The original engima had 5 rotor to choose from and they chose three,
     * This gives 60 * 2
     *
     */
    void setUpRotors() {
        auto generateRotor = [&](std::array<int, keyTotal>& rotor) {
            std::iota(rotor.begin(), rotor.end(), 0); // Fills 0, 1, 2... 25
            std::shuffle(rotor.begin(), rotor.end(), g);
        };

        // Initialize your three rotors with unique shuffles
        generateRotor(rotor1);
        generateRotor(rotor2);
        generateRotor(rotor3);
        generateRotor(rotor4);
        generateRotor(rotor5);
    }
    void setUpRotorNotches() {
        // 1. Initialize Notches
        // Let's say each rotor has one random notch position
        std::uniform_int_distribution<int> notchDist(0, keyTotal-1);

        // Set all to false first, then pick one to be true
        for(int i = 0; i < 94; ++i) {
            rotorOneNotch[i] = rotorTwoNotch[i] = rotorThreeNotch[i] = false;
        }
        //Gives me 30 notches randomly in my rotorNotches
        for (int i = 0; i < 30; ++i) {
            rotorOneNotch[notchDist(g)] = true;
            rotorTwoNotch[notchDist(g)] = true;
            rotorThreeNotch[notchDist(g)] = true;
        }
    }
    void ratchetAndPaulMechanism() {
        rotorOneNotch
    }
    void setUpReflector() {
        // 1. Initialize the Reflector (Must be pairs!)
        std::vector<int> pairTracker(keyTotal);
        std::iota(pairTracker.begin(), pairTracker.end(), 0);
        std::shuffle(pairTracker.begin(), pairTracker.end(), g);

        for (int i = 0; i < keyTotal; i += 2) {
            int first = pairTracker[i];
            int second = pairTracker[i+1];
            reflector[first] = second;
            reflector[second] = first;
        }
    }
    char toChar(int num) {
        if (num )
    }
};

