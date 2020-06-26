#ifndef MYCONVERSIONS_H_INCLUDED
#define MYCONVERSIONS_H_INCLUDED

#include <sstream>

template<class T>
inline std::string float2str(const T& n, const int& no_digits){
    std::ostringstream convert;
    convert.flags(std::ios_base::fmtflags::_S_scientific | std::ios_base::fmtflags::_S_showpos);
    convert.precision(no_digits);
    convert << n;
    return convert.str();
}
template<class T>
inline long double str2float(std::string str){
    T ret;
    std::istringstream convert(str);
    //convert.flags();
    convert >> ret;
    return ret;
}
template<class T>
inline std::string mysprintf(const T& n, const std::string& str){
    char retBuffer[50];
    sprintf(retBuffer, str.c_str(), n);
    return std::string(retBuffer);
}

#endif // MYCONVERSIONS_H_INCLUDED
