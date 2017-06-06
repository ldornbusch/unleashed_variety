// BMText.cpp: Implementierung der Klasse CBMText.
//
//////////////////////////////////////////////////////////////////////

#include "BMText.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CBMText::CBMText(unsigned char * Text,int width)
{
    pText = Text;
    TextWidth = width;
}
/////////////////////////////////////////////////////////////////////////////
CBMText::CBMText()
{

}
/////////////////////////////////////////////////////////////////////////////
CBMText::~CBMText()
{

}
/////////////////////////////////////////////////////////////////////////////
// Alte Write String Methode nur als WriteStringAdd declariert
/////////////////////////////////////////////////////////////////////////////
void CBMText::WriteStringAdd(CBitMap* Dest,int x, int y, char *String, int len)
{
	char l;
	int xco,yco;
	int letter=0;
	if (len<0)	len=32000;
	xco=x;
	yco=y;
	l=String[letter++];
	while ((l!=0)&&(letter<len))
	{
		if (xco<GetWidth())
			WriteCharAdd(Dest, xco,yco,l);
		xco+=FontWidth;
		l=String[letter++];
	}
}
/////////////////////////////////////////////////////////////////////////////
// Neue Write String Routine die keinen BlitAdd aufruft !!!
/////////////////////////////////////////////////////////////////////////////
bool CBMText::WriteString(CBitMap* Dest,int x, int y, char *String, int len)
{
	char l;
	int xco,yco;
	int letter=0;
	
	//if (len <0)	len=32000;

	xco=x;
	yco=y;
	
	l=String[letter++];
	
	while ((l!=0)&&(letter<len))
	{
		if (xco<GetWidth())
			WriteChar(Dest, xco,yco,l);
		xco+=FontWidth;
		l=String[letter++];
	}

	if (l)
		return false; // wenn nicht string ende !
	else
		//return true;	  // 	
		return true;

}
/////////////////////////////////////////////////////////////////////////////
void CBMText::WriteChar(CBitMap* Dest, int x, int y, char strChar)
{
	int xle=0;
	int yle=0;
	int index=0;
	while ((TranslationTable[index]!=strChar)&&TranslationTable[index]!=0) index++;
	yle=(int)(index/CharPerRow)*FontHeight+1;
	xle= (index-(xle * CharPerRow))*FontWidth;
    //BltAdd(xle,yle,FontWidth, FontHeight-1, Dest, x, y);
    BltFast(xle,yle,FontWidth, FontHeight-1, Dest, x, y);
}
//////////////////////////////////////////////////////////////////////////////
void CBMText::WriteCharAdd(CBitMap* Dest, int x, int y, char strChar)
{
	int xle=0;
	int yle=0;
	int index=0;
	while ((TranslationTable[index]!=strChar)&&TranslationTable[index]!=0) index++;
	yle=(int)(index/CharPerRow)*FontHeight+1;
	xle= (index-(xle * CharPerRow))*FontWidth;
    BltAdd(xle,yle,FontWidth, FontHeight-1, Dest, x, y);
    //BltFast(xle,yle,FontWidth, FontHeight-1, Dest, x, y);
}
/////////////////////////////////////////////////////////////////////////////
void CBMText::WriteScatter(CBitMap *Dest, int x, int y, char *String, int scatter, int Len)
{
	x=x+rand()%scatter-(scatter>>1);
	y=y+rand()%scatter-(scatter>>1);
	WriteStringAdd(Dest,x,y,String,Len);
}
//////////////////////////////////////////////////////////////////////////////
bool CBMText::WriteStringWithDelay(CBitMap* Dest,int x,int y,char * strChar,int Len)
{
	//int xle=0;
	//int yle=0;
	//int index=0;
	//while ((TranslationTable[index]!=strChar)&&TranslationTable[index]!=0) index++;
	//yle=(int)(index/CharPerRow)*FontHeight+1;
	//xle= (index-(xle * CharPerRow))*FontWidth;
    //BltAdd(xle,yle,FontWidth, FontHeight-1, Dest, x, y);
	//WriteString(Dest,x,y,strChar,-1);

	//static int delay = 15;

	bool maxstringlenght;


	/*
	if (!delay)		
	{
		delay =60;
		
		maxstringlenght = WriteString(Dest,x,y,strChar,Len);
	}
	else
		maxstringlenght = WriteString(Dest,x,y,strChar,Len);
	*/
	maxstringlenght  = WriteString(Dest,x,y,strChar,Len);

	return maxstringlenght;
}