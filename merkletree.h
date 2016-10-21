#ifndef MERKLETREE_H
#define MERKLETREE_H

#include <vector>
#include <string>
#include <array>
#include <cryptopp/sha.h>

typedef CryptoPP::SHA256 HashFunction;
typedef std::string ItemType; 
static const size_t HASHSIZE = CryptoPP::SHA256::DIGESTSIZE;

class MerkleNode;

class MerkleTree {
public:
	MerkleTree(std::vector<ItemType> items);
	~MerkleTree();

	/* Retrieve tree's merkle root */
	std::array<byte, HASHSIZE> getMerkleRoot() const;

	/* Retrieve merkle path of element. Returns empty vector if element does not exist. */
	std::vector<std::array<byte, HASHSIZE>> getMerklePath(ItemType item) const;
private:
	MerkleNode* head;
};

class MerkleNode {
public:
	MerkleNode(std::array<byte, HASHSIZE> hash);
	~MerkleNode();

	MerkleNode* left;
	MerkleNode* right;
};

#endif	// MERKLETREE_H

