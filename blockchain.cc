#include <iostream>
#include <sstream>
#include <ctime>
#include <string>
#include <vector>
#include <iomanip>
#include <openssl/sha.h>
using namespace std;

string calc_sha256(const string str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}

class Block {
private:
    string data;
    string previous_hash;
    string timestamp;
    int nounce;

public:
    int index;
    string hash;

    Block(int index, string data, string previous_hash) {
        this->index = index;
        
        std::time_t result = std::time(nullptr);
        this->timestamp = asctime(std::localtime(&result));
        this->timestamp.pop_back();

        this->data = data;
        this->previous_hash = previous_hash;
        tie(this->nounce, this->hash) = compute_hash_with_proof_of_work();
    }

    pair<int, string> compute_hash_with_proof_of_work(string difficulty="00") {
        int nounce = 0;
        while(true) {
            string hash_nounce = calc_hash_with_nounce(nounce);
            if(hash_nounce.find(difficulty) == 0) {
                return make_pair(nounce, hash_nounce);
            }
            else {
                ++nounce;
            }
        }
    }

    string calc_hash_with_nounce(int nounce) {
        string sha = to_string(nounce) +
                    to_string(this->index) +
                    this->timestamp +
                    this->data +
                    this->previous_hash;
        return calc_sha256(sha);
    }

    string sstr() {
        stringstream ss;
        ss<< "Block<\n  index: "<< this->index<<",\n  timestamp: "<<
            this->timestamp<<",\n  data: "<<this->data<<",\n  previous_hash: "<<
            this->previous_hash<<",\n  nounce: "<<this->nounce<<",\n  hash: "<<
            this->hash<<endl;
        return ss.str();
  export LDFLAGS="-L/usr/local/opt/openssl/lib"
  export CPPFLAGS="-I/usr/local/opt/openssl/include"    }

};

Block first(string data="Genesis") {
    return Block(0, data, "0");
}

Block next(Block previous, string data="Transaction Data...") {
    return Block(previous.index+1, data, previous.hash);
}



int main() {
    Block b0 = first();
    Block b1 = next( b0, "Transaction Data..." );
    Block b2 = next( b1, "Transaction Data......" );
    Block b3 = next( b2, "More Transaction Data..." );
    vector<Block> blockchain{b0, b1, b2, b3};
    for(Block v: blockchain) {
        cout<<v.sstr()<<" ";
    }
}
