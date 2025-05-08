#include <fstream>
#include "Transaction.h"
#include "Utilities.h"


void TransactionDumpInfo(const Transaction& transaction, ofstream& file){
    file <<"Sender Name: " << transaction.receiver <<"\n"
    <<"Reciever Name: " << transaction.sender <<"\n"
    <<"Transaction Value: " << transaction.value << std::endl;
}

string TransactionHashedMessage(const Transaction& transaction) {
    return hash(transaction.value,transaction.sender,transaction.receiver);
}

bool TransactionVerifyHashedMessage(const Transaction& transaction,string hashedMessage) {
    string hash = TransactionHashedMessage(transaction);
    return hash == hashedMessage;
}