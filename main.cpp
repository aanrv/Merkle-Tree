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

using CryptoPP::SHA256;
using CryptoPP::StringSource;
using CryptoPP::HashFilter;
using CryptoPP::HexEncoder;
using CryptoPP::StringSink;

int main() {
	SHA256 hash;
	string msg = "Hello";
	string digest;

	StringSource s(msg, true, new HashFilter(hash, new HexEncoder(new StringSink(digest))));
	cout << digest << endl;
}

