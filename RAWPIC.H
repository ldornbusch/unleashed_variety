#ifndef __rawpicture__
#define __rawpicture__
/////////////////////////////////////////////////////////////////////////////
// Tempor„re Klass nur um 24 Bit per Pixel Bilder in 32Bpp umzuwandeln
// Wird von CBitMap aus aufgerufen.
// RE->CODED BY MAC SYSTEMS 13.11.99
/////////////////////////////////////////////////////////////////////////////

class PictureContainer
{
    friend class Pcx;
public:

    int PictureHeight;
    int PictureWidth;
    int SizeOfPicture;
    unsigned char * pPicture;

    void ImportPcxPicture(Pcx * pPcxSource);
    void PrintPictureInfos();
    void CopyPicture(unsigned int * pDest);
protected:
    void ErrorPrint(char * errormsg);
public:
    PictureContainer();
    virtual ~PictureContainer();

};

#endif
