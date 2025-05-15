#include "BlockChain.h"
#include <iostream>


int main (int argc, char** argv) {

}









void format(const BlockChain& block_chain , ofstream& target) {
    BlockChainDump(block_chain, target);
}

void compress(BlockChain& block_chain, ofstream& target) {
    BlockChainCompress(block_chain);
    BlockChainDump(block_chain, target);
}