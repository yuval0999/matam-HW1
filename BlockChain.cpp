#include "BlockChain.h"
#include <fstream>
#include <string>
#include <iostream>

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
    newBlock->deletePtr = newBlock;
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
    newBlock->deletePtr = newBlock;
}

//Worst function of mine, check it out please, pay attention that function returns by value

BlockChain BlockChainLoad(ifstream& file) {
    string sender, receiver, timestamp;
    unsigned int value;
    //checked in piazza and we can assume the first line is not empty
    file >> sender >> receiver >> value >> timestamp;
    BlockChain* head = CreateBlock(sender, receiver, value, timestamp);
    BlockChain* iterator = head;
    while (file >> sender >> receiver >> value >> timestamp) {
        iterator -> previousBlock = CreateBlock(sender, receiver, value, timestamp);
        iterator = iterator ->previousBlock;
    }
    return *head;
}

void BlockChainDump(const BlockChain& blockChain, ofstream& file) {
    const BlockChain* currentBlock = &blockChain;
    int index = 1;
    file<<"BlockChain Info:\n";
    while (currentBlock != nullptr) {
        file << index << ".\n"
             << "Sender Name: " << currentBlock->transaction.sender << "\n"
             << "Receiver Name: " << currentBlock->transaction.receiver << "\n"
             << "Transaction Value: " << currentBlock->transaction.value << "\n"
             << "Transaction timestamp: " << currentBlock->timeStamp << "\n";
        currentBlock = currentBlock->previousBlock;
        index++;
    }
}

void BlockChainDumpHashed(const BlockChain &blockChain, ofstream &file) {
    const BlockChain* currentBlock = &blockChain;
    while (currentBlock != nullptr) {
        if (currentBlock->previousBlock != nullptr) {
            file << TransactionHashedMessage(currentBlock->transaction) << "\n" ;
        }
        else {
            file << TransactionHashedMessage(currentBlock->transaction);
        }
        currentBlock = currentBlock->previousBlock;
    }
}

bool BlockChainVerifyFile(const BlockChain &blockChain, std::ifstream &file) {
    const BlockChain* currentBlock = &blockChain;
    string line;
    while (currentBlock != nullptr) {
        if (!(file >> line)) {
            return false;
        }
        if (!TransactionVerifyHashedMessage(currentBlock->transaction, line)) {
            return false;
         }
        currentBlock = currentBlock->previousBlock;
    }
    if (file >> line) {
        return false;
    }
    return true;
}

void BlockChainCompress(BlockChain &blockChain) {
    BlockChain* currentBlock = &blockChain;
    while (currentBlock != nullptr) {
        string currentSender = currentBlock->transaction.sender;
        string currentReceiver = currentBlock->transaction.receiver;
        BlockChain* iterator = currentBlock->previousBlock;

        while (iterator != nullptr &&
                iterator->transaction.receiver == currentReceiver &&
                iterator->transaction.sender == currentSender) {
            currentBlock->transaction.value += iterator->transaction.value;
            currentBlock->previousBlock = iterator->previousBlock;
            iterator = iterator->previousBlock;
        }
        currentBlock = currentBlock->previousBlock;
    }
}

void BlockChainTransform(BlockChain& blockChain, updateFunction function) {
BlockChain* currentBlock = &blockChain;
    do {
        currentBlock->transaction.value = function(currentBlock->transaction.value);
        currentBlock = currentBlock->previousBlock;
    } while (currentBlock->previousBlock != nullptr);
}

BlockChain* CreateBlock(
    const string& sender,
    const string& receiver,
    const unsigned int value,
    const string& timestamp
) {
    BlockChain* block = new BlockChain;
    block ->transaction.sender = sender;
    block ->transaction.receiver = receiver;
    block ->transaction.value = value;
    block ->timeStamp = timestamp;
    block->deletePtr = block;
    block ->previousBlock = nullptr;
    return  block;
}

void DeleteBlockChain(BlockChain* blockChain) {
    while (blockChain != nullptr) {
        BlockChain* currentBlock = blockChain;
        blockChain = blockChain->previousBlock;
        delete currentBlock;
    }
}







