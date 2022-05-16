/**
 * This header and its source are authored by StackOverflow user LihO 
 * Link to original source: https://stackoverflow.com/a/13935718/6699069
 */
#ifndef _BASE64_H_
#define _BASE64_H_

#include <iostream>
#include <vector>
#include <string>
typedef unsigned char BYTE;

std::string base64_encode(BYTE const* buf, unsigned int bufLen);
std::vector<BYTE> base64_decode(std::string const&);

#endif
