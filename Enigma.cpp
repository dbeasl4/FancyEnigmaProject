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

    array<int, keyTotal> rotor1;
    array<int, keyTotal> rotor2;
    array<int, keyTotal> rotor3;
    array<int, keyTotal> rotor4;
    array<int, keyTotal> rotor5;
    array<int, keyTotal> rotor1Inverse;
    array<int, keyTotal> rotor2Inverse;
    array<int, keyTotal> rotor3Inverse;
    array<int, keyTotal> rotor4Inverse;
    array<int, keyTotal> rotor5Inverse;


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
    vector<array<int, keyTotal>> rotors;
    array<int, keyTotal> reflector;
    // 1. Setup Randomness
    std::random_device rd;
    //Mersenne Twister Engine
    std::mt19937 g;
    array<char, keyTotal> indexToChar;
    unordered_map<char, int> charToIndex;
    int currentSalt = 0;




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
        rotors = {rotor1, rotor2, rotor3, rotor4, rotor5};
        shuffleRotors();
    }
    void shuffleRotors() {
        //How many rotors to swap
        uniform_int_distribution<int> dist(1,5);
        int rotorSwap = dist(g);
        vector<int> rotorPositions = {1,2,3,4,5};
        for (int i = 0; i < rotorSwap; i++) {
            uniform_int_distribution<int> dist(0,rotorPositions.size()-1);
            int swapWithPosition = dist(g);
            if (swapWithPosition == 1) {
                swap(rotorPositions[i], rotorPositions[swapWithPosition]);
            }
        }

    }
    void setUpCharMapping() {
        string keyboardChars = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                           "0123456789!@#$%^&*()-_=+[{]};:'\",<.>/?\\|";
        for (int i = 0; i < keyboardChars.length();++i) {
            char c = keyboardChars[i];
            indexToChar[i] = c;
            charToIndex[c] = i;
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

        for (int i = 0; i < keyTotal; i++) {
            rotor1Inverse[rotor1[i]] = i;
            rotor2Inverse[rotor2[i]] = i;
            rotor3Inverse[rotor3[i]] = i;
            rotor4Inverse[rotor4[i]] = i;
            rotor5Inverse[rotor5[i]] = i;
        }
    }
    void setUpRotorNotches() {
        // 1. Initialize Notches
        // Let's say each rotor has one random notch position
        std::uniform_int_distribution<int> notchDist(0, keyTotal-1);

        // Set all to false first, then pick one to be true
        for(int i = 0; i < 94; ++i) {
            rotorOneNotch[i] = rotorTwoNotch[i] = rotorThreeNotch[i] = rotorFourNotch[i] = rotorFiveNotch[i] = false;
        }
        //Gives me 30 notches randomly in my rotorNotches
        for (int i = 0; i < 30; ++i) {
            rotorOneNotch[notchDist(g)] = true;
            rotorTwoNotch[notchDist(g)] = true;
            rotorThreeNotch[notchDist(g)] = true;
            rotorFourNotch[notchDist(g)] = true;
            rotorFiveNotch[notchDist(g)] = true;
        }
    }
    void ratchetAndPaulMechanism() {
        bool stepTwo = rotorOneNotch[currentRotorOnePosition];
        bool stepThree = (stepTwo && rotorThreeNotch[currentRotorThreePosition]);
        bool stepFour = (stepThree && rotorFourNotch[currentRotorFourPosition]);
        bool stepFive = (stepFour && rotorFiveNotch[currentRotorFivePosition]);

        //Begin rotor shifts
        currentRotorOnePosition = (currentRotorOnePosition+1)%keyTotal;

        if (stepTwo) {
            currentRotorTwoPosition = (currentRotorTwoPosition+1)%keyTotal;
        }
        if (stepThree) {
            currentRotorThreePosition = (currentRotorThreePosition+1)%keyTotal;
        }
        if (stepFour) {
            currentRotorFourPosition = (currentRotorFourPosition+1)%keyTotal;
        }
        if (stepFive) {
            currentRotorFivePosition = (currentRotorFivePosition+1)%keyTotal;
        }
    }
    char encryptWithChaos(char input, long long durationMs) {
        //Basically saying not in our bounds of characters and give it back
        if (charToIndex.find(input) == charToIndex.end()) {
            return input;
        }
        //If within our bounds
        shuffleRotors();
        //1. Standard position
        ratchetAndPaulMechanism();

        //2. Timing spin ("Physical Chaos"), grabbing timing for each key pressed
        for (int i = 0; i < (durationMs % keyTotal); i++) {
            ratchetAndPaulMechanism();
        }
        //3. Now adding salt to rotate ("Digital Chaos")
        for (int i = 0 ; i < currentSalt; i++) {
            ratchetAndPaulMechanism();
        }

        // What we are sending out
        int signal = charToIndex[input];

        //Passing through rotors, essentially establishing wire conenctions here.
        signal = (rotors[0][(signal + currentRotorOnePosition) % keyTotal] - currentRotorOnePosition + keyTotal) % keyTotal;
        signal = (rotors[1][(signal + currentRotorTwoPosition) % keyTotal] - currentRotorTwoPosition + keyTotal) % keyTotal;
        signal = (rotors[2][(signal + currentRotorThreePosition) % keyTotal] - currentRotorThreePosition + keyTotal) % keyTotal;
        signal = (rotors[3][(signal + currentRotorFourPosition) % keyTotal] - currentRotorFourPosition + keyTotal) % keyTotal;
        signal = (rotors[4][(signal + currentRotorFivePosition) % keyTotal] - currentRotorFivePosition + keyTotal) % keyTotal;

        signal = reflector[signal];

        //Pass back through after reflector
        signal = (rotor5Inverse[(signal + currentRotorFivePosition) % keyTotal] - currentRotorFivePosition + keyTotal) % keyTotal;
        signal = (rotor4Inverse[(signal + currentRotorFourPosition) % keyTotal] - currentRotorFourPosition + keyTotal) % keyTotal;
        signal = (rotor3Inverse[(signal + currentRotorThreePosition) % keyTotal] - currentRotorThreePosition + keyTotal) % keyTotal;
        signal = (rotor2Inverse[(signal + currentRotorTwoPosition) % keyTotal] - currentRotorTwoPosition + keyTotal) % keyTotal;
        signal = (rotor1Inverse[(signal + currentRotorOnePosition) % keyTotal] - currentRotorOnePosition + keyTotal) % keyTotal;


        //Creating Polyalphabetic Cipiher with Feedback
        currentSalt = signal;

        return indexToChar[signal];

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
};

