#include "BlockChain.h"
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

//I don't know yet if it's supposed to be recursive...

int BlockChainGetSize(const BlockChain& blockChain) {
    if (blockChain.previousBlock == nullptr) {
        return 1;
    }
    return 1 + BlockChainGetSize(*blockChain.previousBlock);
}

int BlockChainPersonalBalance(const BlockChain& blockChain, const string& name) {
    if (blockChain.previousBlock == nullptr) {
        return 0;
    }
    int balance = 0;
    if (blockChain.transaction.receiver == name) {
        balance += blockChain.transaction.value;
    }
    else if (blockChain.transaction.sender == name) {
        balance -= blockChain.transaction.value;
    }
    return balance + BlockChainPersonalBalance(*blockChain.previousBlock, name);
}

void BlockChainAppendTransaction(
        BlockChain& blockChain,
        unsigned int value,
        const string& sender,
        const string& receiver,
        const string& timestamp
) {
    BlockChain* newBlock = new BlockChain;
    newBlock->transaction.value = value;
    newBlock->transaction.sender = sender;
    newBlock->transaction.receiver = receiver;
    newBlock->timeStamp = timestamp;
    newBlock->previousBlock = &blockChain;
}

void BlockChainAppendTransaction(
        BlockChain& blockChain,
        const Transaction& transaction,
        const string& timestamp
) {
    BlockChain* newBlock = new BlockChain;
    newBlock->transaction = transaction;
    newBlock->timeStamp = timestamp;
    newBlock->previousBlock = &blockChain;
}

BlockChain BlockChainLoad(ifstream& file) {
    string line;
    getline(file, line);
    istringstream firstLine(line);
    string sender, receiver, timestamp;
    unsigned int value;
    firstLine >> sender >> receiver >> value >> timestamp;
    BlockChain* head = new BlockChain;
    head->transaction.sender = sender;
    head->transaction.receiver = receiver;
    head->transaction.value = value;
    head->timeStamp = timestamp;
    head->previousBlock = nullptr;
    while (getline(file, line)) {
        istringstream lineStream(line);
        lineStream >> sender >> receiver >> value >> timestamp;
        BlockChainAppendTransaction(*head, value, sender, receiver, timestamp);
    }
    return *head;
}








