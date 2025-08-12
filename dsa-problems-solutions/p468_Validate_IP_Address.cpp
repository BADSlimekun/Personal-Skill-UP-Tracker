// Problem 468 Validate IP Address
/*
Given a string queryIP, return "IPv4" if IP is a valid IPv4 address, 
"IPv6" if IP is a valid IPv6 address or "Neither" if IP is not a correct IP of any type.

A valid IPv4 address is an IP in the form "x1.x2.x3.x4" where 0 <= xi <= 255 and xi cannot contain leading zeros.
For example, "192.168.1.1" and "192.168.1.0" are valid IPv4 addresses while "192.168.01.1",
"192.168.1.00", and "192.168@1.1" are invalid IPv4 addresses.

A valid IPv6 address is an IP in the form "x1:x2:x3:x4:x5:x6:x7:x8" where:

1 <= xi.length <= 4
xi is a hexadecimal string which may contain digits, 
lowercase English letter ('a' to 'f') and upper-case English letters ('A' to 'F').
Leading zeros are allowed in xi.
For example, "2001:0db8:85a3:0000:0000:8a2e:0370:7334" and 
"2001:db8:85a3:0:0:8A2E:0370:7334" are valid IPv6 addresses, while "2001:0db8:85a3::8A2E:037j:7334" and
"02001:0db8:85a3:0000:0000:8a2e:0370:7334" are invalid IPv6 addresses.

*/

#include <string>
using namespace std;

class Solution {
public:
    string validIPAddress(string queryIP) 
    {
        int countIPV4 = 0, countIPV6 = 0;
        for (char c0: queryIP) {
            if (c0 == ':') countIPV6++;
            else if (c0 == '.') countIPV4++;
        }
        if (countIPV4 == 3) return validateIPV4(queryIP);
        else if (countIPV6 == 7) return validateIPV6(queryIP);
        return "Neither";
    }

    string validateIPV4(string &queryIP) 
    {
        size_t start = 0, end = queryIP.find('.');
        string dp;
        while (end!=string::npos) {
            dp = queryIP.substr(start, end-start);
            if (dp.length() == 0 || dp.length() > 3) return "Neither";
            if (dp[0] == '0' && dp.length() > 1) return "Neither";
            for (char xi:dp) {
                if (!isdigit(static_cast<unsigned char>(xi))) return "Neither";  
            }
            int num = stoi(dp);
            if (num < 0 || num > 255) return "Neither";
            start = end + 1;
            end = queryIP.find('.', start);
        }
        // to account for the last one
        dp = queryIP.substr(start);
        if (dp.length() == 0 || dp.length() > 3) return "Neither";
        if (dp[0] == '0' && dp.length() > 1) return "Neither";
        for (char xi:dp) {
            if (!isdigit(static_cast<unsigned char>(xi))) return "Neither";  
        }
        int num = stoi(dp);
        if (num < 0 || num > 255) return "Neither";
    
        return "IPv4";
    }

    string validateIPV6(string &IP) 
    {
        size_t start = 0, end = IP.find(':');
        string dp;
        string hexadigits = "0123456789abcdefABCDEF";
        while (end!=string::npos) {
            dp = IP.substr(start, end-start);
            if (dp.length()==0 || dp.length()>4) return "Neither";
            for (char i:dp) {
                if (hexadigits.find(i) == string::npos) return "Neither";
            }
            start = end + 1;
            end = IP.find(':',start);
        }
        dp = IP.substr(start);
        if (dp.length()==0 || dp.length()>4) return "Neither";
        for (char i:dp) {
            if (hexadigits.find(i) == string::npos) return "Neither";
        }

        return "IPv6";
    }
};


// Approach 2 using regex matching

#include <regex>

class Solution2 {
public:
    string validIPAddress(string queryIP) {
        static const regex patternIPv4(
            R"(^([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\.([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\.([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\.([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])$)",
            regex::ECMAScript | regex::optimize
        );
        static const regex patternIPv6(
            R"(^([0-9a-fA-F]{1,4})\:([0-9a-fA-F]{1,4})\:([0-9a-fA-F]{1,4})\:([0-9a-fA-F]{1,4})\:([0-9a-fA-F]{1,4})\:([0-9a-fA-F]{1,4})\:([0-9a-fA-F]{1,4})\:([0-9a-fA-F]{1,4})$)",
            regex::ECMAScript | regex::optimize
        );

        if (regex_match(queryIP, patternIPv4)) return "IPv4";
        if (regex_match(queryIP, patternIPv6)) return "IPv6";
        return "Neither";
    }
};