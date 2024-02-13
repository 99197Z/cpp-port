#ifndef UTILLS_H
#define UTILLS_H

double ConvertPCTdegC(double percent);

template<typename ... Args>
std::string string_format( const std::string& format, Args ... args );
#endif