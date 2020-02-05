//
//  main.cpp
//  project1
//
//  Created by Justin Lawrence on 2/4/20.
//  Copyright © 2020 Justin Lawrence. All rights reserved.
//

#include <iostream>
#include<fstream>
#include<vector>
using namespace std;

class instructionData{
public:
    string instr;
    string destReg;
    string reg2;
    string reg3;
    
    instructionData(string instr_, string destReg_, string reg2_, string reg3_) {
        instr = instr_;
        destReg = destReg_;
        reg2 = reg2_;
        reg3 = reg3_;
    }
    instructionData() {
        instr = "";
        destReg = "";
        reg2 = "";
        reg3 = "";
    }
};
class regAddrData{
public:
    string regName;
    string value;
    bool token;
    
    regAddrData(string regName_, string value_, bool token_) {
        regName = regName_;
        value = value_;
        token = token_;
    }
    regAddrData() {
        regName = "";
        value = "";
        token = false;
    }
};

int datamemory [16];
int registers [16];
vector <instructionData> instructions;

vector <instructionData> INBState;
vector <instructionData> AIBState;
vector <instructionData> SIBState;
vector <instructionData> PRBState;
vector<regAddrData> ADBState;
vector<regAddrData> REBState;

string printString = "";
//regAddrData REBState;

void getregisters() {
    ifstream inFile;
        //inFile.open("/Users/JustinLawrence/Documents/Embedded_Systems/Project1/Project1/registers.txt");
        inFile.open("registers.txt");

        if (!inFile) {
            cerr << "Unable to open file registers.txt";
            exit(1);   // call system to stop
        }
        string line;
        while (getline(inFile, line))
        {
            string registerVal;
            string value;
            bool addingRegVal = true;
            for (int i = 2; i < line.size()-1; i++) {
                if (addingRegVal) {
                    if (line.at(i) == ',') {
                        addingRegVal = false;
                    }
                    else {
                        registerVal += line.at(i);
                    }
                }
                else {
                    value += line.at(i);
                }
            }
            registers[stoi(registerVal)] = stoi(value);

        }
        inFile.close();
}

void getdateMemory() {
    ifstream inFile;
        //inFile.open("/Users/JustinLawrence/Documents/Embedded_Systems/Project1/Project1/datamemory.txt");
        inFile.open("datamemory.txt");

        if (!inFile) {
            cerr << "Unable to open file registers.txt";
            exit(1);   // call system to stop
        }
        string line;

        while (getline(inFile, line))
        {
            string registerVal;
            string value;
            bool addingRegVal = true;
            for (int i = 1; i < line.size()-1; i++) {
                if (addingRegVal) {
                    if (line.at(i) == ',') {
                        addingRegVal = false;
                    }
                    else {
                        registerVal += line.at(i);
                    }
                }
                else {
                    value += line.at(i);
                }
            }
            datamemory[stoi(registerVal)] = stoi(value);

        }
        inFile.close();
}
void getInstructions() {
    ifstream inFile;
        //inFile.open("/Users/JustinLawrence/Documents/Embedded_Systems/Project1/Project1/instructions.txt");
        inFile.open("instructions.txt");

        if (!inFile) {
            cerr << "Unable to open file registers.txt";
            exit(1);   // call system to stop
        }
        string line;

        while (getline(inFile, line))
        {
            string instr;
            string register1;
            string register2;
            string register3;
            
            bool addingInstr = true;
            bool addingReg1 = false;
            bool addingReg2 = false;
            for (int i = 1; i < line.size()-1; i++) {
                if (addingInstr) {
                    if (line.at(i) == ',') {
                        addingInstr = false;
                        addingReg1 = true;
                        continue;
                    }
                    else {
                        instr += line.at(i);
                    }
                }
                else if (addingReg1){
                    if (line.at(i) == ',') {
                        addingReg1 = false;
                        addingReg2 = true;
                        continue;
                    }
                    else {
                        register1 += line.at(i);
                    }
                }
                else if (addingReg2){
                    if (line.at(i) == ',') {
                         addingReg2 = false;
                         continue;
                     }
                     else {
                         register2 += line.at(i);
                     }
                }
                else {
                    register3 += line.at(i);
                }
            }
            instructionData newInstruction(instr, register1, register2, register3);
            instructions.push_back(newInstruction);

        }
        inFile.close();
}
void printState(int step) {
    printString += "STEP " + to_string(step) + ":\n";
    printString += "INM:";
    for (int i = 0; i < instructions.size(); i++) {
        printString +="<" + instructions.at(i).instr + "," + instructions.at(i).destReg + "," + instructions.at(i).reg2 + "," + instructions.at(i).reg3 + ">,";
    }
    if (!instructions.empty()){ printString.pop_back(); }

    printString += "\nINB:";
    for (int i = 0; i < INBState.size(); i++) {
        printString +="<" + INBState.at(i).instr + "," + INBState.at(i).destReg + "," + INBState.at(i).reg2 + "," + INBState.at(i).reg3 + ">,";
    }
    if (!INBState.empty()){ printString.pop_back(); }
    
    printString += "\nAIB:";
    for (int i = 0; i < AIBState.size(); i++) {
        printString +="<" + AIBState.at(i).instr + "," + AIBState.at(i).destReg + "," + AIBState.at(i).reg2 + "," + AIBState.at(i).reg3 + ">,";
    }
    if (!AIBState.empty()){ printString.pop_back(); }
    
    printString += "\nSIB:";
    for (int i = 0; i < SIBState.size(); i++) {
        printString +="<" + SIBState.at(i).instr + "," + SIBState.at(i).destReg + "," + SIBState.at(i).reg2 + "," + SIBState.at(i).reg3 + ">,";
    }
    if (!SIBState.empty()){ printString.pop_back(); }
    
    printString += "\nPRB:";
    for (int i = 0; i < PRBState.size(); i++) {
        printString +="<" + PRBState.at(i).instr + "," + PRBState.at(i).destReg + "," + PRBState.at(i).reg2 + "," + PRBState.at(i).reg3 + ">,";
    }
    if (!PRBState.empty()){ printString.pop_back(); }
        
   printString += "\nADB:";
    for (int i = 0; i < ADBState.size(); i++) {
        printString +="<"+ADBState.at(i).regName+","+ADBState.at(i).value+">,";
    }
    if (!ADBState.empty()){ printString.pop_back(); } //remove last ','
        
    printString += "\nREB:";
    for (int i = 0; i < REBState.size(); i++) {
        printString +="<"+REBState.at(i).regName+","+REBState.at(i).value+">,";
    }
    if (!REBState.empty()){ printString.pop_back(); } //remove last ','

    printString += "\nRGF:";
    for (int i = 0; i < 16; i++) {
        if (registers[i] != -1) {
            printString += "<R" + to_string(i) + "," +to_string(registers[i])+">,";
        }
    }
    printString.pop_back();//remove last ','
    
    printString +="\nDAM:";
      for (int i = 0; i < 16; i++) {
          if (datamemory[i] != -1) {
              printString += "<" + to_string(i) +","+to_string(datamemory[i])+">,";
          }
      }
    printString.pop_back(); //remove last ','
    printString += "\n\n";
}
int getValueFromRegister(string registerNum) {
    string regNum = "";
    for (int i = 1; i < registerNum.size(); i++) {
        regNum += registerNum[i];
    }
    int registerInt = registers[stoi(regNum)];
    return registerInt;
}

int main(int argc, const char * argv[]) {
    //initilize all of the empty register and data memory to be -1
    for (int i = 0; i<16; i++){
        datamemory[i] = -1;
        registers[i] = -1;
    }
    getregisters();
    getdateMemory();
    getInstructions();
    
    int stepCounter = 1;
    printState(0);
    
    while (true) {
        bool stillGoing = false; //keeps track if something is stil chaning
        if (!ADBState.empty()){
            //STORE:
            //gets the value stored in the register
            regAddrData curItem = ADBState.front();
            datamemory[stoi(curItem.value)] = getValueFromRegister(curItem.regName);
            ADBState.erase(ADBState.begin());
            stillGoing = true;
        }
        if (!SIBState.empty()) { //if there is a token in SIB...
            //Address Calculation:
            //the format will be <opcode, dest register, source value1, source value2>
            instructionData curItem = SIBState.front();
            int destAddress = stoi(curItem.reg2) + stoi(curItem.reg3); //value in Register + offset
            ADBState.push_back(regAddrData(curItem.destReg, to_string(destAddress), true));
            //ADBState = regAddrData(curItem.destReg, to_string(destAddress), true);
            SIBState.erase(SIBState.begin()); //remove front item
            stillGoing = true;
        }
        if (!REBState.empty()){ //there is a token!
            //WRITE to register file:
            regAddrData curItem = REBState.front();
            string regNum = "";
            for (int i = 1; i < curItem.regName.size(); i++) {
                regNum += curItem.regName[i];
            }
            registers[stoi(regNum)] = stoi(curItem.value);
            REBState.erase(REBState.begin()); //removes token
            stillGoing = true;
        }
        if (!PRBState.empty()){
            //MULT2
            instructionData curItem = PRBState.front();

            int val1 = stoi(curItem.reg2);
            int val2 = stoi(curItem.reg3);
            REBState.push_back(regAddrData(curItem.destReg, to_string(val1 * val2), true));
            PRBState.erase(PRBState.begin()); //removes token
            stillGoing = true;
        }
        if (!AIBState.empty()){ //sees if there is a token for AIB
            instructionData curItem = AIBState.front();

            if (curItem.instr == "ADD" || curItem.instr == "SUB") {
                //Add - Subtract Unit (ASU)
                int val1 = stoi(curItem.reg2);
                int val2 = stoi(curItem.reg3);
                if (curItem.instr == "ADD") { //ADD
                    REBState.push_back(regAddrData(curItem.destReg, to_string(val1 + val2), true));
                }
                else { //SUB
                    REBState.push_back(regAddrData(curItem.destReg, to_string(val1 - val2), true));
                }
            }
            else {
                // Multiply Unit – Stage 1 (MLU1)
                PRBState.push_back(instructionData(curItem.instr, curItem.destReg, curItem.reg2, curItem.reg3));
            }
            AIBState.erase(AIBState.begin()); //removes token
            stillGoing = true;
        }
        if (!INBState.empty()) { //sees if the token for INB state is true
            instructionData curItem = INBState.front();
            if (curItem.instr == "ST") {
                //ISSUE2:
                SIBState.push_back(instructionData(curItem.instr, curItem.destReg, curItem.reg2, curItem.reg3));
            }
            else {
                //ISSUE1:
                AIBState.push_back(instructionData(curItem.instr, curItem.destReg, curItem.reg2, curItem.reg3));
            }
            INBState.erase(INBState.begin()); //removes token
            stillGoing = true;
        }
        if (!instructions.empty()) { //sees if there is any instructions left
            //DECODE:
            instructionData newInstr = instructions.front();
            
            string reg3Val = newInstr.reg3;
            
            //get value in reg 2 (source value1)
            string reg2Val = to_string(getValueFromRegister(newInstr.reg2));
            
            if (newInstr.instr != "ST") { //if its not a store(an instruction that takes an offset, get value in source val2)
                reg3Val = to_string(getValueFromRegister(newInstr.reg3));
            }
            
            INBState.push_back(instructionData(newInstr.instr, newInstr.destReg, reg2Val, reg3Val));
            
            instructions.erase(instructions.begin()); //remove front item
            stillGoing = true;
        }
        if (!stillGoing) {
            printString.pop_back();
            printString.pop_back(); //removes new line at the end
            
            ofstream out("simulation.txt");
            out << printString;
            out.close();
            //cout << printString;
            break;
        }
        printState(stepCounter);
        stepCounter += 1;
    }
    
    return 0;
}
