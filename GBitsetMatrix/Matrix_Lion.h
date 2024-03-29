//                           Поиск триадами

// Подпрограмма получения максимума коэффициента корреляции по маске, в пределах
// прямоугольника поиска. Поиск триадами.
// -----------------------------------------------------------------------------
int GBitsetMatrix::setMatrix_Lion(GBitMask32 *mask32,
										int xSrc0,
										int ySrc0,
										int xSrc1,
										int ySrc1,
										int dlt,
										int *maxX,
										int *maxY,
										ofstream &c_out_,
										int print) {
/**/

	int sx0,sy0,sx1,sy1;
	int x,y,m,n,t;                 //  ,p,d
	int LfAmSum,RgAmSum;
	int Lf,Rg,LfMem,RgMem, Delta;  //float Lf,Rg,LfMem,RgMem, Delta;
	int LfxP, RgxP, LfyP, RgyP;
	int MaxCorX=0,MaxCorY=0,AdrMaxCorX,AdrMaxCorY;
	int ch=0,correlation=0;     //    ,wVh M,
	//отключение в config.h всех DP  -  #define DP(x) //c_out<<x;
    print=1;
	int printP=0;
	// заменмть *100 на res<<7  *128  //
	
	// заполнение 1 массива кэша масок функции setMatrix_Point
	//memset(maskCor,1,384*128);   // 128*3*32*4
	///		 for ( x=0; x < 384*32; x++ ) { maskCor[x]=1;  }


#ifdef REPORT
	DA("Matrix_Lion_1@@ dlt="<<dlt<<" xSrc0="<<xSrc0<<" xSrc1="<<xSrc1<<" ySrc0="<<ySrc0<<" ySrc1="<<ySrc1<<" mask32.mHOn="<<mask32->mHOn);
#endif

#ifdef REPORT
	int color=dlt;
	if(dlt==0)color=6;  ////red
#ifdef DEBUGLVL_RGB		
	if(print)drawDataRGB[0][ySrc0][xSrc0-32]=color;    ////green
	if(print&&dlt==0)drawDataRGB[0][ySrc1][xSrc1-32]=5;
#endif
#ifdef DEBUGLVL		
	//drawData[0][ySrc0][xSrc0-32]=0;    ////green
	//drawData[0][ySrc1][xSrc1-32]=0;
#endif
#endif	


	//static clock_t  tm_start1=clock();
	//clock_t  tm_end; double time;  DP(END); //DP(tm<<" "<<t);


/*
// константы для алгоритма подсчета единиц в 128 или в 64р словах
static const unsigned int constF[12]={              // 12  16
	0x33333333, 0x33333333, 0x33333333, 0x33333333,
	0x55555555, 0x55555555, 0x55555555, 0x55555555,
	0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f  // ,
//	0x00000000, 0x00000000, 0x00000000, 0x00000000  // TEST
};

// переход в режим Assembler, загрузка констант
__asm {     //  movups (без требования выравнивания)
	// загрузка констант в регистр 64p ММХ ( считаем в байтах - int*4)
	movq            mm5, qword ptr constF+16;   // 55 Пересылка данных (64 бит)
	movq            mm6, qword ptr constF;      // 33 из/в регистр ММХ  MOVDQU-128
	movq            mm7, qword ptr constF+32;   // 0f

	// загрузка констант в регистр 128p SSE
	//  movups без требования выравнивания  ( MOVAPS должны быть выровнены )
#ifdef WIN32
	movups          xmm5, dqword ptr constF+16;  // 55  Пересылка данных (128 бит)
	movups          xmm6, dqword ptr constF;     // 33  + MOVDQU
	movups          xmm7, dqword ptr constF+32;  // 0f
//	movups          xmm2, dqword ptr constF+48;  // TEST
#else
	movups          xmm5, [constF+16];  // 55  Пересылка данных (128 бит)
	movups          xmm6, [constF];     // 33  + MOVDQU
	movups          xmm7, [constF+32];  // 0f
	//	movups          xmm2, dqword ptr constF+48;  // TEST
#endif

} // __asm
//Инструкция movq выполняется за шесть тактов, тогда как для pshufw требуется только два
*/


__asm {
		emms;
	} // emms; команда обеспечивает переход процессора от исполнения MMX-команд
	// к исполнению обычных команд с плавающей запятой:
	// она устанавливает значение 1 во всех разрядах слова состояния.


	////for ( int i=0; i<100000; i++ ){    //проверка быстродействия   3.5 sec
	
   	// заполнение массива кэша функции setMatrix_Point
	// в адесах где лежит 255 коэффициент корреляции (0-100) не занесен
	//unsigned char cacheCor[128*34];  // массив кэша коэффициента корреляции по области поиска максимума
	memset(cacheCor,255,4096);  //128*32

	
	// Задание области скольжение маски по смещению dlt или по координатам x,y
	if(dlt>0){
		sx0=xSrc0-dlt,    sx1=xSrc0+dlt;
		sy0=ySrc0-dlt,    sy1=ySrc0+dlt;
	}else{  //  dlt=-dlt
		sx0=xSrc0,        sx1=xSrc1;
		sy0=ySrc0,        sy1=ySrc1;
	   //			     mH=sy1 - sy0;
	} // if(dlt
	
	AdrMaxCorX=sx0;
	AdrMaxCorY=sy0;
	
	int X,Y,X0,X1,Y0,Y1,Cor,maxCor,AdrCorX,AdrCorY;
	
	// выбор шага сетки первичного разбиения области поиска максимума коэффициента корреляции
	// высота h и ширина w области поиска максимума коэффициента корреляции
	int w=sx1-sx0;       int h=sy1-sy0;
	int maxwh=w;         if (h > w) { maxwh=h; }
	// stepwh=3 при maxwh<16 (сетка 4x4),  stepwh=4 при maxwh>=16 (сетка 5x5),  stepwh=5 maxwh>=32....          // (sx1-sx0)  (sy1-sy0)
	int stepwh=3 + maxwh/16;  // 4 + 16   // 4 + maxwh/16;
	int dx=64*w/stepwh;       int dy=64*h/stepwh;     // res<<8  *256
	//if (dx < 2) {dx=2;}      if (dy < 2) {dy=2;}
	if (dx < 128) {dx=128;}   if (dy < 128) {dy=128;}
	// int dx=20;           int dy=20;
	// количество узлов сетки stepwh по высоте и ширине одинакого,
	// но величины шагов dx,dy, как правило разные
	DP("//maxwh="<<maxwh<<"  //stepwh="<<stepwh<<END); DP(END);
	
	
	// интервал поиска по сетке максимума коэффициента корреляции.
	int SX0=sx0*64;             int SX1=sx1*64;
	int SY0=sy0*64;             int SY1=sy1*64;
	
	// сдвиг периметра сетки к центру области поиска нам пол шага.
	int ddx=dx>>1;            int ddy=dy>>1;
	X0=SX0 + ddx;             Y0=SY0 + ddy;
	X1=SX1 - ddx;             Y1=SY1 - ddy;
	
	DP(END);
	// формирование сетки (обычно 5x5), поиск максимума коэффициента корреляции в сетке
	maxCor=Cor=0;     t=0;
	for ( Y=Y0; Y <= Y1;  Y+=dy ) {   // || maxCor < 100
		//for ( Y=Y0+dy/2; Y <= Y1-dy/2;  Y+=dy ) {   // || maxCor < 100
		y=Y/64;   DP(END);            // p=y*w;  ///////////////////////////
		for ( X=X0; X <= X1; X+=dx ) {
			//for ( X=X0+dx/2; X <= X1-dx/2; X+=dx ) {
			x=X/64;                                        // res>>8  /256
			#ifdef REPORT
              static int count_=0;
			  if(print){
				 /*if(count_==134){
					int L=1;
					printP=1;
				 }else{printP=0;}
				 //cout<<"count="<<count_<<" Cor="<<Cor<<END;
				 if(count_==135){
				 int l=1;
				 }
				 count_++;
				 */
			  }
			#endif
			Cor=setMatrix_Point(mask32, x,y, sx0,sy0,w,&ch, c_out,printP); t+=1-ch;

			if ( Cor > maxCor ) { maxCor=Cor;     AdrCorX=x; AdrCorY=y; }
			if ( maxCor==100 ){correlation=maxCor; AdrMaxCorX=x;  AdrMaxCorY=y; goto maxCorEND;}
#ifdef DEBUGLVL_RGB	
			if(print){
				if(mask32->id==0){
					drawDataRGB[0][y][x-32]=3;
				}else{
					drawDataRGB[0][y][x-32]=5;
				}
			}
#endif
#ifdef DEBUGLVL_GREY
				if(mask32->id==0){
					drawData[0][y][x-32]/=2;
				}else{
					drawData[0][y][x-32]*=0.9;
				}
#endif
			
			DP(Cor<<" ,"<<x<<","<<y<<","<<"\t");
			///	DP(Cor<<","<<AdrCorX<<","<<AdrCorY<<"  "<<"\t");
		} // x
	} // y
	if ( maxCor<25 )  { correlation=1; goto maxCorEND; }   // correlation=maxCor;
	
	DP(END);
	DP("  maxCor="<<maxCor<<"   AdrCorX="<<AdrCorX<<"   AdrCorY="<<AdrCorY<<"   t="<<t<<END);
	
	
	
	// интервалы поиска X0-X1, Y0-Y1 для нахождения max коэф. корр. ( 2*dx на dy*2 ).
	X0=AdrCorX*64-dx;   if (X0<SX0) {X0=SX0;}    // if (X0<sx0) {X0=sx0;}
	X1=AdrCorX*64+dx;   if (X1>SX1) {X1=SX1;}    // if (X1>sx1) {X1=sx1;}
	Y0=AdrCorY*64-dy;   if (Y0<SY0) {Y0=SY0;}    // if (Y0<sy0) {Y0=sy0;}
	Y1=AdrCorY*64+dy;   if (Y1>SY1) {Y1=SY1;}    // if (Y1>sy1) {Y1=sy1;}
	/**/
	
	
	// интерактивное нахождение максимума функции коэффициента корреляции Lion
	// (поиск триадами float). Функция состоит из: блока перемножения масок
	// Off, ON и алгоритма подсчета единиц. Алгоритм находит максимум коэффициента
	// корреляции в прямоугольнмке с координатами sx0,sy0,sx1,sy1; 										int xSrc0,
	
	////////////////////////////////////////////////////////////////////////////////
	DP(END); DP(sx0<<" sx0 "); DP(sx1<<" sx1 "); DP(sy0<<" sy0 "); DP(sy1<<" sy1 "); DP(END);
	
	
	
	// алгоритм находит максимум коэффициента корреляции в пределах от sx0 ,sy0 до sx1,sy1
	
	// абсолютная точность итераций absolute Precision
	Delta=1;  // DP(END);DP(Delta<<" Delta ");DP(END);  // Delta=2-m;  if ( Delta< 1 ) Delta=1;
	// координаты старта поиска
	AdrMaxCorX=AdrCorX;  AdrMaxCorY=AdrCorY;   // t=0;
	
	// цикл чередования поиска по осям X и Y , максимума коэффициента корреляции
	for ( m=0; m<3; m++ ){
		
		// нахождение максимума коэффициента корреляции по оси X
		Lf=0;   Rg=X1-X0;   // интервал поиска X0-X1   // M*(X1-X0)/100;
		DP(END); DP(Lf<<" Lf "); DP(Rg<<" Rg "); DP(END);
		DP(END); DP("AdrMaxCorY="<<AdrMaxCorY<<END);
		for ( n=0; n<7; n++ ){  // 10
			
			LfMem=(Lf+Lf+Rg)/3;  LfxP=(LfMem+X0)/64;  // треть влево (Lf+Lf+Rg)/3; sx0
			// возврат коэффициента корреляции в точке LfxP, AdrMaxCorY.
			LfAmSum=setMatrix_Point(mask32, LfxP,AdrMaxCorY, sx0,sy0,w,&ch, c_out,printP); t+=1-ch;
			RgMem=(Lf+Rg+Rg)/3;  RgxP=(RgMem+X0)/64;  // треть вправо  (Lf+Rg+Rg)/3;
			// возврат коэффициента корреляции в точке RgxP, AdrMaxCorY.
			RgAmSum=setMatrix_Point(mask32, RgxP,AdrMaxCorY, sx0,sy0,w,&ch, c_out,printP); t+=1-ch;
			
			DP("  LfMem="<<LfMem<<"   RgMem="<<RgMem<<"   LfAmSum="<<LfAmSum<<"   RgAmSum="<<RgAmSum<<END);
			DP("  LfxP="<<LfxP<<"   RgxP="<<RgxP<<"   RgxP-LfxP="<<RgxP-LfxP<<"   t="<<t<<END);
			
			if ( LfAmSum < RgAmSum) { Lf=LfMem; } else { Rg=RgMem; }
			if ( abs(RgxP-LfxP)<Delta || RgAmSum-LfAmSum==0 ) break;   // fabs(Rg-Lf)
			///				 if ( abs(RgxP-LfxP)<Delta ) break;
			
		} // for n
		
		//  максимум коэффициента корреляции (correlation), его адрес
		MaxCorX=(LfAmSum + RgAmSum)/2;   AdrMaxCorX=(LfxP + RgxP)/2;
		DP("//MaxCorX="<<MaxCorX<<"  //AdrMaxCorX="<<AdrMaxCorX<<END); DP(END);
		
		///////	 if ( MaxCorY==MaxCorX || MaxCorY == 100 || MaxCorX==100 ) break;  // ОПОРНАЯ
		if ( MaxCorY==MaxCorX ) break;
		
		// нахождение максимума коэффициента корреляции по оси Y
		Lf=0;   Rg=Y1-Y0;   // интервал поиска Y0-Y1
		DP(END); DP("AdrMaxCorX="<<AdrMaxCorX<<END);
		for ( n=0; n<7; n++ ){ // 10
			
			LfMem=(Lf+Lf+Rg)/3;  LfyP=(LfMem+Y0)/64;  // треть влево (Lf+Lf+Rg)/3;
			// возврат коэффициента корреляции в точке AdrMaxCorX, LfyP.
			LfAmSum=setMatrix_Point(mask32, AdrMaxCorX,LfyP, sx0,sy0,w,&ch, c_out,printP); t+=1-ch;
			RgMem=(Lf+Rg+Rg)/3;  RgyP=(RgMem+Y0)/64;  // треть вправо  (Lf+Rg+Rg)/3;
			// возврат коэффициента корреляции в точке AdrMaxCorX, RgyP.
			RgAmSum=setMatrix_Point(mask32, AdrMaxCorX,RgyP, sx0,sy0,w,&ch, c_out,printP); t+=1-ch;
			
			DP("  LfMem="<<LfMem<<"   RgMem="<<RgMem<<"   LfAmSum="<<LfAmSum<<"   RgAmSum="<<RgAmSum<<END);
			DP("  LfyP="<<LfyP<<"   RgyP="<<RgyP<<"   RgyP-LfyP="<<RgyP-LfyP<<"   t="<<t<<END);
			// DP("//MaxCorX="<<MaxCorX<<"  //AdrMaxCorX="<<AdrMaxCorX<<END); DP(END);
			
			if ( LfAmSum < RgAmSum) { Lf=LfMem; } else { Rg=RgMem; }
			if ( abs(RgyP-LfyP)<Delta || RgAmSum-LfAmSum==0 ) break;   // fabs(Rg-Lf)
			///				 if ( abs(RgyP-LfyP)<Delta ) break;
			
		} // for n
		
		//  максимум коэффициента корреляции (correlation), его адрес
		MaxCorY=(LfAmSum + RgAmSum)/2;   AdrMaxCorY=(LfyP + RgyP)/2;
		DP("//MaxCorY="<<MaxCorY<<"  //AdrMaxCorY="<<AdrMaxCorY<<END); DP(END);
		
		//			 if ( RgxP-LfxP <= 1 && RgyP-LfyP<= 1 ) break;
		///////	     if ( MaxCorY==MaxCorX || MaxCorY == 100 || MaxCorX==100 ) break;  // ОПОРНАЯ
		if ( MaxCorY==MaxCorX ) break;
		
	} // for m
	
	/**/
	
	////////////////////////////////////////////////////////////////////////////////
	
	
	
	//int correlation=(MaxCorX+MaxCorY)/2;   ////// maxX=AdrAmMAX-border-32;   maxY=yP-border;
	correlation=MaxCorY;    if ( MaxCorX > MaxCorY ) { correlation=MaxCorX; }
	
   //  DT(" maxCorEND="<<maxCor);  DT(" DLT="<<correlation-maxCor);  DT(" correlation="<<correlation<<END);
   //  DT(" stepwh="<<stepwh);
	
maxCorEND:; // переход

	
	// ВИЗУАЛИЗАЦИЯ цифровая
	DP(" //__correlation="<<correlation<<END);
	DP(END<<"// w*h/(t+1)="<<w*h/(t+1)<<",  t="<<t<<END);
	////
	DP(" maxX="<<AdrMaxCorX<<END);
	
	DP(n+1<<"-n- "); DP(Lf<<" Lf ");
	DP(Rg<<" Rg "); DP(Delta<<" Delta "); DP(END);
	
	
	//____________
	/**/
	
	////} // i  //проверка быстродействия
/*
	__asm {
		emms;
	} // emms; команда обеспечивает переход процессора от исполнения MMX-команд
	// к исполнению обычных команд с плавающей запятой:
	// она устанавливает значение 1 во всех разрядах слова состояния.

	//DM("2@@")tm_end=clock(); tm_end-=tm_start; time=tm_end/CLOCKS_PER_SEC;DM("time="<<time<<END);
	//DP("Matrix_Lion_2@@")tm_end=clock(); tm_end-=tm_start1; time=tm_end/CLOCKS_PER_SEC;DP("time="<<time<<END<<END);
*/

	*maxX=AdrMaxCorX;
	*maxY=AdrMaxCorY;
	mask32->correlation=correlation;
	
	return correlation;
	
	
	
	////////////////////////////////////////////////////////////////////////////////
	
	//			 maxX=maxX - border - 32;
	//			 maxY=maxY - border;
	/**/
	
	//------------------------------------------------------------------------------
	
	/*
	 //		 int qx=100*w/9;      int qy=100*h/9;
//DP("//qx="<<qx<<"  //qy="<<qy<<END); DP(END);
DP(END);
		 maxCor=0;
		 for ( Y=Y0; Y <= Y1; Y+=qy ) {
		   y=Y/100;  DP(END);      //p=y*dx;
		   for ( X=X0; X <= X1; X+=qx ) {
			 x=X/100;
			 Cor=setMatrix_Point(mask32, x,y, c_out);   t++;
			 if ( Cor > maxCor ) { maxCor=Cor;     AdrCorX=x; AdrCorY=y; }
			 if ( maxCor==100 )  { MaxCorX=maxCor; goto maxCorEND; }
//			 drawDataRGB[0][y][x-32]=0;
			  DP(Cor<<","<<x<<","<<y<<"  "<<"\t");
///			 DP(Cor<<","<<AdrCorX<<","<<AdrCorY<<"  "<<"\t");
		   } // x
		 } // y
		 if ( maxCor<30 )  { goto maxCorEND; }
//		 maxCorEND:;
DP(END);
DP(END);DP("  maxCor="<<maxCor<<"   AdrCorX="<<AdrCorX<<"   AdrCorY="<<AdrCorY<<"   t="<<t<<END);
*/

//------------------------------------------------------------------------------
 /*
			 // ПРИНЦИП РАБОТЫ поиска триадами
// это метод для поиска максимумов и минимумов функции, которая либо сначала
// строго возрастает, затем строго убывает, либо наоборот. Троичный поиск определяет,
// что минимум или максимум не может лежать либо в первой, либо в последней трети
// области, и затем повторяет поиск на оставшихся двух третях.

//  Функция поиска триада (F, влево, вправо, абсолютная точность)
	  function ternarySearch(f, left, right, absolutePrecision)
// left and right are the current bounds; the maximum is between them
// слева и справа являются нынешние границы; максимум между ними
	if (right-left < absolutePrecision) return (left+right)/2
	leftThird := (left*2+right)/3   // Треть влево
	rightThird := (left+right*2)/3  // Треть вправо
	if (f(leftThird) < f(rightThird))
		return ternarySearch(f, leftThird, right, absolutePrecision)
	else
		return ternarySearch(f, left, rightThird, absolutePrecision)
end
  */
/*
	  // работоспособное решение задачи нахождение максимума функции
	  Lf=0;   Rg=X0-X1;   // интервал поиска X0-X1
	 for ( n=0; n<10; n++ ){           d+=M;

		   dt=LfMem=(Lf+Lf+Rg)/3;               // треть влево (Lf+Lf+Rg)/3;
		   LfAmSum=FFFF( dt/4, 2 );             // получение отклика функции
												// в которой ищем максимум
		   dt=RgMem=(Lf+Rg+Rg)/3;               // треть вправо  (Lf+Rg+Rg)/3;
		   RgAmSum=FFFF( dt/4, 2 );             // получение отклика функции

		   DM("  LfMem="<<LfMem<<"   RgMem="<<RgMem<<"   LfAmSum="
		   <<LfAmSum<<"   RgAmSum="<<RgAmSum<<END); DM(Rg-Lf<<" Rg-Lf "); DM(END);

		  if ( LfAmSum < RgAmSum) { Lf=LfMem; } else { Rg=RgMem; }
		  if ( fabs(Rg-Lf)<Delta || RgAmSum-LfAmSum==0 ) break;

	 } // n
*/
//------------------------------------------------------------------------------


}//_____________________________________________________________________________

/**/
/*
					   //  CPU //

// Вычисление коэффициента корреляции в точке xP, yP.
// -----------------------------------------------------------------------------
int GBitsetMatrix::setMatrix_PointT(GBitMask32 *mask32,
										int xP,
										int yP,
										int sx0,
										int sy0,
										int w,
										int *ch,
										ofstream &c_out,
										int print
										){
  int correlation=0;
  unsigned int m,p;
  //int sx0,sy0,sx1,sy1;
  int Sb,Si,SumBitOn,SumBitOff,deltaSum;
  int nr=nrows; //nc=ncolumns;  // SH(ncolumns);  SH(nrows);
  int mH=mask32[0].mH; //mW=32; // mH(8-96) высота и mW (32) ширина (габарит) битовой
					   // маски перефирийной области  "Off"



///DP(" TIME_START setMatrix_Point ON TEST mask32[0].mH="<<mask32[0].mH<<END);

///TIME_START

///mask32->printMask(c_out);         DP(END);DP(END);

					//////////// КЕШ  ///////////////
			int q=xP-sx0 + (yP-sy0)*w;    correlation=cacheCor[q];  *ch=0;
			//DP((short)cacheCor[q]<<","<<xP<<","<<yP<<","<<"\t");

////	 	if ( correlation != 255) { DP(correlation<<"+"); *ch=1; return correlation; }
			if ( correlation != 255) { *ch=1; return correlation; }

// TEST
////Sb=0xFFFFFFFF;

///TIME_START
////for (int count = 0; count < 50000; count++) {  //проверка быстродействия

	  // Вычисление коэффициента корреляции в точке xP, yP.
	   p=yP+xP*nr;
	   SumBitOn=SumBitOff=0;
	   // Суммы маски mask32 по  Y
	   for ( m=0; m < mH; m++ ) {
		 // Умножение маски на транспонированный скан SInt
		 Si=sInt[p+m];
		 ////Si=maskCor[p+m];
		 if(Si != 0) {
		 Sb=Si & mask32[0].On[m];
		   if(Sb != 0) {                                            //0xFFFFFFFF
			// Регистровые суммы маски mask32.On[m] (подсчет единиц) по  X
			Sb-=(Sb >> 1) & 0x55555555;                // 2-битовые ячейки
			Sb=(Sb & 0x33333333) + ((Sb >> 2) & 0x33333333); // 4-битовые
			Sb=(Sb + (Sb >> 4)) & 0x0F0F0F0F;                // 8-битоовые
			Sb+=Sb >> 8;                               // 16-битовые
			Sb+=Sb >> 16;                              // 32-битовая ячейка
			SumBitOn+=Sb & 0x3F;  // 63 Обнуляем старшие разряды, содержащие "мусор"
		   }
			// Умножение маски на транспонированный скан SInt
		   Sb=Si & mask32[0].Off[m];
		   if(Sb != 0) {                                            //0xFFFFFFFF
			// Регистровые суммы маски mask32.Off[m] (подсчет единиц) по  X
			Sb-=(Sb >> 1) & 0x55555555;                // 2-битовые ячейки
			Sb=(Sb & 0x33333333) + ((Sb >> 2) & 0x33333333); // 4-битовые
			Sb=(Sb + (Sb >> 4)) & 0x0F0F0F0F;                // 8-битоовые
			Sb+=Sb >> 8;                               // 16-битовые
			Sb+=Sb >> 16;                              // 32-битовая ячейка
			SumBitOff+=Sb & 0x3F;  // 63 Обнуляем старшие разряды, содержащие "мусор"
		   }
		 } // if
	   } // m

////} //проверка быстродействия

	   // разность сумм по маск
	   deltaSum=SumBitOn-SumBitOff;
	   if(deltaSum<0){deltaSum=0;}

	   if(deltaSum<1||mask32[0].NMask<1)return 0;

	   // коэффициент корреляции  // КЭШ  //
	   //correlation=deltaSum*100/mask32[0].NMask;
	   correlation=cacheCor[q]=deltaSum*100/mask32[0].NMask;

if(drawDataRGB[0][yP][xP-32])drawDataRGB[0][yP][xP-32]=
  drawDataRGB[0][yP][xP-32] - ((254)<<8)/(mask32[0].NMask+1);

///if(drawDataRGB[0][yP][xP-32])drawDataRGB[0][yP][xP-32]=
///drawDataRGB[0][yP][xP-32] - ((deltaSum)<<8)/(mask32[0].NMask+1); // - s*0.8-20  (SumBit/4)  SumBtMax
//                                  +4


///TIME_PRINT
//SH(time);

///DP("SumBitOn="<<SumBitOn<<" SumBitOff="<<SumBitOff<<" deltaSum"<<deltaSum<<END);
///DP(" deltaSum="<<deltaSum<<" mask32[0].NMask"<<mask32[0].NMask<<END);
//SH(SumBtMaxOff); SH(" ");

///DM(END); DM(mH<<" mH ")DM(END);

//DP("//__correlation="<<correlation<<END);
///DP(" TIME_PRINT setMatrix_Point TEST"); DP(END);

return correlation;

//	   for ( m=mH >> 1;  m > 0;  m-- ) {

}//_____________________________________________________________________________
*/


						//  ASM  Assembler MMX 64 //

// Вычисление коэффициента корреляции по маске единичных битов  IEST
// -----------------------------------------------------------------------------


int GBitsetMatrix::setMatrix_PointT(GBitMask32 *mask32,
										int xP,
										int yP,
										int sx0,
										int sy0,
										int w,
										int *ch,
										ofstream &c_out,
										int print){

  unsigned int p;
  //int y,x,n,m,p;
  int correlation=0;
  int SumBitOn,SumBitOff,deltaSum;
  int nr=nrows;  // nc=ncolumns;  // SH(ncolumns);  SH(nrows);
  int mH=mask32[0].mH; //mW=32; // mH(8-96) высота и mW (32) ширина (габарит) битовой
					   // маски перефирийной области  "Off"
  unsigned int *sIntA=sInt,   *maskOnA=mask32[0].On,   *maskOffA=mask32[0].Off;


				//////////// КЕШ  ///////////////
			int q=xP-sx0 + (yP-sy0)*w;    correlation=cacheCor[q];  *ch=0;
			//DP((short)cacheCor[q]<<","<<xP<<","<<yP<<","<<"\t");

			//if ( correlation != 255) { DP(correlation<<"+"); *ch=1; return correlation; }
			if ( correlation != 255) { *ch=1; return correlation; }
/**/

//TIME_START
////for (int count = 0; count < 50000; count++) {  //проверка быстродействия

//// Вычисление коэффициента корреляции в точке xP, yP. Assembler ////

p=(yP+xP*nr)<<2;  // *4 считаем в байтах  //p=yP+xP*nr;  //SumBitOn=SumBitOff=0;

//p=(yP+xP*nr);
///for ( m=0; m < mH; m+=2 ) {
#ifdef REPORT
if(print){
/*
char str[128]; // char *string;   строка результата
	unsigned int test=sIntA[p];
	itoa(test,str,2); // двоичное представление числа   StrM[x]
	cout<<str<<endl;
*/
}
#endif
//------------------------------------------------------------------------------

// переход в режим Assembler  // MMX64
__asm {

// Установка накопительных регистров MMX64  mm3 и mm4  в ноль
// mov eax, 0	 // xor eax, eax  	// pxor mm3, mm3;    // ecx, 0; ebx, 0;
			xor             edx, edx         // установка ноля  0x00000000
			pxor            mm3, mm3;        // установка ноля  0x0000000000000000
			pxor            mm4, mm4;        // установка ноля  0x0000000000000000

			// вычисление адреса транспонированного скана sIntA
///			mov       ebx, sIntA;            //  вычисление адреса слайса скана
///			add       ebx, p;                //  добавление к адресу слайса смещения на переменную p

////  начало цикла по переменной в регистре  edx, шаг цикла 8, количество циклов mH/2
//  8 число байт в MMX64, считаем 2 слайса за один цикл
//  количество циклов: mH*8/2 = mH*4 = mH<<2 // shl     ecx, 2;
			mov       ecx, mH;               //  загрузка количества циклов mH
			lea       ecx,[ecx*4];           //  умножение  ecx на *4 + смещение на один (8) цикл
   loop_start:



// pshufw          mm1, mm0, 0xE4; // pshufd можно добавить быстрое копирование одного регистра в другой.

//------------------------------------------------------------------------------
//  Регистры EAX,EBX,EDX,ECX называют рабочими регистрами               // neg
//  кроме того в процессе выполнения этой программы служебно не модифицируются
//  регистры  ebx  ecx  esp  esi  edi    кроме ebp         //  shl       eax, 2;

			// вычисление адреса транспонированного скана sIntA
			mov       eax, sIntA;            //  вычисление адреса слайса скана
///			mov       eax, ebx;
			add       eax, p;                //  добавление к адресу слайса смещения на переменную p
			lea       eax, [eax+edx];        //  добавление к адресу маски переменной цикла
			movq      mm1, qword ptr [eax];  //  movq загрузка слайса скана в регистр mm1

			// переход (единици не считаем), если два (64) подряд идущих слайса равны нолю
			pxor      mm0, mm0;              // загрузка ноля  0x0000000000000000
			pcmpeqd   mm0, mm1;              // если источник равен приемнику, то результат равен
			movd      eax, mm0;	             // 0ffffffffh; Результат помещается в приемник eax.
			cmp       eax, 0ffffffffh;
			je        if_zero;               // je переход если  флаг ноля FZ=0     jz

			// вычисление адреса, умножение маски On на транспонированный скан sIntA
			mov       eax, maskOnA;          //  вычисление адреса маски maskOnA
///			add       eax, edx;
			lea       eax, [eax+edx];        //  добавление к адресу маски переменной цикла
			movq      mm0, qword ptr [eax];  //  загрузка маски maskOnA в регистр mm1
			pand      mm0, mm1;              //  применение & маски к слайсу скана

		   // вычисление адреса, умножение маски Off на транспонированный скан SInt
			mov       eax, maskOffA          //  вычисление адреса маски maskOffA
///			add       eax, edx;
			lea       eax, [eax+edx];        //  добавление к адресу маски переменной цикла
			movq      mm2, qword ptr [eax];  //  загрузка маски maskOffA в регистр mm2
			pand      mm2, mm1;              //  применение & маски к слайсу скана


// forMAC  LDDQU . По возможности используйте инструкцию SSE3 LDDQU вместо MOVDQU.
// forMAC  MOVDQA (MOVe Aligned Double Quadword) 6 1 FP_MOVE — перемещение выровненных 128 бит  ( MOVAPS )
//------------------------------------------------------------------------------

// Регистровые (MMX64) суммы маски (подсчет единиц) mask32[0].On по  X
		   //	 pshufw          mm1, mm0, 0xE4;  // работает
			movq            mm1, mm0;   // v
            //pshufw          mm1, mm0, 0xE4
			psrld           mm0, 1;     // v >> 1          Логический сдвиг вправо, с заполнением старших бит нулями
			pand            mm0, mm5;   // (v >> 1) & 0x55555555
			psubd           mm1, mm0;   // w = v - ((v >> 1) & 0x55555555)  Вычитание упакованных байт (слов или двойных слов) без насыщения
			movq            mm0, mm1;   // w               Пересылка данных (64 бит) из/в регистр ММХ
			psrld           mm1, 2;     // w >> 2
			pand            mm0, mm6;   // w & 0x33333333  Побитовое логическое И над всеми битами источника и приемника. Результат помещается в  приемник.
			pand            mm1, mm6;   // (w >> 2)  & 0x33333333           Побитовое логическое И
			paddd           mm0, mm1;   // x = (w & 0x33333333) + ((w >> 2) & 0x33333333)  Сложение упакованных байт (слов или двойных слов) без насыщения
			movq            mm1, mm0;   // x               Пересылка данных (64 бит) из/в регистр ММХ
			psrld           mm0, 4;     // x >> 4          Логический сдвиг вправо
			paddd           mm0, mm1;   // x + (x >> 4)
			pand            mm0, mm7;   // y = (x + (x >> 4) & 0x0F0F0F0F)
			pxor            mm1, mm1;   //                 Исключающее ИЛИ  ==1
			psadbw          mm0, mm1;   // сборка (разностями) и сложением вдоль
			paddq           mm3, mm0;   // регистра, старшие 3 слова из 4 слов обнуляем


// Регистровые (MMX64) суммы маски (подсчет единиц) mask32[0].Off по  X
			movq            mm0, mm2;   //  mm2
			movq            mm1, mm2;   //  копирование одного регистра в другой.
			psrld           mm0, 1;     // v >> 1     psrld    PSLLDQ
			pand            mm0, mm5;   // (v >> 1) & 0x55555555
			psubd           mm1, mm0;   // w = v - ((v >> 1) & 0x55555555)
			movq            mm0, mm1;   // w
			psrld           mm1, 2;     // w >> 2
			pand            mm0, mm6;   // w & 0x33333333
			pand            mm1, mm6;   // (w >> 2)  & 0x33333333
			paddd           mm0, mm1;   // x = (w & 0x33333333) + ((w >> 2) & 0x33333333)
			movq            mm1, mm0;   // x
			psrld           mm0, 4;     // x >> 4
			paddd           mm0, mm1;   // x + (x >> 4)
			pand            mm0, mm7;   // y = (x + (x >> 4) & 0x0F0F0F0F)
			pxor            mm1, mm1;   // Исключающее ИЛИ == 1
			psadbw          mm0, mm1;   // сборка (разностями) и сложениемвдоль
			paddq           mm4, mm0;   // регистра, старшие 3 слова из 4 слов обнуляем
										// суммирование единиц по всей маске  Off

//------------------------------------------------------------------------------

  if_zero:

////  окончание цикла, шаг цикла 8       // add  +8   // sub -8
			add             edx, 8;      // в edx находится переменная цикла
			cmp             edx, ecx;    // в ecx находится количество циклов mH*4
			// ИЗМЕНЕНО  jle 1280 +64  на   jl 1280,
			jl             loop_start;   // jl перейти, jle перейти, если меньше или равно
//------------------------------------------------------------------------------

// заносим сумму единиц в накопительную переменную  SumBitOff , SumBitOn
			movd            SumBitOn,  mm3;
			movd            SumBitOff, mm4;


// возврат из режима Assembler
 } // __asm
//------------------------------------------------------------------------------
///} // m



////} //проверка быстродействия

	   // разность сумм по маск
	   deltaSum=SumBitOn-SumBitOff;
	   if(deltaSum<0){deltaSum=0;}

	   if(deltaSum<1||mask32[0].NMask<1)return 0;

	   // коэффициент корреляции  // КЭШ  //
	   ////correlation=deltaSum*100/mask32[0].NMask;
	   correlation=cacheCor[q]=deltaSum*100/mask32[0].NMask;
	   //DP("//__correlation="<<correlation<<END);
#ifdef DEBUGLVL_RGB	
	   if(print){
		  //if(drawDataRGB[0][yP][xP-32])drawDataRGB[0][yP][xP-32]=
		  //	 drawDataRGB[0][yP][xP-32] - ((254)<<8)/(mask32[0].NMask+1);
           drawDataRGB[0][yP][xP-32]=7;
	   }
#endif
 /**/


//TIME_PRINT
//SH(time);

	   return correlation;


}//_____________________________________________________________________________
/**/


/**/


						//  ASM  Assembler SSE2 128 //

// Вычисление коэффициента корреляции по маске единичных битов  IEST
// -----------------------------------------------------------------------------


int GBitsetMatrix::setMatrix_Point(GBitMask32 *mask32,
										int xP,
										int yP,
										int sx0,
										int sy0,
										int w,
										int *ch,
										ofstream &c_out,
										int print){
  //int A=sInt;
/*  static int pR=0; pR++;
  //cout<<"_"<<pR<<"/ ";
  if(pR==85){
  pR++;
  //cout<<sIntA[0];
  }
 */
 unsigned int p;
  //int y,x,n,m,p;
  int correlation=255;
  int SumBitOn,SumBitOff,deltaSum;
  int nr=nrows;  // nc=ncolumns;  // SH(ncolumns);  SH(nrows);
  int mH=mask32[0].mH; //mW=32; // mH(8-96) высота и mW (32) ширина (габарит) битовой
					   // маски перефирийной области  "Off"
  unsigned int *sIntA=sInt,   *maskOnA=mask32[0].On,   *maskOffA=mask32[0].Off;

/*  static int count=0;
  if(print){
  cout<<count<<" "; count++;
  }
*/				//////////// КЕШ  ///////////////
			int q=xP-sx0 + (yP-sy0)*w;    
	        correlation=cacheCor[q];  
	        *ch=0;
			//DP((short)cacheCor[q]<<","<<xP<<","<<yP<<","<<"\t");

			//if ( correlation != 255) { DP(correlation<<"+"); *ch=1; return correlation; }
			if ( correlation != 255) { *ch=1; return correlation; }
/**/

//TIME_START
////for (int count = 0; count < 50000; count++) {  //проверка быстродействия

//// Вычисление коэффициента корреляции в точке xP, yP. Assembler ////

p=(yP+xP*nr)<<2;  // *4 считаем в байтах  //p=yP+xP*nr;  //SumBitOn=SumBitOff=0;
///for ( m=0; m < mH; m+=2 ) {
#ifdef REPORT
if(print){
/*
char str[128]; // char *string;   строка результата
	unsigned int test=sIntA[p];
	itoa(test,str,2); // двоичное представление числа   StrM[x]
	cout<<str<<endl;
*/
}
#endif
//------------------------------------------------------------------------------

// переход в режим Assembler  // SSE2 128
__asm {

//// Установка накопительных регистров SSE2  mm3 и mm4  в ноль
// mov eax, 0	 // xor eax, eax  	// pxor mm3, mm3;    // ecx, 0; ebx, 0;
			xor           edx,  edx;         // установка ноля   0x00000000
			pxor          xmm3, xmm3;        // установка ноля   0x00000000000000000000000000000000
			pxor          xmm4, xmm4;        // установка ноля   0x00000000000000000000000000000000
		  //pcmpeqd       xmm2, xmm2         // установка единиц 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF


////  начало цикла по переменной в регистре  edx=0, шаг цикла 16, количество циклов mH*4
//  16 число байт в SSE128, 4 число байт в 32 разрядном слове, считаем 4 слайса за один цикл
//  количество циклов:  mH*16/4 = mH*4 = mH<<2       // shl     ecx, 2;

			mov       ecx, mH;               //  загрузка количества циклов mH
			shl       ecx, 2;                //  умножение  ecx на *4    add eax,eax =*2
		   //	lea       ecx,[ecx*4];           //  *4 + смещение на один (16) цикл
   loop_start:                               //  lea       ecx,[ecx*4+16];


// По возможности используйте инструкцию SSE3 LDDQU вместо MOVDQU.
//  Например, инструкция paddq выполняется за шесть тактов на регистрах XMM, но только за два такта - на MMX.
// pshufw          mm1, mm0, 0xE4; // (два такта) быстрое копирование одного регистра в другой.
// movq (шесть тактов)  // pshufw mm0, mm0, 0xE4 // pshufd xmm0, xmm0, 0xE4

/// +++++ movdqu — перемещение невыровненных 128 бит из источника в приемник.( MOVDQA должны быть выровнены )
// PADDQ        xmm3, xmm0;   // ++ packsswb  +xPADDQ      movlhps  MOVHPS  MOVHLPS   MOVLPS


/**/

//------------------------------------------------------------------------------
//  Регистры EAX,EBX,EDX,ECX называют рабочими регистрами               // neg
//  кроме того в процессе выполнения этой программы служебно не модифицируются
//  регистры  ebx  ecx  esp  esi  edi    кроме ebp

// "Выражения asm должны сохранять регистры EDI, ESI, ESP, EBP и EBX, но могут
// свободно изменять регистры EAX, ECX и EDX."


		  // вычисление адреса транспонированного скана sIntA
		  mov       eax, sIntA;              //  вычисление адреса слайса скана
		  add       eax, p;                  //  добавление к адресу слайса смещения на переменную p
		  lea       eax, [eax+edx];          //  добавление к адресу транспонированного скана sIntA переменной цикла
		  //  добавление к адресу транспонированного скана sIntA переменной цикла
		  LOAD_PTR    xmm1, [eax];  // movups ebx загрузка слайса скана в регистр xmm1


		  // переход (единици не считаем), если четыре (128) подряд идущих слайса равны нолю
		  pxor           xmm0, xmm0;         // загрузка ноля  0x0000000000000000 .....    2 1 MMX_ALU
		  PCMPEQD        xmm0, xmm1;         // сравнение на равенство упакованных двойных слов. 2 1 MMX_ALU
		  MOVMSKPS       eax,  xmm0;         // в младшую тетраду регистра eax заносится знаковая маска  6 2 FP_MISC
		  cmp            eax,  0xF;          // je, сравнение eax и 00000000000000000000000000001111;
//		  comiss        xmm0, xmm1;          //  ja, comiss - работает
		  ///test            eax,  0xF;
		  je        if_zero;                 // je (cmp) переход если  флаг ноля FZ=0     jz
											  // jnz (test)	// JNE/JNZ – перейти, если меньше или равно.


		  // вычисление адреса маски On добавление к адресу переменной цикла edx
		  mov       eax, maskOnA;            // вычисление адреса маски On
		  lea       eax, [eax+edx];
		  LOAD_PTR    xmm0,  [eax];  //  esi загрузка маски maskOnA в регистр mxm0

		  // вычисление адреса маски Off добавление к адресу переменной цикла edx
		  mov       eax, maskOffA;           //  вычисление адреса маски maskOffA
		  lea       eax, [eax+edx];
		  LOAD_PTR    xmm2, [eax];  //  edi загрузка маски maskOffA в регистр xmm2

		  //  умножение & маски On, Off на транспонированный скан sIntA (слайс скана)
		  pand      xmm0, xmm1;              //  применение & маски к слайсу скана
		  pand      xmm2, xmm1;              //  применение & маски к слайсу скана

// forMAC  LDDQU . По возможности используйте инструкцию SSE3 LDDQU вместо MOVDQU. (LOAD_PTR)
// forMAC  MOVDQA (MOVe Aligned Double Quadword) 6 1 FP_MOVE — перемещение выровненных 128 бит  ( MOVAPS )
//------------------------------------------------------------------------------
//MOVDQU
// Регистровые (SSE2 128) суммы маски (подсчет единиц) mask32[0].On по  X
			MOV_PTR          xmm1, xmm0;         //  movups /// movss (без требования выравнивания)
		  // pshufd          xmm1, xmm0, 0xE4;
			psrld           xmm0, 1;            // v >> 1          Логический сдвиг вправо, с заполнением старших бит нулями
			pand            xmm0, xmm5;         // (v >> 1) & 0x55555555
			psubd           xmm1, xmm0;         // w = v - ((v >> 1) & 0x55555555)  Вычитание упакованных байт (слов или двойных слов) без насыщения
			MOV_PTR          xmm0, xmm1;         // w               Пересылка данных (64 бит) из/в регистр ММХ
			psrld           xmm1, 2;            // w >> 2
			pand            xmm0, xmm6;         // w & 0x33333333  Побитовое логическое И над всеми битами источника и приемника. Результат помещается в  приемник.
			pand            xmm1, xmm6;         // (w >> 2)  & 0x33333333           Побитовое логическое И
			paddd           xmm0, xmm1;         // x = (w & 0x33333333) + ((w >> 2) & 0x33333333)  Сложение упакованных байт (слов или двойных слов) без насыщения
			MOV_PTR          xmm1, xmm0;         // x               Пересылка данных (64 бит) из/в регистр ММХ
			psrld           xmm0, 4;            // x >> 4          Логический сдвиг вправо
			paddd           xmm0, xmm1;         // x + (x >> 4)
			pand            xmm0, xmm7;         // y = (x + (x >> 4) & 0x0F0F0F0F)
			pxor            xmm1, xmm1;         // установка ноля  0x0000000000000000    Исключающее ИЛИ  ==1
			psadbw          xmm0, xmm1;         // сборка (разностями) и сложением вдоль регистра, старшие 3 слова из 4 слов обнуляем

			pshufd          xmm0, xmm0, 0xd8;   //11011000b=0xd8// исходное 11100100b = 0xE4 ,  инверсное 0x7F; unpckhps
			psadbw          xmm0, xmm1;         // сборка (разностями)
			paddq           xmm3, xmm0;         // — сложение учетверенных слов.   // 6 taktov



// Регистровые (SSE2 128) суммы маски (подсчет единиц) mask32[0].Off по  X
			MOV_PTR          xmm0, xmm2;         //  копирование одного регистра в другой.
			MOV_PTR          xmm1, xmm2;         //  копирование одного регистра в другой.
			psrld           xmm0, 1;            //  PSLLDQ
			pand            xmm0, xmm5;
			psubd           xmm1, xmm0;
			MOV_PTR          xmm0, xmm1;
			psrld           xmm1, 2;
			pand            xmm0, xmm6;
			pand            xmm1, xmm6;
			paddd           xmm0, xmm1;
			MOV_PTR          xmm1, xmm0;
			psrld           xmm0, 4;
			paddd           xmm0, xmm1;
			pand            xmm0, xmm7;
			pxor            xmm1, xmm1;
			psadbw          xmm0, xmm1;         // раздельная (по старшей и младшей половинаи регистра xmm) сборка суммы единиц

			pshufd          xmm0, xmm0, 0xd8;    //11011000b;   // перемещение суммы единиц из старшей половины регистра в младшею
			psadbw          xmm0, xmm1;         // сборка (разностями) те сумма единиц старшей (перемещенной) и младшей половинам регистра
			paddq           xmm4, xmm0;         // суммирование единиц по всей маске  On



  if_zero:

////  окончание цикла, шаг цикла 16      // add  +16  // sub -16
			add             edx, 16;     // в edx находится переменная цикла
			cmp             edx, ecx;    // в ecx находится количество циклов mH*4
			jl             loop_start;   // jl перейти, jle перейти, если меньше или равно
//------------------------------------------------------------------------------

// заносим сумму единиц в накопительную переменную  SumBitOff , SumBitOn
			movd            SumBitOn,  xmm3;
			movd            SumBitOff, xmm4;

// возврат из режима Assembler
 } // __asm

//------------------------------------------------------------------------------

/**/

// emms требуется только после работы с MMX, так как там регистры шарятся с FPU.
// у SSE собственный пул регистров и emms уже не нужно.

////} //проверка быстродействия

	   // разность сумм по маск
	   deltaSum=SumBitOn-SumBitOff;
	   if(deltaSum<0){deltaSum=0;}

	   if(deltaSum<1||mask32[0].NMask<1)return 0;

	   // коэффициент корреляции  // КЭШ  //
	   ////correlation=deltaSum*100/mask32[0].NMask;
	   correlation=cacheCor[q]=deltaSum*100/mask32[0].NMask;
	   //DP("//__correlation="<<correlation<<END);
#ifdef DEBUGLVL_RGB	
	   if(print){
	   if(drawDataRGB[0][yP][xP-32])drawDataRGB[0][yP][xP-32]=
		 drawDataRGB[0][yP][xP-32] - ((254)<<8)/(mask32[0].NMask+1);
       }
#endif
 /**/


//TIME_PRINT
//SH(time);

	   return correlation;

/**/

}//_____________________________________________________________________________


/**/
//------------------------------------------------------------------------------
/*
 Для выделения памяти с выравниванием используется функция:

void *_mm_malloc(int size, int align)

	 size – объем выделяемой памяти в байтах (как в malloc),

	 align – выравнивание в байтах.

Для освобождения памяти, выделенной таким образом, используется функция:

void _mm_free(void *p);

Например:

	  float *x; // массив для обработки с помощью инструкций SSE

	  x=(float)_mm_malloc(N*sizeof(float),16);

	  // … здесь обработка …

	  _mm_free(x);
*/

/*
http://alglib.blogspot.com/2008/12/memset-sse.html
Когда мы используем SSE, следует учитывать, что память, с которой мы работаем,
должна быть выровнена к 16 байтам (иначе пересылка содержимого регистров XMM в
память будет крайне медленной). Поэтому работа новой функции разбивается на
три этапа. Первый заполняем память командой rep stosb, до того момента пока
указатель не будет кратен 16, затем помещаем во все байты регистра XMM0, и в
 цикле копируем его в память. И, наконец, вновь при помощи rep stosb заполняем
 остаток. На первом и последнем шаге размер буфера будет от 0 и до 15 байт.

  http://programmersclub.ru/assembler12/
 Все вышеописанные команды предназначены удобно использовать в циклах, т.е.
 при повторяющихся операциях. Префикс rep нужен для повторения команды обработки
 блоков количество раз, которое указано в регистре ecx.

  Профилировщик позволяет быстро выделить куски, с максимальным вкладом в время
  работы программы, и начинать оптимизацию с них.
   CodeAnalyst от AMD, он достаточно простой в работе, бесплатен

*/

/*
			pshufw          mm1, mm0, 0xE4;
			movups          xmm1, xmm0;
			psrld           xmm0, 1;
			pand            xmm0, xmm5;
			psubd           xmm1, xmm0;
			movups          xmm0, xmm1
			psrld           xmm1, 2;
			pand            xmm0, xmm6;
			pand            xmm1, xmm6;
			paddd           xmm0, xmm1;
			movups          xmm1, xmm0;
			psrld           xmm0, 4;
			paddd           xmm0, xmm1;
			pand            xmm0, xmm7;
			pxor            xmm1, xmm1;
			psadbw          xmm0, xmm1;
			paddq           xmm3, xmm0;
*/

// команда обеспечивает переход процессора от исполнения
// обычных команд с плавающей запятой к исполнению  MMX-команд.

///for (p= a; p < &a[n]; p++) n+=*p   // fast

// CPUID ; // Идентификация процессора с помощью команды CPUID //0Fh, 0A2h CPUID instruction
/*
				mov   ecx, m;
				add   ecx, p;
				shl   ecx, 2;
				mov   eax, sIntA;
				add   eax, ecx;
				movq  mm4, qword ptr [eax];
*/

/*
Записать 0xAADDAADDAADDAADD в регистр mm0:

movd eax, 0xAADD   // mov eax,0.
movd mm0, eax
pshufw mm0, mm0, 0x0

Примечание: Константа 0x0 скопирует содержимое первого слова в регистре, т.е. "AADD",
во все остальные слова регистра mm0.
*/

/*
paddb mmreg2, mmregl
paddb mmregЗ, mmreg2
не может выполняться одновременно,
*/

//Инструкции SSE2 параллельно оперируют двумя значениями двойной точности. В остальном они ничем ни отличаются от SSE.
//замену ADDPS на ADDPD, MULPS на MULPD, MOVAPS на MOVAPD и так далее

// _declspec( align(c) )  // X=16
// директива поставив которую (MS VC), перед идентификатором типа переменной,
// мы дадим компилятору задание выровнить адрес переменной по границе X байт

// Форматы данных и команды их обработки процессоров Pentium IV
// http://www.bestreferat.ru/referat-53991.html

// Команды MMX ( начиная с Pentium MMX, K6 )
// http://www.codenet.ru/progr/optimize/mmx.php
//------------------------------------------------------------------------------
/*
Подсчет популяции единичных бит Новые инструкции SSE4.2
POPCNT r, r/m* — (Return the Count of Number of Bits Set to 1)
Подсчет числа единичных бит. Три варианта инструкции: для 16, 32 и 64-х битных регистров.
*/

/*
PCMPEQD rxmm1, rxmm2/m128

Действие: команды сравнивают на равенство элементы источника и приемника и формируют элементы результата по следующему принципу:

• если элемент источника равен соответствующему элементу приемника, то элемент результата в зависимости от применяемой команды устанавливается равным одному из следующих значений: 0ffh, 0ffffh, 0ffffffffh;

• если элемент источника не равен соответствующему элементу приемника, то элемент результата в зависимости от применяемой команды устанавливается равным одному из следующих значений: 00h, 0000h, 00000000h.

Результат помещается в операнд приемник.


PCMPGTD rxmm1, rxmm2/m128

Действие: команда производит сравнение по условию "больше чем" элементов операндов источника и приемника и формирует элементы результата по следующему принципу:

• если элемент приемника больше соответствующего элемента источника, то элемент результата в зависимости от применяемой команды устанавливается равным одному из следующих значений: 0ffh, 0ffffh, 0ffffffffh;

• если элемент приемника не больше соответствующего элемента источника, то элемент результата в зависимости от применяемой команды устанавливается равным одному из следующих значений: 00h, 0000h, 00000000h.

Результат помещается в операнд приемник.
*/

// comiss приёмник, источник – сравнение одной пары чисел с установкой флагов.
//Выполняет сравнение нулевых вещественных чисел. Если равны, то ZF=1, CF=0.
//Если приёмник меньше, то ZF=0, CF=1. Если приёмник больше, то ZF=0, CF=0.
//После этих команд можно пользоваться командами условного перехода ja, jae, jb, jbe, je, jne.
/*
проверка на равенство PCMPEQB(byte),PCMPEQW(word),PCMPEQD(dword)
сравнение PCMPGTB,PCMPGTW,PCMPGTD
;пример работает только для dword???
movq mm0,[dataA]
movq mm1,[dataB]
movq mm2,mm1
pcmpgtb mm2,mm0
movq [dataD],mm2
psubb mm1,mm0
movq [dataE],mm1
pand mm2,mm1
paddb mm0,mm2
;идея скачана у K.Kасперского
; sub ebx,eax
; sbb ecx,ecx
; and ecx,ebx
; add eax,ecx
итог команды if(ebx<eax)
then eax=eax+(ebx-eax)
else eax=eax
ps: команда PFMIN работает на AMD
Удачи!
*/
/*
Регистры общего назначения
EAX = (16+AX=(AH+AL))
EBX = (16+BX=(BH+BL))
ECX = (16+CX=(CH+CL))
EDX = (16+DX=(DH+DL))
ESI = (16+SI)
EDI = (16+DI)
EBP = (16+BP)
ESP = (16+SP)

Регистры EAX,EBX,EDX,ECX называют рабочими регистрами. Регистры EDI, ESI –
индексные регистры, играют особую роль в строковых операциях. Регистр EBP
обычно используется для адресации в стеке параметров и локальных переменных.
Регистр ESP - указатель стека, автоматически модифицируется командами
PUSH, POP, RET, CALL. Явно используется реже.
*/
/*
Это позволяет умножать регистр с помощью LEA на такие числа:
//lea   edx, [edx+2]
LEA EAX,[EAX + EAX*2] ;3
LEA EAX,[EAX*4]       ;4
LEA EAX,[EAX + EAX*4] ;5
LEA EAX,[EAX*8]       ;8
LEA EAX,[EAX + EAX*8] ;9
*/
// выборка адресов подряд  Код одного типа (ММХ или FPU) должен быть по возможности сгруппирован
//------------------------------------------------------------------------------

/*
// TEST
static const unsigned int st64[4]={
	0x10010011, 0x10010011, 0x10010011, 0x10010011
};

static const unsigned int stFF[4]={
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
};
// TEST
movups          xmm0, dqword ptr stFF;
movups          xmm2, dqword ptr stFF;
*/

/*
PSLLDQ          xmm0, 1;
psadbw          xmm0, xmm1;

PSLLDQ          xmm0, 8;
PSRLDQ          xmm0, 8;
*/
/// MOVDQ2Q         mm0, xmm0        // Пересылка 64-битного целого из ММХ в ХММ
//PSRLDQ          xmm0, 8;     // 1 PSRLDQ сдвиг вправо приемника на число байт

			   //	jnz             loop_start;  //  переход если  флаг ноля FZ=0
								 // JNE/JNZ – перейти, если меньше или равно.

				//   Q-64 битовая переменная (QuadWord)

				////sar   ecx, 1;     //  деление количества циклов mH на 2

//   LDDQU       xmm5, dqword ptr constN; // загрузка 64-битного значения из памяти или исходного регистра (биты [63-0]) с дублированием в обоих нижней и верхней частях
//   MOVDDUP     xmm5, C55;               // значительного увеличения производительности при загрузке невыровненных 128-битных значений
//   MOVUPS      xmm5, dqword ptr constN;  // Пересылка 128-битных данных между памятью и регистрами ХММ или между регистрами ХММ (без требования выравнивания)

//psrld    PSLLDQ  Логический сдвиг вправо упакованных слов (двойных, учетверенных)
//на количество бит, указанных в операнде- источнике, с заполнением старших бит нулями


/*
// заносим сумму единиц в накопительную переменную  SumBitOff , SumBitOn
// копирование значений счетчиков единиц в масках  Off и On в регистры eax и ecx
			//mov             SumBitOn,  eax;  // ecx
			//mov             SumBitOff, ecx;  // ebx
*/


				//test    ecx,ecx    ;  //  если ecx =0 то продолжаем цикл
				//jz      loop_start ;

/*
			//MOVDQ2Q         mm0,  xmm0;
			//paddq           mm1,  mm0;

			movd            ebx,  xmm0;	        // 0ffffffffh; Результат помещается в приемник eax.
			add             esi, ebx;
// esi  edi
*/
/*
			mov             SumBitOn,  edi
			mov             SumBitOff, esi
*/
//				add             eax, SumBitOn  //SumBitOn=SumBitOn & 0x7F;
//				mov             SumBitOn, eax

//			test    eax, 0ffh;      //   11111111111111111111111111111111b;

//------------------------------------------------------------------------------

		  //pcmpeqd   xmm0, xmm1;              // если источник равен приемнику, то результат равен
		  //pshufd          xmm0, xmm0, 11011000b;   // исходное 11100100b = 0xE4 ,  инверсное 0x7F; unpckhps

		  ////		  pcmpeqd   xmm0, xmm1;              // если источник равен приемнику, то результат равен
		//  psadbw          xmm0, xmm1;
		//  CMPEQSS     xmm0, xmm1;     // CMPSS ,000b// cmpeq_ss  CMPEQSS    CMPEQPS
		// CMPPD    xmm0, xmm1,0;
////		  movd      eax,  xmm0;              // 0ffffffffh; Результат помещается в приемник eax.
////		  cmp       eax, 0ffffffffh;
											 ///  CMPSD   xmm0, xmm1,000b;
		  //comiss     xmm2, xmm0;          //0 1    // команда сравнивает пару значений по половине 128, если приемник=источник	0F=SF=AF=PF=CF=0; ZF=1

		  //------------------------------------------------------------------------------

				// MOVDDUP       xmm1,EAX
				// lddqu         xmm1, xmmword ptr maskON; //dqword
				//punpckldq       mm3, word ptr sInt+p+m;
				//MOVUPS XMM1, dword ptr sInt+p+m ;      // НЕ выровнена

				/// PINSRW XMM1,EAX,0
				///// PSLLDQ  XMM0,5; pslldq shift double quadword left logical (5 bytes)

//				movd            mm0, d3;    // v_low
//				punpckldq       mm0, d5;    // v

/*
				movq            mm5, C55;   // Пересылка данных (64 бит) из/в регистр ММХ  MOVDQU-128
				movq            mm6, C33;
				movq            mm7, COF;
*/
		//        movd            mm0, word ptr v;   // обмен данными между: MMX регистрами, стандартными 32-битным регистрами, а также ячейками памяти. (Movd, Movq)
		//        punpckldq       mm0, word ptr v + 4;

//------------------------------------------------------------------------------

  //		 int Buf16[16]={0};
  ///        memcpy (Buf2, BufTR, size*szf);
  // intarray=(int*)malloc(20*sizeof(int));
  /// malloc работает быстрее, чем calloc, в связи с отсутствием функции обнуления выделяемой
  // обнуление ImBuf, ReBuf ( где ImBuf, ReBuf глобальные )
  // memset(ReBuf,0,szf*(ncnr+512)
  ///char *cacheCor=(char*)malloc(wVh+1);      //* sizeof(signed char));  запрос памяти min без очистки 0
  //  освобождение памяти (1)
  ////  if ( cacheCor )  free(cacheCor);      // if (a)  // if (a!=NULL)   ////
  //  memset(cacheCor,255,4096);  //128*32


/*
		 // тест 4 точки
		 drawDataRGB[0][sy0][sx0-32]=0;
		 drawDataRGB[0][sy0][sx1-32]=0;
		 drawDataRGB[0][sy1][sx0-32]=0;
		 drawDataRGB[0][sy1][sx1-32]=0;
*/


/*
// Градиентный метод наискорейшего спуска. В этом методе направления соседних
// шагов ортогональны. Метод наискорейшего спуска требует решения на каждом
// шаге задачи одномерной оптимизации (нахождение максимума Lion). Этот метод
// требует меньшего числа операций, чем градиентный метод с постоянным шагом.

	// нахождение градиента по X и Y
	int GradP, GradX, GradY;
	//xP=yP=0;
	xP=(sx0+sx1)/2;  yP=(sy0+sy1)/2;
	GradP=setMatrix_Point(mask32, xP,  yP,   c_out);
	GradX=setMatrix_Point(mask32, xP+1,yP,   c_out);
	GradY=setMatrix_Point(mask32, xP,  yP+1, c_out);

	GradX=GradP-GradX;   GradY=GradP-GradY;
	// нахождение максимума градиента по X и Y
	// GradP=GradY;   if ( GradX > GradY ) { GradP=GradX; }
DP("  GradX="<<GradX<<"   GradY="<<GradY<<"   xP="<<xP<<"   yP="<<yP<<END);
*/


 /*
///////////////////////////

			// вычисление адреса транспонированного скана sIntA
///	mov          eax,  BuffA;
			mov       eax, sIntA;            //  вычисление адреса слайса скана
	 //		lea       eax, [sIntA];
			add       eax, p;                //  добавление к адресу слайса смещения на переменную p
		///	mov       eax, ebx;
			//lea       eax, [eax+ebx];      //  добавление к адресу транспонированного скана sIntA переменной цикла
		  //  добавление к адресу транспонированного скана sIntA переменной цикла
		  movups    xmm1, dqword ptr [ebx+edx];  // ebx загрузка слайса скана в регистр xmm1

		   // переход (единици не считаем), если четыре (128) подряд идущих слайса равны нолю
		   //	movss      eax, mm0; -----------
		  pxor      xmm0, xmm0;              // загрузка ноля  0x0000000000000000  // CMPPD xmm, xmm/m128, 0
		  pcmpeqd   xmm0, xmm1;              // если источник равен приемнику, то результат равен
	   // CMPPD    xmm0, xmm1,0;
		  movd      eax,  xmm0;              // 0ffffffffh; Результат помещается в приемник eax.
		  cmp       eax, 0ffffffffh;
		  je        if_zero;                 // je переход если  флаг ноля FZ=0     jz


		//  вычисление адреса маски maskOnA
///	mov          eax,  BuffA;
			mov       eax, maskOnA;
			//add       eax, edx;
			//lea       eax, [eax+edx];        //  добавление к адресу маски переменной цикла
		  // добавление к адресу переменной цикла, умножение маски On на транспонированный скан sIntA
		  movups    xmm0, dqword ptr [esi+edx];  //  esi загрузка маски maskOnA в регистр mxm1
		  pand      xmm0, xmm1;              //  применение & маски к слайсу скана

///	mov          eax,  BuffA;
			mov       eax, maskOffA
			//add       eax, edx;
			//lea       eax, [eax+edx];        //  добавление к адресу маски переменной цикла
		  // добавление к адресу переменной цикла, умножение маски Off на транспонированный скан SInt
		  movups    xmm2, dqword ptr [edi+edx];  //  edi загрузка маски maskOffA в регистр xmm2
		  pand      xmm2, xmm1;              //  применение & маски к слайсу скана

///////////////////////////
*/

/*
		  // добавление к адресу транспонированного скана sIntA переменной цикла  movups
		  MOVDQU    xmm1, dqword ptr [ebx+edx];  // movups ebx загрузка слайса скана в регистр xmm1  LDDQU

		  // переход (единици не считаем), если четыре (128) подряд идущих слайса равны нолю
		  pxor           xmm0, xmm0;         // загрузка ноля  0x0000000000000000 .....
		  PCMPEQD        xmm0, xmm1;         // сравнение на равенство упакованных двойных слов.
		  MOVMSKPS       eax,  xmm0;         // в младшую тетраду регистра eax заносится знаковая маска
		  cmp            eax,  0xF;          // je, сравнение eax и 00000000000000000000000000001111;
//		  comiss        xmm0, xmm1;          //  ja, comiss - работает
		  je        if_zero;                 // je переход если  флаг ноля FZ=0     jz
											   // JNE/JNZ – перейти, если меньше или равно.
										   //     jz

		  // добавление к адресу переменной цикла, умножение маски On на транспонированный скан sIntA
		  MOVDQU    xmm0, dqword ptr [esi+edx];  //  esi загрузка маски maskOnA в регистр mxm1
//		  pand      xmm0, xmm1;              //  применение & маски к слайсу скана

		  // добавление к адресу переменной цикла, умножение маски Off на транспонированный скан SInt
		  MOVDQU    xmm2, dqword ptr [edi+edx];  //  edi загрузка маски maskOffA в регистр xmm2

		  pand      xmm0, xmm1;              //  применение & маски к слайсу скана
		  pand      xmm2, xmm1;              //  применение & маски к слайсу скана
*/


/*
			///movd            eax, mm0;

			// 3f (127p) Обнуляем старшие разряды, содержащие "мусор"
			///and             eax, 3fh;
			//заносим сумму единиц в накопительнй регистр  ecx
			///add             ecx, eax;
//	  		   mov             SumBitOn, ecx;

			///movd            eax, mm0;

			// 3f (127p) Обнуляем старшие разряды, содержащие "мусор"
			///and             eax, 3fh;   // 000000000000000000000000000111111b;
			//заносим сумму единиц в накопительнй регистр  ebx
			///add             ebx, eax;
 //			mov             SumBitOff, ebx;

			///mov             SumBitOff, ebx;
			///mov             SumBitOn,  ecx;
*/

			// тест состоящий из едениц в 64р слове
			//unsigned long long test1[1]={ 0xFFFFFFFFFFFFFFFF};
			//static const long long d5=0x00000000FFFFFFFF;

///static const unsigned long long C55[1]={0x5555555555555555};
///static const unsigned long long C33[1]={0x3333333333333333};
///static const unsigned long long COF[1]={0x0f0f0f0f0f0f0f0f};

/*
Используя программку предыдущего докладчика:
tav's code 525552
c code 158260
ip's c code 104830

ftp://download.intel.com/software/products/compilers/downloads/ - это подборка компиляторов
ftp://download.intel.com/software/products/vtune/downloads/ - это VTune,  он НАМНОГО важнее компилятора.
Особенно полезен если его гонять на родном процессоре, хотя и на Athlon'е он общую ситуацию показывает точно.

Перед тем как ставить найди пиратский диск с ними, возьми от туда только файлы
лицензий и скопируй себе на винт. Далее спокойно ставь, у меня дома древняя пиратская
лицензия от 6 ого компилятора подошла к последним версиям этих программ.

*/
/**/

