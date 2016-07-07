#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

class Packet {

#define def_append(X) void append(X val) { \
	if (buffer_size <= used_capacity + sizeof(val)) expand_and_double(sizeof(val)); \
	*reinterpret_cast<X*>(buffer + used_capacity) = val; \
	used_capacity += sizeof(X); \
}

#define def_read(X) void read(X& val) {  \
	val = *reinterpret_cast<X*>(cursor); \
	cursor += sizeof(X); \
}


	unsigned char* buffer;
	unsigned char* cursor;
	size_t buffer_size = 0;
	size_t used_capacity = 0;

	size_t message_length = 0;

	void expand_and_double(size_t additional_capacity = 0) {
		unsigned char* new_buffer = new unsigned char[(used_capacity + additional_capacity) * 2];
		std::copy(buffer, buffer + used_capacity, new_buffer);
		delete[] buffer;
		buffer = new_buffer;
	}

public:
	Packet() : buffer(new unsigned char[1]), cursor(buffer) {}
	Packet(unsigned char* buf, size_t size) { expand_and_double(size); std::copy(buf, buf + size, buffer); }
	~Packet() { delete[] buffer; }

	Packet& operator += (Packet rhs) {
		if (buffer_size <= used_capacity + rhs.used_capacity) 
			expand_and_double(rhs.used_capacity);

		std::copy(rhs.buffer, rhs.buffer + rhs.used_capacity, buffer + used_capacity);
		used_capacity += rhs.used_capacity;

		return *this;
	}

	def_append(int)
	def_append(char)
	def_append(double)
	def_append(long)
	void append(std::string s) {
		unsigned char* insertion_point = buffer + used_capacity;
		*reinterpret_cast<size_t*>(insertion_point) = s.length();
		std::copy(s.begin(), s.end(), insertion_point + sizeof(size_t));
		used_capacity += s.length();
	}

	def_read(int)
	def_read(char)
	def_read(double)
	def_read(long)
	void read(std::string& s) {
		size_t length = *reinterpret_cast<size_t*>(cursor);
		cursor += sizeof(size_t);

		s = std::string(const_cast<const char*>(reinterpret_cast<char*>(cursor)), length);
		cursor += length;
	}
	

};

class Serializer {
	Packet packet;

public:
	Serializer() {}

	template <typename T, typename ...Types>
	Packet serialize(T first, Types ... args) {
		packet.append(first);
		packet += serialize(args...);
		return packet;
	}

	template <typename T>
	Packet serialize(T last) {
		packet.append(last);
		return packet;
	}
};

class Deserializer {
	Packet packet;

public:
	Deserializer(Packet p) : packet(p) {}

	template <typename T, typename ...Types>
	void deserialize(T& first, Types ... args) {
		packet.read(first);
		deserialize(args...);
	}

	template <typename T>
	void deserialize(T& last) {
		packet.read(last);
	}
};

int main() {
	Serializer serializer;
	Packet p = serializer.serialize(1, 10, "oh shit waddup");

	Deserializer deserializer(p);
	int i, j;
	std::string s;
	deserializer.deserialize(i, j, s);

	std::cout << i << j << s << std::endl;
}