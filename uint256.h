#ifndef UINT256_H
#define UINT256_H

#include <vector>
#include <string>

template<uint32_t BITS>
class blob {
protected:
	static constexpr int width = BITS / 8;
	uint8_t data[width];
public:
	blob()
	{
		memset(data, 0, sizeof(data));
	}

	// constructor
	explicit blob(const std::vector<uint8_t> &v);
	explicit blob(const std::vector<unsigned char> &v);
	explicit blob(const unsigned char *arr, const int size = width);

	bool is_null()
	{
		for(int i = 0; i < width; ++i) {
			if(data[i] != 0) {
				return false;
			}
		}
		return true;
	}

	void set_null()
	{
		memset(data, 0, sizeof(data));
	}

	// compare
	int compare(const blob &b) const
	{
		// this.data < b.data, val < 0
		// this.data == b.data, val == 0
		// this.data > b.data, val > 0
		return memcmp(data, b.data, sizeof(data));
	}

	// operators
	friend bool operator==(const blob &a, const blob &b)
	{
		return a.compare(b) == 0;
	}

	friend bool operator!=(const blob &a, const blob &b)
	{
		return a.compare(b) != 0;
	}

	friend bool operator<(const blob &a, const blob &b)
	{
		return a.compare(b) < 0;
	}
	
	std::string get_hex() const;
	void set_hex(const char *arr);
	void set_hex(const std::string &str);
	std::string to_string() const;

	// iterators
	unsigned char *begin()
	{
		return &data[0];
	}

	unsigned char *end()
	{
		return &data[width];
	}

	const unsigned char *begin() const
	{
		return &data[0];
	}

	const unsigned char *end() const 
	{
		return &data[width];
	}

	// size
	uint32_t size() const
	{
		return sizeof(data);
	}
};

class uint256: public blob<256> {
public:
	uint256() {}
	explicit uint256(const std::vector<unsigned char> &v): blob<256>(v) {  }
};

#endif
