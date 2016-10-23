#include <iostream>
#include <vector>
#include <string>
#include <algorithm>		// copy
#include <cryptopp/sha.h>	// SHA256
#include <cryptopp/filters.h>	// sources, sinks, filters
#include <cryptopp/hex.h>	// HexEncoder
#include "merkletree.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::array;
using std::copy;
using std::ostream;

using CryptoPP::SHA256;
using CryptoPP::ArraySource;
using CryptoPP::StringSource;
using CryptoPP::HashFilter;
using CryptoPP::HexEncoder;
using CryptoPP::ArraySink;
using CryptoPP::StringSink;

/* Converts a byte array to a hex string */
string hashToHex(array<byte, MerkleNode::HASHSIZE> hash);

ostream& operator<<(ostream& os, vector<MerkleNode::HashArray> arr);

ostream& operator<<(ostream& os, vector<string> v);

int main(int argc, char** argv) {
	if (argc == 1) {
		// is this the correct notation? hmm
		cout << "Usage: " << argv[0] << " item1 [...]" << endl;
	}

	// get items
	vector<MerkleTree::ItemType> items;
	for (int i = 1; i < argc; ++i) {
		items.push_back(string(argv[i]));
	}

	try {
		cout << "Items:\n" << items << endl;
		// create tree
		MerkleTree testTree(items);

		// get merkle root
		cout << "Merkle Root: " << hashToHex(testTree.getMerkleRoot()) << endl;

		// verify existence
		// unable to verify existence.
		// are we living in a simulation? is free-will an illusion? is there an alternate universe without pain, sorrow, and segmentation faults?
		if (testTree.itemExists(argv[1])) cout << "Item: " << argv[1] << " exists." << endl;
		else cout << "Item: " << argv[1] << " does not exist." << endl;

		// get merkle path
		cout << "Getting merkle path for: " << argv[1] << endl;
		cout << testTree.getMerklePath(argv[1]) << endl;
	} catch (const char* msg) {
		cout << msg << endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

string hashToHex(array<byte, MerkleNode::HASHSIZE> hash) {
	string digest;
	ArraySource(hash.data(), MerkleNode::HASHSIZE, true, new HexEncoder(new StringSink(digest)));
	return digest;
}

ostream& operator<<(ostream& os, vector<MerkleNode::HashArray> arr) {
	os << "[\n";
	for (auto it = arr.begin(); it != arr.end(); ++it) {
		os << "\t" << hashToHex(*it);
		if (it + 1 != arr.end()) os << ",\n";
	}
	os << "\n]" << endl;
	return os;
}

ostream& operator<<(ostream& os, vector<string> v) {
	os << "[\n";
	for (auto it = v.begin(); it != v.end(); ++it) {
		os << "\t" << *it;
		if (it + 1 != v.end()) os << ",\n";
	}
	os << "\n]" << endl;
	return os;

}

