#include <vector>
#include <iostream>
#include <string>

//class Packet {
//	unsigned char* 
//};

class Serializable {

public:
	int i;
	std::string s;

	unsigned char* to_bytes() {
		unsigned char* packet = new unsigned char[sizeof(size_t) + 50];
		unsigned char* placeholder = packet;
		size_t len = s.length();
		*reinterpret_cast<size_t*>(packet) = s.length(); packet += sizeof(size_t);
		*reinterpret_cast<int*>(packet) = i; packet += sizeof(int);

		for (auto i = 0; i < s.length(); ++i) *reinterpret_cast<char*>(packet++) = s[i];

		return placeholder;
	}
};

template <typename T, typename ...Types>
std::vector<unsigned char> serialize(T first, Types ... args) {
	std::vector<unsigned char> buffer;
	std::vector<unsigned char> other = serialize(args...);
	buffer.push_back(serialize(first).data()[0]);
	buffer.insert(buffer.end(), other.begin(), other.end());
	return buffer;
}

std::vector<unsigned char> serialize(int i) {
	std::vector<unsigned char> buffer;
	buffer.push_back(*reinterpret_cast<unsigned char*>(&i));
	return buffer;
}

std::vector<unsigned char> serialize(char c) {
	std::vector<unsigned char> buffer;
	buffer.push_back(*reinterpret_cast<unsigned char*>(&c));
	return buffer;
}

int main()
{
	int i;
	char c;

	std::cin >> i >> c;
	std::vector<unsigned char> buffer = serialize(i, c);
	std::cout << (int)buffer[0] << (char)buffer[1] << std::endl << std::endl;


	Serializable s{ 1, "oh shit waddup" };

	unsigned char* serialized = s.to_bytes();
	size_t length = *reinterpret_cast<size_t*>(serialized); serialized += sizeof(size_t);
	int j = *reinterpret_cast<int*>(serialized); serialized += sizeof(int);
	char* c_string = reinterpret_cast<char*>(serialized);
	std::string str;
	str.append(c_string, c_string + length);

	std::cout << j << str;

	delete[] serialized;
}

