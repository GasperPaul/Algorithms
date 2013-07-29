#ifndef LONGARITHMETICS
#define LONGARITHMETICS

#include<string>
using std::string;
#define max(x,y) (x)>(y)?(x):(y)

namespace gasper{
	
	string reverse(string a)
	{
		int k = a.length();
		for (int i = 0; i<k/2; i++)
		{
			char tmp = a[i];
			a[i] = a[k-i-1];
			a[k-i-1] = tmp;
		}
		return a;
	}
	
	string add(string a, string b, const int base = 10)
	{
		if (a.length()==0) return (b.length()==0 ? "0" : b);
		if (b.length()==0) return a;
		
		a = reverse(a);
		b = reverse(b);
		
		unsigned int k = max(a.length(), b.length());
		for(int i = a.length(); i<k; i++) a+="0";
		for(int i = b.length(); i<k; i++) b+="0";
		
		int p = 0;
		string result = "";
		for(int i = 0; i<k; i++)
		{
			unsigned int tmp = (a[i]-48)+(b[i]-48)+p;
			result += tmp%base + 48;
			p = tmp/base;
		}
		if (p) result += p + 48;
		return reverse(result);
	}

	string delz(string a)
	{
		while(true)
			if (a[0]==48) 
				a = a.substr(1,a.length());
			else break;
		return a.length() ? a : "0";
	}

	int cmp(string a, string b)
	{
		a = delz(a);
		b = delz(b);
		if (a.length()!=b.length()) return (a.length()>b.length()?1:-1);
	
		for (int i = 0; i<a.length(); i++)
		{
			if (a[i]!=b[i]) return (a[i]>b[i]?1:-1);
		}
		return 0;
	}
	
	string sub(string a, string b, const int base = 10)
	{
		if (!cmp(a,b)) return "0";
		if (cmp(a,b)<0) return "Error: first argument must be greater or equal to second.";
	
		a = reverse(a);
		b = reverse(b);
		
		unsigned int k = max(a.length(), b.length());
		for(int i = a.length(); i<k; i++) a+="0";
		for(int i = b.length(); i<k; i++) b+="0";
		
		string result = "";
		for (int i = 0; i<k; i++)
		{
			int diff = a[i]-b[i];
			if (diff>=0)
				result += diff + 48;
			else
			{
				int j = i+1;
				while(j<=k)
				{
					a[j] = (a[j]-48+(base-1))%base + 48;
					if (a[j]-48 == base-1)
						j++;
					else 
						break;
				}
				result += diff+base+48;
			}
		}
		
		return delz(reverse(result));
	}
	
	string mul(string a, string b, const int base = 10)
	{
		if (a.length()==0 || b.length()==0) return "0";
		
		unsigned int k = max(a.length(), b.length());
		for(int i = a.length(); i<k; i++) a="0"+a;
		for(int i = b.length(); i<k; i++) b="0"+b;
		
		int res = 0;
		string result = "";
		if (k == 1)
		{
			res = (a[0]-48)*(b[0]-48);
			if (res<base)
				result += (char)res+48;
			else
			{
				result += (char)(res/base+48);
				result += (char)(res%base+48);
			}
			return result;
		}
		
		unsigned int m0 = (k+1)/2;
		string	x1 = a.substr(m0, k-m0),
				x0 = a.substr(0, m0),
				y1 = b.substr(m0, k-m0),
				y0 = b.substr(0, m0);
		string	p0 = mul(x0,y0,base),
				p1 = mul(add(x0,x1,base), add(y0,y1,base), base),
				p2 = mul(x1,y1,base);
		string z = sub(p1, add(p0,p2,base), base);
		for(int i = 0; i<k/2; i++)
		{
			p0+="00";
			z+="0";
		}
		
		return delz(add(add(p0,z,base), p2, base));
	}
	
	string inc(string a, const string increment = "1", const int base = 10)
	{
		return add(a, increment, base);
	}
	
	string dec(string a, const string decrement = "1", const int base = 10)
	{
		return sub(a, decrement, base);
	}
	
	string div(string a, string b, const int base = 10)
	{
		if (a.length() == 0 || (a = delz(a)) == "0") return "0";
		if (b.length() == 0 || (b = delz(b)) == "0") return "Error: division by zero.";
		if (cmp(a,b)<0) return "0";
		string div = "0";
		while (cmp((a = sub(a,b,base)), b)>=0)
			div = inc(div, "1", base);
		return inc(div, "1", base);
	}
	
	string mod(string a, string b, const int base = 10)
	{
		if (a.length() == 0 || (a = delz(a)) == "0") return "0";
		if (b.length() == 0 || (b = delz(b)) == "0") return "Error: division by zero.";
		while (cmp((a = sub(a,b,base)), b)>=0);
		return a;
	}
	
}

#endif