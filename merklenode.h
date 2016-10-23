#ifndef MERKLENODE_H
#define MERKLENODE_H

#include <cryptopp/sha.h>
#include <array>

class MerkleNode {
public:
	typedef CryptoPP::SHA256 HashFunction;				/* SHA256 will be used as the hash function */
	static const size_t HASHSIZE = CryptoPP::SHA256::DIGESTSIZE;	/* Size of the hashes */
	typedef std::array<byte, HASHSIZE> HashArray;			/* std::array will be used to store hashes */

	MerkleNode(HashArray hash);
	~MerkleNode();

	HashArray getHash() const;

	MerkleNode* left;
	MerkleNode* right;
private:
	HashArray hash;
};

#endif	// MERKLENODE_H

