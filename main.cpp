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

typedef CryptoPP::SHA256 HashFunction;
typedef std::string ItemType;
const size_t HASHSIZE = CryptoPP::SHA256::DIGESTSIZE;

/* Given list of strings, returns a corresponding list of hashes */
vector<array<byte, HASHSIZE>> getHashes(vector<ItemType> list);

/* Calculates the merkle root of a list of hashes */
array<byte, HASHSIZE> calculateMerkleRoot(vector<array<byte, HASHSIZE>> list);

/* Converts a byte array to a hex string */
string hashToHex(array<byte, HASHSIZE> hash);

ostream& operator<<(ostream& os, vector<array<byte, HASHSIZE>> arr);

ostream& operator<<(ostream& os, vector<string> v);

int main(int argc, char** argv) {
	if (argc <= 1) {
		// is this the correct notation? hmm
		cout << "Usage: " << argv[0] << " item1 [...]" << endl;
		return EXIT_FAILURE;
	}

	// get items
	vector<ItemType> items;
	for (int i = 1; i < argc; ++i) {
		items.push_back(string(argv[i]));
	}

	// create tree
	MerkleTree testTree(items);

	// get merkle root
	cout << hashToHex(testTree.getMerkleRoot()) << endl;

	// get merkle path
	cout << "Getting merkle path for: " << argv[1] << endl;
	cout << testTree.getMerklePath(argv[1]) << endl;

	cout << "\n\n" << endl;
	// get hashes
	vector<array<byte, HASHSIZE>> itemHashes = getHashes(items);
	cout << hashToHex(calculateMerkleRoot(itemHashes)) << endl;
}

vector<array<byte, HASHSIZE>> getHashes(vector<ItemType> list) {
	// store hashes of items in a vector
	vector<array<byte, HASHSIZE>> hashes;
	for (auto it = list.begin(); it != list.end(); ++it) {
		HashFunction hash;
		array<byte, HASHSIZE> digest;

		// echo item | hash > digest
		StringSource s(*it, true, new HashFilter(hash, new ArraySink(digest.data(), HASHSIZE)));
		hashes.push_back(digest);
	}
	return hashes;
}

array<byte, HASHSIZE> calculateMerkleRoot(vector<array<byte, HASHSIZE>> hashes) {
	cout << "Calculating merkle root:" << endl;
	// if odd number of items, duplicate last one
	cout << hashes << endl;

	// keep hashing in pairs until one item left
	while (hashes.size() != 1) {
		if (hashes.size() % 2 == 1) hashes.push_back(hashes.back());
		vector<array<byte, HASHSIZE>> tmp_hashes;

		for (auto it = hashes.begin(); it != hashes.end(); it += 2) {
			auto first = it;
			auto second = it + 1;

			// concat items and store in source
			const size_t sourceLen = HASHSIZE * 2;
			array<byte, sourceLen> source;
			copy(first->begin(), first->end(), source.begin());
			copy(second->begin(), second->end(), source.begin() + HASHSIZE);

			// hash source to digest
			array<byte, HASHSIZE> digest;
			HashFunction hash;
			ArraySource s(source.data(), sourceLen, true, new HashFilter(hash, new ArraySink(digest.data(), HASHSIZE)));

			// save hash to temporary hash list
			tmp_hashes.push_back(digest);
		}
		hashes = tmp_hashes;
		cout << hashes << endl;
	}

	return hashes.back();
}

string hashToHex(array<byte, HASHSIZE> hash) {
	string digest;
	ArraySource(hash.data(), HASHSIZE, true, new HexEncoder(new StringSink(digest)));
	return digest;
}

ostream& operator<<(ostream& os, vector<array<byte, HASHSIZE>> arr) {
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
