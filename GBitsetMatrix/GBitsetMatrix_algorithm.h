
//-------------------------------АЛГОРИТМЫ------------------------------------//



//##


// -----------------------------------------------------------------------------
// ++++++
// ФИЛЬТР НИЗКИХ ЧАСТОТ  (ГАУССИАН - Gaussian), попытка минимальной реализации.

// Адаптивный F(KHig) нолевой бордюр вокруг массивов.

// 2 массива

// -----------------------------------------------------------------------------

    // треугольная весовая функция

    // y(n)= -y(n-2)+2y(n-1)+x(n-2k-2)-2x(n-k-1)+xn


 void GBitsetMatrix::filterHigh(vector<int> &In, int KHig){


 int y,Khg;
 short size=In.size();

//////////////////////////////////////////////  всего 2 массива

 if ( KHig<2 ) KHig=1;   if ( KHig>80 ) KHig=80;   // if ( KHig>63 ) KHig=63;
 // для фильтра низких частот  KhgN=KHig*N+1 ,  при KHig=64 :
 //      129           257             385
	 int  s2=KHig*2+1,  s4=KHig*4;    //s6=KHig*6+1;

// vector<int> BufIn(size+s6);
// vector<int> lt1(size+s6);
// int BufIn[10000]={0};
// int lt1[10000]={0};

      // обнуление ImBuf,ReBuf ( где ImBuf,ReBuf глобальные )
      memset(ImBuf,0,4*(ncnr+512));      memset(ReBuf,0,4*(ncnr+512));
      //for ( y=0; y < size+s6; y++ ){BufIn[y]=lt1[y]=0;}

      // Входной буфер BufIn[y+s2], In[y]
      for ( y=0; y < size; y++ ){ImBuf[y+s2]=In[y];}
      Khg=KHig*KHig;
      for ( y=s2; y < size+s4; y++ ) { //прямоугольная весовая функция, прямая фильтрация
         ReBuf[y]=-ReBuf[y-2]+2*ReBuf[y-1]+ImBuf[y-KHig-KHig]-2*ImBuf[y-KHig]+ImBuf[y]; //k=15+
      }
      for ( y=s2; y < size+s4; y++ ) { ReBuf[y]=ReBuf[y]/Khg;  ImBuf[y]=0; } // Norm // y=0; size+s6;
      // memset(ImBuf,0,4*(ncnr+512));
      for ( y=size+s4; y > s2; y-- ) { //прямоугольная весовая функция, инверсная фильтрация
         ImBuf[y]=-ImBuf[y+2]+2*ImBuf[y+1]+ReBuf[y+KHig+KHig]-2*ReBuf[y+KHig]+ReBuf[y];//k=15-
      }
      for ( y=s2; y < size+s2; y++ ) { In[y-s2]=ImBuf[y]/Khg; }
      // Выходной буфер In[y-s2]
/**/

}//_____________________________________________________________________________



//##

// -----------------------------------------------------------------------------
// ++++++
// Полосовой фильтр Гауссиан (ГАУССИАН - Gaussian), попытка минимальной реализации.

// Входной буфер &In. Адаптивный F(KHig) нолевой бордюр вокруг массивов

// 4 массива

// -----------------------------------------------------------------------------


 void GBitsetMatrix::filterLaplasian( vector<int> &In, int KHig, int KLow){  // filterLaplasianM1



 int y,Khg,Klw,S;
 int size=In.size();

 int KHigh=KHig; // заплатка - запрет на модификацию KHig
 if ( KHig<2 ) KHig=1;   if ( KHig>63 ) KHig=63;
 if ( KLow<2 ) KLow=2;   if ( KLow>63 ) KLow=63;
 S=KLow;  if ( S < KHig ) S=KHig;
 // Sn=129        257        385         514        при S=64,   SN=KHig*N+1
 int   S2=S*2+1,  S4=S*4+1,  s6=S*6+1,   S8=S*8+2;

///////////////

// обнуление BufIn,ImBuf,ReBuf,BufOu ( где BufIn,ImBuf,ReBuf,BufOu глобальные )
        memset(BufIn,0,4*(ncnr+512));   memset(BufOu,0,4*(ncnr+512));
        memset(ReBuf,0,4*(ncnr+512));   memset(ImBuf,0,4*(ncnr+512));


    for ( y=0; y < size; y++ ){BufIn[y+S4]=In[y];} // Входной буфер Buf In[y]

    Khg=KHig*KHig; Klw=KLow*KLow;
    for ( y=S2; y < size+s6; y++ ) { //треугольная весовая функция, прямая фильтрация
        ImBuf[y]=-ImBuf[y-2]+2*ImBuf[y-1]+BufIn[y-KHig-KHig]-2*BufIn[y-KHig]+BufIn[y];
        ReBuf[y]=-ReBuf[y-2]+2*ReBuf[y-1]+BufIn[y-KLow-KLow]-2*BufIn[y-KLow]+BufIn[y];
        }
    for ( y=0; y < size+S8; y++ )  { // Norm
        BufIn[y]=ImBuf[y]/Khg;  BufOu[y]=ReBuf[y]/Klw;  ImBuf[y]=ReBuf[y]=0;
        }
    for ( y=size+s6; y > S2; y--) { //треугольная весовая функция, инверсная фильтрация
        ImBuf[y]=-ImBuf[y+2]+2*ImBuf[y+1]+BufIn[y+KHig+KHig]-2*BufIn[y+KHig]+BufIn[y];
        ReBuf[y]=-ReBuf[y+2]+2*ReBuf[y+1]+BufOu[y+KLow+KLow]-2*BufOu[y+KLow]+BufOu[y];
        }
    for ( y=S4; y < size+S4; y++ ){ In[y-S4]=ImBuf[y]/Khg - ReBuf[y]/Klw; }
    // Выход In[y-S4]
/**/

KHig=KHigh; // заплатка - запрет на модификацию KHig

///////////////
/*
 vector<int> BufIn(size+S8);   // 514
 vector<int> BufOut1(size+S8);
 vector<int> BufOut2(size+S8);
 vector<int> lt1(size+S8);
 vector<int> lt2(size+S8);

    for ( y=0; y < size; y++ ){BufIn[y+S4]=In[y];} // Входной буфер Buf In[y]

	  Khg=KHig*KHig; Klw=KLow*KLow;
      for ( y=S2; y < size+s6; y++ ) { //треугольная весовая функция, прямая фильтрация
         lt1[y]=-lt1[y-2]+2*lt1[y-1]+BufIn[y-KHig-KHig]-2*BufIn[y-KHig]+BufIn[y]; //k=15+
         lt2[y]=-lt2[y-2]+2*lt2[y-1]+BufIn[y-KLow-KLow]-2*BufIn[y-KLow]+BufIn[y]; //k=63+
      }
      for ( y=0; y < size+S8; y++ ) { lt1[y]=lt1[y]/Khg; lt2[y]=lt2[y]/Klw; }//Norm
      for ( y=size+s6; y > S2; y--) { //треугольная весовая функция, инверсная фильтрация
         BufOut1[y]=-BufOut1[y+2]+2*BufOut1[y+1]+lt1[y+KHig+KHig]-2*lt1[y+KHig]+lt1[y];//k=15-
         BufOut2[y]=-BufOut2[y+2]+2*BufOut2[y+1]+lt2[y+KLow+KLow]-2*lt2[y+KLow]+lt2[y];//k=63-
      }
      for ( y=S4; y < size+S4; y++ ){ In[y-S4]=BufOut1[y]/Khg-BufOut2[y]/Klw; }
      // Выход In[y-S4]
*/

// DM(size<<" sizeG ");  DM(END);

/*

    BufIn.assign(siz,0); BufOut1.assign(siz,0); BufOut2.assign(siz,0);

    lt1.assign(siz,0);   lt2.assign(siz,0);
*/


}//_____________________________________________________________________________


//##

// -----------------------------------------------------------------------------
// ++++++
// Полосовой фильтр Гауссиан (ГАУССИАН - Gaussian), попытка минимальной реализации.
// Входной буфер &HBuf, &GBuf. Адаптивный F(KHig) нолевой бордюр вокруг массивов
// 4 массива
// -----------------------------------------------------------------------------

 void GBitsetMatrix::filterLaplasianM( vector<int> &HBuf,
                                 vector<int> &GBuf,
                                 int KHig, int KLow){

 int y,Khg,Klw,S;
 int size=HBuf.size();

 int KHigh=KHig; // заплатка - запрет на модификацию KHig
 if ( KHig<2 ) KHig=1;   if ( KHig>63 ) KHig=63;
 if ( KLow<2 ) KLow=2;   if ( KLow>63 ) KLow=63;
 S=KLow;  if ( S < KHig ) S=KHig;
 // Sn=129        257        385         514        при S=64,   KhgN=KHig*N+1
 int   S2=S*2+1,  S4=S*4+1,  s6=S*6+1,   S8=S*8+2;

 ///////////////

// обнуление BufIn,ImBuf,ReBuf,BufOu ( где BufIn,ImBuf,ReBuf,BufOu глобальные )
        memset(BufIn,0,4*(ncnr+512));   memset(BufOu,0,4*(ncnr+512));
        memset(ReBuf,0,4*(ncnr+512));   memset(ImBuf,0,4*(ncnr+512));
        GBuf.assign(ncnr,0);
        
//    for ( y=0; y < size+S8; y++ )  { BufIn[y]=BufOu[y]=ReBuf[y]=ImBuf[y]=0; }

    for ( y=0; y < size; y++ ){BufIn[y+S4]=HBuf[y];} // Входной буфер Buf HBuf[y]
    Khg=KHig*KHig; Klw=KLow*KLow;
    for ( y=S2; y < size+s6; y++ ) { //треугольная весовая функция, прямая фильтрация
        ImBuf[y]=-ImBuf[y-2]+2*ImBuf[y-1]+BufIn[y-KHig-KHig]-2*BufIn[y-KHig]+BufIn[y];
        ReBuf[y]=-ReBuf[y-2]+2*ReBuf[y-1]+BufIn[y-KLow-KLow]-2*BufIn[y-KLow]+BufIn[y];
        }
    for ( y=0; y < size+S8; y++ )  { // Norm
        BufIn[y]=ImBuf[y]/Khg;  BufOu[y]=ReBuf[y]/Klw;  ImBuf[y]=ReBuf[y]=0;
        }
    for ( y=size+s6; y > S2; y--) { //треугольная весовая функция, инверсная фильтрация
        ImBuf[y]=-ImBuf[y+2]+2*ImBuf[y+1]+BufIn[y+KHig+KHig]-2*BufIn[y+KHig]+BufIn[y];
        ReBuf[y]=-ReBuf[y+2]+2*ReBuf[y+1]+BufOu[y+KLow+KLow]-2*BufOu[y+KLow]+BufOu[y];
        }
    for ( y=S4; y < size+S4; y++ ){ GBuf[y-S4]=ImBuf[y]/Khg - ReBuf[y]/Klw; }
    // Выход GBuf[y-S4]
/**/

KHig=KHigh; // заплатка - запрет на модификацию KHig

 ///////////////

/*
 vector<int> BufIn(size+S8);   // 514
 vector<int> BufOut1(size+S8);
 vector<int> BufOut2(size+S8);
 vector<int> lt1(size+S8);
 vector<int> lt2(size+S8);


    for ( y=0; y < size; y++ ){BufIn[y+S4]=HBuf[y];} // Входной буфер HBuf[y]
                                                     // Входной буфер In[y]
      Khg=KHig*KHig; Klw=KLow*KLow;
      for ( y=S2; y < size+s6; y++ ) { //треугольная весовая функция, прямая фильтрация
         lt1[y]=-lt1[y-2]+2*lt1[y-1]+BufIn[y-KHig-KHig]-2*BufIn[y-KHig]+BufIn[y]; //k=15+
         lt2[y]=-lt2[y-2]+2*lt2[y-1]+BufIn[y-KLow-KLow]-2*BufIn[y-KLow]+BufIn[y]; //k=63+
      }
      for ( y=0; y < size+S8; y++ ) { lt1[y]=lt1[y]/Khg; lt2[y]=lt2[y]/Klw; }//Norm
      for ( y=size+s6; y > S2; y--) { //треугольная весовая функция, инверсная фильтрация
         BufOut1[y]=-BufOut1[y+2]+2*BufOut1[y+1]+lt1[y+KHig+KHig]-2*lt1[y+KHig]+lt1[y];//k=15-
         BufOut2[y]=-BufOut2[y+2]+2*BufOut2[y+1]+lt2[y+KLow+KLow]-2*lt2[y+KLow]+lt2[y];//k=63-
      }
      for ( y=S4; y < size+S4; y++ ){ GBuf[y-S4]=BufOut1[y]/Khg-BufOut2[y]/Klw; }
      // Выход GBuf   // Выход In[y-S4]
*/

//DM(size<<" sizeG ");  DM(END);
/*
 vector<int> BufIn(size+514);
 vector<int> BufOut1(size+514);
 vector<int> BufOut2(size+514);
 vector<int> lt1(size+514);
 vector<int> lt2(size+514);

   for ( y=0; y < size; y++ ){BufIn[y+257]=HBuf[y];} // Входной буфер BufIn[y]

      Khg=KHig*KHig; Klw=KLow*KLow;
      for ( y=129; y < size+385; y++ ) { //треугольная весовая функция, прямая фильтрация
         lt1[y]=-lt1[y-2]+2*lt1[y-1]+BufIn[y-KHig-KHig]-2*BufIn[y-KHig]+BufIn[y]; //k=15+
         lt2[y]=-lt2[y-2]+2*lt2[y-1]+BufIn[y-KLow-KLow]-2*BufIn[y-KLow]+BufIn[y]; //k=63+
      }
      for ( y=0; y < size+514; y++ ) { lt1[y]=lt1[y]/Khg; lt2[y]=lt2[y]/Klw; }//Norm
      for ( y=size+385; y > 129; y--) { //треугольная весовая функция, инверсная фильтрация
         BufOut1[y]=-BufOut1[y+2]+2*BufOut1[y+1]+lt1[y+KHig+KHig]-2*lt1[y+KHig]+lt1[y];//k=15-
         BufOut2[y]=-BufOut2[y+2]+2*BufOut2[y+1]+lt2[y+KLow+KLow]-2*lt2[y+KLow]+lt2[y];//k=63-
      }
      for ( y=257; y < size+257; y++ ){ GBuf[y-257]=BufOut1[y]/Khg-BufOut2[y]/Klw; }
      // Выход In[y-257]
*/
/*
    BufIn.assign(siz,0); BufOut1.assign(siz,0); BufOut2.assign(siz,0);
    lt1.assign(siz,0);   lt2.assign(siz,0);
*/


}//_____________________________________________________________________________


//##


//  ПОЛОСОВОЙ ФИЛЬТР (ГАУССИАН - Gaussian)
//  ----------------------------------------------------------------------------
    // прямоугольная весовая функция
    // y(n)= y(n-1)+xn-x(n-2k-1)
    // треугольная весовая функция
    // y(n)= -y(n-2)+2y(n-1)+x(n-2k-2)-2x(n-k-1)+xn

 void GBitsetMatrix::filterLaplasianM1(vector<int> &In, int KHig, int KLow){ // filterLaplasian
 //int KHig=8+1; //порядок высокочастотного фильтра (k=15) 2< KLow,KHig =<63
 //int KLow=63+1; //порядок низкочастотного фильтра (k=63) 2< KLow,KHig =<63
                // KLow >< Hig
 int y,Klw,Khg;
 short size=In.size();
 //int size=nrows;  HBuf.assign(ncolumns,0);
 vector<int> BufIn(size+514);
 vector<int> BufOut1(size+514);
 vector<int> BufOut2(size+514);
 vector<int> lt1(size+514);
 vector<int> lt2(size+514);


// TIME_START
	int KHigh=KHig; // заплатка - запрет на модификацию KHig
	if ( KHig<2 ) KHig=1;   if ( KHig>63 ) KHig=63;
    if ( KLow<2 ) KLow=2;   if ( KLow>63 ) KLow=63;
/*
    BufIn.assign(size+514,0);
    BufOut1.assign(size+514,0); BufOut2.assign(size+514,0);
    lt1.assign(size+514,0);     lt2.assign(size+514,0);
*/
   for ( y=0; y < size; y++ ){BufIn[y+257]=In[y];} // Входной буфер In[y]
     ///sum=0;
/*    for ( y=0; y < 257; y++ ){BufIn[y]=0;BufOut1[y]=0;BufOut2[y]=0;            //0000
    lt1[y]=0;lt2[y]=0;}                                                        //0000
    for ( y=size+257; y < size+514; y++ ){BufIn[y]=0;BufOut1[y]=0;BufOut2[y]=0;//0000
    lt1[y]=0;lt2[y]=0;}                                                        //0000
*/    //for ( y=0; y < size+514; y++ ){BufIn[y]=0;} BufIn[257+size/2]=1000;sum=0;//TEST
    //BufIn[257+32]=5000; BufIn[257+size/2]=5000; BufIn[size+257-32]=5000;     //TEST
    /// BufIn.empty(); BufIn.resize(size+514)
      Khg=KHig*KHig; Klw=KLow*KLow;
      for ( y=129; y < size+385; y++ ) { //треугольная весовая функция, прямая фильтрация
         lt1[y]=-lt1[y-2]+2*lt1[y-1]+BufIn[y-KHig-KHig]-2*BufIn[y-KHig]+BufIn[y]; //k=15+
         lt2[y]=-lt2[y-2]+2*lt2[y-1]+BufIn[y-KLow-KLow]-2*BufIn[y-KLow]+BufIn[y]; //k=63+
      }
      for ( y=0; y < size+514; y++ ) { lt1[y]=lt1[y]/Khg; lt2[y]=lt2[y]/Klw; }//Norm
      for ( y=size+385; y > 129; y--) { //треугольная весовая функция, инверсная фильтрация
         BufOut1[y]=-BufOut1[y+2]+2*BufOut1[y+1]+lt1[y+KHig+KHig]-2*lt1[y+KHig]+lt1[y];//k=15-
         BufOut2[y]=-BufOut2[y+2]+2*BufOut2[y+1]+lt2[y+KLow+KLow]-2*lt2[y+KLow]+lt2[y];//k=63-
      }
      for ( y=257; y < size+257; y++ ){ In[y-257]=BufOut1[y]/Khg-BufOut2[y]/Klw; // Выход In[y-257]
       //  if(y<=size/2+257){Varu=y-257;} else {Varu=(size+257-y);}
       //  BufOut1[y-257]=(BufOut1[y]*1)*Varu/size;//временная регилировка усиления (вдоль y)
       //   In[y-257]=BufOut1[y];                   // Выходной буфер BufOut1
       ///   BufOut1[y-257]=BufOut1[y]/1;
       //// sum=sum+BufOut1[y]; //подсчет постоянной составляющей
		}

KHig=KHigh; // заплатка - запрет на модификацию KHig

//// DM(sum/size<<"summa "); //контроль постоянной составляющей
// DM("ГАУССИАН "); TIME_PRINT

/*
#ifdef DEBUGLVL
                static short delta=0.8*ncolumns;
                for(y=0; y < drawData->rows(); y++) {
              //DM(BufOut1[y]/200<<" ");
                if(BufOut1[y]>=0){
                  for(x =delta; x < (BufOut1[y])+delta; x++ )drawData[0][y][x]*=0.6;
                 }else{

                  for(x =delta; x > (BufOut1[y])+delta; x-- )drawData[0][y][x]*=0.6;

                 }

               }

#endif

*/


}//_____________________________________________________________________________


//##
                               
// -------------------------------_ST_------------------------------------------
// ++++++
// Полосовой фильтр Гауссиан (ГАУССИАН - Gaussian), попытка минимальной реализации.

// Входной ST буфер *BufS, *BufTL. Адаптивный F(KHig) нолевой бордюр вокруг массивов

// 4 ST массива

// -----------------------------------------------------------------------------


 void GBitsetMatrix::filterLaplasStM( int *Buf1, int *BufTL, int size, int KHig, int KLow ){



 int y,Klw,Khg,S;

 int KHigh=KHig; // заплатка - запрет на модификацию KHig
 if ( KHig<2 ) KHig=1;   if ( KHig>63 ) KHig=63;
 if ( KLow<2 ) KLow=2;   if ( KLow>63 ) KLow=63;
 S=KLow;  if ( S < KHig ) S=KHig;
 // Sn=129        257        385         514        при S=64,   KhgN=KHig*N+1
 int   S2=S*2+1,  S4=S*4+1,  s6=S*6+1,   S8=S*8+2;


// обнуление BufIn,ImBuf,ReBuf,BufOu ( где BufIn,ImBuf,ReBuf,BufOu глобальные )
        memset(BufIn,0,4*(ncnr+512));   memset(BufOu,0,4*(ncnr+512)); // szf=4;
        memset(ReBuf,0,4*(ncnr+512));   memset(ImBuf,0,4*(ncnr+512));
        memset(BufTL,0,4*(size+32 ));

	for ( y=0; y < size; y++ ){BufIn[y+S4]=Buf1[y];} // Входной буфер Buf Buf1[y]
	Khg=KHig*KHig; Klw=KLow*KLow;
	for ( y=S2; y < size+s6; y++ ) { //треугольная весовая функция, прямая фильтрация
		ImBuf[y]=-ImBuf[y-2]+2*ImBuf[y-1]+BufIn[y-KHig-KHig]-2*BufIn[y-KHig]+BufIn[y];
		ReBuf[y]=-ReBuf[y-2]+2*ReBuf[y-1]+BufIn[y-KLow-KLow]-2*BufIn[y-KLow]+BufIn[y];
        }
	for ( y=0; y < size+S8; y++ )  { // Norm
		BufIn[y]=ImBuf[y]/Khg;  BufOu[y]=ReBuf[y]/Klw;  ImBuf[y]=ReBuf[y]=0;
		}
	for ( y=size+s6; y > S2; y--) { //треугольная весовая функция, инверсная фильтрация
		ImBuf[y]=-ImBuf[y+2]+2*ImBuf[y+1]+BufIn[y+KHig+KHig]-2*BufIn[y+KHig]+BufIn[y];
		ReBuf[y]=-ReBuf[y+2]+2*ReBuf[y+1]+BufOu[y+KLow+KLow]-2*BufOu[y+KLow]+BufOu[y];
		}
	for ( y=S4; y < size+S4; y++ ){ BufTL[y-S4]=ImBuf[y]/Khg - ReBuf[y]/Klw; }
	// Выход BufTL[y-S4]
/**/

KHig=KHigh; // заплатка - запрет на модификацию KHig


}//_____________________________________________________________________________



/*

//##
//
// вертикальные и горизонтальные локальные сумммы массива графики bits_data
// vertical and horizontal local summmy //
// -----------------------------------------------------------------------------

void GBitsetMatrix::BitsSummmyWH(int NSumm){

#ifdef DEBUGLVL
#ifdef DRAW1
  int y,x,p,pp;
  int res,resD,w=ncolumns; //
  bool d,*d1,*d2;

  //LStS массив заполненный (в PageWorldDet) вертикальными  суммами for PageWorldDet
  //BufL массив заполненный (в PageWorldDet) вертикальными дифференциальными суммами for PageWorldDet
  //int size=HStr.size();
  //vector<int> HStr(ncnr); // массив заполненный горизонтальными суммами
  //vector<int> HStS(ncnr); // массив заполненный горизонтальными дифференциальными суммами

///TIME_START

			  HStr.assign(ncnr,0);   HStS.assign(ncnr,0);

	  if ( NSumm==1)  {                                                  // != 0
			  // в HStr кладём горизонтальные суммы
			  // в HStS кладём горизонтальные дифференциальные суммы
			  for ( y=0; y < nrows; y++ ) {      // read image make sum
				res=resD=0;  d1=A+y*w;
				for ( x=0; x < ncolumns; x++ ) { // подсчет  горизонтальных сумм
					d2=d1+x;       res+=*d2;
					d=*d2-*(d2+2); resD+=d;      // (d2+1)  //d=*(d1+x);
					//DM(*d2); ///
				}
				HStr[y]=res;
				HStS[y]=resD;  /// <<4 <<5
//              drawData[0][y][x]=255+d*196;
				//DM(" res "<<res); DM(END); ///
			  }

///DM(" горизонтальные суммы ");
///TIME_PRINT

		 } // if

//      else { HBuf=HStr; GBuf=HStS; }


#endif
#endif
// вертикальные и горизонтальные локальные сумммы


#ifdef DEBUGLVL
   // ВИЗУАЛИЗАЦИЯ аналоговая горизонтальных сумм (черная)
   int M=8; static short delta=50;
   for(y=0; y < drawData->rows(); y++) {
	if(HStr[y]>=0){ for (x=delta; x<(HStr[y]/M)+delta; x++)drawData[0][y][x]*=0.1; }
	else          { for (x=delta; x>(HStr[y]/M)+delta; x--)drawData[0][y][x]*=0.1; }

   } // y++  // 0.8*ncolumns;

#endif


}//_____________________________________________________________________________

*/
/**/


//##
//                                                     *
//                                                     *                                                       *
// Визуализация аналоговая вертикальных графиков L     *
// -----------------------------------------------------------------------------

void GBitsetMatrix::LDraw(vector<int> &HBuf, int sm, int color){

#ifdef DEBUGLVL
#ifdef WIN32_DRAW
 int y,x,in,m;
 int size=HStr.size();
 TPoint ris[25000];
 // ВАЖНО - сумма вдоль текста HBuf[y] поделена на 8 и на 4  (8*4=32)
				   m=32;
		   for (  y=0; y < nrows; y++ ) {
				   x=sm + HBuf[y]/m;
				   ris[y].x=x;

				   ris[y].y=nrows-y;  //заполнение массива координат линии

		   } // for y
		in=ImageProcessor->ImageEnView1->AddNewObject();
		ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)color;
		ImageProcessor->ImageEnView1->ObjPenWidth[in]=1,0; // 0,5
		ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
		ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,nrows-2);

		ImageProcessor->ImageEnView1->Update();
#endif
#endif
 // size=nrows;
}//_____________________________________________________________________________



//                                                                      *

//                                                                      *                                                       *
// Визуализация аналоговая вертикальных графиков H переменной длинны    *
// -----------------------------------------------------------------------------

void GBitsetMatrix::LDrawVar(vector<int> &HBuf, int HDrawSize, int sm, int scope, int color){

#ifdef DEBUGLVL
#ifdef WIN32_DRAW
 int y,x,in,m;
 TPoint ris[25000];

 // WDrawSize - размер, sm - смещение по вертикали,   scope - масштаб=32, color - цвет
 // ВАЖНО - сумма вдоль текста HBuf[y] поделена на 8 и на 4  (8*4=32)
           if(scope<=0) scope = 1;  //  m=32;
           for (  y=0; y < HDrawSize; y++ ) {
                   x=sm + HBuf[y]/scope;
                   ris[y].x=x;

                   ris[y].y=HDrawSize-y;  //заполнение массива координат линии

           } // for y
		in=ImageProcessor->ImageEnView1->AddNewObject();
		ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)color;
		ImageProcessor->ImageEnView1->ObjPenWidth[in]=0,5; // 0,5
		ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
		ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,HDrawSize-2);

		ImageProcessor->ImageEnView1->Update();
#endif
#endif

}//_____________________________________________________________________________

//##

//                                                    |

//                                                    |

//                                                    |                                                        *

// Визуализация аналоговая вертикальных линий         |
// ---------------------------------------------------------------------------

void GBitsetMatrix::LineVertical(int sm, int color){

#ifdef DEBUGLVL
#ifdef WIN32_DRAW
 int in;
 TPoint ris[4];   // 4

        //заполнение массива координат линии
        ris[0].x=sm;      ris[1].x=sm;    ris[2].x=sm;
        ris[0].y=nrows;   ris[1].y=0;     ris[2].y=0;
		in=ImageProcessor->ImageEnView1->AddNewObject();
		ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)color;
		ImageProcessor->ImageEnView1->ObjPenWidth[in]=0,1;
		ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
		ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,2);

		ImageProcessor->ImageEnView1->Update();

 #endif
 #endif

}//_____________________________________________________________________________

//##


// Визуализация аналоговая горизонаальных графиков W  *******

// -----------------------------------------------------------------------------


void GBitsetMatrix::WDraw(vector<int> &HBuf, int sm, int color){ //  LStr

#ifdef DEBUGLVL
#ifdef WIN32_DRAW
 int y,x,in,m,size;
 TPoint ris[50000];
 // ВАЖНО - сумма вдоль текста HBuf[y] поделена на 8 и на 4  (8*4=32)
           //заполнение массива координат линии
                  m=16; sm=nrows-sm;
           for (  x=0; x < ncolumns; x++ ) {
                   y=sm - HBuf[x]/m;
                   ris[x].y=y;   // амплитуда

                   ris[x].x=x;   // горизонтальная разертка

           } // for x
		in=ImageProcessor->ImageEnView1->AddNewObject();
		ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)color;
		ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
		ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,ncolumns-2);

		ImageProcessor->ImageEnView1->Update();
 #endif
 #endif
}//_____________________________________________________________________________

//##


// Визуализация аналоговая горизональных графиков W переменной длинны ******* **

// -----------------------------------------------------------------------------


void GBitsetMatrix::WDrawVar(vector<int> &HBuf, int WDrawSize, int sm, int scope, int color){

#ifdef DEBUGLVL
#ifdef WIN32_DRAW

 // WDrawSize - размер, sm - смещение по вертикали,   scope - масштаб=8, color - цвет
 int y,x,in,size;
 TPoint ris[50000];
 // ВАЖНО - сумма вдоль текста HBuf[y] поделена на 8 и на 4  (8*4=32)
           //заполнение массива координат линии
           if(scope<=0) scope = 1;
           sm=nrows-sm;
           for (  x=0; x < WDrawSize; x++ ) {
                   y=sm - HBuf[x]/scope;
                   ris[x].y=y;   // амплитуда

                   ris[x].x=x;   // горизонтальная разертка

           } // for x
		in=ImageProcessor->ImageEnView1->AddNewObject();
		ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)color;
		ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
		ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,WDrawSize-2);

		ImageProcessor->ImageEnView1->Update();
 #endif
 #endif
}//_____________________________________________________________________________

//##

//                                                       * * *

//                                                      *     *

//                                                      *     *

// Визуализация аналоговая полярные координаты           * * *

// -----------------------------------------------------------------------------


// Polar coordinates  полярные координаты
void GBitsetMatrix::PolarCoordinates(vector<int> &HBuf,
                               vector<int> &GBuf,
                               int LDrawSize,
                               int smX,  int smY,
                               int scope,int color){

#ifdef DEBUGLVL
#ifdef WIN32_DRAW

 // WDrawSize - круговая длинна , smY - смещение по вертикали,   scope - масштаб=8, color - цвет
 int y,x,i,in,size;
 TPoint ris[50000];

           if(nrows<=ncolumns) { size = nrows; }
           else { size = ncolumns; }

           if(scope<=0) scope = 1;
           //smY=nrows-smY;
           //заполнение массива координат линии
           for (  i=0; i < size; i++ ) {
                   y=smY - GBuf[i]/scope;
                   x=smX - HBuf[i]/scope;
                   ris[i].y=y;   // вертикальная разертка

                   ris[i].x=x;   // горизонтальная разертка

           } // for i
		in=ImageProcessor->ImageEnView1->AddNewObject();
		ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)color;
		ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
		ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,size-2);

		ImageProcessor->ImageEnView1->Update();
 #endif
 #endif
}//_____________________________________________________________________________





//                                                                   ***********

//                                                                   ***********

// Визуализация аналоговая  строк (прямоугольников) в структурах     ***********
// -----------------------------------------------------------------------------

void GBitsetMatrix::stringDraw(int color){
#ifdef DEBUGLVL
#ifdef WIN32_DRAW
int in;
TPoint ris[9]; // 6

           // заполнение массива координат прямоугольника
		   for (int m=0; m < strArray->size(); m++ )
              {
                   ris[0].x=strArray[0][m].x0;
                   ris[0].y=nrows-strArray[0][m].y0;

                   ris[1].x=strArray[0][m].x0;

                   ris[1].y=nrows-strArray[0][m].y1;

                   ris[2].x=strArray[0][m].x1;

                   ris[2].y=nrows-strArray[0][m].y1;

                   ris[3].x=strArray[0][m].x1;

                   ris[3].y=nrows-strArray[0][m].y0;

                   ris[4].x=strArray[0][m].x0;

                   ris[4].y=nrows-strArray[0][m].y0;

                   //ris[5].x=strArray[0][m].x0;

                   //ris[5].y=nrows-strArray[0][m].y0;

                    in=ImageProcessor->ImageEnView1->AddNewObject();

                    ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)color; // цвет линии
                    ImageProcessor->ImageEnView1->ObjPenWidth[in]=0,1;  // толщина линии
                    ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
                    ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,4); // 5
           } // m
           ImageProcessor->ImageEnView1->Update();
// x0,y0,x1,y1
// x0,y0 координаты левого нижнего угла прямоугольника строки (глифа, слога, буквы).
// x1,y1 коорддинаты правого верхнего угла прямоугольника строки (глифа, слога, буквы).
#endif
#endif
}//_____________________________________________________________________________



//                                             ***  **  ***

//                                             **   **  **
//                                             ***  **  ***
//                                             ***  *   ***
//                                             * *  **  ***
//                                             ***  **  ***

// Визуализация прямоугольников вертикальных колонок в структурах в пределах разворота.
// -----------------------------------------------------------------------------

void GBitsetMatrix::frameDraw(int color){

#ifdef DEBUGLVL
#ifdef WIN32_DRAW  //
int in;
int x0,x1,y0,y1;    // координаты прямоугольника вокруг колонок
int nFrm,nFrame, nClm,nColumn;            //, Mean

TPoint ris[9];     // количество координат для прямоугольника 10 (0,1,2...9)=9
//...(in,ris,4);   // количество узлов для прямоугольника      5 (0,1,2,3,4)=4
int MeanCol=16; //____________________


         nFrame = frameArray.size();
         for ( nFrm=0; nFrm < nFrame; nFrm++ ){ // Цикл по количеству колонок nFrame
              frameOCR *wF=&frameArray[nFrm];

      ////
              // заполнение массива горизонтальными блоками описывающих
              // многоугольники колонок
              nColumn=frameArray[nFrm].columnArray.size();
              for ( nClm=0; nClm < nColumn; nClm++ ){ // Цикл по количеству блоков колонок Clm
				  columnOCR *wC=&frameArray[nFrm].columnArray[nClm];
				  x0=wC->xc0;   x1=wC->xc1;   y0=wC->yc0;   y1=wC->yc1;

                   ris[0].x=x0;
				   ris[0].y=nrows-y0;

                   ris[1].x=x0;

				   ris[1].y=nrows-y1;

				   ris[2].x=x1;

                   ris[2].y=nrows-y1;

                   ris[3].x=x1;

                   ris[3].y=nrows-y0;

                   ris[4].x=x0;

				   ris[4].y=nrows-y0;

					in=ImageProcessor->ImageEnView1->AddNewObject();
					if(wF->frameFlag>0){
					ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)0xff1400;
					}  // цвет линии светло синий
					ImageProcessor->ImageEnView1->ObjPenWidth[in]=MeanCol/6; // толщина линии
                    ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
                    ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,4);
                  } // nClm
/**/  ////

//
              // заполнение массива нешними габаритами прямоугольника вокруг
              // многоугольника вертикальной колонки
              //x0=wF->xf0;   x1=wF->xf1;   y0=wF->yf0;   y1=wF->yf1;
              x0=wF->xfg0;   x1=wF->xfg1;   y0=wF->yfg0;   y1=wF->yfg1;
              
                   ris[0].x=x0;
                   ris[0].y=nrows-y0;

                   ris[1].x=x0;

                   ris[1].y=nrows-y1;

                   ris[2].x=x1;

                   ris[2].y=nrows-y1;

                   ris[3].x=x1;

                   ris[3].y=nrows-y0;

                   ris[4].x=x0;

                   ris[4].y=nrows-y0;


                    in=ImageProcessor->ImageEnView1->AddNewObject();
//                  DM(wF->frameFlag);
                    if(wF->frameFlag>0){
					ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)0xffbb00;
					}  // цвет линии светло синий
					ImageProcessor->ImageEnView1->ObjPenWidth[in]=MeanCol/6; // толщина линии
                    ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
                    ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,4); //3
//DM("yfg0="<<y0<<" yfg1="<<y1<<" xfg0="<<x0<<" xfg1="<<x1<<END);
/**/


/*      ////
              nColumn=frameArray[nFrm].columnArray.size();
              for ( nClm=0; nClm < nColumn; nClm++ ){
                  columnOCR *wC=&frameArray[nFrm].columnArray[nClm];
                  x0=wC->xc0;   x1=wC->xc1;   y0=wC->yc0;   y1=wC->yc1;

// заполнение массива координат векторов вокруг колонок
// работает, отображается на графике, но не дописана.
// TPoint ris[3];   // количество координат для вектора 4 (0,1,2,3)=3
// ...(in,ris,1);   // количество узлов для вектора     2     (0,1)=1
                   ris[0].x=x0;
                   ris[0].y=nrows-y0;
                   ris[1].x=x0;

                   ris[1].y=nrows-y1;

                    in=ImageProcessor->ImageEnView1->AddNewObject();
                    if(wF->frameFlag>0){
                    ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)0xff1400;
                    }  // цвет линии светло синий
                    ImageProcessor->ImageEnView1->ObjPenWidth[in]=MeanCol/3; // толщина линии
                    ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
                    ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,1);
                  } // nClm
*/  ////
         } // nFrm
         ImageProcessor->ImageEnView1->Update();
/**/
// цвет линии красный  0x000000FF,  светло синий  0xff1400,  салатовый  0xffff00

// x0,y0,x1,y1
// x0,y0 координаты левого нижнего угла прямоугольника строки (глифа, слога, буквы).
// x1,y1 координаты правого верхнего угла прямоугольника строки (глифа, слога, буквы).

// Визуализация прямоугольниками в структурах, универсальная.
// void GBitsetMatrix::quadrateDraw(int color, int thickness){

#endif    //
#endif

}//_____________________________________________________________________________



//                                             ***  **  ***


// Визуализация прямоугольников вертикальных колонок в структурах строк в пределах разворота.
// структура заполненена горизонтальными блоками L описывающих многоугольники колонок
// -----------------------------------------------------------------------------

void GBitsetMatrix::lineDraw(int color){

#ifdef DEBUGLVL
#ifdef WIN32_DRAW  //
int strInd,m,in;
int x0,x1,y0,y1;        // координаты прямоугольника вокруг блоков слов (колонок)
int NString, Mean;
int MaxString=60; // 10 //MaxString - ограничение на колличество выводимых на эран строк
TPoint ris[9];          // ( если больше графический порт вешается)
int MeanCol=16; //____________________


         // заполнение массива координат прямоугольника
         NString = strArray->size();
         if ( NString > MaxString ) NString=MaxString;

         for ( strInd=0; strInd < NString; strInd++ ){
           Mean = strArray[0][strInd].MeanSA;
		   for ( m=0; m < strArray[0][strInd].lineArray.size(); m++ ){
            lineOCR *wL=&strArray[0][strInd].lineArray[m];
                   x0=wL->xL0;   x1=wL->xL1;   y0=wL->yL0;   y1=wL->yL1;

                   ris[0].x=x0;
                   ris[0].y=nrows-y0;

                   ris[1].x=x0;

                   ris[1].y=nrows-y1;

                   ris[2].x=x1;

                   ris[2].y=nrows-y1;

                   ris[3].x=x1;

                   ris[3].y=nrows-y0;

                   ris[4].x=x0;

                   ris[4].y=nrows-y0;

//                   ris[5].x=x0;

//                   ris[5].y=nrows-y0;

                    in=ImageProcessor->ImageEnView1->AddNewObject();
//                  DM(wL->lineFlag);
                    if(wL->lineFlag>0){
                    ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)0x00ff00;
                    }  // цвет линии зеленый
                    ImageProcessor->ImageEnView1->ObjPenWidth[in]=MeanCol/12; // толщина линии
                    ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
                    ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,4);
           } // m
         } // strInd
         ImageProcessor->ImageEnView1->Update();
/**/
// цвет линии красный  0x000000FF,  светло синий  0xff1400,  салатовый  0xffff00

// x0,y0,x1,y1
// x0,y0 координаты левого нижнего угла прямоугольника строки (глифа, слога, буквы).
// x1,y1 координаты правого верхнего угла прямоугольника строки (глифа, слога, буквы).


#endif    //
#endif

// Подпрограмма для получения координат блоков слов в пределах страницы.

}//_____________________________________________________________________________



//                                                    ******************

//                                                    *                *

//                                                    ******************

// Визуализация аналоговая  прямоугольников в структурах для блоков букв
// -----------------------------------------------------------------------------

void GBitsetMatrix::blockDraw(int color){

#ifdef DEBUGLVL
#ifdef WIN32_DRAW  //
int strInd,m,in;
int x0,x1,y0,y1;          // координаты прямоугольника вокруг блоков букв
int NString, Mean;
int MaxString=60; // 10 //MaxString - ограничение на колличество выводимых на эран строк
TPoint ris[9];            // ( если больше графический порт вешается)



         // заполнение массива ккоординат прямоугольника
         NString = strArray->size();
         if ( NString > MaxString ) NString=MaxString;

         for ( strInd=0; strInd < NString; strInd++ ){
           Mean = strArray[0][strInd].MeanSA;
		   for ( m=0; m < strArray[0][strInd].blockArray.size(); m++ ){
            blockOCR *wB=&strArray[0][strInd].blockArray[m];
                   x0=wB->xb0;   x1=wB->xb1;   y0=wB->yb0;   y1=wB->yb1;

                   ris[0].x=x0;
                   ris[0].y=nrows-y0;

                   ris[1].x=x0;

                   ris[1].y=nrows-y1;

                   ris[2].x=x1;

                   ris[2].y=nrows-y1;

                   ris[3].x=x1;

                   ris[3].y=nrows-y0;

                   ris[4].x=x0;

                   ris[4].y=nrows-y0;

                   //ris[5].x=x0;

                   //ris[5].y=nrows-y0;

                    in=ImageProcessor->ImageEnView1->AddNewObject();
//                  DM(wB->langFlag);
/*
                    if(wB->langFlag==0){
                    ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)0x000000FF;
                    }  // цвет линии красный
                    if(wB->langFlag==1){
                    ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)0xffff00;
                    }  // цвет линии салатовый
                    if(wB->langFlag==2){
                    ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)0xff1400;
                    }  // цвет линии светло синий
*/
//                  DM(wB->blockFlag);
					if(wB->blockFlag==0){ // блоки вне колонок
                    ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)0x000000FF;
					}  // цвет линии красный
					if(wB->blockFlag>0 && wB->blockFlag<1000 ){ // блоки внутри колонок
					ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)0x00ff00;
					}  // цвет линии зеленый
/*
					if(wB->blockFlag>1000){ // пустые блоки
					ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)0x00fff0;
					}  // цвет линии желтый
*/
					ImageProcessor->ImageEnView1->ObjPenWidth[in]=Mean/8;;  // толщина линии
                    ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
                    ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,4);
           } // m
         } // strInd
		 ImageProcessor->ImageEnView1->Update();
/**/


// x0,y0,x1,y1
// x0,y0 координаты левого нижнего угла прямоугольника строки (глифа, слога, буквы).
// x1,y1 координаты правого верхнего угла прямоугольника строки (глифа, слога, буквы).


#endif    //
#endif
}//_____________________________________________________________________________


//                                                           *******

//                                                           *     *

//                                                           *******

// Визуализация аналоговая  прямоугольников в структурах для букв
// -----------------------------------------------------------------------------

void GBitsetMatrix::wordDraw(int color){

#ifdef DEBUGLVL
#ifdef WIN32_DRAW  //
int strInd,m,in,strFlag;
int x,x0,x1,y0,y1;        // ,xt0,yt0,xt1,yt1
int NString,MaxString=60; // MaxString - ограничение на колличество выводимых на эран строк
TPoint ris[9];            // ( если больше графический порт вешается) 6

// x0,x1, y0,y1     координаты квадрата вокруг буквы


         // заполнение массива координат прямоугольника
         NString = strArray->size();
		 if ( NString > MaxString ) NString=MaxString;

		 for ( strInd=0; strInd < NString; strInd++ ){
		 strFlag=strArray[0][strInd].stringFlag;
		   // если stringFlag==0 то строку не заполняем квадратиками букв (в строке линия)
		   for ( m=0; strFlag==1 && m < strArray[0][strInd].wordArray.size(); m++ ){
			wordOCR *wP=&strArray[0][strInd].wordArray[m];
				   x0=wP->x0;   x1=wP->x1;   y0=wP->y0;   y1=wP->y1;

				   ris[0].x=x0;
				   ris[0].y=nrows-y0;

				   ris[1].x=x0;

				   ris[1].y=nrows-y1;

				   ris[2].x=x1;

				   ris[2].y=nrows-y1;

				   ris[3].x=x1;

				   ris[3].y=nrows-y0;

				   ris[4].x=x0;

				   ris[4].y=nrows-y0;

				   //ris[5].x=x0;

				   //ris[5].y=nrows-y0;

					in=ImageProcessor->ImageEnView1->AddNewObject();
//                    DM(wP->stackFlag);
					if(wP->stackFlag>0){
					ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)0xffff00; // цвет линии салатовый
					}else{
					ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)0xff1400; // цвет линннии светло синий
					}
					ImageProcessor->ImageEnView1->ObjPenWidth[in]=1;  // толщина линии
					ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
					ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,4); // 5
		   } // m
		  //} // if
		 } // strInd
		 ImageProcessor->ImageEnView1->Update();
/**/


// x0,y0,x1,y1
// x0,y0 координаты левого нижнего угла прямоугольника строки (глифа, слога, буквы).
// x1,y1 координаты правого верхнего угла прямоугольника строки (глифа, слога, буквы).


#endif    //
#endif
}//_____________________________________________________________________________


//                                                           ******

//                                                           *    *

//                                                           ******

// Вииизуализация аналоговая  прямоугольников в структурах для точек
// -----------------------------------------------------------------------------

void GBitsetMatrix::pointDraw(int color){

#ifdef DEBUGLVL
#ifdef WIN32_DRAW  //
int strInd,in,m,Lp,Cp;
int x0,x1,y0,y1;
int NString,MaxString=60; // MaxString - ограничение на колличество выводдимых на эран строк
TPoint ris[9];            // ( если больше графический порт вешается)
//// DM("NhIm");

         // заполнение массива координат прямоугольника
         NString = strArray->size();
         if ( NString > MaxString ) NString=MaxString;

         for ( strInd=0; strInd < NString; strInd++ ){
		   for ( m=0; m < strArray[0][strInd].wordArray.size(); m++ ){
                   wordOCR *wP=&strArray[0][strInd].wordArray[m];
                   x0=wP->xp0;   x1=wP->xp1;   y0=wP->yp0;   y1=wP->yp1;

                   ris[0].x=x0;
                   ris[0].y=nrows-y0;

                   ris[1].x=x0;

                   ris[1].y=nrows-y1;

                   ris[2].x=x1;

                   ris[2].y=nrows-y1;

                   ris[3].x=x1;

                   ris[3].y=nrows-y0;

                   ris[4].x=x0;

                   ris[4].y=nrows-y0;

                   //ris[5].x=x0;

                   //ris[5].y=nrows-y0;
                    Lp=0.5; Cp=color;  // толщина линии, цвет
                    if ( strArray[0][strInd].wordArray[m].tsertoFlag ) // != 0
                    { Lp=1.5; Cp=0x0000AA00; } // зеленый
                    if ( strArray[0][strInd].wordArray[m].spaceFlag )
                    { Lp=2.5; Cp=0xAF00AA00; } // светло серый
                    in=ImageProcessor->ImageEnView1->AddNewObject();
                    ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)Cp; // цвет линии
                    ImageProcessor->ImageEnView1->ObjPenWidth[in]=Lp; // толщина линии
                    ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
                    ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,4);
           } // m
         } // strInd
         ImageProcessor->ImageEnView1->Update();


// x0,y0,x1,y1
// x0,y0 координаты левого нижнего угла прямоугольника строки (глифа, слога, буквы).
// x1,y1 координаты правого верхнего угла прямоугольника строки (глифа, слога, буквы).


/**/
#endif    //
#endif
}//_____________________________________________________________________________





//                                                           ******

//                                                           ******


// Визуализация аналоговая  прямоугольников в структурах для огласовок
// -----------------------------------------------------------------------------

void GBitsetMatrix::vowelDraw(int color){

#ifdef DEBUGLVL
#ifdef WIN32_DRAW  //
int strInd,m,in;
int x0,x1,y0,y1;        // xv0,xv1, yv0,yv1
int NString,MaxString=60; // 10MaxString - ограничение на колличество выводимыых на эран строк
TPoint ris[9];            // ( если больше графический порт вешается)


// xv0,xv1, yv0,yv1; координаты прямоугольника вокруг огласовоки


         // заполнение массива координат прямоугольника
         NString = strArray->size();
         if ( NString > MaxString ) NString=MaxString;

         for ( strInd=0; strInd < NString; strInd++ ){
		   for ( m=0; m < strArray[0][strInd].wordArray.size(); m++ ){
            wordOCR *wP=&strArray[0][strInd].wordArray[m];
                   x0=wP->xv0;   x1=wP->xv1;   y0=wP->yv0;   y1=wP->yv1;

                   ris[0].x=x0;
                   ris[0].y=nrows-y0;

                   ris[1].x=x0;

                   ris[1].y=nrows-y1;

                   ris[2].x=x1;

                   ris[2].y=nrows-y1;

                   ris[3].x=x1;

                   ris[3].y=nrows-y0;

                   ris[4].x=x0;

                   ris[4].y=nrows-y0;

                   //ris[5].x=x0;

                   //ris[5].y=nrows-y0;

                    in=ImageProcessor->ImageEnView1->AddNewObject();
//                  DM(wP->vowelFlag);
                    if(wP->vowelFlag>0){
                    ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)0xffff00; // цвет линии
                    }else{
                    ImageProcessor->ImageEnView1->ObjPenColor[in]=(TColor)0xff1400;
                    }
                    ImageProcessor->ImageEnView1->ObjPenWidth[in]=1;  // толщина линии
                    ImageProcessor->ImageEnView1->ObjKind[in]=iekPOLYLINE;
                    ImageProcessor->ImageEnView1->SetObjPolylinePoints(in,ris,4);
           } // m
         } // strInd
		 ImageProcessor->ImageEnView1->Update();
/**/


// x0,y0,x1,y1
// x0,y0 координаты левого нижнего угла прямоугольника строки (глифа, слога, буквы).
// x1,y1 координаты правого верхнего угла прямоугольника строки (глифа, слога, буквы).


#endif    //
#endif
}//_____________________________________________________________________________





//##

// Sorting  сортировка

// ---------------------------------------------------------------------------

void GBitsetMatrix::Sort(vector<int> &GBuf,int NSort){

   //int y,x;
 // сортировка
   int trash=0;
   bool f=true;
   int i,j,Nff;

					//  DM(NSort<<" NSort+ ");
					if( NSort==0) { return; }
  /*
#ifdef DEBUGLVL
			  DM(END);  DM(END);
			  for ( x=0; x < NSort;  x++ ){      // x < d+1;
                DM(GBuf[x]<<"\t"); if (x%10==0)DM(END);  /// ТАБЛИЦА
				  }//for x
              DM(END);
#endif
  */

                        // "сортировка Шейкера".
				 // http://www.abc-it.lv/index.php/id/610

							  Nff=NSort;  // -1 ////
	 for ( i=1; (i<=Nff) && (f=true) ; i++)
	   {
		  f=false;
		  for ( j=i; j<=Nff-i; j++) // проходим с лева на право
             {
				if (GBuf[j]>GBuf [j+1]) // если число слева больше числа
			   {
				  trash=GBuf[j]; // справа, то меняем местами
                  GBuf[j]=GBuf [j+1]; // справа собираются большие числа
				  GBuf[j+1]=trash;
				  f=true;
			   }
			 }
			   for ( j=Nff-i-1; j>i ; j--)  // проходим с права на лево
				 {
				   if (GBuf[j]<GBuf[j-1])  // если число справа меньше числа
				   {
				   trash=GBuf[j]; // слева, то меняем местами
                   GBuf[j]=GBuf[j-1]; // слева собираются меньшие числа
				   GBuf[j-1]=trash;
				   f=true;
				 }
                 }
			 }
/*
#ifdef DEBUGLVL
			  DM(END);  DM(END);
			  for ( x=0; x < NSort;  x++ ){      // x < d+1;
				DM(GBuf[x]<<"\t"); if (x%10==0)DM(END);  /// ТАБЛИЦА
				  }//for x
			  DM(END);
#endif
  */
}//_____________________________________________________________________________








////////////////////////////////////////////////////////////////////////////////


 /*

http://window.edu.ru/window_catalog/pdf2txt?p_id=22254&p_page=3


Статические и динамические массивы

Статическим массивом называют набор данных, для хранения которого перед
началом функционирования программы выделяется фиксированное место в
памяти, освобождаемое после завершения работы программы.
В отличие от этого место для хранения динамических массивов выделяется и
освобождается в процессе выполнения программы. В одних случаях эти операции
осуществляются системой автоматически.
Например, когда отводится память для хранения локальных массивов в
процедуурах и функциях. В других случаях пользователю предоставляется
возможность запросить участок памяти нужного размера и в дальнейшем -
освободить его. Только таким способом в программах можно завести массив
переменного размера.
В Си для запроса и освобождения памяти используются следующие системные
функциии:
q=(тип_q *)calloc(n_el,s_el);        //запрос памяти с очисткой;
q=(тип_q *)farcalloc(n_el,s_el);     //запрос памяти с очисткой;
q=(тип_q *)malloc(n_byte);           //запрос памяти в ближней "куче"
q=(тип_q *)farmalloc(n_byte); //запрос памяти в дальней "куче"
q_new=realloc(q_old,n_byte); //изменение размера блока
q_new=farrealloc(q_old,n_byte);      //изменение размера блока
free(q);                             //освобождение памяти
farfree(q);                    //освобождение памяти
В приведенных выше обращениях q обозначает указатель на тип данных
элементов массива, заменяющий имя массива. Параметры n_el и s_el задают
соответственно количество элементов в массиве и длину каждого элемента в
байтах. Параметр n_byte определяет количество запрашиваемых байтов.
Максимальный размер сегмента памяти, предоставляемого в ближней "куче",
равен 65521 байт. Добавка far означает, что программа использует дальние
указатели типа far или huge, которые позволяют адресоваться к дальней "куче" и
использовать сегменты размером более 64 кБ. Любая функция выделения памяти
возвращает начальный адрес или "нулевой" указатель (NULL) в случае отсутствия
свободной памяти запрашиваемого размера. Для того чтобы нормально работать с
предоставленным фрагментом памяти, возвращаемый адрес обязательно должен
быть приведен к типу указателя q.
Функция realloc (farrealloc) позволяет перераспределить ранее выделенную
память. При этом новый размер массива может быть как меньше предыдущего,
так и больше его. Если система выделит память в новом месте, то все предыдущие
значения, к которым программа обращалась по указателю q_old будут переписаны
на новое место автоматически.
В новых версиях Borland C++ появились две более удобные процедуры для
запроса и освобождения памяти, не нуждающиеся в дополнительном указании о
приведении типа возвращаемого адреса:
q=new тип[n_el];  //запрос памяти под массив из n_el элементов;
q=new тип;        //запрос памяти под скалярную переменную;

                                      26


delete q[n_el];            //освобождение памяти, занятой массивом;
delete q;                  //освобождение памяти, занятой массивом или
                           //скалярной переменной;
Процедура освобождения памяти не чистит указатель, "смотревший" на начало
возвращаемого сегмента. Запись по такому указателю после возврата памяти
приводит к трудно обнаруживаемым ошибкам. Поэтому к правилам "хорошего
тона" в программировании относится и сброс указателей после возврата
динамически ззапрашивавшейся памяти:
q=NULL;
Не менее хорошее правило заключается и в проверке, выделена ли запрашиваемая
память после обращения к соответствующей процедуре. Например, в Си, не
контролирующем запись по нулевому адресу, после стирания нескольких первых
элементов несуществующего массива происходит зависание операционной
системы.
При динамическом распределении памяти для массивов следует описать
соответствующий указатель и присваивать ему значение при помощи функции
calloc. Одномерный массив a[10] из элементов типа float можно создать
следующим образом
 float *a;
 a=(float*)(calloc(10,sizeof(float));// a=(float*)malloc(10*sizeof(float));
 if (!a) // if (a==NULL)
 {printf (“out of memory\n); //выход за пределы памяти
         exit (1);
         }
Аналогично может быть распределена память и для 2-мерного массива N?M
 a=(float*)(calloc(N*M,sizeof(float));// a=(float*)malloc(N*M*sizeof(float));
В этом случае он рассматривается как аналог одномерного массива из N?M
элементов.
Для создания по- настоящему двумерного массива вначале нужно распределить
память для массива указателей на одномерные массивы, а затем распределять
память для одномерных массивов Пусть, например, требуется создать массив

a[n][m], это можно сделать при помощи следующего фрагмента программы:


 
   main ()
       {  double **a;
          int n,m,i;
          scanf("%d %d",&n,&m);
          a=(double **)calloc(m,sizeof(double *));
          for (i=0; i<=m; i++) a[i]="(double" *)
          calloc(n,sizeof(double)); . . . . } 
Аналогичным образом можно распределить память и для трехмерного массива
размером n,m,l. Следует только помнить, что ненужную для дальнейшего выполнения
программы память следует освобождать при помощи функции free.


 
   main ()
       {  long ***a;
          int n,m,l,i,j;
          scanf("%d %d %d",&n,&m,&l);
//           --------  распределение памяти --------
          a=(long ***)calloc(m,sizeof(long **));
          for (i=0; i<=m; i++) 
          { a[i]="(long" **)calloc(n,sizeof(long *)); 
          for (j="0;" i<="l;" j++)
          a[i][j]="(long" *)calloc(l,sizeof(long)); } . . .  
//           освобождение памяти
          for (i="0;" i<="m;" i++) 
          { for (j="0;" j<="l;" j++) 
          free (a[i][j]); free (a[i]);} free (a);}

Рассмотрим еще один интересный пример, в котором память для массивов
распределяется в вызываемой функции, а используется в вызывающей. В таком
случае в вызываемую функцию требуется передавать указатели, которым будут
присвоены адреса выделяемой для массивов памяти.

Пример:


  
   main()
   {   int vvod(double ***, long **);
       double **a;    //  указатель для массива  a[n][m]
       long *b;       //   указатель для массива  b[n]
       vvod (&a,&b);
        ..   // в функцию vvod передаются адреса указателей,
        ..   // а не их значения
        ..
    }
    int vvod(double ***a, long **b)
    {   int n,m,i,j;
        scanf (" %d %d ",&n,&m);
        *a=(double **)calloc(n,sizeof(double *));
        *b=(long *)calloc(n,sizeof(long));
         for (i=0; i<=n; i++) 
        *a[i]="(double" *)calloc(m,sizeof(double)); ..... } 
Отметим также то обстоятельство, что указатель на массив не обязательно должен

показывать на начальный элемент некоторого массива. Он может быть сдвинут так,

что начальный элемент будет иметь индекс отличный от нуля, причем он может быть

как положительным так и отрицательным.




Динамическое размещение массивов

Пример:

    int main()
    {  float *q, **b;
       int i, j, k, n, m;
       scanf("%d %d",&n,&m);
       q=(float *)calloc(m,sizeof(float));
    // сейчас указатель q показывает на начало массива
       q[0]=22.3;
       q-=5;
    // теперь начальный элемент массива имеет индекс 5,
    // а конечный элемент индекс n-5
       q[5]=1.5;
    // сдвиг индекса не приводит к перераспределению
    // массива в памяти и изменится начальный элемент
       q[6]=2.5;   /*  -  это второй элемент
       q[7]=3.5;   /*  -  это третий элемент
       q+=5;
    // теперь начальный элемент вновь имеет индекс 0,
    // а значения элементов q[0], q[1], q[2] равны
    // соответственно 1.5, 2.5, 3.5
       q+=2;
    // теперь начальный элемент имеет индекс -2,
    // следующий -1, затем 0 и т.д. по порядку
       q[-2]=8.2;
       q[-1]=4.5;
       q-=2;
    // возвращаем начальную индексацию, три первых
    // элемента массива q[0], q[1], q[2], имеют
    // значения 8.2, 4.5, 3.5
       q--;
    // вновь изменим индексацию .
    // Для освобождения области памяти в которой размещен
    // массив q используется функция free(q), но поскольку
    // значение указателя q смещено, то выполнение
    // ф-я free(q) приведет к непредсказуемым последствиям
    // Для правильного выполнения этой функции
    // указатель q должен быть воозвращен в первоначальное
    // положение
       free(++q);
    // Рассмотрим возможность изменения индексации и
    // освобождения памяти для двумерного массива
       b=(float **)calloc(m,sizeof(float *));
       for (i=0; i < m; i++)
           b[i]=(float *)calloc(n,sizeof(float));
    // После распределения памяти начальным элементом
    // массива будет элемент b[0][0]
    // Выполним сдвиг индексов так, чтобы начальным
    // элементом стал элемент b[1][1]
       for (i=0; i < m ; i++) --b[i];
       b--;
    // Теперь присвоим каждому элементу массива сумму его
    // индексов
       for (i=1; i<=m; i++) for (j="1;" j<="n;" j++) 
       b[i][j]="(float)(i+j);" 
    // Обратите внимание на начальные значенииия счетчиков
    // циклов i и j, он начинаются с 1 а не с 0
    // Возвратимся к прежней индексации
    for (i="1;" i<="m;" i++) ++b[i]; b++; 
    //Выполним освобождение памяти
    for (i="0;" i < m; i++) free(b[i]); free(b); ... 
    return 0; } 
В качестве последнего примера рассмотрим динамическое распределение памяти
для массива указателей на функции, имеющие один входной параметр типа double и
возвращающие значение типа double.

Пример:


    double cos(double);
    double sin(double);
    double tan(double);
   int main()
   { double (*(*masfun))(double);
     double x=0.5, y;
     int i;
     masfun=(double(*(*))(double))
              calloc(3,sizeof(double(*(*))(double)));
     masfun[0]=cos;
     masfun[1]=sin;
     masfun[2]=tan;
     for (i=0; i<3; i++); { y="masfun[i](x);" 
     printf("\n x="%g" y="%g",x,y);" } 
     return 0; } 



..............


*/

