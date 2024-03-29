#include "OCRString/php2stl.h"
int strLen(string &input){return input.size();}
int strLenW(wstring &input){return input.size();}

unsigned int strToHash(const char *str, unsigned int len){
	unsigned int hash = 0;
	unsigned int i = 0;
	
	for (i = 0; i < len; str++, i++) {
		hash = (hash * 1664525) + (unsigned char)(*str) + 1013904223;
	}
	return hash;
}



wstring UTF_to_Unicode(string &input) {
        if(input.size()==0)return L"";
//cout<<"input="<<input<<endl;
	unsigned char ints;
	int bits=0;
	string val;
	wstring value;
	//val   = array();
	unsigned char z,y,x,w,v,u,c,b;
	int i;
	int len=strLen(input);
	int bit1,bit2,bit3,bit4,bit5,bit6;


	for(i=0; i<len; i++){
		c=input[i]; //cout<<"c="<<(short)c<<endl;
		if(c > 128){
			if((c >= 254||c < 192)){value=(wchar_t*)input.c_str(); return value;}
			if(c >= 192) bits=2;
			if(c >= 224) bits=3;
			if(c >= 240) bits=4;
			if(c >= 248) bits=5;
			if(c >= 252) bits=6;
			if((i+bits) > len) {value=(wchar_t*)input.c_str(); return value;}
			//cout<<"i="<<i<<" bits="<<bits<<" len="<<len<<endl;

			while(bits > 1){
				i++;
				b=input[i];
				if(b < 128 || b > 191){value=(wchar_t*)input.c_str(); return value;}
				bits--;
			}
		}
	}

	//cout<<"input1="<<input<<endl;


 	//if(!is_utf8(input)return input;

	//bit1  = pow(64, 0);   //php code
	//bit2  = pow(64, 1);
	//bit3  = pow(64, 2);
	//bit4  = pow(64, 3);
	//bit5  = pow(64, 4);
	//bit6  = pow(64, 5);

	bit1 = 1;
	bit2 = 64;
	bit3 = 4096;
	bit4 = 262144;
	bit5 = 16777216;
	bit6 = 1073741824;
    //char buff[33];

	for(i=0; i< strLen( input ); i++){

		ints = (unsigned char)input[i];

		z     = (unsigned char) input[i];
		y     = (unsigned char) input[i+1] - 128;
		x     = (unsigned char) input[i+2] - 128;
		w     = (unsigned char) input[i+3] - 128;
		v     = (unsigned char) input[i+4] - 128;
		u     = (unsigned char) input[i+5]  - 128;

		if( ints <= 127 ){
			// 1 bit
			//sprintf(buff,"%d",(z * bit1));
			//val += "&#";
			//val+=buff;
			//val+=";";
			value+=(wchar_t)(z*bit1);
			//val[]  = value;
		}

		if( ints >= 192 && ints <= 223 ){
			// 2 bit
			//sprintf(buff,"%d",((z-192) * bit2 + y * bit1));
			//val += "&#";
			//val+=buff;
			//val+=";";
		    value += (wchar_t)((z-192) * bit2 + y * bit1);
			//val[]  = value;
		}

		if( ints >= 224 && ints <= 239 ){
			// 3 bit
			//sprintf(buff,"%d",((z-224) * bit3 + y * bit2 + x * bit1));
			//val += "&#";
			//val+=buff;
			//val+=";";
			value += (wchar_t)((z-224) * bit3 + y * bit2 + x * bit1);
			//val[]  = value;
		}
		if( ints >= 240 && ints <= 247 ){
			// 4 bit
			//sprintf(buff,"%d",((z-240) * bit4 + y * bit3 + x * bit2 + w * bit1));
			//val += "&#";
			//val+=buff;
			//val+=";";
			value+=(wchar_t)((z-240) * bit4 + y * bit3 + x * bit2 + w * bit1);
			//val[]  = value;
		}
		if( ints >= 248 && ints <= 251 ){
			// 5 bit

			//sprintf(buff,"%d",((z-248) * bit5 + y * bit4 + x * bit3 + w * bit2 + v * bit1));
			//val += "&#";
			//val+=buff;
			//val+=";";
			value+=(wchar_t)((z-248) * bit5 + y * bit4 + x * bit3 + w * bit2 + v * bit1);
			//val[]  = value;
		}
		if( ints == 252 && ints == 253 ){
			// 6 bit
			//sprintf(buff,"%d",((z-252) * bit6 + y * bit5 + x * bit4 + w * bit3 + v * bit2 + u * bit1));
			//val += "&#";
			//val+=buff;
			//val+=";";
			value+=(wchar_t)((z-252) * bit6 + y * bit5 + x * bit4 + w * bit3 + v * bit2 + u * bit1);
			//val[]  = value;
		}
		if( ints == 254 || ints == 255 ){
			cout<<"Wrong Result!"<<endl;
		}


	}

	//cout << "val="<<val<<endl;

	//if( array === False ){
	return value;
	//}
	//if(array === True ){
	//	val     = str_replace('&#', '', value);
	//	val     = explode(';', val);
	//	len = count(val);
	//	unset(val[len-1]);

	//	return unicode = val;
	//}

}//____________________________________________________________________


string Unicode_to_UTF( wstring &input){

	int c=0, b=0, flag=0, i;
	int bits=0;
	int len=strLenW(input); //cout<<"len="<<len<<END;
	if(!len)return "";
	int byte1,byte2,byte3,byte4,byte5,byte6;
	string utf="";


	for(i=0; i<len; i++){
		c=input[i];
		// test BOM and remove it
	   //	if(i==0&&c==0xEF)continue;
	   //	if(i==1&&c==0xBB)continue;
	   //	if(i==2&&c==0xBF)continue;
		//

		utf+=(char)c;  //cout<<"c="<<c<<" ="<<(char)input[i]<<END;
		if(c > 128){
			if(c >= 252) bits=6;
			if(c >= 248) bits=5;
			if(c >= 240) bits=4;
			if(c >= 224) bits=3;
			if(c >= 192) bits=2;
			flag=1; i=len;
		}
		if((i+bits) > len) {flag=1; i=len;}//need convert
		while(bits > 1){
			i++;
			b=input[i];
			if(b < 128 || b > 191) {flag=1; i=len;}//need convert
			bits--;
		}
	}
	//cout<<"flag="<<flag<<END;
	if(!flag==1)return utf;
	utf = "";
	//if(!is_array(input)){
	//input     = str_replace('&#', '', input);
	//input     = explode(';', input);
	//len = count(input);
	//unset(input[len-1]);
	//for(i=0; i < len; i++)inputArray.push_back(input[i]);

	//}else{count=count(input);}

	for(i=0; i < len; i++){

	    // test BOM and remove it
	   //	if(i==0&&c==0xEF)continue;
	   //	if(i==1&&c==0xBB)continue;
	   //	if(i==2&&c==0xBF)continue;
		//print input[i];
		//print "__i<br>";
		if ( input[i] <128 ){
			byte1 = input[i];
			utf  += (char)byte1;
		}

			//__set_win_1251 charset___
			if(input[i]>191&&input[i]<256)input[i]+=848;
			if(input[i]==184)input[i]=0x0451;
			if(input[i]==168)input[i]=0x0401;


		if ( input[i] >=128 && input[i] <=2047 ){
			byte1 = 192 + (int)(input[i] / 64);
			byte2 = 128 + (input[i] % 64);
			utf  += (char)byte1;
			utf  += (char)byte2;



		}
		if ( input[i] >=2048 && input[i] <=65535){
			byte1 = 224 + (int)(input[i] / 4096);
			byte2 = 128 + ((int)(input[i] / 64) % 64);
			byte3 = 128 + (input[i] % 64);

			utf  += (char)byte1;
			utf  += (char)byte2;
			utf  += (char)byte3;
			//print utf;
			//print "___<br>";
		}
		if ( input[i] >=65536 && input[i] <=2097151){

			byte1 = 240 + (int)(input[i] / 262144);
			byte2 = 128 + ((int)(input[i] / 4096) % 64);
			byte3 = 128 + ((int)(input[i] / 64) % 64);
			byte4 = 128 + (input[i] % 64);
			utf  += (char)byte1;
			utf  += (char)byte2;
			utf  += (char)byte3;
			utf  += (char)byte4;
		}
		if ( input[i] >=2097152 && input[i] <=67108863){

			byte1 = 248 + (int)(input[i] / 16777216);
			byte2 = 128 + ((int)(input[i] / 262144) % 64);
			byte3 = 128 + ((int)(input[i] / 4096) % 64);
			byte4 = 128 + ((int)(input[i] / 64) % 64);
			byte5 = 128 + (input[i] % 64);
			utf  += (char)byte1;
			utf  += (char)byte2;
			utf  += (char)byte3;
			utf  += (char)byte4;
			utf  += (char)byte5;
		}
		if ( input[i] >=67108864 && input[i] <=2147483647){

			byte1 = 252 + (input[i] / 1073741824);
			byte2 = 128 + ((input[i] / 16777216) % 64);
			byte3 = 128 + ((input[i] / 262144) % 64);
			byte4 = 128 + ((input[i] / 4096) % 64);
			byte5 = 128 + ((input[i] / 64) % 64);
			byte6 = 128 + (input[i] % 64);
			utf  += (char)byte1;
			utf  += (char)byte2;
			utf  += (char)byte3;
			utf  += (char)byte4;
			utf  += (char)byte5;
			utf  += (char)byte6;
		}
	}

	//cout<<"utf="<<utf<<END;
	return utf;
}//_____________________________________________________________________


string Unicode_to_UTF( wchar_t input){
wstring in=L"";
in+=input;
return Unicode_to_UTF(in);

}//_____________________________________________________________________

string Unicode_to_UTF(string &input){
	
	wstring strW;
	for(int i=0;i<input.size();i++){
	strW+=(wchar_t)(unsigned char)input[i];
		//cout<<input[i]<<"="<<(short)strW[i]<<END;	
	}
	//cout<<"strW.size()="<<strW.size();
	return Unicode_to_UTF(strW);
	
}//_____________________________________________________________________

string substr(int inInd,int count, string &srcString ){
return srcString.substr(inInd,count);
}

string substr(int inInd,string &srcString ){
return srcString.substr(inInd,srcString.size()-inInd);
}

string substrT(const char *buf, string &srcString){
return srcString.substr(srcString.rfind(buf),srcString.size()-srcString.rfind(buf));
}

string substrH(const char *buf, string &srcString){
return srcString.substr(0,srcString.rfind(buf));
}//_____________________________________________________________________________


vector<string> explode(const string& delimiter,
					   const string& input){

	vector<string> result;
	int start;
	string data=input;
	start=1;
	while ( start != -1){
		start = data.find(delimiter); 
		if(start>-1){
		  result.push_back(substr(0,start,data));
		  data=substr(start+delimiter.size(),data);
		}else{
		  if(data.size())result.push_back(data);
		}
	}
    return result;
}//_____________________________________________________________________________

vector<wstring> explodeW(const wstring& delimiter,
						 const wstring& input){

	vector<wstring> result;
	int n = input.length();
	int start, stop;
	// найти первую букву, не разделитель
	start = input.find_first_not_of(delimiter);
	while ( (start >= 0) && (start < n) ){
		// найти конец текущего слова
		stop = input.find_first_of(delimiter, start);
		if ( (stop < 0) || (stop > n) ) stop = n;
		// добавить слово к списку слов
		result.push_back(input.substr(start, stop-start));
		// найти начало следующего слова
		start = input.find_first_not_of(delimiter,stop+1);
	}
    return result;
}//_____________________________________________________________________________


vector<wordOCR> explode(const vector<string>& delimiter, const string& input){
	
	vector<wordOCR> result;
	int n = input.length();
	int start, stop, minStop,dlt;
	string delimeter;
	start = 0;
	int count =delimiter.size();
	
	
	while (start <n){
		wordOCR strItem;
		minStop=1000; dlt=start;
		for(int i=0;i<count;i++){
			    stop = input.find(delimiter[i], start);
			    //cout<<" stop="<<stop<<" delimiter[i]="<<delimiter[i] <<END;
				if(stop<minStop&&stop>-1){minStop=stop; strItem.delimeter=delimiter[i];}
			   
		} 
		//start-=1;
		//if(start<0)start=0;	
		//cout<<" start="<<start<<" minStop="<<minStop<<END;
	   strItem.name=input.substr(start,minStop-start);	
		if(strItem.name=="")strItem.name=strItem.delimeter;
	    //cout<<"n="<<strItem.name<<"/ d="<< strItem.delimeter<<" start="<<start<<" minStop="<<minStop<<" s="<<strItem.delimeter.size()<<END;
	   start=minStop+strItem.delimeter.size();
	   result.push_back(strItem);
	}
    return result;
	

}


vector<stringOCR> explode(const vector<string>& delimiter, stringOCR &input){
	
	vector<stringOCR> result;
	int n = input.length();
	int start, stop, minStop,dlt;
	string delimeter_;
	start = 0;
	int count =delimiter.size();
	
	//cout<<"input.str=/"<<input.str<<"/"<<endl;
	
	while (start <n){
		stringOCR strItem;
		minStop=1000; dlt=start;
		for(int i=0;i<count;i++){
			stop = input.find(delimiter[i], start);
			//cout<<" stop="<<stop<<" delimiter[i]="<<delimiter[i] <<END;
			if(stop<minStop&&stop>-1){minStop=stop; delimeter_=delimiter[i];}
			
		} 
		//start-=1;
		//if(start<0)start=0;	
		strItem=input.substr(start,minStop-start);	//cout<<"strItem.str=/"<<strItem.str<<"/"<<endl;
		strItem.delimeter=delimeter_;
		//cout<<" start="<<start<<" minStop="<<minStop<<"strItem="<<strItem.str<<" strItem.size()="<<strItem.size()<<END; 
		if(strItem.str=="")strItem.str=strItem.delimeter;
	    //cout<<"n="<<strItem.str<<"/ d="<< strItem.delimeter<<" start="<<start<<" minStop="<<minStop<<" s="<<strItem.delimeter.size()<<END;
		start=minStop+1;
		if(strItem.str!=" ")result.push_back(strItem);
	}
    return result;
	
	
}//___________________________________________________________________________



string implode(const string& delimiter,
			   const vector<string>& input){
	string result;
	unsigned int i;
	for(i=0;i<input.size();i++){
		result+=input[i];
		result+=delimiter;
	}

	return result;
}//_____________________________________________________________________________

wstring implodeW(const wstring& delimiter,
				 const vector<wstring>& input){
	wstring result;
	unsigned int i;
	for(i=0;i<input.size();i++){
		result+=input[i];
		result+=delimiter;
	}
	return result;
}//_____________________________________________________________________________

string implode(const string& delimiter,  const vector<wordOCR>& input){
string result;
unsigned int i;
for(i=0;i<input.size();i++){
	result+=input[i].name;
	result+=delimiter;
}
return result;
}//_____________________________________________________________________________

//fast file reading in memory
void readText(char *buff, const char *path){
	FILE * hFile;
	hFile = fopen( path, "r");
	if (hFile == NULL){
	return; 
	}else{
		fseek(hFile, 0, SEEK_END);
		unsigned long  size= ftell(hFile);   //cout<<" size="<<size<<endl;
		fseek(hFile, 0, SEEK_SET);
		buff = (char *)malloc(size+1);
		if (size != fread(buff, sizeof(char), size, hFile)){
			free(buff);
			return;
		}
		fclose(hFile);
	}
	//cout<<"done read"<<endl;
}

//fast file reading in memory
string readText(const char *path){

/*	ifstream srcInput;
	srcInput.open(path);
	string str,srcLine;

	while(getline(srcInput, srcLine,'\n')){
		str+=srcLine+"\n";
	}
	srcInput.close();
	return str;
*/


	string str;
	//cout<<"start read "<<path;

	FILE * hFile;
	hFile = fopen( path, "rb");
	char *buff;
	unsigned long  size;
	if (hFile == NULL){
		cout<<"no open file "<<path;
		return "";
	}else{
		fseek(hFile, 0, SEEK_END);
		size= ftell(hFile);   //cout<<" size="<<size<<endl;
		fseek(hFile, 0, SEEK_SET);
		buff = (char *)malloc(size+1);
		unsigned long readDataSize=fread(buff, sizeof(char), size, hFile);
		if (readDataSize != size){
			free(buff);
			cout<<"no read file "<<path<<" readDataSize="<<readDataSize<<endl;
			return "";
		}
		fclose(hFile);
	}
	//cout<<"done read size="<<size<<endl;
	str.resize(size);
	memcpy(&str[0],buff,size);
	free(buff);
	return str;


}




//file reading in vector
void readText(vector<string> &strVector, string &inFile){
	ifstream srcInput;
	srcInput.open(inFile.c_str());
	string str,srcLine;

	while(getline(srcInput, srcLine,'\n')){  //cout<<"s="<<srcLine<<endl;
		strVector.push_back(srcLine);
	}
	srcInput.close();

}

string readText(string &inFile){
return   readText(inFile.c_str());
}

void readMapXML(map<string,string>&inputMap,string &path){

	xml_node col,data,resultSet,letterBaseXML, st;
	xml_document doc;
	string str0,str1;
	if(!doc.load_file(path.c_str())){
		cout<<path<<" not loaded"<<END;return;
	}
	
	resultSet = doc.child("letterBase");
	// Iterate through books
	for (xml_node row = resultSet.child("rec"); row; row = row.next_sibling("rec")){			
		str0=row.attribute("k").value();	
		str1=row.attribute("v").value();
		//cout<<"str1="<<str1<<" str0="<<str0<<END;
		inputMap[str1]=str0;		
	}	
		
}//_____________________________________________________________________________

void  writeText(char *buff, unsigned int size, const char *path){
	FILE * hFile;
	hFile = fopen( path, "w");
	if (hFile == NULL){
		return; 
	}else{
		fwrite(buff, sizeof(char), size,hFile);
	}
	fclose(hFile);
}//_____________________________________________________________________________________

//file write from vector
void writeText(vector<string> &strVector, string &outFile){
	ofstream srcOutput;
	srcOutput.open(outFile.c_str());
	for(int i=0;i<strVector.size();i++){
		srcOutput<<strVector[i]<<endl;
	}
	srcOutput.close();	
}

//file write from vector
void writeText(vector<string> &strVector, const char* dest){
	string outFile=dest;
	writeText(strVector,outFile);

}

void writeText(string &outStr, string &outFile){
	FILE * hFile;
	hFile = fopen( outFile.c_str(), "wb");
	if (hFile == NULL){
		cout<<outFile<<" not open"<<END;
		return;
	}else{
		fwrite(outStr.c_str(), sizeof(char), outStr.size(),hFile);
	}
	fclose(hFile);

}

void writeText(string &outStr, const char *path){
	string outFile=path;
    writeText(outStr,outFile);
}

void writeText(const char *data, string &outFile){
	string outStr=data;
    writeText(outStr,outFile);
}

void writeToLog(string &outStr, const char *path,const char *flagFile){

	string str="1";
	while(str!=""){
     	str=readText(flagFile);
		cout<<"WAIT IN LINE"<<endl;
#ifdef MACOS
		sleep(1);
#else
		Sleep(1000);
#endif
	}
	str="1";
	writeText(str,flagFile);
	
	//FILE * hFile;
	//hFile = fopen( path, "a");
	//fwrite(outStr.c_str(), sizeof(char), outStr.size(), hFile);
	//fclose(hFile);

	ofstream srcOutput;
	srcOutput.open(path,fstream::out|fstream::app);
	if(!srcOutput){cout<<path<<" not open"<<END; exit(0);}
	srcOutput<<outStr;
	srcOutput.close();	
	
	str="";
	writeText(str,flagFile);
}

bool is_file(string &path){
	///cout<<"start check "<<path;
    if(!path.size())return 0;
	FILE * hFile;
	hFile = fopen( path.c_str(), "r");
    if(!hFile)return 0;
	fseek(hFile, 0, SEEK_END);
	unsigned long  size= ftell(hFile);   //cout<<" size="<<size<<endl;   
	if(size<4)return 0; 
	
	fclose(hFile);
	//cout<<"_exist";
	return 1;
	
}


bool fileOpen(string &path){
	cout<<"start check "<<path;
	
	FILE * hFile;
	hFile = fopen( path.c_str(), "r");
	char buff[5];
	string str;
	unsigned int size;
	if (hFile == NULL){
		return 1; 
	}else{
		fseek(hFile, 0, SEEK_END);
		unsigned long  size= ftell(hFile);   cout<<" size="<<size<<endl;   
		if(size<4)return 0; 
		
		fseek(hFile, size-4, SEEK_SET);
		fread(buff, sizeof(char), 4, hFile);
		fclose(hFile);
	}
	buff[4]=0;
		cout<<"done read "<<buff<<endl;
	str=buff;
	if(str=="<r/>"){
		fseek(hFile, size-4, SEEK_SET);
		memset(buff,0,4);
		fwrite(buff, sizeof(char), 4, hFile);
		return 1;
	}	
	return 0;
}


void readFormData(commandData *inputData){
#ifdef MACOS
		cout<<getenv("CONTENT_LENGTH")<<endl<<getenv("CONTENT_TYPE")<<"<br>"<<endl;
		int cl = atoi(getenv("CONTENT_LENGTH"));
		char *REQUEST_METHOD=getenv("REQUEST_METHOD");
		char *CONTENT_LENGTH=getenv("CONTENT_LENGTH");
		Post p; // Get the query string
		CGImap query(p);
	      //cout<<"inputData->data[\"inputPath\"]="<<inputData->data["inputPath"]<<END;
		  OUT_S(inputData->data["inputPath"]);


		  for(CGImap::iterator it = query.begin();
			it != query.end(); it++) {
			out <<"<"<<(*it).first<<">"<<(*it).second<<"</"<<(*it).first<<">"<<endl;
		  }
			out.close();
	        string str="";
		    readInput(inputData,str);
#endif

}//_____________________________________________________________________________

void readPostString(commandData *inputData, string &data){
	CGImap query(data);
	//cout<<"inputData->data[\"inputPath\"]="<<inputData->data["inputPath"]<<END;
	//OUT_S(inputData->data["inputPath"]);
	for(CGImap::iterator it = query.begin();
		it != query.end(); it++) {
		//out <<"<"<<(*it).first<<">"<<(*it).second<<"</"<<(*it).first<<">"<<endl;
		inputData->data[(*it).first]=(*it).second;
		cout<<"//@@@___________<"<<(*it).first<<">"<<(*it).second<<"</"<<(*it).first<<">"<<endl;
	}
	//out.close();
	string str="";
}//_____________________________________________________________________________


struct data_traverser: public xml_tree_walker{commandData *inputData;virtual bool for_each(xml_node& n)	{
		static string name;
		//for (int i = 0; i < depth(); ++i) cout << "  "; // indentation

		if (n.type() == node_element){
		   //cout <<"name="<<n.name() << END;
		   name=n.name();
		}else{
		   inputData->data[name]=n.value();
			//cout<<"inputData->data["<<name<<"]="<< inputData->data[name]<<endl;
		    //cout <<"name="<<name<<" value="<<n.value() << END;
		}
		return true; // continue traversal
	}

};
/*

// You can also traverse the whole tree (or a subtree)

//data_traverser t;
//doc.traverse(t);
//or   doc.first_child().traverse(t);
*/


string readInput(commandData *inputData, string &inputStr){
xml_document doc,strDoc;
xml_node mainNode;
xml_node book, data, key;
string str,srcLine,str_;
char *dataString;
inputData->start=0;
	
	
//сначала нужно сохранить данные которые уже были в inputData. по неизветстной причине 
// новые данные удается внести только если в inputData стерты прежние данные;
	
	string path1=inputData->data["tablePath"];
	string path2=inputData->data["uniTablePath"];
	string path3=inputData->data["wordsDataPath"];
	string path4=inputData->data["inputPath"];
	string path5=inputData->data["statPath"];
	string path6=inputData->data["OCRBasePath"];
	string path7=inputData->data["dictPath"];
	string path8=inputData->data["correctionTable"];
	string path9=inputData->data["root"];
	
	inputData->data.erase(inputData->data.begin(), inputData->data.end());   //??not overwrite inputData->data["ocrData"]="";

	inputData->data["tablePath"]=path1;
	inputData->data["uniTablePath"]=path2;
	inputData->data["wordsDataPath"]=path3;
	inputData->data["inputPath"]=path4;
	inputData->data["statPath"]=path5;
	inputData->data["OCRBasePath"]=path6;
	inputData->data["dictPath"]=path7;  //глупо, но пока не нашлось другого решения
    inputData->data["correctionTable"]=path8;
	inputData->data["root"]=path9;
	
	
	
if(inputStr.size()){
    dataString=(char*)malloc(inputStr.size()+1); 
	memcpy(dataString, inputStr.c_str(),inputStr.size() ); dataString[inputStr.size()]=0;
	doc.parse(dataString);
	data_traverser t;
	t.inputData=inputData;
	doc.traverse(t);
	free(dataString);
	
}else{
	 if(doc.load_file(inputData->data["inputPath"].c_str())){
		 data_traverser t;
		 t.inputData=inputData;
		 doc.traverse(t);
	 }else{
		 cout<<inputData->data["inputPath"]<<" not loaded"<<END;
	 }	 
}	
    cout<<"read inputData->data[fileList]="<<inputData->data["fileList"]<<endl;

	
	if(inputData->data["inputFolder"]!="")inputData->data["inputFolder"]=str_replace("\\","/",inputData->data["inputFolder"]);
	if(inputData->data["fileList"]!="")inputData->data["fileList"]=str_replace("\\","/",inputData->data["fileList"]);
	if(inputData->data["fileList"]!="")inputData->data["fileList"]=str_replace("/ "," ",inputData->data["fileList"]);

	if(inputData->data["fileList"]!=""){
		inputData->fileList=explode(":|:",inputData->data["fileList"]);
//#ifdef MACOS
//		if(inputData->data["InputMethod"]=="fileList"){
//			for(int i=0;i<inputData->fileList.size();i++){
//				if(inputData->fileList[i][0]!='/')inputData->fileList[i]=inputData->data["inputFolder"]+"/"+inputData->fileList[i];		
//				   //cout<<"read inputData->fileList[i]"<<inputData->fileList[i]<<END;
//			}
//		}
//#endif	
		inputData->data["inputFile"]=inputData->fileList[0];	
	
	}
	
	//cout<<" read inputData->data[inputFile]="<<inputData->data["inputFile"]<<endl;
	
	if(inputData->data["outFolder"]=="")inputData->data["outFolder"]=inputData->data["inputFolder"];	
	if(inputData->data["ocrData"]=="tib"||inputData->data["ocrData"]=="ocrBatch"){
		inputData->data["OCRTables"]=inputData->data["inputFolder"]+"/_OCRTables";
#ifdef MACOS
		mkdir (inputData->data["OCRTables"].c_str(), 777);
#endif
	}
	
	if(inputData->data["setBase"]!=""){
	   inputData->data["setBase"]=str_replace("{","<",inputData->data["setBase"]);
	   inputData->data["setBase"]=str_replace("}",">",inputData->data["setBase"]);
	}
	if(inputData->data["setLetter"]!=""){
	   inputData->data["setLetter"]=str_replace("{","<",inputData->data["setLetter"]);
	   inputData->data["setLetter"]=str_replace("}",">",inputData->data["setLetter"]);
	}

	if(inputData->data["drawCorrelation"]!=""){
						  str_=inputData->data["drawCorrelation"];
						  str_=str_replace("{","<",str_);
						  str_=str_replace("}",">",str_);
						  char *cbuf=(char*)malloc(str_.size()+1);
						  memcpy(cbuf,str_.c_str(),str_.size());
						  cbuf[str_.size()]=0;
						  if(!strDoc.parse(cbuf)){DS("srcStr not loaded"<<END);return "";}
						  data = strDoc.child("glyph");
						  inputData->data["srcStr"]=str_;
						  inputData->x0=atoi(data.attribute("x0").value());
						  inputData->x1=atoi(data.attribute("x1").value());
						  inputData->y0=atoi(data.attribute("y0").value());
						  inputData->y1=atoi(data.attribute("y1").value());
						  inputData->idNumber=atoi(data.attribute("id").value());
						  inputData->data["name"]=data.attribute("n").value();
						  inputData->data["draw"]="drawLetter";
	}


return str;

}//_____________________________________________________________________________

string str_replace(const char *srcChar, const char *destChar, string &srcString ){
    //cout<<" srcString="<<srcString<<endl;
	string src=srcChar;
	string dest=destChar;
	if(src==dest)return srcString;
	string::size_type pos = 0;
	string result=srcString;
	int foundFlag=1;
	while(foundFlag){
		foundFlag=0;
		while ( (pos = result.find(src, pos)) != string::npos ) {
			result.erase(pos, src.size());
			result.insert(pos,dest);
			//cout<<"s="<<srcString<<endl;
			foundFlag=1;
			pos++;
		}
		pos=0;
	}	
	return result;

}//_____________________________________________________________________________

string str_replace(const char *srcChar, string &dest, string &srcString ){

	string src=srcChar;
	if(src==dest)return srcString;
	string::size_type pos = 0;
	int foundFlag=1;
	string result=srcString;
	while(foundFlag){
		foundFlag=0;
		while ( (pos = result.find(src, pos)) != string::npos ) {
			result.erase(pos, src.size());
			result.insert(pos,dest);
			foundFlag=1;
			pos++;
		}
		pos=0;
	}
	return result;

}//_____________________________________________________________________________


string str_replace(const char *srcChar, const char *destChar, const char *srcStringChar ){

	string src=srcChar;
	string dest=destChar;
	string srcString=srcStringChar;
	if(src==dest)return srcString;

	string::size_type pos = 0;
	int foundFlag=1;
	string result=srcString;
	while(foundFlag){
		foundFlag=0;
		while ( (pos = result.find(src, pos)) != string::npos ) {
			result.erase(pos, src.size());
			result.insert(pos,dest);
			foundFlag=1;
			pos++;
		}
		pos=0;
	}	
	return result;

}//_____________________________________________________________________________

string str_replace(string &src, string &dest, string &srcString , int count){
	if(src==dest)return srcString;
	string::size_type pos = 0;
	int index=0;
	//int foundFlag=1;
	string result=srcString;
	while ( (pos = result.find(src, pos)) != string::npos&&index<count ) {
		result.erase(pos, src.size());
		result.insert(pos,dest);
		pos++;index++;
	}
	return result;

}//_____________________________________________________________________________

string str_replace(const char *srcChar, const char *destChar, const char *srcStringChar, int count ){

	string src=srcChar;
	string dest=destChar;
	string srcString=srcStringChar;
	if(src==dest)return srcString;
	string::size_type pos = 0;
	int index=0;
	string result=srcString;
	while ( (pos = result.find(src, pos)) != string::npos&&index<count ) {
		result.erase(pos, src.size());
		result.insert(pos,dest);
		pos++;index++;
	}
	return result;

}//_____________________________________________________________________________


string str_replace(string &src, string &dest, string &srcString ){
	//cout <<"start src="<<src<<" dest="<<dest<<" srcString="<<srcString<<endl;
	if(src==dest)return srcString;
	string::size_type pos = 0;
	int foundFlag=1;
	string result=srcString;
	while(foundFlag){
		foundFlag=0;
		while ( (pos = result.find(src, pos)) != string::npos ) {
			result.erase(pos, src.size());
			result.insert(pos,dest);
			foundFlag=1;
			pos++;
		}
		pos=0;
	}
		//cout <<"done result="<<result<<endl;
	return result;
	
}//_____________________________________________________________________________


string str_replace(vector<string> &srcArray, const char* destData, string &srcString ){
	//cout <<"start"<<endl;
	string::size_type pos = 0;
	string dest=destData;
	int foundFlag;
	string result=srcString;
	
	for(int i=0;i<srcArray.size();i++){
		foundFlag=1;
		while(foundFlag){
			foundFlag=0;
			if(srcArray[i]==result)continue;
			while ( (pos = result.find(srcArray[i], pos)) != string::npos ) {
				result.erase(pos, srcArray[i].size());
				result.insert(pos,dest);
				foundFlag=1;
				pos++;
			}
			pos=0;
		}
	}	
	//cout <<"done"<<endl;
	return result;
	
}//_____________________________________________________________________________

string ptrToStr(const char*st,int ln){

	string str;
	str.resize(ln);
	memcpy(&str[0], st,ln);
    return str;
}


string fileName(string &path){
	string str=path;
	return str.erase(0,str.rfind("/")+1);
}

string intToHex(int input){
	string result;
	char buffer [10];

	sprintf (buffer, "%X", input);
	result=buffer;

	return result;
}
string charToHex(unsigned char input){
	string result;
	char buffer1 [3];
	char buffer2 [3];
	
	
	sprintf (buffer1, "%X", input);
	if(buffer1[1]==0){
		buffer2[0]='0';
		buffer2[1]=buffer1[0];
		buffer2[2]=0;
		result=buffer2;
	}else{
		result=buffer1;		
	}
	
	return result;
}

string HTMLtoTxt(string &input){
	string destStr;
	int flag=0;
	
	for(int a=0;a<input.size();a++){
		if(input[a]=='<')flag=1;
		if(input[a]=='>'){flag=0;continue;}
	    if(!flag)destStr+=input[a];
	}
	return destStr;
}



void convertFileToUTF8(const char* path){
/*	
	string srcLine;
	string destString;
	ifstream textData(path);
	if( !textData ){
		cout<<"Source path "<<textData<<" not exist"<<endl;
	}else{
		
		while (getline(textData, srcLine,'\n')){
			
			destString+=string_to_UTF(srcLine,"windows-1251")+"\n";
			
			
		}
		
	} textData.close();
	ofstream c_out;  c_out.open(path);
	c_out<<destString;
	c_out.close();
*/	
}//_____________________________________________________________________________


void convertDirectoryToUTF(commandData *inputData){
#ifdef PC
	char apath[1024];
	DIR *dir;
	struct dirent *entry;
	struct stat attribut;
	int result;

	if( ( dir=opendir(inputData->data["folderPath"]))!=NULL){

		while((entry=readdir(dir))!=NULL){

			memset(apath,'\0',sizeof(apath));
			strcpy(apath,path);
			strcat(apath,"/");
			strcat(apath,(char*)entry->d_name);

			if(strcmp((char*)entry->d_name,".")!= 0&&strcmp((char*)entry->d_name,"..")!= 0){
				printf("%s\n",apath);
			}
			if( stat( apath, &attribut ) == -1 )
				printf( "stat(%s) failed\n", apath );
			else{
				if((attribut.st_mode & S_IFDIR)&&strstr(apath,".")==NULL){
					convertDirectoryToUTF( apath );
				}else{
					char pch[4];
					strncpy (pch,apath+strlen(apath)-3,3);
					cout<<"pch=" <<pch<<endl;
					if(pch=="php"){
						
						cout<<"need convert"<<apath<<endl;
						convertFileToUTF8(apath);
						
					}
				}
			}
		}
	}
	else
		printf("opendir(%s) failed\n", path);

	closedir( dir );
#endif

}//_____________________________________________________________________________


void readInputData(commandData* inputData, string ext){

	if(inputData->data["InputMethod"]=="fileList"){
		if(!inputData->fileList.size()){
			readDirectoryToArray(inputData->fileList,inputData->data["inputFolder"],ext);
		}
	}else{
	    ofstream c_out; c_out.open("/_out_text.txt");
		for(int i=0;i<inputData->fileList.size();i++){
			c_out<<inputData->fileList[i]<<endl;
			//cout<<inputData->fileList[i]<<i<<endl;
		}
		c_out.flush();
		c_out.close();
	    inputData->fileList.resize(0);
		inputData->fileList.push_back("/_out_text.txt");
	}	
}


int readDirectoryToArray(vector<string>&fileList, string dirPath, string ext){

	DIR *dir;
	struct dirent *entry;
	struct stat attribut;
	//int result;
	string str;
	cout<<"dirPath="<<dirPath<<END;
	if(dirPath=="")return 0;
	string path=dirPath;
	
	vector<string>fileType;
	if(ext=="img"){
		fileType.push_back(".jpg");
		fileType.push_back(".JPG");
		fileType.push_back(".jpeg");
		fileType.push_back(".JPEG");
		fileType.push_back(".tif");
		fileType.push_back(".TIF");
		fileType.push_back(".tiff");
		fileType.push_back(".TIFF");
		fileType.push_back(".gif");
		fileType.push_back(".GIF");
		fileType.push_back(".png");
		fileType.push_back(".PNG");
	}else if(ext=="txt"){
		fileType.push_back(".txt");
		fileType.push_back(".TXT");
		fileType.push_back(".h");
		fileType.push_back(".cpp");
        fileType.push_back(".mm");
        fileType.push_back(".c");
	}else if(ext=="html"){
		fileType.push_back(".php");
		fileType.push_back(".PHP");
		fileType.push_back(".htm");
		fileType.push_back(".HTM");
		fileType.push_back(".html");
		fileType.push_back(".HTML");
		fileType.push_back(".phtml");
	}else if(ext=="rtf"){
	    fileType.push_back(".rtf");
		fileType.push_back(".doc");
		fileType.push_back(".RTF");
		fileType.push_back(".DOC");
	}else if(ext=="xml"){	
		fileType.push_back(".xml");
		fileType.push_back(".XML");
	}else {
		fileType.push_back(ext);
	}
	int fileFlag=0;
	
	if( ( dir=opendir(path.c_str()))!=NULL){
		while((entry=readdir(dir))!=NULL){
			//memset(apath,'\0',sizeof(apath));
			//strcpy(apath,path.c_str());
			//strcat(apath,"/");
			path=dirPath;
			path+="/";
			str=(char*)entry->d_name;
			if(str[0]=='.')continue;
			if(str.find("_mark")!=-1)continue;
			if(str.find("_press")!=-1)continue;
			path+=str;

			//cout<<"path="<<path<<END;
			
			
			if( stat( path.c_str(), &attribut ) == -1 ){
				//cout<<"stat "<<path <<" failed"<<END;
			}else{
				if((attribut.st_mode & S_IFDIR)){
					//cout<<"recurcion read "<<path<<END;
					fileFlag=readDirectoryToArray(fileList,path, ext);
				}else{
					int count=0;
					for(int i=0;i<fileType.size();i++){
						if(path.rfind(fileType[i])==path.size()-fileType[i].size()){
							count++;
							fileFlag=1;  
						    break;
						}
					}          
					if(count==0)continue;
					//cout<<path<<END;
				          fileList.push_back(path);				}
			}
		}
	closedir( dir );
	}else  printf("opendir(%s) failed\n", dirPath.c_str());
	cout<<" fileFlag="<<fileFlag;
	return fileFlag;
	
}//_____________________________________________________________________________


void readDirectory(vector<string>&fileList,vector<string>&folderList,string dirPath){
	
	DIR *dir;
	struct dirent *entry;
	struct stat attribut;
	//int result;
	string str;   dirPath=str_replace("%20", " ", dirPath);
	//cout<<"dirPath="<<dirPath<<END;
	if(dirPath=="")exit(0);
	string path=dirPath;
	return;
	if( ( dir=opendir(path.c_str()))!=NULL){
		while((entry=readdir(dir))!=NULL){
			//memset(apath,'\0',sizeof(apath));
			//strcpy(apath,path.c_str());
			//strcat(apath,"/");
			path=dirPath;
			path+="/";
			str=(char*)entry->d_name;
			if(str[0]=='.')continue;
			path+=str;
			//cout<<"path="<<path<<END;
			
			
			if( stat( path.c_str(), &attribut ) == -1 ){
				//cout<<"stat "<<path <<" failed"<<END;
			}else{
				if((attribut.st_mode & S_IFDIR)){
					//cout<<"recurcion read "<<path<<END;
					//readDirectory(fileList,path);
					path=str_replace("//", "/", path);
					folderList.push_back(path);
				}else{
					//cout<<path<<END;
				path=str_replace("//", "/", path);
				fileList.push_back(path);				}
			}
		}
	}else  printf("opendir(%s) failed\n", dirPath.c_str());
	
	closedir( dir );
	
	
}//_____________________________________________________________________________



size_t  write_data(void *ptr,
					 size_t size,
					 size_t nmemb,
					 FILE *stream){

	static int first_time=1;
	char outfilename[FILENAME_MAX] = "body.out";
	static FILE *outfile;
	size_t written;
	if (first_time) {
		first_time = 0;
		outfile = fopen(outfilename,"w");
		if (outfile == NULL) {
			return -1;
		}
		fprintf(stderr,"The body is <%s>\n",outfilename);
	}
	written = fwrite(ptr,size,nmemb,outfile);
	return written;

}//_____________________________________________________________________________________


		//char cbuf[12];
		//for(int in=5744;in<70000;in++){
		//string str="http://www.buddhistdoor.com/glossary/word_eng.php?word_id=";
		//sprintf(cbuf,"%d",in);
		//str+=cbuf;
		//cout<<"yagpo_dict -curl "<<str<<endl<<"move body.out body"<<cbuf<<".out"<<endl;
		//}

		//if(argVector.size()<2)usage(argVector);

		//if(argVector[1]=="-curl"){
		//	libCurl(argVector);
		//}

void libCurl(vector<string>&argVector){
#ifdef PC

HINSTANCE cl = NULL;

  if((cl = LoadLibrary( "libcurl.dll" ))==NULL ){
  cout<<"I can't load libcurl";
  FreeLibrary(cl); exit(0);
  }
  CURL*(__stdcall *curl_easy_init)();
  CURLcode(__stdcall *curl_easy_setopt )
   (CURL *curl, CURLoption option, ...);
  CURLcode (__stdcall *curl_easy_perform )
   (CURL *curl);
  CURLcode(__stdcall *curl_easy_getinfo )
   (CURL *curl, CURLINFO info, ...);
  void (__stdcall *curl_easy_cleanup )
   (CURL *curl);
  struct curl_slist *(__stdcall
   *curl_slist_append )
  (struct curl_slist *list,const char *string);
  void (__stdcall *curl_slist_free_all)
  (struct curl_slist * list);

// присваиваем указателям функций  соответствующие адреса функции DLL
  curl_easy_init =(CURL*(__stdcall*)())GetProcAddress(cl,"curl_easy_init" );
  curl_easy_setopt =(CURLcode(__stdcall *)(CURL *curl, CURLoption option, ...))
  GetProcAddress(cl, "curl_easy_setopt");
  curl_easy_perform =(CURLcode(__stdcall *)(CURL *curl))
  GetProcAddress(cl, "curl_easy_perform");
  curl_easy_cleanup =(void (__stdcall *)(CURL *curl))
  GetProcAddress(cl, "curl_easy_cleanup");
  curl_easy_getinfo=(CURLcode(__stdcall *)
  (CURL *curl, CURLINFO info, ...))GetProcAddress(cl, "curl_easy_getinfo");
  curl_slist_append=(curl_slist*(__stdcall *)
  (struct curl_slist *list,const char *string))GetProcAddress(cl,"curl_slist_append");
  curl_slist_free_all=(void(__stdcall *)
	 (struct curl_slist *list))
	 GetProcAddress (cl,"curl_slist_free_all");

	 char cbuf[10];   int i=0;

	//for(int in=5744;in<70000;in++){
		CURL *curl_handle;
		char headerfilename[FILENAME_MAX] = "head.out";
		FILE *headerfile;
		int rc=0;
		curl_handle = curl_easy_init();
		//string str="http://www.buddhistdoor.com/glossary/word_eng.php?word_id=";
		//sprintf(cbuf,"%d",id);
		//str+=cbuf;
		string str=argVector[2];
		cout<<"start download "<<str<<endl;
		curl_easy_setopt(curl_handle,   CURLOPT_URL, str.c_str());
		curl_easy_setopt(curl_handle,   CURLOPT_NOPROGRESS  ,1);
		//curl_easy_setopt(curl_handle,   CURLOPT_MUTE        ,1);
		curl_easy_setopt(curl_handle,   CURLOPT_WRITEFUNCTION,&write_data);
		headerfile = fopen(headerfilename,"w");
		if (headerfile == NULL) {
			curl_easy_cleanup(curl_handle);
			exit(0);
		}
		curl_easy_setopt(curl_handle,   CURLOPT_WRITEHEADER ,headerfile);
		curl_easy_perform(curl_handle);
		printf("The head is <%s>\n",headerfilename);
		fclose(headerfile);
		curl_easy_cleanup(curl_handle);
   //}
   exit(10);

#endif
}//_____________________________________________________________________________

#ifdef MACOS
/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 */
char* itoa(int value, char* result, int base) {
	// check that the base if valid
	if (base < 2 || base > 36) { *result = '\0'; return result; }
	
	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;
	
	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while ( value );
	
	// Apply negative sign
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}
#endif

/**
 * C++ version 0.4 std::string style "itoa":
 * Contributions from Stuart Lowe, Ray-Yuan Sheu,
 * Rodrigo de Salvo Braz, Luc Gallant, John Maloney
 * and Brian Hunt
 */
string itoa(int value, int base) {

	std::string buf;
#ifdef MACOS
	// check that the base if valid
	if (base < 2 || base > 16) return buf;

	enum { kMaxDigits = 35 };
	buf.reserve( kMaxDigits ); // Pre-allocate enough space.

	int quotient = value;

	// Translating number to string with base:
	do {
		buf += "0123456789abcdef"[ std::abs( quotient % base ) ];
		quotient /= base;
	} while ( quotient );
	
	// Append the negative sign
	if ( value < 0) buf += '-';
	
	reverse( buf.begin(), buf.end() );
 #endif
	return buf;
}




// РАСПАКОВКА на экран, представления целого числа int
// -----------------------------------------------------------------------------
void binaryPrint(unsigned int input, int radix){
	
	//cout<<"int ="<<input<<" radix="<<radix<<endl;
	
	char c[33];
	// int radix  - где radix = 2 для двоичного представления целого числа (binary number);
	//			        radix = 8 для восьмеричного... и т.д.
	char cbuf[33]; // char *string;   строка результата
	char *p;
	//DC("START binaryNumber"<<END);
	
	// Вывод на экран числа в двоичнном виде  00000000000000001111111111111111
	memset(cbuf,48,32);
	memset(c,0,33);
	cbuf[32]=0;
	//c[31]=0;
	
	itoa((unsigned int)input,c,2);
	
	p=cbuf+(32-strlen(c));
	//DC("//__"<<str<<END);
	memcpy(p,c,strlen(c));
	cout<<cbuf;
	//DC("print binaryNumber "<<END); DC(END);
}//_____________________________________________________________________________
/**/

void memprint(const char* ptr, int size){
	char *memBlock=new char(size+1);
	memcpy(memBlock, ptr, size);
	memBlock[size]=0;
	cout<<memBlock;
	delete memBlock;
}

void error(char *msg){
    perror(msg);
    exit(1);
}

int bsearch(hashRecord *hashVector,unsigned  size,int value){
    int low = 0;
	int mid;
	int N=size;
    int high = N;
	while (low < high) {
		mid = (low + high) / 2;
		if (hashVector[mid].hash < value)
			low = mid + 1; 
		else
			//can't be high = mid-1: here A[mid] >= value,
			//so high can't be < mid if A[mid] == value
			high = mid; 
	}
	// high == low, using high or low depends on taste 
	if ((low < N) && (hashVector[low].hash == value)){
		return low ;// found
	}else{
		return -1; // not found        
    }
}//________________________________________________	

string decodeURLString(string URLstr) {
    const int len = URLstr.length();
    string result;
    for(int i = 0; i < len; i++) {
		if(URLstr[i] == '+')
			result += ' ';
		else if(URLstr[i] == '%') {
			result +=
			translateHex(URLstr[i + 1]) * 16 +
			translateHex(URLstr[i + 2]);
			i += 2; // Move past hex code
		} else // An ordinary character
			result += URLstr[i];
    }
    return result;
}
// Translate a single hex character; used by
// decodeURLString():
char translateHex(char hex) {
    if(hex >= 'A')
		return (hex & 0xdf) - 'A' + 10;
    else
		return hex - '0';
}

unsigned int getID(string data){ //extract ID from xml data
    string str=decodeURLString(data);
	int pos=str.find("glyph id=\""); //cout<<"pos="<<pos<<endl;
	int idNumber;
	if(pos!=-1){
		str=substr(pos+10, str.find("\"",pos+11)-pos ,str);
		//cout<<"str="<<str<<endl;
	    idNumber=strtoul (str.c_str(),NULL,0);
		return idNumber;
	}
	if(pos==-1){
		int pos=str.find("mask g=\""); //cout<<"pos="<<pos<<endl;
		int idNumber;
		if(pos!=-1){
			str=substr(pos+8, str.find("\"",pos+9)-pos ,str);
			//cout<<"str="<<str<<endl;
			idNumber=strtoul (str.c_str(),NULL,0);
			return idNumber;
		}
	}
	return 0;

}

int indexById(vector<stringOCR>&correctionTable,unsigned int in){
	for(int i=0;i<correctionTable.size();i++){
    	if(correctionTable[i].idNumber==in)return i;	
	}
return -1; 

};
