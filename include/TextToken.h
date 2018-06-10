//Copyright(c) 2018
//Authors: Fabian Wahlster
//Website: https://twitter.com/singul4rity
//Contact: f.wahlster@tum.de

//////////////////////////////////////////////////////////////////////////////
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute copies of the Software, and
//to permit persons to whom the Software is furnished to do so, subject to the
//following conditions:
//
//- The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//- Naming the author(s) of this software in any of the following locations:
//	About page, README file, credits.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//THE SOFTWARE.
//////////////////////////////////////////////////////////////////////////////

#ifndef HLX_TEXTTOKEN_H
#define HLX_TEXTTOKEN_H

#include "ByteStream.h"
#include "StringHelpers.h"
#include "FileStream.h"
#include "Vector.h"
#include <map>
#include <iomanip>

namespace hlx
{
	template<typename T>
	inline std::ostringstream& operator<<(typename std::enable_if<std::is_enum<T>::value, std::ostringstream>::type& stream, const T& e)
	{
		stream /*<< "0x"*/ << std::hex << static_cast<uint64_t>(e);
		//stream << std::to_string(static_cast<typename std::underlying_type<T>::type>(e));
		return stream;
	}

	// read uint8_t as integral, not char
	inline std::istringstream& operator >> (std::istringstream& stream, uint8_t& _u)
	{
		uint16_t uProxy = 0u;
		stream >> std::hex >> uProxy;
		_u = static_cast<uint8_t>(uProxy);
		return stream;
	}

	template<typename T>
	inline std::istringstream& operator >> (typename std::enable_if<std::is_enum<T>::value, std::istringstream>::type& stream, T& e)
	{
		/*std::underlying_type<T>::type*/ uint64_t Proxy = {};

		stream >> std::hex >> Proxy;
		e = static_cast<T>(Proxy);

		return stream;
	}

	class TextToken
	{
	private:
		std::string m_key;//name
		std::multimap<std::string, TextToken> children;
		std::multimap<std::string, std::string> values;

		template <typename stream_t = const bytes>
		std::string read_value(basestream<stream_t>& stream, char seperator = '\"');

	public:
		//default constructor
		TextToken(const std::string& _sKey = {}) : m_key(_sKey) {};
		//parse key-value from stream
		template <typename stream_t = const bytes>
		TextToken(basestream<stream_t>& stream, std::string key, bool keys_to_lower = false);
		//parse kv from file
		TextToken(const hlx::string& filepath, std::string key, bool keys_to_lower = false, std::ios_base::openmode mode = std::ios_base::binary | std::ios_base::in);

		inline bool IsEmpty() const { return children.empty() && values.empty(); }
		inline const std::string& getKey(void) const { return m_key; }
		inline void setKey(const std::string& _sKey) { m_key = _sKey; }
		inline size_t getChildCount() const { return children.size(); }
		inline size_t getValueCount() const { return values.size(); }

		//populate token from bytestream
		template <typename stream_t = const bytes>
		void Initialize(basestream<stream_t>& stream, std::string key, bool keys_to_lower);

		//write key-value tree to stream
		void serialize(bytestream& stream, std::string tabs = {}) const;

		//write key-value tree to file
		bool serialize(const hlx::string& filepath, std::ios_base::openmode mode = std::ios_base::out | std::ios_base::trunc) const;

		//get the raw value data (string) with the corresponding key
		std::string getValue(std::string _key, const std::string& _sDefaultValue = {}, bool keys_to_lower = false) const;

		//get all raw values (string) with the corresponding key
		std::vector<std::string> getAllValues(std::string _key, bool keys_to_lower = false) const;

		//get the child token with the corresponding key
		bool getToken(std::string _key, TextToken& out, bool keys_to_lower = false);

		//get the const child token with the corresponding key
		bool getTokenConst(std::string _key, TextToken& out, bool keys_to_lower = false) const;

		// append token at key
		void addToken(std::string _key, const TextToken& token, bool keys_to_lower = false);

		// set / replace token at key
		void setToken(std::string _key, const TextToken& token, bool keys_to_lower = false);

		// remove tokens at key
		void remove(const std::string& _sKey, bool _bRange = false);

		//get all child tokens with the corresponding key
		std::pair<std::multimap<std::string, TextToken>::iterator, std::multimap<std::string, TextToken>::iterator> getAllTokens(std::string _key, bool keys_to_lower = false);
		//---------------------------------------------------------------------------------------------------

		//get all const child tokens with the corresponding key
		std::pair<std::multimap<std::string, TextToken>::const_iterator, std::multimap<std::string, TextToken>::const_iterator> getAllTokens(std::string _key, bool keys_to_lower = false) const;

		//get all child tokens from this token
		std::vector<TextToken> getAllTokens(void) const;

		//get the token using a complete path (e.g. AppState->getTokenFromPath("UserConfig/CheckGuid");
		bool getTokenFromPath(std::string path, TextToken& out, bool keys_to_lower = false, char separator = '/');

		//---------------------------------------------------------------------------------------------------
		// helper functions
		template <class T>
		void SetPrecision(std::ostringstream& ss) const {}

		template <>
		void SetPrecision<float>(std::ostringstream& ss) const
		{
			ss.precision(std::numeric_limits<float>::max_digits10);
		}

		template <>
		void SetPrecision<double>(std::ostringstream& ss) const
		{
			ss.precision(std::numeric_limits<double>::max_digits10);
		}

		//---------------------------------------------------------------------------------------------------
		// template implementations

		//get primitive value by key
		template <class T> T get(std::string key, const T& _DefaultValue = {}, bool keys_to_lower = false) const
		{
			T ret = _DefaultValue;
			if (keys_to_lower)
			{
				hlx::to_lower(key);
			}

			std::multimap<std::string, std::string>::const_iterator itr = values.find(key);
			if (itr == values.cend())
			{
				return ret;
			}

			std::istringstream stream(itr->second, std::ios_base::in);
			stream >> ret;

			return ret;
		}

		//---------------------------------------------------------------------------------------------------

		// get math vector with given dimension
		template <class T, int Dim> VecType<T, Dim> getVector(std::string key, const VecType<T, Dim>& _DefaultValue = {}, bool keys_to_lower = false) const
		{
			if (keys_to_lower)
			{
				hlx::to_lower(key);
			}

			VecType<T, Dim> ret = _DefaultValue;
			std::multimap<std::string, std::string>::const_iterator itr = values.find(key);

			if (itr == values.cend())
			{
				return ret;
			}

			std::istringstream stream(itr->second, std::ios_base::in);

			for (int i = 0; i < Dim; ++i)
			{
				stream >> ret[i];
			}

			return ret;
		}
		//---------------------------------------------------------------------------------------------------

		//get primitive value from path
		template <class T> T getFromPath(std::string path, std::string key, const T& _DefaultValue = {}, bool keys_to_lower = false) const
		{
			TextToken token;
			if (getTokenFromPath(path, object, keys_to_lower))
			{
				if (keys_to_lower)
				{
					hlx::to_lower(key);
				}
				return token.get<T>(key, _DefaultValue, keys_to_lower);
			}

			return _DefaultValue;
		}
		//---------------------------------------------------------------------------------------------------

		//get all primitive values with the specified key
		template <class T> std::vector<T> getAll(std::string key, bool keys_to_lower = false) const
		{
			T val = T();
			std::vector<T> ret;

			if (keys_to_lower)
			{
				hlx::to_lower(key);
			}

			auto itr = values.equal_range(key);

			for (std::multimap<std::string, std::string>::const_iterator it = itr.first; it != itr.second; ++it)
			{
				std::istringstream stream(it->second, std::ios_base::in);
				stream >> val;
				ret.push_back(val);
			}

			return ret;
		}
		//---------------------------------------------------------------------------------------------------

		//get all primitive values from this token
		template <class T> std::vector<T> getAll(void) const
		{
			T val = T();
			std::vector<T> ret;

			for (std::multimap<std::string, std::string>::const_iterator itr = values.begin(); itr != values.end(); ++itr)
			{
				std::istringstream stream(itr->second, std::ios_base::in);
				//from_hex<T>(stream, itr->second);
				stream >> val;
				ret.push_back(val);
			}

			return ret;
		}
		//---------------------------------------------------------------------------------------------------

		//get all key value pairs (with the value being a string) from this token
		template <class T> std::vector<std::pair<T, std::string> > getAllPairs(void) const
		{
			T key = T();
			std::vector<std::pair<T, std::string> > ret;

			for (std::multimap<std::string, std::string>::const_iterator itr = values.begin(); itr != values.end(); ++itr)
			{
				std::istringstream stream(itr->first, std::ios_base::in);
				stream >> key;
				ret.push_back({ key,itr->second });
			}

			return ret;
		}
		//---------------------------------------------------------------------------------------------------

		//get all primitive key value pairs from this token
		template <class KeyType, class ValueType> std::vector<std::pair<KeyType, ValueType> > getAllPairs(void) const
		{
			KeyType key = {};
			ValueType val = {};
			std::vector<std::pair<KeyType, ValueType> > ret;

			for (std::multimap<std::string, std::string>::const_iterator itr = values.begin(); itr != values.end(); ++itr)
			{
				std::istringstream stream(itr->first + ' ' + itr->second, std::ios_base::in);
				stream >> key >> val;
				ret.push_back({ key,val });
			}

			return ret;
		}
		//---------------------------------------------------------------------------------------------------

		//adds primitive value using the specified key
		template <class T>
		void add(std::string _key, const T& value, bool keys_to_lower = false, bool _bNoDuplicates = false)
		{
			if (keys_to_lower)
			{
				hlx::to_lower(_key);
			}

			if (_bNoDuplicates && values.count(_key) > 0)
			{
				return;
			}

			std::ostringstream ss;

			// set maximum precision based on type
			SetPrecision<T>(ss);

			ss << value;

			values.insert({ _key,ss.str() });
		}
		//---------------------------------------------------------------------------------------------------

		template <> void add(std::string _key, const std::string& value, bool keys_to_lower, bool _bNoDuplicates)
		{
			if (keys_to_lower)
			{
				hlx::to_lower(_key);
			}

			if (_bNoDuplicates && values.count(_key) > 0)
			{
				return;
			}

			values.insert({ _key, value });
		}
		//---------------------------------------------------------------------------------------------------

		template<class T, class itr = std::iterator<std::forward_iterator_tag, T>>
		void addContainer(std::string _key, itr _begin, itr _end, bool keys_to_lower = false, bool _bNoDuplicates = false)
		{
			if (std::distance(_begin, _end) == 0u)
			{
				return;
			}

			if (keys_to_lower)
			{
				hlx::to_lower(_key);
			}

			if (_bNoDuplicates && values.count(_key) > 0)
			{
				return;
			}

			for (; _begin != _end; ++_begin)
			{
				std::ostringstream ss;

				// set maximum precision based on type
				SetPrecision<T>(ss);

				ss << *_begin;

				values.insert({ _key,ss.str() });
			}
		}
		//---------------------------------------------------------------------------------------------------


		//replaces the primitive value at the specified key, or adds the value if the key doesnt exist
		template <class T> void set(std::string _key, const T& value, bool keys_to_lower = false)
		{
			if (keys_to_lower)
			{
				hlx::to_lower(_key);
			}

			std::multimap<std::string, std::string>::iterator itr = values.find(_key);

			std::ostringstream ss;
			ss << value;

			if (itr != values.end())
			{
				itr->second = ss.str();
			}
			else
			{
				values.insert({ _key, ss.str() });
			}
		}
		//---------------------------------------------------------------------------------------------------

		// avoid uint8_t printed as a char
		template <> void set(std::string _key, const uint8_t& value, bool keys_to_lower)
		{
			std::ostringstream ss;
			ss /*<< "0x"*/ << std::hex << static_cast<uint16_t>(value);
			set(_key, ss.str()/*std::to_string(value)*/, keys_to_lower);
		}

		void set(std::string _key, const uint8_t& value)
		{
			std::ostringstream ss;
			ss /*<< "0x"*/ << std::hex << static_cast<uint16_t>(value);
			set(_key, ss.str()/*std::to_string(value)*/, false);
		}

		//---------------------------------------------------------------------------------------------------
		// set math vector with given dimension
		template <class T, int Dim> void setVector(std::string _key, const VecType<T, Dim>& value, bool keys_to_lower = false)
		{
			if (keys_to_lower)
			{
				hlx::to_lower(_key);
			}

			std::ostringstream ss;

			for (int i = 0; i < Dim; ++i)
			{
				ss << value[i];
				if (i < Dim - 1)
				{
					ss << " ";
				}
			}

			std::multimap<std::string, std::string>::iterator itr = values.find(_key);

			if (itr != values.end())
			{
				itr->second = ss.str();
			}
			else
			{
				values.insert({ _key, ss.str() });
			}
		}
		//---------------------------------------------------------------------------------------------------

		//string version
		template <> void set(std::string _key, const std::string& value, bool keys_to_lower)
		{
			if (keys_to_lower)
			{
				hlx::to_lower(_key);
			}

			std::multimap<std::string, std::string>::iterator itr = values.find(_key);

			if (itr != values.end())
			{
				itr->second = value;
			}
			else
			{
				values.insert({ _key, value });
			}
		}
		//---------------------------------------------------------------------------------------------------


	}; // TextToken

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename stream_t>
	inline TextToken::TextToken(basestream<stream_t>& stream, std::string _key, bool keys_to_lower)
	{
		Initialize(stream, _key, keys_to_lower);
	}

	//---------------------------------------------------------------------------------------------------

	inline TextToken::TextToken(const hlx::string& filepath, std::string _key, bool keys_to_lower, std::ios_base::openmode mode)
	{
		fbytestream file(filepath, mode);
		if (file.is_open())
		{
			std::unique_ptr<const_bytestream> bstream = file.make_const_bytestream();
			if (bstream != nullptr)
			{
				Initialize(*bstream, _key, keys_to_lower);
			}
		}
	}
	//---------------------------------------------------------------------------------------------------

	//replaces the token at the specified key
	inline void TextToken::setToken(std::string _key, const TextToken& token, bool keys_to_lower)
	{
		if (keys_to_lower)
		{
			hlx::to_lower(_key);
		}

		std::multimap<std::string, TextToken>::iterator itr = children.find(_key);

		if (itr != children.end())
		{
			itr->second = token;
		}
		else
		{
			children.insert({ _key,token });
		}
	}
	//---------------------------------------------------------------------------------------------------

	//adds a token using the specified key
	inline void TextToken::addToken(std::string _key, const TextToken& token, bool keys_to_lower)
	{
		if (keys_to_lower)
		{
			hlx::to_lower(_key);
		}

		children.insert({ _key,token });
	}
	//---------------------------------------------------------------------------------------------------

	//get all child tokens with the corresponding key
	inline std::pair<std::multimap<std::string, TextToken>::iterator, std::multimap<std::string, TextToken>::iterator> TextToken::getAllTokens(std::string _key, bool keys_to_lower)
	{
		if (keys_to_lower)
		{
			hlx::to_lower(_key);
		}

		return children.equal_range(_key);
	}
	//---------------------------------------------------------------------------------------------------

	//get all const child tokens with the corresponding key
	inline std::pair<std::multimap<std::string, TextToken>::const_iterator, std::multimap<std::string, TextToken>::const_iterator> TextToken::getAllTokens(std::string _key, bool keys_to_lower) const
	{
		if (keys_to_lower)
		{
			hlx::to_lower(_key);
		}

		return children.equal_range(_key);
	}

	//---------------------------------------------------------------------------------------------------
	template<typename stream_t>
	inline std::string TextToken::read_value(basestream<stream_t>& stream, char separator)
	{
		char c = read_whitespaces<char>(stream);
		std::string ret;
		// remove whitespaces, tabs, linefeed
		if (c == separator)
		{
			while (stream.good() && (c = stream.get<char>()) != separator)
			{
				ret += c;
			}
		}
		else
		{
			ret += c;
		}
		return ret;
	}
	//---------------------------------------------------------------------------------------------------
	// parse the key-value system from given bytestream
	template<typename stream_t>
	inline void TextToken::Initialize(basestream<stream_t>& stream, std::string _key, bool keys_to_lower)
	{
		bool bTokenAsValue = false;

		if (keys_to_lower)
		{
			hlx::to_lower(_key);
		}

		this->m_key = _key;
		std::string token, value;

		while (stream.good())
		{
			token = read_value(stream);
			if (keys_to_lower)
			{
				hlx::to_lower(token);
			}

			if (token == "}")
			{
				break;
			}
			else if (token == "]")
			{
				bTokenAsValue = false;
				continue;
			}
			else if (token == ";" || token == "#")
			{//comment
				/*value = */skip_until(stream, '\n'); // skip the rest of the line
				continue;
			}
			else if (bTokenAsValue == false)
			{
				value = read_value(stream);
			}

			if (token == _key)
			{
				continue;
			}
			else if (value == "{")
			{
				children.insert(std::pair<std::string, TextToken >(token, TextToken(stream, token, keys_to_lower)));
			}
			else if (value == "[")
			{
				value = token;
				bTokenAsValue = true;
				continue;
			}
			else
			{
				if (bTokenAsValue == true)
				{
					values.insert(std::pair<std::string, std::string>(value, token));
				}
				else
				{
					values.insert(std::pair<std::string, std::string>(token, value));
				}
			}
		}
	}
	//---------------------------------------------------------------------------------------------------

	inline bool TextToken::getToken(std::string _key, hlx::TextToken& out, bool keys_to_lower)
	{
		if (keys_to_lower)
		{
			hlx::to_lower(_key);
		}

		std::multimap<std::string, TextToken>::iterator itr = children.find(_key);

		if (itr == children.end())
		{
			return false;
		}

		out = itr->second;
		return true;
	}
	//---------------------------------------------------------------------------------------------------

	inline bool TextToken::getTokenConst(std::string _key, hlx::TextToken& out, bool keys_to_lower) const
	{
		if (keys_to_lower)
		{
			hlx::to_lower(_key);
		}

		std::multimap<std::string, TextToken>::const_iterator itr = children.find(_key);

		if (itr == children.end())
		{
			return false;
		}

		out = itr->second;
		return true;
	}
	//---------------------------------------------------------------------------------------------------

	inline std::vector<TextToken> TextToken::getAllTokens(void) const
	{
		std::vector<TextToken> ret;
		std::multimap<std::string, TextToken>::const_iterator itr;

		for (itr = children.cbegin(); itr != children.cend(); ++itr) {
			ret.push_back(itr->second);
		}

		return ret;
	}

	//---------------------------------------------------------------------------------------------------

	inline bool TextToken::getTokenFromPath(std::string path, TextToken& out, bool keys_to_lower, char separator)
	{
		size_t pidx, idx = path.find_first_of(separator, 0);

		if (idx >= path.size())
		{
			return false;
		}

		if (getToken(path.substr(0, idx), out, keys_to_lower) == false)
		{
			return false;
		}

		pidx = idx; bool b = true;
		TextToken tmp;
		while ((idx = path.find_first_of(separator, idx + 1)) < path.size() && b)
		{
			b = out.getToken(path.substr(++pidx, idx - pidx), tmp, keys_to_lower);
			out = tmp;
			pidx = idx;
		}

		if (++pidx < path.size() && b)
		{
			b = out.getToken(path.substr(pidx), tmp, keys_to_lower);
			out = tmp;
			return b;
		}

		return true;
	}

	//---------------------------------------------------------------------------------------------------

	inline std::string TextToken::getValue(std::string _key, const std::string& _sDefaultValue, bool keys_to_lower) const
	{
		if (keys_to_lower)
		{
			hlx::to_lower(_key);
		}

		std::multimap<std::string, std::string>::const_iterator itr = values.find(_key);
		if (itr == values.cend())
		{
			return _sDefaultValue;
		}
		return itr->second;
	}

	//---------------------------------------------------------------------------------------------------
	inline std::vector<std::string> TextToken::getAllValues(std::string _key, bool keys_to_lower) const
	{
		if (keys_to_lower)
		{
			hlx::to_lower(_key);
		}

		std::vector<std::string> ret;
		std::pair<std::multimap<std::string, std::string>::const_iterator, std::multimap<std::string, std::string>::const_iterator> itr = values.equal_range(_key);

		for (std::multimap<std::string, std::string>::const_iterator it = itr.first; it != itr.second; ++it) {
			ret.push_back(it->second);
		}

		return ret;
	}
	//---------------------------------------------------------------------------------------------------

	inline void TextToken::remove(const std::string& _sKey, bool _bRange)
	{
		std::pair<std::multimap<std::string, std::string>::iterator, std::multimap<std::string, std::string>::iterator> itr = values.equal_range(_sKey);

		for (std::multimap<std::string, std::string>::iterator it = itr.first; it != itr.second;)
		{
			it = values.erase(it);

			if (_bRange == false)
			{
				break;
			}
		}
	}
	//---------------------------------------------------------------------------------------------------

	inline void TextToken::serialize(bytestream& stream, std::string tabs) const
	{
		stream.put(tabs + "\"" + this->m_key + "\"\n" + tabs + "{\n", false);

		for (std::multimap<std::string, std::string>::const_iterator itr = values.cbegin(); itr != values.cend(); ++itr)
		{
			stream.put(tabs + "\t\"" + itr->first + "\"\t\t\"" + itr->second + "\"\n", false);
		}

		for (std::multimap<std::string, TextToken>::const_iterator itr = children.cbegin(); itr != children.cend(); ++itr)
		{
			itr->second.serialize(stream, tabs + "\t");
		}

		stream.put(tabs + "}\n", false);
	}
	//---------------------------------------------------------------------------------------------------

	inline bool TextToken::serialize(const hlx::string& filepath, std::ios_base::openmode mode) const
	{
		fbytestream file(filepath, mode);

		if (file.is_open())
		{
			bytes buffer;
			bytestream bstream(buffer);

			serialize(bstream);

			file.put<bytes>(buffer);

			file.close();

			return true;
		}
		else
		{
			return false;
		}
	}
	//---------------------------------------------------------------------------------------------------

} // hlx
#endif // HLX_TEXTTOKEN_H
