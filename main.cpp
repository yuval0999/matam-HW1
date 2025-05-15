#include "BlockChain.h"
#include <iostream>
#include "Utilities.h"

#define VERIFY_SUCCESS "Verification passed"
#define VERIFY_FAIL "Verification failed"
static void hash(const BlockChain& blockChain, ofstream& target);
static void verify(const BlockChain& blockChain, ifstream& target);
static void compress(BlockChain& blockChain, ofstream& target);
static void format(const BlockChain& blockChain , ofstream& target);

int main (int argc, char** argv) {
    if (argc != 4) {
        std::cout << getErrorMessage();
        return 0;
    }
    ofstream target;
    ifstream source;
    string function;
    source.open(argv[2]);
    function = argv[1];
    BlockChain blockChain = BlockChainLoad(source);
    source.close();
    if (function == "format") {
        target.open(argv[3]);
        format(blockChain, target);
    }
    else if (function == "hash") {
        target.open(argv[3]);
        hash(blockChain, target);
    }
    else if (function == "compress") {
        target.open(argv[3]);
        compress(blockChain, target);
    }
    else if (function == "verify") {
        source.open(argv[3]);
        verify(blockChain, source);
        source.close();
    }
    target.close();
    return 0;
}
static void format(const BlockChain& blockChain , ofstream& target) {
    BlockChainDump(blockChain, target);
}

static void hash(const BlockChain& blockChain, ofstream& target) {
    BlockChainDumpHashed(blockChain, target);
}

static void compress(BlockChain& blockChain, ofstream& target) {
    BlockChainCompress(blockChain);
    BlockChainDump(blockChain, target);
}

static void verify(const BlockChain& blockChain, ifstream& target) {
    if (BlockChainVerifyFile(blockChain, target)) {
        std::cout << VERIFY_SUCCESS;
    }
    else {
        std::cout << VERIFY_FAIL;
    }
}




