#include <iostream>
#include <string>
#include <bitset>

// implementation by bitset
bool shift_and_bitset(std::string source, std::string pattern)
{
    size_t src_len = source.length(), pat_len = pattern.length();
    // const int matched_mark = 2 ^ (pattern - 1);
    // assume character set is A-Za-z, 52 chars
    std::bitset<64> B[52]; // pat_len should less than or equal to 64
    std::bitset<64> D; // the bitset representing the prefix-matching status, initialized as 0x00...
    
    // initialization of B
    for (size_t i = 0; i < pat_len; i++)
    {
        // set the i-bit of B[pattern[i]] as 1
        B[pattern[i]-'A'].set(i, true);
    }

    for (size_t i = 0; i < src_len; i++)
    {
        // D = ((D << 1) | 1) & B[pattern[i] - 'A'];
        // D <<= 1;
        // D.set(0, 1);
        D = (D << 1).set(0,1) & B[source[i] - 'A'];
        // D &= B[source[i] - 'A'];
        // std::cout << "D:" << D << "B[pattern[i] - 'A']" << B[source[i] - 'A'] << std::endl;
        if(D[pat_len - 1]) 
        {
            std::cout << "Matched end at pos " << i << std::endl;
            return true;
        }
    }

    // return D[pat_len - 1];
    return false;
}

// implementation by pure bitwise calculation
bool shift_and_bitwise(std::string source, std::string pattern)
{
    size_t src_len = source.length(), pat_len = pattern.length();
    
}

int main()
{
    std::string src("ABCDABCCscABCASSW"), pat("ABCA");

    std::cout << shift_and_bitset(src, pat) << std::endl;

    return 0;
}