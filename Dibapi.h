// dibapi.h   λͼ����ͷ�ļ�������1998��4��
//

#ifndef _INC_DIBAPI
#define _INC_DIBAPI

/* Handle to a DIB */
DECLARE_HANDLE(HDIB);

/* DIB constants */
#define PALVERSION   0x300

/* DIB Macros*/

#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))
    //Windows3.0���BITMAPINFOHEADER�ĵ�һ���ֶΣ�
    //               DWORD biSize(���ṹ��Ҫ����ֽ���)

#define RECTWIDTH(lpRect)     ((lpRect)->right - (lpRect)->left)
#define RECTHEIGHT(lpRect)    ((lpRect)->bottom - (lpRect)->top)


// WIDTHBYTES performs DWORD-aligning of DIB scanlines.  The "bits"
// parameter is the bit count for the scanline (biWidth * biBitCount),
// and this macro returns the number of DWORD-aligned bytes needed
// to hold those bits.

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

/* Function prototypes */
//In dibapi.cpp
BOOL      WINAPI  PaintDIB (HDC, LPRECT, HDIB, LPRECT, CPalette* pPal);
BOOL      WINAPI  CreateDIBPalette(HDIB hDIB, CPalette* cPal);
LPSTR     WINAPI  FindDIBBits (LPSTR lpbi);
DWORD     WINAPI  DIBWidth (LPSTR lpDIB);
DWORD     WINAPI  DIBHeight (LPSTR lpDIB);
WORD      WINAPI  PaletteSize (LPSTR lpbi);
WORD      WINAPI  DIBNumColors (LPSTR lpbi);
HGLOBAL   WINAPI  CopyHandle (HGLOBAL h);
HDIB      WINAPI  BitmapToDIB (HBITMAP hBitmap, HPALETTE hPal);
HBITMAP   WINAPI  DIBToBitmap (HDIB hDIB, HPALETTE hPal);

//{{���ݵĴ���
//�������λͼ
HBITMAP   WINAPI  Create24BitsBitmap(DWORD Width,DWORD Height);
HBITMAP   WINAPI  Create8BitsBitmap(DWORD Width,DWORD Height);

//256���Ҷ�λͼ�����á�ȡ
inline BYTE  WINAPI GetGrayPixel(BYTE FAR* pImage,DWORD i,DWORD j,DWORD Width,DWORD Height)
{return *(pImage+(Height-1-j)*WIDTHBYTES(Width * 8)+i);}

inline BYTE  WINAPI SetGrayPixel(BYTE FAR* pImage,DWORD i,DWORD j,DWORD Width,DWORD Height,BYTE Color)
{return *(pImage+(Height-1-j)*WIDTHBYTES(Width * 8)+i)=Color;}
//}}

//In File.cpp
BOOL      WINAPI  SaveDIB (HDIB hDib, CFile& file);
HDIB      WINAPI  ReadDIBFile(CFile& file);

/*
* In Color.cpp
*  CreateBIPalette()   - Creates a GDI palette given a pointer            *
*                                      to a BITMAPINFO structure.         *    *
*                                                                             *
*/
HPALETTE WINAPI CreateBIPalette (LPBITMAPINFOHEADER lpbi);

//In Copy.cpp
//In Print.cpp
//In Draw.cpp
//In Errors.cpp


#endif //!_INC_DIBAPI
