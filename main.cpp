#include <iostream>
#include <cryptopp/hex.h>
#include <vector>
#include <string>
#include "merkletree.h"

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::string;
using std::vector;
using std::array;
using std::copy;
using std::ostream;

/* Calculates hash of item */
MerkleNode::HashArray itemHash(MerkleTree::ItemType item);

/* Converts a byte array to a hex string */
string hashToHex(const MerkleNode::HashArray& hash);

/* Print tree */
ostream& operator<<(ostream& os, const MerkleTree& tree);

/* Print list of hashes */
ostream& operator<<(ostream& os, const vector<MerkleNode::HashArray>& arr);

/* Print vector of strings */
ostream& operator<<(ostream& os, const vector<string>& v);

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
		cout << "Items:\n" << items << "\n" << endl;
		
		// create tree
		MerkleTree testTree(items);
		cout << "Tree:\n" << testTree << "\n" << endl;

		// get merkle root
		cout << "Merkle Root: " << hashToHex(testTree.getMerkleRoot()) << endl;

		// verify existence of an item
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

MerkleNode::HashArray itemHash(MerkleTree::ItemType item) {
	MerkleNode::HashFunction func;
	MerkleNode::HashArray digest;
	CryptoPP::StringSource s(item, true, new CryptoPP::HashFilter(func, new CryptoPP::ArraySink(digest.data(), MerkleNode::HASHSIZE)));
	return digest;
}

string hashToHex(const MerkleNode::HashArray& hash) {
	string digest;
	CryptoPP::ArraySource(hash.data(), MerkleNode::HASHSIZE, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(digest)));
	return digest;
}

ostream& operator<<(ostream& os, const MerkleTree& tree) {
	os << "{\n";
	size_t level = 0;
	size_t nodesReturned;
	do {
		vector<MerkleNode::HashArray> hashes = tree.getHashesAtLevel(level);
		if ((nodesReturned = hashes.size()) > 0) cout << hashes << endl;
		++level;
	} while (nodesReturned > 0);
	os << "}" << endl;

	return os;
}

ostream& operator<<(ostream& os, const vector<MerkleNode::HashArray>& arr) {
	os << "[\n";
	for (auto it = arr.begin(); it != arr.end(); ++it) {
		os << "\t" << hashToHex(*it);
		if (it + 1 != arr.end()) os << ",\n";
	}
	os << "\n]" << flush;
	return os;
}

ostream& operator<<(ostream& os, const vector<string>& v) {
	os << "[\n";
	for (auto it = v.begin(); it != v.end(); ++it) {
		os << "\t" << *it << ": " << hashToHex(itemHash(*it));
		if (it + 1 != v.end()) os << ",\n";
	}
	os << "\n]" << flush;
	return os;

}

