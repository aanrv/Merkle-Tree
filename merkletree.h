#ifndef MERKLETREE_H
#define MERKLETREE_H

#include <vector>
#include <string>
#include <array>
#include <cryptopp/sha.h>

class MerkleNode;

class MerkleTree {
public:
	typedef CryptoPP::SHA256 HashFunction;				/* SHA256 will be used as the hash function */
	typedef std::string ItemType;					/* The tree will hold hashes of strings */
	static const size_t HASHSIZE = CryptoPP::SHA256::DIGESTSIZE;	/* Size of the hashes */
	typedef std::array<byte, MerkleTree::HASHSIZE> HashArray;	/* std::array will be used to store hashes */

	MerkleTree(std::vector<ItemType> items);
	~MerkleTree();

	/* Retrieve tree's merkle root */
	HashArray getMerkleRoot() const;

	/* Retrieve merkle path of element. Returns empty vector if element does not exist. */
	std::vector<HashArray> getMerklePath(ItemType item) const;
private:
	/* Given a list of items and a position within a tree, calculates the hash and creates a node. */
	MerkleNode* calculateNode(int distanceFromLeaves, std::vector<MerkleTree::ItemType>& items);

	/* Given two hashes, concats them and calculates their hash */
	HashArray concatHash(HashArray a, HashArray b) const;

	MerkleNode* head;
};

class MerkleNode {
public:
	MerkleNode(MerkleTree::HashArray hash);
	~MerkleNode();

	MerkleNode* left;
	MerkleNode* right;

private:
	MerkleTree::HashArray hash;
};

#endif	// MERKLETREE_H

