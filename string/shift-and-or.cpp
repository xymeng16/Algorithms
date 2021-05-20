#include <iostream>
#include <string>
#include <bitset>
#include "../util.h"
#include <cstring>

// implementation by bitset
int shift_and_bitset(std::string source, std::string pattern)
{
    size_t src_len = source.length(), pat_len = pattern.length();
    // const int matched_mark = 2 ^ (pattern - 1);
    // assume character set is A-Za-z, 52 chars
    std::bitset<64> B[52]; // pat_len should less than or equal to 64
    std::bitset<64> D;     // the bitset representing the prefix-matching status, initialized as 0x00...

    // initialization of B
    for (size_t i = 0; i < pat_len; i++)
    {
        // set the i-bit of B[pattern[i]] as 1
        B[pattern[i] - 'A'].set(i, true);
    }

    for (size_t i = 0; i < src_len; i++)
    {
        D = (D << 1).set(0, 1) & B[source[i] - 'A'];
        // std::cout << "D:" << D << "B[pattern[i] - 'A']" << B[source[i] - 'A'] << std::endl;
        if (D[pat_len - 1])
        {
            // std::cout << "Matched end at pos " << i << std::endl;
            return i;
        }
    }

    // return D[pat_len - 1];
    return -1;
}

// implementation by pure bitwise calculation
int shift_and_bitwise(std::string source, std::string pattern)
{
    size_t src_len = source.length(), pat_len = pattern.length();
    unsigned long long mask = 1 << (pat_len - 1);
    unsigned long long B[52] = {0};
    unsigned long long D = 0;

    // initialization of B
    for (size_t i = 0; i < pat_len; i++)
    {
        // set the i-bit of B[pattern[i]] as 1
        B[pattern[i] - 'A'] |= (1 << i);
    }

    for (size_t i = 0; i < src_len; i++)
    {
        D = (((D << 1)) | 1) & B[source[i] - 'A'];
        if (D & (1 << (pat_len - 1)))
        {
            // std::cout << "Matched end at pos " << i << std::endl;
            return i;
        }
    }

    return -1;
}

// an improvement of shift-and, using 0 to indicate a match and 1 to indicate an unmatch
// hence, 1 and 0 in every bitvectors should be swapped, and hence the (OR 0x01) operation is no more required
int shift_or_bitwise(std::string source, std::string pattern)
{
    size_t src_len = source.length(), pat_len = pattern.length();
    unsigned long long mask = ~(1 << (pat_len - 1));
    unsigned long long B[52];

    memset(B, ~0, sizeof(unsigned long long) * 52);
    unsigned long long D = ~0; // 111..111

    // initialization of B
    for (size_t i = 0; i < pat_len; i++)
    {
        // set the i-bit of B[pattern[i]] as 0
        B[pattern[i] - 'A'] &= ~(1 << i);
    }

    for (size_t i = 0; i < src_len; i++)
    {
        D = (((D << 1))) | B[source[i] - 'A'];
        if (/* ~((D >> (pat_len - 1)) & 1) & 1 */!((D >> (pat_len - 1)) & 1))
        {
            // std::cout << "Matched end at pos " << i << std::endl;
            return i;
        }
    }

    return -1;
}

// shift-or implementation by bitset
int shift_or_bitset(std::string source, std::string pattern)
{
    size_t src_len = source.length(), pat_len = pattern.length();
    // const int matched_mark = 2 ^ (pattern - 1);
    // assume character set is A-Za-z, 52 chars
    std::bitset<64> B[52];                                  // pat_len should less than or equal to 64
    std::bitset<64> D = std::move(std::bitset<64>{}.set()); // the bitset representing the prefix-matching status, initialized as 0x00...

    for (size_t i = 0; i < 52; i++)
    {
        B[i].set(); // set all to 1
    }

    // initialization of B
    for (size_t i = 0; i < pat_len; i++)
    {
        // set the i-bit of B[pattern[i]] as 1
        B[pattern[i] - 'A'].set(i, false);
    }

    for (size_t i = 0; i < src_len; i++)
    {
        D = (D << 1) | B[source[i] - 'A'];
        // std::cout << "D:" << D << "B[pattern[i] - 'A']" << B[source[i] - 'A'] << std::endl;
        if (~D[pat_len - 1])
        {
            // std::cout << "Matched end at pos " << i << std::endl;
            return i;
        }
    }

    // return D[pat_len - 1];
    return -1;
}

int main()
{
    std::string src("ABCCXBCAABCCXBCAABCCXBCPOQUUWAABCCXBCAABCCPOQUUWNNXKSHSLSNXBCAABCCXBCAABCCXBCAABCCPOQUUWNNXKSHSLSNXBCAABCCCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXPOQUUWBCAABCCXBCAABCCCCXBCAABCCXBCAABPOQUUWNNXKSHSLSNJNFPOQUUCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCCCXBCAABCCPOQUUWNNXKSHSLSNJNFPOQUUXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAAPOQUUWBCCCCXBCAABCCPOQUUWXBCAABCCXBCAABCPOQUUWNNXKSHSLSNJNFPOQUUCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAABPOQUUWNNXKSHSLSNJNFPOQUUCCXBCAABCCXBCAABCCXBCAABCCCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAABCCXBCAPOQUUWNNXKSHSLSNJNFPOQUUWNNXKSHSLSNJNFPOQUUWNNXKSHSLSNJNFJDKLABCABCA"), pat("POQUUWNNXKSHSLSNJNFPOQUUWNNX");

    const unsigned int test_num = 100;
    
    // std::cout << "bitset implementation: " << shift_and_bitset(src, pat) << std::endl;
    // std::cout << "bitwise implementation: " << shift_and_bitwise(src, pat) << std::endl;

    // std::cout << "bitset shift-or implementation " << shift_or_bitset(src, pat) << std::endl;
    // std::cout << "bitwise shift-or implementation " << shift_or_bitwise(src, pat) << std::endl;

    std::cout << "Scale of chrono:" << (double)std::chrono::high_resolution_clock::period::num / std::chrono::high_resolution_clock::period::den << std::endl; // Results in a nanosecond.

    std::cout << "bitset shift-and implementation time: " << exec_time(shift_and_bitset, src, pat) << std::endl;
    std::cout << "bitwise shift-and implementation time: " << exec_time(shift_and_bitwise, src, pat) << std::endl;

    std::cout << "bitset shift-or implementation time: " << exec_time(shift_or_bitset, src, pat) << std::endl;
    std::cout << "bitwise shift-or implementation time: " << exec_time(shift_or_bitwise, src, pat) << std::endl;

    return 0;
}