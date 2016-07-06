#include <vector>
#include <iostream>

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
	std::cout << (int)buffer[0] << (char)buffer[1];
}

