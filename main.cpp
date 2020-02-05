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
    string reg1;
    string reg2;
    string reg3;
    bool token;
    
    instructionData(string instr_, string reg1_, string reg2_, string reg3_, bool token_) {
        instr = instr_;
        reg1 = reg1_;
        reg2 = reg2_;
        reg3 = reg3_;
        token = token_;
    }
    instructionData() {
        instr = "";
        reg1 = "";
        reg2 = "";
        reg3 = "";
        token = false;
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
instructionData instructions [16];
int numInstr = 0;
int curInstr = 0;
instructionData INBState;
instructionData AIBState;
instructionData SIBState;
instructionData PRBState;
regAddrData ADBState;
vector<regAddrData> REBState;

string printString = "";
//regAddrData REBState;

void getregisters() {
    ifstream inFile;
        inFile.open("/Users/JustinLawrence/Documents/Embedded_Systems/Project1/Project1/registers.txt");
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
        inFile.open("/Users/JustinLawrence/Documents/Embedded_Systems/Project1/Project1/datamemory.txt");
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
        inFile.open("/Users/JustinLawrence/Documents/Embedded_Systems/Project1/Project1/instructions.txt");
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
            instructionData newInstruction(instr, register1, register2, register3, false);
            instructions[numInstr] = newInstruction;
            numInstr += 1;

        }
        inFile.close();
}
void printState(int step) {
    printString += "\nSTEP: " + to_string(step) + ":\n";
    printString += "INM:";
    for (int i = curInstr; i < numInstr; i++) {
        printString +="<" + instructions[i].instr + "," + instructions[i].reg1 + "," + instructions[i].reg2 + "," + instructions[i].reg3 + ">";
        if (i > numInstr) { printString += ","; }
    }
    printString += "\nINB:";
    if (INBState.token){
        printString += "<"+INBState.instr + ","+INBState.reg1+","+INBState.reg2+","+INBState.reg3+">";
    }
    printString += "\nAIB:";
    if (AIBState.token){
        printString +="<"+AIBState.instr+","+AIBState.reg1+","+AIBState.reg2+","+AIBState.reg3+">";
    }
    printString += "\nSIB:";
    if (SIBState.token){
        printString +="<"+SIBState.instr+","+SIBState.reg1+","+SIBState.reg2+","+SIBState.reg3+">";
    }
    printString += "\nPRB:";
    if (PRBState.token){
        printString +="<"+PRBState.instr+","+PRBState.reg1+","+PRBState.reg2+","+PRBState.reg3+">";
    }
   printString += "\nADB:";
    if (ADBState.token){
        printString +="<"+ADBState.regName+","+ADBState.value+">";
    }
    printString += "\nREB:";
    for (int i = 0; i < REBState.size(); i++) {
        printString +="<"+REBState.at(i).regName+","+REBState.at(i).value+">";
    }
    
    printString += "\nRGF:";
    for (int i = 0; i < 16; i++) {
        if (registers[i] != 0) {
            printString += "<R" + to_string(i) + "," +to_string(registers[i])+">";
        }
        if (i > 16) { printString += ","; }

    }
    printString +="\nDAM:";
      for (int i = 0; i < 16; i++) {
          if (datamemory[i] != 0) {
              printString += "<" + to_string(i) +","+to_string(datamemory[i])+">,";
          }
          if (i > 16) { printString += ","; }
      }
    printString += "\n";
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
    
    getregisters();
    getdateMemory();
    getInstructions();
    int stepCounter = 1;
    printState(0);
    
    while (true) {
        if (ADBState.token){
            //STORE:
            //gets the value stored in the register
            datamemory[stoi(ADBState.value)] = getValueFromRegister(ADBState.regName);
            ADBState.token = false;
        }
        if (SIBState.token) { //if there is a token in SIB...
            //Address Calculation:
            //the format will be <opcode, dest register, source value1, source value2>
        
            int destAddress = stoi(SIBState.reg2) + stoi(SIBState.reg3); //value in Register + offset
            ADBState = regAddrData(SIBState.reg1, to_string(destAddress), true);
            SIBState.token = false;
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
        }
        if (PRBState.token){
            int val1 = stoi(PRBState.reg2);
            int val2 = stoi(PRBState.reg3);
            REBState.push_back(regAddrData(PRBState.reg1, to_string(val1 * val2), true));
            PRBState.token = false;
        }
        if (AIBState.token){ //sees if there is a token for AIB
            //Add - Subtract Unit (ASU)
            if (AIBState.instr == "ADD" || AIBState.instr == "SUB") {
                int val1 = stoi(AIBState.reg2);
                int val2 = stoi(AIBState.reg3);
                if (AIBState.instr == "ADD") {
                    REBState.push_back(regAddrData(AIBState.reg1, to_string(val1 + val2), true));
                }
                else {
                    REBState.push_back(regAddrData(AIBState.reg1, to_string(val1 - val2), true));
                }
                AIBState.token = false;
            }
            // Multiply Unit – Stage 1 (MLU1)
            else {
                PRBState = instructionData(AIBState.instr, AIBState.reg1, AIBState.reg2, AIBState.reg3, true);
                AIBState.token = false; //takes away AIB token
            }
        }
        if (INBState.token) { //sees if the token for INB state is true
            if (INBState.instr == "ST") {
                //ISSUE2:
                SIBState = instructionData(INBState.instr, INBState.reg1, INBState.reg2, INBState.reg3, true);
            }
            else {
                //ISSUE1:
                AIBState = instructionData(INBState.instr, INBState.reg1, INBState.reg2, INBState.reg3, true);
            }
            INBState.token = false; //remove item from the INB State
        }
        if (curInstr < numInstr) { //sees if there is any instructions left
            //DECODE:
            instructionData newInstr = instructions[curInstr];
            
            string reg3Val = newInstr.reg3;
            
            //get value in reg 2 (source value1)
            string reg2Val = to_string(getValueFromRegister(newInstr.reg2));
            
            if (newInstr.instr != "ST") { //if its not a store(an instruction that takes an offset, get value in source val2)
                reg3Val = to_string(getValueFromRegister(newInstr.reg3));
            }
            
            INBState = instructionData(newInstr.instr, newInstr.reg1, reg2Val, reg3Val, true);
            curInstr += 1;
        }
        printState(stepCounter);
        stepCounter += 1;
        if (stepCounter > 16) {
            ofstream out("output.txt");
            out << printString;
            out.close();
            break;
        }
    }
    
    return 0;
}
