#ifndef PHP2STL_HEADER
#define PHP2STL_HEADER
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <dirent.h>
#ifdef MACOS
   #include <unistd.h>
#endif
#include <sys/stat.h>
#include "OCRTypes/OCRTypes.h"
#include "pugixml/pugixml.hpp"
#include "cgi/CgiParser.h"

using namespace std;
using namespace ocr;
using namespace pugi;


int strLen(string &input);
int strLenW(wstring &input);
wstring UTF_to_Unicode(string &input);
string Unicode_to_UTF( wstring &input);
string Unicode_to_UTF( wchar_t input);
string Unicode_to_UTF(string &input);

vector<string> explode(const string& delimiter, const string& input);
vector<wstring> explodeW(const wstring& delimiter, const wstring& input);
vector<wordOCR> explode(const vector<string>& delimiter, const string& input);
vector<stringOCR> explode(const vector<string>& delimiter, stringOCR& input);
string implode(const string& delimiter,  const vector<string>& input);
wstring implodeW(const wstring& delimiter,  const vector<wstring>& input);
string implode(const string& delimiter,  const vector<wordOCR>& input);

string substr(int inInd,int count, string &srcString );
string substr(int inInd,string &srcString );
string substrT(const char *buf, string &srcString);
string substrH(const char *buf, string &srcString);
void readText(char *buff, const char *path);
void readText(vector<string> &strVector, string &inFile);
string readText(ifstream &inFile);
string readText(const char *buf);
string readText(string &path);


void writeText(vector<string> &strVector, string &outFile);
void writeText(vector<string> &strVector, const char* dest);
void writeText(string &outStr, string &outFile);
void writeText(char *buff, unsigned int size, const char *path);
void writeText(string &outStr, const char *path);
void writeText(const char *data, string &outFile);
void writeToLog(string &outStr, const char *path, const char *flagFile);  ///few process can share log file. Used status of flag file for lock 
bool is_file(string &path);
bool fileOpen(string &path);

int bsearch(hashRecord *hashVector,unsigned  size,int value);  ///analog of c++ binary_search. Return index of element or -1 if not found 

void readMapXML(map<string,string>&inputMap,string &path);

void readFormData(commandData *inputData);
void readPostString(commandData *inputData, string &data);
string readInput(commandData *inputData, string &inputStr);

string str_replace(string &src, string &dest, string &srcString );
string str_replace(const char *srcChar, string &dest, string &srcString);
string str_replace(const char *srcChar, const char *destChar, string &srcString );
string str_replace(const char *srcChar, const char *destChar, const char *srcStringChar );
string str_replace(string &src, string &dest, string &srcString , int count);
string str_replace(const char *srcChar, const char *destChar, const char *srcStringChar, int count );
string str_replace(vector<string> &srcArray, string &dest, string &srcString );
string str_replace(vector<string> &srcArray, const char* destData, string &srcString );

string fileName(string &path);
string ptrToStr(const char*st,int ln);
string HTMLtoTxt(string &input);
string decodeURLString(string URLstr);
char translateHex(char hex);
unsigned int getID(string data);

string intToHex(int input);
string charToHex(unsigned char input);
unsigned int strToHash(const char *str, unsigned int len);
void convertFileToUTF8(commandData *inputData);
void convertDirectoryToUTF(commandData *inputData);
int readDirectoryToArray(vector<string>&fileList,string dirPath, string ext);
void readDirectory(vector<string>&fileList,vector<string>&folderList,string dirPath);
void readInputData(commandData* inputData, string ext);
/** @brief  main interface function. Read request from POST string and process data
*/
string processInput(commandData *inputData, string &dataStr);


// -----------------------------------------------------------------------------
/**
 * C++ version 0.4 std::string style "itoa":
 * Contributions from Stuart Lowe, Ray-Yuan Sheu,
 * Rodrigo de Salvo Braz, Luc Gallant, John Maloney
 * and Brian Hunt
 */
string itoa(int value, int base);
/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 */
#ifdef MACOS
char* itoa(int value, char* result, int base);
#endif

void binaryPrint(unsigned int input, int radix);
void memprint(const char* ptr, int size);

void libCurl(commandData *inputData);
size_t  write_data(void *ptr,
					 size_t size,
					 size_t nmemb,
					 FILE *stream);

void error(char *msg);
int indexById(vector<stringOCR>&correctionTable,unsigned int in);




#endif
