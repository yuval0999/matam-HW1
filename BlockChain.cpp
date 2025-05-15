#include "BlockChain.h"
#include <fstream>
#include <string>

using namespace std;

//Changed Implementation of first two functions, recursion is not needed

BlockChain* createBlock(
    const string& sender,
    const string& receiver,
    const unsigned int value,
    const string& timestamp
);
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
    string sender, receiver, timestamp;
    unsigned int value;
    //checked in piazza and we can assume the first line is not empty
    file >> sender >> receiver >> value >> timestamp;
    BlockChain* head = createBlock(sender, receiver, value, timestamp);
    BlockChain* iterator = head;
    while (file >> sender >> receiver >> value >> timestamp) {
        iterator -> previousBlock = createBlock(sender, receiver, value, timestamp);
        iterator = iterator ->previousBlock;
    }
    return *head;
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
        if (!getline(file, line)) {
            return false;
        }
        if (!TransactionVerifyHashedMessage(currentBlock->transaction, line)) {
            return false;
        }
        currentBlock = currentBlock->previousBlock;
    }
    if (getline(file, line)) {
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
        BlockChain* lastBlock = currentBlock;
        while (iterator != nullptr) {
            if (iterator->transaction.sender == currentSender &&
                iterator->transaction.receiver == currentReceiver) {
                currentBlock->transaction.value += iterator->transaction.value;
                currentBlock->timeStamp = iterator->timeStamp;
                lastBlock->previousBlock = iterator->previousBlock;
                delete iterator;
                iterator = lastBlock->previousBlock;
            }
            else {
                lastBlock = iterator;
                iterator = iterator->previousBlock;
            }
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

BlockChain* createBlock(
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
    block ->previousBlock = nullptr;
    return  block;
}


void DeleteBlockCHain(BlockChain& blockChain) {
    BlockChain* currentBlock = &blockChain;
    while (currentBlock != nullptr) {
        BlockChain* nextBlock = currentBlock->previousBlock;
        delete[] currentBlock;
        currentBlock = nextBlock;
    }
}







