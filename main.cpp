#include <iostream>
#include <vector>
#include <string>
#include <crypto++/sha.h>	// SHA256
#include <cryptopp/filters.h>	// StringSink, HashFilter
#include <cryptopp/hex.h>	// HexEncoder

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;

using CryptoPP::SHA256;
using CryptoPP::StringSource;
using CryptoPP::HashFilter;
using CryptoPP::HexEncoder;
using CryptoPP::StringSink;

vector<string> getHashes(vector<string> list);

int main() {
	vector<string> list;
	list.push_back("c");
	list.push_back("b");
	list.push_back("a");

	vector<string> hashes = getHashes(list);
	for (auto it = hashes.begin(); it != hashes.end(); ++it) cout << *it << endl;
}

vector<string> getHashes(vector<string> list) {
	// store hashes in vector
	vector<string> hashes;
	for (auto it = list.rbegin(); it != list.rend(); ++it) {
		SHA256 hash;
		string digest;

		// sourceString | sha256 | hex > digest
		StringSource s(*it, true, new HashFilter(hash, new HexEncoder(new StringSink(digest))));
		hashes.push_back(digest);
	}
	return hashes;
}

