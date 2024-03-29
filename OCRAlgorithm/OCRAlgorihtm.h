//C-
//C- This software is subject to, and may be distributed under, the
//C- GNU General Public License, either Version 2 of the license,
//C- or (at your option) any later version. The license should have
//C- accompanied the software or you may obtain a copy of the license
//C- from the Free Software Foundation at http://www.fsf.org .
//C-
//C- This program is distributed in the hope that it will be useful,
//C- but WITHOUT ANY WARRANTY; without even the implied warranty of
//C- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//C- GNU General Public License for more details.
//C-


#ifndef HEADER_OCRALGORITHM
#define HEADER_OCRALGORITHM

#include "config.h"
#include<string>
#include<vector>
#include<list>
#include<map>
#include <time.h>
#include <math.h>
//#include "GBitmask.h"

using namespace std;

namespace ocr {



class OCRFilter{
  protected:
         OCRFilter(void);
		 OCRFilter(int size);
  public:

		 virtual ~OCRFilter();
		 int ncnr;      ///<����������� �������� �������� ��������
		 int szf;       ///< ������ � 32� �������� sfz=4=32/8;  szf=sizeof(int);
		 int *ImBuf;    ///< ������ ������ � �������� 0
		 int ImBf;      ///< ������� ������ ������� ImBuf
		 int *ReBuf;    ///< ������ ������ � �������� 0
		 int ReBf;      ///< ������� ������ ������� ReBuf
		 int *BufIn;    ///< ������ ������ � �������� 0
		 int BfIn;      ///< ������� ������ ������� ImBuf
		 int *BufOu;    ///< ������ ������ � �������� 0
		 int BfOu;      ///< ������� ������ ������� ImBuf

 /**\brief Constructs a OCRFilter with #size# buffers handlers*/
 static OCRFilter* create(const int size){return new OCRFilter(size); }

 /// ������ ������ ������.
 void filterHigh(vector<int> &In,int KHig);
 /// ��������� ������ �������� (Gaussian).
 void filterLaplasian(vector<int> &In, int KHig, int KLow);

 /// ��������� ������ �������� (Gaussian), ���������� ������. ������� ����� HBuf[y]
 void filterLaplasian(vector<int> &HBuf, vector<int> &GBuf, int KHig, int KLow);

 /// ��������� ������ �������� (Gaussian), ����� ������. ������� ST ����� BufS[y]
 void filterLaplasian(int *Buf1, int *BufTL, int size,  int KHig, int KLow);

};


  /// Sorting  ����������
  void Sort(vector<int> &GBuf, int NSort);



};

#endif