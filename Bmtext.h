// BMText.h: Schnittstelle für die Klasse CBMText.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BMTEXT_H__01AD9087_BAD1_11D3_B8D6_0000E8699C86__INCLUDED_)
#define AFX_BMTEXT_H__01AD9087_BAD1_11D3_B8D6_0000E8699C86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdlib.h>
#include "BitMap.h"

class CBMText : public CBitMap  
{
public:
    CBMText(unsigned char *pText,int width);
    CBMText();
	virtual ~CBMText();
//Inline Interface GET/SET - Routinen
	int GetPixPerCharRow()			{ return PixPerCharRow;			}
	int GetCharPerRow()				{ return CharPerRow;				}
	char* GetTranslationTable()	{ return TranslationTable;	}
	int GetFontHeight()				{ return FontHeight;				}
	int GetFontWidth()					{ return FontWidth;					}
	int GetTextMode()					{ return TextMode;					}
	void SetPixPerCharRow(int iParam)		{ PixPerCharRow=iParam;		}
	void SetCharPerRow(int iParam)			{ CharPerRow=iParam;			}
	void SetTranslationTable(char* iParam){ TranslationTable=iParam;}
	void SetFontHeight(int iParam)			{ FontHeight=iParam;			}
	void SetFontWidth(int iParam)				{ FontWidth=iParam;				}
	void SetTextMode(int iParam)				{ TextMode=iParam;				}
//  Public Methods
	bool WriteString(CBitMap* Dest, int x, int y, char* String, int len=-1);
	void WriteStringAdd(CBitMap* Dest, int x, int y, char* String, int len=-1);
	void WriteScatter(CBitMap* Dest, int x, int y, char* String, int scatter=3, int Len=-1);
	//void WriteStringWithDelay(CBitMap* Dest,int x.int y,char * String,int delay=1);
    bool WriteStringWithDelay(CBitMap* Dest,int x,int y,char * strChar,int Len);
private:
	int PixPerCharRow;			//The Number of Pixels in every CharRow (= CharPerRow*FontWidth*FontHeight)
	int CharPerRow;					//How many Chars in everyRow of the BMap
	char* TranslationTable;	//The TranslationTable to find any Chars
	int FontHeight;					//The Height of Font in Pixels
	int FontWidth;					//The Width of Font in Pixels
	int TextMode;						//The Blitmode: Solid, transp, halfhalf, additional: 0-4
    unsigned char * pText;
    int TextWidth;

    void WriteChar(CBitMap* Dest, int x, int y, char strChar);
    void WriteCharAdd(CBitMap* Dest, int x, int y, char strChar);

};

#endif // !defined(AFX_BMTEXT_H__01AD9087_BAD1_11D3_B8D6_0000E8699C86__INCLUDED_)
