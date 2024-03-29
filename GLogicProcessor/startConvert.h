
string GLogicProcessor::startConvert(commandData *inputData){
	cout<<"Start inputData->data[\"inputFolder\"]"<<inputData->data["inputFolder"]<<END;
	string str,path;
	vector<string> strVector;     
	//sregex date = sregex::compile( "&agrave;" );
	//std::string format( "ā" );
	
	 int index=0;
     DIR *dir;
	int fileFlag;

	 while(index<inputData->fileList.size()){
		if( ( dir=opendir(inputData->fileList[index].c_str()))!=NULL){
			if(inputData->data["ocrData"]=="RTFToYagpo"){
			    fileFlag=readDirectoryToArray(inputData->fileList, inputData->fileList[index],"rtf");
			}else{
			    fileFlag=readDirectoryToArray(inputData->fileList, inputData->fileList[index],"txt");
			}
			inputData->fileList.erase(inputData->fileList.begin()+index);
			continue;
		}
		index++;
	}

	if(inputData->data["ocrData"]=="CXS_to_UTF"){
		loadMapFilePali("CXS_UTF_HTML.xml");
		cout<<"fileList.size()="<<inputData->fileList.size();
		for(int i=0;i<inputData->fileList.size();i++){
			inputData->data["inputFile"]=inputData->fileList[i];
			cout<<"convert "<<i<<" from "<<inputData->fileList.size()<<" "<<inputData->fileList[i]<<END;
			strVector.resize(0);
			readText(strVector,inputData->data["inputFile"]);
			for(int m=0;m<strVector.size();m++){
				//strVector[m]= regex_replace(  strVector[m], date, format );
				strVector[m]=Unicode_to_UTF(strVector[m]);
				convertCXS_to_UTF_nocopy(strVector[m]);
				//cout<<strVector[m];
			}
			writeText(strVector,inputData->data["inputFile"]);

		}

	}

	if(inputData->data["ocrData"]=="ConcatenateFolder"){
		cout<<"fileList.size()="<<inputData->fileList.size();
		ofstream srcOutput;
		str=inputData->data["inputFolder"];
		str+="/allText.txt";
		srcOutput.open(str.c_str());
		for(int i=0;i<inputData->fileList.size();i++){
			inputData->data["inputFile"]=inputData->fileList[i];
			cout<<"cat "<<i<<" from "<<inputData->fileList.size()<<" "<<inputData->fileList[i]<<END;
			srcOutput<<readText(inputData->data["inputFile"].c_str())<<endl;
		}
		srcOutput.close();
	}

	if(inputData->data["ocrData"]=="LowerCase"){
		loadMapFilePali("CXS_UTF_HTML.xml");
		cout<<"fileList.size()="<<inputData->fileList.size();
		for(int i=0;i<inputData->fileList.size();i++){
			inputData->data["inputFile"]=inputData->fileList[i];
			cout<<"convert "<<i<<" from "<<inputData->fileList.size()<<" "<<inputData->fileList[i]<<END;
			strVector.resize(0);
			readText(strVector,inputData->data["inputFile"]);
			int step=0;
			for(int m=0;m<strVector.size();m++){
				//strVector[m]= regex_replace(  strVector[m], date, format );
				//strVector[m]=Unicode_to_UTF(strVector[m]);
				lowerCase_nocopy(strVector[m]);
				if(step==1000){
					cout<<m<<"."<<strVector[m]<<END;
				step=0;}step++;
			}
			writeText(strVector,inputData->data["inputFile"]);

		}

	}

	//cout<<"inputData->data[\"ocrData\"]="<<inputData->data["ocrData"]<<END;

	if(inputData->data["ocrData"]=="PaliUTFToEng"){

		cout<<"PaliUTFToEng"<<END;

	}
	if(inputData->data["ocrData"]=="RTFToYagpo"){
		ofstream c_out; c_out.open("/_out.txt");
		cout<<"RTFToYagpo inputData->fileList.size()="<<inputData->fileList.size()<<END;
		string mainString;
		for(int i=0;i<inputData->fileList.size();i++){
			mainString="";
#ifdef MACOS
			RTFtoYagpoConverter(inputData,mainString,inputData->fileList[i]);
#endif
			string path=inputData->fileList[i]+"_out.txt";
			cout<<"path="<<path<<END;
			writeText(mainString, path);
			c_out<<report;
		}
	}

	if(inputData->data["ocrData"]=="dWylieToYagpo"){
		string mainString;
		if(inputData->data["InputMethod"]=="fileList"){
			cout<<"dWylieToYagpoConverter inputData->fileList.size()="<<inputData->fileList.size()<<END;
			for(int i=0;i<inputData->fileList.size();i++){
				strVector.resize(0); 
				path=inputData->fileList[i]+"_out.txt";
				readText(strVector, inputData->fileList[i]);
				mainString="";
				int step=0;
				for(int i=0;i<strVector.size();i++){
					if(step==1000){cout<<i<<" ";step=0;}step++;
					mainString+=dWylieToYagpoConverter(strVector[i]);
				}	
				writeText(mainString, path);
			}				
		}else{
			for(int i=0;i<inputData->fileList.size();i++){
			    mainString="";	
			    mainString+=dWylieToYagpoConverter(inputData->fileList[i])+"\n";   
			}
			return mainString;	
		}					
	}
	if(inputData->data["ocrData"]=="dSinhalaASCIToYagpo"){
		string mainString;
		string path=inputData->data["tablePath"]+"codePages/SinhalaASCI.xml";
		readMapXML(SinhalaASCI,path);
		cout<<"SinhalaASCI.size()="<<SinhalaASCI.size()<<END;
		
		if(inputData->data["InputMethod"]=="fileList"){
			cout<<"dSinhalaASCIToYagpo inputData->fileList.size()="<<inputData->fileList.size()<<END;
			int step=0;
			for(int i=0;i<inputData->fileList.size();i++){
				strVector.resize(0);
				path=inputData->fileList[i]+"_out.txt";
				readText(strVector, inputData->fileList[i]);
				cout<<"strVector.size()="<<strVector.size()<<END;
				mainString="";
				for(int i=0;i<strVector.size();i++){
					mainString+=dSinhalaASCIToYagpo(strVector[i])+"\n";
					if(step==1000){cout<<i<<" ";step=0;}step++;
				}
				writeText(mainString, path);
			}
		}else{
			int step=0;
			for(int i=0;i<inputData->fileList.size();i++){
				mainString="";
				mainString+=dSinhalaASCIToYagpo(inputData->fileList[i]);
				if(step==1000){cout<<".";step=0;}step++;
			}
			cout<<mainString<<END;
		}
	}
	if(inputData->data["ocrData"]=="SinhalaMettaToYagpo"){
		string mainString;
		string path=inputData->data["tablePath"]+"codePages/SinhalaMetta.xml";
		readMapXML(SinhalaASCI,path);
		cout<<"SinhalaASCI.size()="<<SinhalaASCI.size()<<END;
		
		if(inputData->data["InputMethod"]=="fileList"){
			cout<<"dSinhalaASCIToYagpo inputData->fileList.size()="<<inputData->fileList.size()<<END;
			
			for(int i=0;i<inputData->fileList.size();i++){
				strVector.resize(0);
				path=inputData->fileList[i]+"_out.txt";
				readText(strVector, inputData->fileList[i]);
				cout<<"strVector.size()="<<strVector.size()<<END;
				mainString=""; int step=0;
				for(int i=0;i<strVector.size();i++){  //cout <<"next string "<<i<<" ="<<strVector[i]<<endl;
					if(strVector[i].size()){
					   mainString+=SinghalaASCIToYagpo(strVector[i])+"\n";
					}else{mainString+="\n";}	
					if(step==1000){cout<<i<<" ";step=0;}step++;
				}
				writeText(mainString, path);
			}
		}else{   
			int step=0;
			for(int i=0;i<inputData->fileList.size();i++){
  				mainString="";
				mainString+=dSinhalaASCIToYagpo(inputData->fileList[i]);
				if(step==1000){cout<<".";step=0;}step++;
			}
			cout<<mainString<<END;
		}
		cout<<"DONE CONVERT";
	}
	

	if(inputData->data["ocrData"]=="WylieToYagpo"){
		string mainString; cout<<" inputData->data[InputMethod]="<<inputData->data["InputMethod"]<<endl;
		if(inputData->data["InputMethod"]=="fileList"){
			cout<<"WylieToYagpoConverter inputData->fileList.size()="<<inputData->fileList.size()<<END;
			for(int i=0;i<inputData->fileList.size();i++){
				strVector.resize(0);
				path=inputData->fileList[i]+"_out.txt"; cout<<" path="<<path;
				readText(strVector, inputData->fileList[i]);
				mainString="";
				int step=0;
				for(int i=0;i<strVector.size();i++){ 
					if(step==1000){cout<<i<<" ";step=0;}step++;
					mainString+=WylieToYagpoConverter(strVector[i]);
					mainString+="\n";
				}
				writeText(mainString, path);
				cout<<"done convert";
			}
		}else{
			mainString="";
			for(int i=0;i<inputData->fileList.size();i++){
				mainString+=WylieToYagpoConverter(inputData->fileList[i])+"\n";
			}
			return mainString;
		}
	}

	if(inputData->data["ocrData"]=="TibUniToWylie"){
		string mainString;
		if(inputData->data["InputMethod"]=="fileList"){
			cout<<"YagpoToWylieConverter inputData->fileList.size()="<<inputData->fileList.size()<<END;
			for(int i=0;i<inputData->fileList.size();i++){
				strVector.resize(0);
				path=inputData->fileList[i]+"_out.txt";
				readText(strVector, inputData->fileList[i]);
				mainString="";
				for(int i=0;i<strVector.size();i++){
					mainString+=TibUniToWylie(strVector[i],1);
					mainString+="\n";
				}
				writeText(mainString, path);
				cout<<"done convert";
			}
		}else{
			mainString="";
			for(int i=0;i<inputData->fileList.size();i++){
				mainString+=TibUniToWylie(inputData->fileList[i],1);
				mainString+="\n";
			}
			return mainString;
		}
	}

	if(inputData->data["ocrData"]=="YagpoToWylie"){
		string mainString;
		if(inputData->data["InputMethod"]=="fileList"){
			cout<<"YagpoToWylieConverter inputData->fileList.size()="<<inputData->fileList.size()<<END;
			for(int i=0;i<inputData->fileList.size();i++){
				strVector.resize(0);
				path=inputData->fileList[i]+"_out.txt";
				readText(strVector, inputData->fileList[i]);
				mainString="";
				for(int i=0;i<strVector.size();i++){
					mainString+=TibUniToWylie(strVector[i],2);
					mainString+="\n";
				}
				writeText(mainString, path);
				cout<<"done convert";
			}
		}else{
			mainString="";
			for(int i=0;i<inputData->fileList.size();i++){
				mainString+=TibUniToWylie(inputData->fileList[i],2);
				mainString+="\n";
			}
			return mainString;
		}
	}




	if(inputData->data["ocrData"]=="YagpoToUnicode"){
		string mainString;
		if(inputData->data["InputMethod"]=="fileList"){
			cout<<"YagpoToUnicode inputData->fileList.size()="<<inputData->fileList.size()<<END;
			for(int i=0;i<inputData->fileList.size();i++){
				strVector.resize(0);
				path=inputData->fileList[i]+"_out.txt";
				readText(strVector, inputData->fileList[i]);
				mainString="";
				for(int n=0;n<strVector.size();n++){
					mainString+=YagpoToUni(strVector[n]);
					mainString+="\n";
				}
				writeText(mainString, path);
			}
		}else{
			mainString="";
			for(int i=0;i<inputData->fileList.size();i++){
				mainString+=YagpoToUni(inputData->fileList[i]);
				mainString+="\n";
			}
			return mainString;
		}
	}

	if(inputData->data["ocrData"]=="UnicodeToYagpo"){
		string mainString;
			if(inputData->data["InputMethod"]=="fileList"){
			cout<<"YagpoToUnicode inputData->fileList.size()="<<inputData->fileList.size()<<END;
			for(int i=0;i<inputData->fileList.size();i++){
				strVector.resize(0);
				path=inputData->fileList[i]+"_out.txt";
				readText(strVector, inputData->fileList[i]);
				mainString="";
				for(int n=0;n<strVector.size();n++){
					mainString+=UnicodeToYagpo(strVector[n]);
					mainString+="\n";
				}
				writeText(mainString, path);
			}
		}else{
			mainString="";
			for(int i=0;i<inputData->fileList.size();i++){
				mainString+=UnicodeToYagpo(inputData->fileList[i]);
				if(i)mainString+="\n";
			}
			return mainString;
		}
	}

	if(inputData->data["ocrData"]=="TXTtoXML"){
		string mainString;
		if(inputData->data["InputMethod"]=="fileList"){
			cout<<"TXTtoXML inputData->fileList.size()="<<inputData->fileList.size()<<END;
			for(int i=0;i<inputData->fileList.size();i++){
				strVector.resize(0);
				path=inputData->fileList[i]+".xml";
				path=str_replace(".doc.txt", "" ,path);
				path=str_replace(".DOC.txt", "" ,path);
				readText(strVector, inputData->fileList[i]);
				cout<<"strVector.size()="<<strVector.size()<<END;
				mainString="<text:text xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:text=\"http://www.tbrc.org/models/text#\" RID=\"\" lang=\"bo_ZH\" volume=\"\" num=\"\" fromWork=\"lam.dre\" fromVolume=\"\" start=\"0\" last=\"\">";
				for(int n=0;n<strVector.size();n++){
					if(strVector[n].find("FILE",0)==string::npos&&strVector[n].find("PAGE",0)==string::npos){
						//if(strVector[n].find("Corel",0)!=string::npos)cout<<inputData->fileList[i]<<END;
					mainString+=strVector[n];
					mainString+="\n";
					}
				}
				mainString+="</text:text>";
				cout<<path<<END;
				writeText(mainString, path);
			}
		}else{
			mainString="start\n";
			for(int i=0;i<inputData->fileList.size();i++){
				mainString+=YagpoToUni(inputData->fileList[i]);
			}
			cout<<mainString<<END;
		}
	}

	if(inputData->data["ocrData"]=="TXTtoHTML"){
		string mainString;
		if(inputData->data["InputMethod"]=="fileList"){
			cout<<"TXTtoHTML inputData->fileList.size()="<<inputData->fileList.size()<<END;
			for(int i=0;i<inputData->fileList.size();i++){

				strVector.resize(0);
				path=inputData->fileList[i]+".xml";
				path=str_replace(".doc.txt", "" ,path);
				path=str_replace(".DOC.txt", "" ,path);
				readText(strVector, inputData->fileList[i]);
				cout<<"inputData->fileList[i]="<<inputData->fileList[i]<<" strVector.size()="<<strVector.size()<<END;
				//continue;
				/*
				mainString="<text:text xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:text=\"http://www.tbrc.org/models/text#\" RID=\"\" lang=\"bo_ZH\" volume=\"\" num=\"\" fromWork=\"lam.dre\" fromVolume=\"\" start=\"0\" last=\"\">";
				for(int n=0;n<strVector.size();n++){
					if(strVector[n].find("FILE",0)==string::npos&&strVector[n].find("PAGE",0)==string::npos){
						//if(strVector[n].find("Corel",0)!=string::npos)cout<<inputData->fileList[i]<<END;
						mainString+=strVector[n];
						mainString+="\n";
					}
				}
				mainString+="</text:text>";
				cout<<path<<END;
				writeText(mainString, path);
				*/
			}
		}else{
			mainString="start\n";
			for(int i=0;i<inputData->fileList.size();i++){
				mainString+=YagpoToUni(inputData->fileList[i]);
			}
			cout<<mainString<<END;
		}
	}

	if(inputData->data["ocrData"]=="UTF8"){
		string mainString;
		if(inputData->data["InputMethod"]=="fileList"){
			DT("YagpoToUnicode inputData->fileList.size()="<<inputData->fileList.size()<<endl);
			for(int i=0;i<inputData->fileList.size();i++){
				strVector.resize(0);
				path=inputData->fileList[i];
				DT("convert path "<<path<<endl);
				readText(strVector, inputData->fileList[i]);
				mainString="";
				for(int n=0;n<strVector.size();n++){
					mainString+=Unicode_to_UTF(strVector[n]);
					mainString+="\n";
				}
				writeText(mainString, path);
			}
		}else{
			mainString="";
			for(int i=0;i<inputData->fileList.size();i++){
				mainString+=YagpoToUni(inputData->fileList[i]);
				mainString+="\n";
			}
			return mainString;
		}
	}


return "";

}//________________________________________________________________________________________________________________


/*
 void GLogicProcessor::pageToBase(commandData *inputData){

 readVectorBase(inputData);
 baseImg=GBitmap::create(3000,6000);
 GBitmap *img=GBitmap::create(150,150);
 int border;
 string str;
 int x0,y0,w,h,xImg=0,yImg=0;
 baseImg->fill(0);
 
 if(!inputData->fileList.size()){
 readDirectoryToArray(inputData);
 }
 
 for(int i=0;i<inputData->fileList.size();i++){
 str=inputData->fileList[i]+".html";   cout<<str<<END;
 inputData->data["inputFile"]=inputData->fileList[i];
 inputData->data["inputFileName"]=inputData->data["siteName"];
 inputData->data["inputFileName"]+=substr(inputData->data["siteRoot"].size(),inputData->data["inputFile"]);
 readPageHTML(inputData);
 string str=inputData->data["inputFile"];
 LoadImageData(str);
 setBit=GBitset::createResize(pechaImg,1,1,1);
 setBit->pageStringDetector(strArray); // Подпрограмма выделения строк и букв ПЕЧА ( РЕЛСЫ )
 border=setBit->border_size();
 setOCR=GBitsetOCR::createResize(pechaImg,1,1);
 setOCR->setData(inputData,
 vectorBase,
 aliKali,
 strArray,
 iLeft,
 iTop,
 border,
 ALL_LETTER);
 mainString=setOCR->mainString;
 
 for (int index=0;index<strArray.size();index++){
 for (int in=0;in<strArray[index].wordArray.size();in++){
 if(strArray[index].wordArray[in].correlation<80&&strArray[index].wordArray[in].correlation>0){
 img->fill(255);
 if(yImg>39)goto drawBasePoint;
 if(strArray[index].wordArray[in].w<5)continue;
 if(strArray[index].wordArray[in].OCRIndex==3)continue;
 cout<<" . ";
 x0=strArray[index].wordArray[in].x0-70;
 y0=strArray[index].y0-50;
 LoadImageData(img,x0,y0, 150-1, 150-1);
 baseImg->drawImg(img,xImg*150,yImg*150);
 xImg++;  if(xImg==20){xImg=0;yImg++;}
 }
 }
 }
 
 if(setBit!=NULL){
 setBit->destroy();
 delete setBit;
 }
 if(setOCR!=NULL){
 //inputData->c_out<<"</div></body></html>";
 drawStrArray(inputData,border);
 setOCR->destroy();
 delete setOCR;
 strArray.resize(0);
 }
 
 pechaImg->destroy();
 
 }
 drawBasePoint:;
 str=inputData->data["inputFolder"]+"/__letterBase.jpg";
 WriteImageData(baseImg,str,0);
 
 }//____________________________________________________________________________
 
 void GLogicProcessor::startOCRBatch(commandData *inputData){
 
 string strHeaderHTML,srcLine,str;
 string  path=inputData->data["tablePath"]+"/header.xml";
 replace(path.begin(),path.end(),'\\','/');
 
 if(!inputData->fileList.size()){
 readDirectoryToArray(inputData);
 }
 
 
 if(inputData->data["ocrData"]=="ocrBatch"){
 
 ifstream headerData(path.c_str());
 
 
 if( !headerData ){
 cout<<"Source path "<<path<<" not exist"<<endl;
 return;
 }
 while (getline(headerData, srcLine,'\n')){strHeaderHTML+=srcLine; strHeaderHTML+="\n";}
 
 
 
 for(int i=0;i<inputData->fileList.size();i++){
 str=inputData->fileList[i]+".html";   cout<<str<<END;
 inputData->data["inputFile"]=inputData->fileList[i];
 inputData->data["inputFileName"]=inputData->data["siteName"];
 inputData->data["inputFileName"]+=substr(inputData->data["siteRoot"].size(),inputData->data["inputFile"]);
 //readPageHTML(inputData);
 inputData->c_out.open(str.c_str());
 inputData->c_out<<strHeaderHTML;
 pechaDataLoaded=0;
 startOCR(inputData);
 inputData->c_out.close();
 }
 }else{
 
 for(int i=0;i<inputData->fileList.size();i++){
 str=inputData->fileList[i]+".html";
 cout<<str<<END;
 
 inputData->data["inputFile"]=inputData->fileList[i];
 readPageHTML(inputData);
 //inputData->c_out.open(str.c_str());
 }
 //inputData->c_out.close();
 
 
 }
 
 
 }//____________________________________________________________________________
 
 
 */

