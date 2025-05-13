#include "BlockChain.h"
#include <fstream>
#include <string>

using namespace std;

//Changed Implementation of first two functions, recursion is not needed


int BlockChainGetSize(const BlockChain& blockChain) {
    int size = 0;
    const BlockChain* currentBlock = &blockChain;
    while (currentBlock != nullptr) {
        size++;
        currentBlock = currentBlock->previousBlock;
    }
    return size;
}

int BlockChainPersonalBalance(const BlockChain& blockChain, const string& name) {
    int balance = 0;
    const BlockChain* currentBlock = &blockChain;
    while (currentBlock != nullptr) {
        if (currentBlock->transaction.receiver == name) {
            balance += currentBlock->transaction.value;
        }
        else if (currentBlock->transaction.sender == name) {
            balance -= currentBlock->transaction.value;
        }
        currentBlock = currentBlock->previousBlock;
    }
    return balance;
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

//Worst function of mine, check it out please, pay attention that function returns by value

BlockChain BlockChainLoad(ifstream& file) {
    string line;
    string sender, receiver, timestamp;
    unsigned int value;
    BlockChain tail;
    BlockChain* head = nullptr;
    while (file >> sender >> receiver >> value >> timestamp) {
        if (head == nullptr) {
            tail.transaction.sender = sender;
            tail.transaction.receiver = receiver;
            tail.transaction.value = value;
            tail.timeStamp = timestamp;
            tail.previousBlock = nullptr;
            head = &tail;
        }
        else {
            BlockChain* newBlock = new BlockChain;
            newBlock->transaction.sender = sender;
            newBlock->transaction.receiver = receiver;
            newBlock->transaction.value = value;
            newBlock->timeStamp = timestamp;
            newBlock->previousBlock = head;
            head = newBlock;
        }
    }
    BlockChain result = *head;
    while (head != nullptr) {
        BlockChain* ptr = head;
        head = head->previousBlock;
        delete ptr;
    }
    return result;
}

void BlockChainDump(const BlockChain& blockChain, ofstream& file) {
    const BlockChain* currentBlock = &blockChain;
    while (currentBlock->previousBlock != nullptr) {
        file << "Sender name: " << currentBlock->transaction.sender << "\n"
             << "Receiver name: " << currentBlock->transaction.receiver << "\n"
             << "Transaction nalue: " << currentBlock->transaction.value << "\n"
             << "Transaction timestamp: " << currentBlock->timeStamp << "\n";
        currentBlock = currentBlock->previousBlock;
    }
}








