#ifndef BIGINT_H
#define BIGINT_H

#include<string>
using std::string;

namespace gasper {

	template<unsigned base> 
	class BigInt
	{
	public:
		const static BigInt Zero;
		const static BigInt One;

		BigInt() : str("0") { }
		BigInt(const string& _str);
		BigInt(const char* _str) : BigInt(string(_str)) { }
		BigInt(const BigInt& _other) : str(_other.str), sign(_other.sign) { }
		template<unsigned rBase> BigInt(const BigInt<rBase>& rhs) { swap(*this, rhs.ToBase<base>()); }
		template<typename T> BigInt(T _num) : BigInt(std::to_string(_num)) { static_assert(is_integral<T>::value, "Cannot construct BigInt from provided type."); }
		virtual ~BigInt() { }

		BigInt& operator=(BigInt rhs) { swap(str, rhs.str); return *this; }
		template<unsigned rBase> inline BigInt& operator=(const BigInt<rBase>& rhs) { return operator=(BigInt(rhs)); }

		bool operator==(const BigInt& rhs) const { return (str == rhs.str) && (sign == rhs.sign || str == "0"); }
		bool operator<(const BigInt& rhs) const 
		{ 
			if (sign != rhs.sign) return !sign;
			if (str.length() != rhs.str.length()) return sign ? str.length() < rhs.str.length() : !(str.length() < rhs.str.length()); 
			auto i = str.begin();
			auto j = rhs.str.begin();
			for (; i != str.end(); ++i, ++j)
				if (*i - *j) return sign ? (*i - *j) < 0 : !((*i - *j) < 0);
			return false; 
		}
		inline bool operator!=(const BigInt& rhs) const { return !(*this == rhs); }
		inline bool operator>(const BigInt& rhs) const { return rhs < *this; }
		inline bool operator<=(const BigInt& rhs) const { return !(rhs > *this); }
		inline bool operator>=(const BigInt& rhs) const { return !(*this < rhs); }

		BigInt& operator+=(const BigInt& rhs);
		BigInt& operator-=(const BigInt& rhs);
		BigInt& operator*=(const BigInt& rhs);
		BigInt& operator/=(const BigInt& rhs);
		BigInt& operator%=(const BigInt& rhs);
		BigInt& operator^=(const BigInt& rhs);
		template<unsigned rBase> inline BigInt& operator+=(const BigInt<rBase>& rhs) { return operator+=(BigInt(rhs)); }
		template<unsigned rBase> inline BigInt& operator-=(const BigInt<rBase>& rhs) { return operator-=(BigInt(rhs)); }
		template<unsigned rBase> inline BigInt& operator*=(const BigInt<rBase>& rhs) { return operator*=(BigInt(rhs)); }
		template<unsigned rBase> inline BigInt& operator/=(const BigInt<rBase>& rhs) { return operator/=(BigInt(rhs)); }
		template<unsigned rBase> inline BigInt& operator%=(const BigInt<rBase>& rhs) { return operator%=(BigInt(rhs)); }
		template<unsigned rBase> inline BigInt& operator^=(const BigInt<rBase>& rhs) { return operator^=(BigInt(rhs)); }

		inline BigInt& operator++() { return operator+=(One); }
		inline BigInt operator++(int) { return operator+=(One); }
		inline BigInt& operator--() { return operator-=(One); }
		inline BigInt operator--(int) { return operator-=(One); }
		inline BigInt operator-() { return 0 - *this; }

		template<typename T> inline BigInt& operator+=(T rhs) { static_assert(is_integral<T>::value, "Cannot add with nonintegral types."); return operator+=(BigInt<10>(rhs)); }
		template<typename T> inline BigInt& operator-=(T rhs) { static_assert(is_integral<T>::value, "Cannot subtract with nonintegral types."); return operator-=(BigInt<10>(rhs)); }
		template<typename T> inline BigInt& operator*=(T rhs) { static_assert(is_integral<T>::value, "Cannot multiply with nonintegral type."); return operator*=(BigInt<10>(rhs)); }
		template<typename T> inline BigInt& operator/=(T rhs) { static_assert(is_integral<T>::value, "Cannot divide by nonintegral type."); return operator/=(BigInt<10>(rhs)); }
		template<typename T> inline BigInt& operator%=(T rhs) { static_assert(is_integral<T>::value, "Cannot divide by modulus of nonintegral type."); return operator%=(BigInt<10>(rhs)); }
		template<typename T> inline BigInt& operator^=(T rhs) { static_assert(is_integral<T>::value, "Cannot get to power of nonintegral type."); return operator^=(BigInt<10>(rhs)); }

		inline string ToString() { return (!sign ? "-" : "") + str; }
		inline explicit operator string() { return (!sign ? "-" : "") + str; }

		inline unsigned Base() { return base; }
		inline string BaseAsString() { return std::to_string(base); }
		template<unsigned toBase> inline BigInt<toBase> ToBase() const { return BigInt<toBase>((!sign ? "-" : "") + str); }

		inline BigInt Abs() const { return BigInt(str); }
		inline bool IsNegative() { return !sign || str == "0"; }
		inline bool IsPositive() { return sign || str == "0"; }

	private:
		string str;
		bool sign = true;
	};

	template<unsigned base> const BigInt<base> BigInt<base>::Zero = BigInt<base>("0");
	template<unsigned base> const BigInt<base> BigInt<base>::One = BigInt<base>("1");
	
	template<unsigned lBase, unsigned rBase> inline BigInt<lBase> operator+(BigInt<lBase> lhs, const BigInt<rBase>& rhs) { return lhs += rhs; }
	template<unsigned lBase, unsigned rBase> inline BigInt<lBase> operator-(BigInt<lBase> lhs, const BigInt<rBase>& rhs) { return lhs -= rhs; }
	template<unsigned lBase, unsigned rBase> inline BigInt<lBase> operator*(BigInt<lBase> lhs, const BigInt<rBase>& rhs) { return lhs *= rhs; }
	template<unsigned lBase, unsigned rBase> inline BigInt<lBase> operator/(BigInt<lBase> lhs, const BigInt<rBase>& rhs) { return lhs /= rhs; }
	template<unsigned lBase, unsigned rBase> inline BigInt<lBase> operator%(BigInt<lBase> lhs, const BigInt<rBase>& rhs) { return lhs %= rhs; }
	template<unsigned lBase, unsigned rBase> inline BigInt<lBase> operator^(BigInt<lBase> lhs, const BigInt<rBase>& rhs) { return lhs ^= rhs; }

	template<unsigned lBase, typename T> inline BigInt<lBase> operator+(BigInt<lBase> lhs, T rhs) { return lhs += rhs; }
	template<unsigned lBase, typename T> inline BigInt<lBase> operator-(BigInt<lBase> lhs, T rhs) { return lhs -= rhs; }
	template<unsigned lBase, typename T> inline BigInt<lBase> operator*(BigInt<lBase> lhs, T rhs) { return lhs *= rhs; }
	template<unsigned lBase, typename T> inline BigInt<lBase> operator/(BigInt<lBase> lhs, T rhs) { return lhs /= rhs; }
	template<unsigned lBase, typename T> inline BigInt<lBase> operator%(BigInt<lBase> lhs, T rhs) { return lhs %= rhs; }
	template<unsigned lBase, typename T> inline BigInt<lBase> operator^(BigInt<lBase> lhs, T rhs) { return lhs ^= rhs; }

	template<unsigned lBase, typename T> inline bool operator==(const BigInt<lBase>& lhs, const T& rhs) { return lhs == BigInt<lBase>(rhs); }
	template<unsigned lBase, typename T> inline bool operator!=(const BigInt<lBase>& lhs, const T& rhs) { return !(lhs == BigInt<lBase>(rhs)); }
	template<unsigned lBase, typename T> inline bool operator< (const BigInt<lBase>& lhs, const T& rhs) { return lhs < BigInt<lBase>(rhs); }
	template<unsigned lBase, typename T> inline bool operator> (const BigInt<lBase>& lhs, const T& rhs) { return BigInt<lBase>(rhs) < lhs; }
	template<unsigned lBase, typename T> inline bool operator<=(const BigInt<lBase>& lhs, const T& rhs) { return !(BigInt<lBase>(rhs) > lhs); }
	template<unsigned lBase, typename T> inline bool operator>=(const BigInt<lBase>& lhs, const T& rhs) { return !(lhs < BigInt<lBase>(rhs)); }
	
	template<unsigned rBase, typename T> inline bool operator==(const T& lhs, const BigInt<rBase>& rhs) { return BigInt<rBase>(lhs) == rhs; }
	template<unsigned rBase, typename T> inline bool operator!=(const T& lhs, const BigInt<rBase>& rhs) { return !(BigInt<rBase>(lhs) == rhs); }
	template<unsigned rBase, typename T> inline bool operator< (const T& lhs, const BigInt<rBase>& rhs) { return BigInt<rBase>(lhs) < rhs; }
	template<unsigned rBase, typename T> inline bool operator> (const T& lhs, const BigInt<rBase>& rhs) { return rhs < BigInt<rBase>(lhs); }
	template<unsigned rBase, typename T> inline bool operator<=(const T& lhs, const BigInt<rBase>& rhs) { return !(rhs > BigInt<rBase>(lhs)); }
	template<unsigned rBase, typename T> inline bool operator>=(const T& lhs, const BigInt<rBase>& rhs) { return !(BigInt<rBase>(lhs) < rhs); }
	
	template<unsigned base>
	std::ostream& operator<<(std::ostream& os, BigInt<base>& obj)
	{
		os << obj.ToString();
		return os;
	}
	
	template<unsigned base>
	std::istream& operator>>(std::istream& is, BigInt<base>& obj)
	{
		string tmp;
		is >> tmp;
		obj(BigInt<10>(tmp));
		return is;
	}

	template<unsigned base>
	BigInt<base>::BigInt(const string& _str)
	{
		if (_str.empty())
		{
			str = "0";
			return;
		}
		auto i = _str.begin();
		if (*i == '+' || *i == '-') sign = (*i++ == '+');
		for (; i != _str.end(); ++i)
			if ('0' > *i || *i > '9')
				throw new exception("Initiallization string contains illegal characters.");
		str = (!(*(_str.begin()) == '+' || *(_str.begin()) == '-') ? _str : _str.substr(1));
		while (*(str.begin()) == '0' && str.begin()+1 != str.end()) str.erase(0, 1);
	}

	template<unsigned base>
	BigInt<base>& BigInt<base>::operator+=(const BigInt<base>& rhs)
	{
		if (sign != rhs.sign)
		{
			const auto rhsModulus = rhs.Abs();
			bool tmp = *this > rhsModulus;
			sign = true;
			operator-=(rhsModulus);
			sign = tmp;
			return *this;
		}

		unsigned p = 0;
		auto i = str.rbegin();
		auto j = rhs.str.rbegin();
		for (; i != str.rend(); ++i, j != rhs.str.rend() ? ++j : j)
		{
			unsigned tmp = *i - '0' + p + (j != rhs.str.rend() ? *j -'0' : 0);
			*i = char(tmp % base + '0');
			p = tmp / base;
		}
		if (j != rhs.str.rend())
		{
			auto tmp_str = string(j, rhs.str.rend());
			std::reverse(tmp_str.begin(), tmp_str.end());
			i = tmp_str.rbegin();
			while (p && i != tmp_str.rend())
			{
				unsigned tmp = *i + p - '0';
				*i++ = char(tmp % base + '0');
				p = tmp / base;
			}
			str = tmp_str + str;
		}
		if (p) str = char(p + '0') + str;
		return *this;
	}

	template<unsigned base>
	BigInt<base>& BigInt<base>::operator-=(const BigInt<base>& rhs)
	{
		if (*this == rhs) return (*this = Zero);
		if (sign != rhs.sign)
		{
			sign = true;
			*this += rhs.Abs();
			sign = !rhs.sign;
			return *this;
		}
		if (Abs() < rhs.Abs()) //throw exception("First argument must be greater or equal to second.");
		{
			*this = rhs - *this;
			sign = !sign;
			return *this;
		}

		auto i = str.rbegin();
		auto j = const_cast<BigInt<base>&>(rhs).str.rbegin();
		for (; j != rhs.str.rend(); ++i, ++j)
		{
			long diff = *i - *j;
			if (diff >= 0)
				*i = char(diff + '0');
			else
			{
				auto k = i + 1;
				while (k != str.rend())
				{
					*k = char((*k - '0' + base - 1) % base + '0');
					if (*k - '0' == base - 1)
						++k;
					else
						break;
				}
				*i = char(diff + base + '0');
			}
		}
		while (*(str.begin()) == '0' && str.begin() + 1 != str.end()) str.erase(0, 1);
		return *this;
	}

	template<unsigned base>
	BigInt<base>& BigInt<base>::operator*=(const BigInt<base>& rhs)
	{
		if (*this == Zero || rhs == Zero) return (*this = Zero);
		sign = (sign && rhs.sign) || (sign == rhs.sign);

		unsigned p = 0;
		if (str.length() == 1 && rhs.str.length() == 1)
		{
			p = (str[0] - '0') * (rhs.str[0] - '0');
			str[0] = char(p % base + '0');
			if (p >= base)
				str = char(p / base + '0') + str;
			return *this;
		}

		unsigned m0 = (str.length() >= rhs.str.length() ? str.length() : rhs.str.length()) / 2;
		BigInt<base> x1(m0>str.length() ? str : string(str.end() - m0, str.end())),
			x0(m0>str.length() ? "0" : string(str.begin(), str.end() - m0)),
			y1(m0>rhs.str.length() ? rhs.str : string(rhs.str.end() - m0, rhs.str.end())),
			y0(m0>rhs.str.length() ? "0" : string(rhs.str.begin(), rhs.str.end() - m0));
		BigInt<base> p0 = x0 * y0, p1 = (x0 + x1) * (y0 + y1), p2 = x1 * y1;
		BigInt<base> z = p1 - (p0 + p2);
		if (p0 != Zero) p0.str.append(2 * m0, '0');
		if (z != Zero) z.str.append(m0, '0');
		*this = p0 + p2 + z;
		return *this;
	}

	template<unsigned base>
	BigInt<base>& BigInt<base>::operator/=(const BigInt<base>& rhs)
	{
		if (rhs == Zero) throw exception("Division by zero.");
		if (*this == Zero) return *this;
		if (Abs() < rhs.Abs()) return (*this = Zero);

		bool tmp = (sign && rhs.sign) || (sign == rhs.sign);
		sign = true;
		BigInt<base> div = Zero;

		const auto rhsModulus = rhs.Abs();
		while ((*this = *this - rhsModulus) >= rhsModulus)
			div++;
		sign = tmp;
		return *this = (++div);
	}

	template<unsigned base>
	BigInt<base>& BigInt<base>::operator%=(const BigInt<base>& rhs)
	{
		if (rhs == Zero) throw exception("Division by zero.");
		if (*this == Zero) return *this;
		
		bool tmp = (sign && rhs.sign) || (sign == rhs.sign);
		sign = true;
		const auto rhsModulus = rhs.Abs();

		while ((*this = *this - rhsModulus) >= rhsModulus);
		sign = tmp;
		return *this;
	}
	
	template<unsigned base>
	BigInt<base>& BigInt<base>::operator^=(const BigInt<base>& rhs)
	{
		if (rhs == Zero) return (*this = One);
		if (!rhs.sign) return (*this = Zero);
		auto power = (rhs.sign ? rhs : rhs.Abs()), powerBase = *this;
		while (power-- != Zero) 
			*this *= powerBase;
		return *this;
	}
	
}

using BigInt = gasper::BigInt<10>;

#endif // !BIGINT_H
