#include <iostream>
#include <vector>
#include <string>
#include <crypto++/sha.h>	// SHA256
#include <cryptopp/filters.h>	// sources, sinks, and filters
#include <cryptopp/hex.h>	// HexEncoder

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::array;

using CryptoPP::SHA256;
using CryptoPP::ArraySource;
using CryptoPP::StringSource;
using CryptoPP::HashFilter;
using CryptoPP::HexEncoder;
using CryptoPP::ArraySink;
using CryptoPP::StringSink;

vector<array<byte, SHA256::DIGESTSIZE>> getHashes(vector<string> list);

int main() {
	vector<string> items;
	items.push_back("c");
	items.push_back("b");
	items.push_back("a");

	vector<array<byte, SHA256::DIGESTSIZE>> hashes = getHashes(items);
	for (auto it = hashes.begin(); it != hashes.end(); ++it) {
		string hashHex;
		// byte array to string
		ArraySource s(it->data(), SHA256::DIGESTSIZE, true, new HexEncoder(new StringSink(hashHex)));
		cout << hashHex << endl;
	}
}

vector<array<byte, SHA256::DIGESTSIZE>> getHashes(vector<string> list) {
	// store hashes in vector
	vector<array<byte, SHA256::DIGESTSIZE>> hashes;
	for (auto it = list.rbegin(); it != list.rend(); ++it) {
		SHA256 hash;
		array<byte, SHA256::DIGESTSIZE> digest;

		// echo *it | hash > digest
		StringSource s(*it, true, new HashFilter(hash, new ArraySink(digest.data(), SHA256::DIGESTSIZE)));
		hashes.push_back(digest);
	}
	return hashes;
}

