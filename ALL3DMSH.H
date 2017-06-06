// All3DMesh.h: Schnittstelle für die Klasse CAll3DMesh.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALL3DMESH_H__82CA5161_5C07_11D3_A6F8_0080AD40DBC7__INCLUDED_)
#define AFX_ALL3DMESH_H__82CA5161_5C07_11D3_A6F8_0080AD40DBC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+++++++                                                ++++++++
++++       CAll3DMesh-Klasse kapselt die Polygone und      ++++
++                                                           ++ 
++           ermöglicht Instanzierungen. All3DMesh wird      ++ 
++                                                           ++ 
++           durch CAll3DObject instanziert und kann so in   ++ 
++                                                           ++ 
++           einer Scene dargestellt werden.                 ++ 
++++                                                       ++++
+++++++       code: LouyThorn 990826                   ++++++++
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
- 26. Aug 99:	Jawoll! Finally I start to encode my next Engine:
	[23:40h]		All3D II. In C++!! Yeah Today I started to define 
				some DataStructures and I have spend a lot of 
				Time to think about the Architecture. I met 
				another Coder (MAC SYSTEM) who will start with me
				to encode this Piece of Gold!!! I will make it very 
				modern with 4x4 Matrices, different Clippings,
				Very tough HSR ...
				I am very excited... So we will see..
- 03.Sept 99:	Das Erste Fenster steht nun schon und die ersten 
	[01:58h]		Datenstrukturen auch. Ich werde die Architektur der
				Engine entwerfen, während Jens die Low_level 
				GFX-Routinen schreibt. am 19.09.ist erster Code-Abgleich
				Heute habe ich nur das Fenster auf den Screen gebracht,
				damit ich irgendwo hin Zeichnen kann..
- 11.Sept 99: Nun steht die Windows PolyFill Routine, die in einem 
	[17:00h]		BitMap-Object gekapselt ist. Ausserdem noch einige 
				Get/Set-Routinen und ein TEST-CAll3DObject hinzugefügt.
				Irgendetwas funktioniert nicht, wenn ich ein CAll3DVector- 
				Array mittels new CAll3DVector[x] definiere... der Destruktor 
				stürzt dann immer ab ?!?!?!Noch gerade eben den Fehler gefunden:
				Bei Objekt-ARRAYs mus der Destruktor mit delete [] ... 
				aufgerufen werden. Mal wieder was dazu gelernt...
- 13.Sept 99: Das TestCAll3DObject kann nun von BitMap mittels einer 
	[01:45h]	ShowObject Routine angezeigt werden. Ausserdem heute noch 
				4x4 Matrizen hinzugefügt und denm Vector noch einige Operatoren
				(+,-,s-mul, x-mul) spendiert, wie auch eine w-Koordinate für 
				homogene Koords. .. Heute muss ich nun leider schlafen gehen..
				Morgen wieder arbeiten. Der Grundstein steht nun in etwa, 
				aber ich muss noch viel an  der Architektur arbeiten, damit die
				Pipeline auch nur die Faces/Vertices bearbeitet, die auch 
				sichtbar sind...
- 19.Sept 99: So, HiddenLine und Colors stehen... Heute war das erste Meeting... 
	  [07:34h]	in 14 Tagen werden wir weitersehen. Heute total viele dumme 
				Fehler gemacht, die viel Zeit gekostet haben (Normale vom Ursprung 
				weg transformiert und dann Z-Check(!) usw.) Auch mit den Farben hat 
				das nicht auf anhieb geklappt. Morgen kommt Lightshading dran und 
				Camera/Lights. Dabei wird auch die Engine Struktur der Befehlsobjekte
				implementiert.
- 24.Sept 99: Shading (only Directional) steh auch und ich habe ein 80-Faces Mesh aus Max importiert...
	  [04:50h]		Die Implementierung des Shadings ging relativ schnell, aber 
				Transformation stimmt noch nicht, ebenso wie Licht noch nicht 100% lauft
				in 4std Muss ich wiederarbeiten...
- 25.Sept 99: Shading laeuft nun 100% (wenigstens Monochrom Directional) Ich hatte in 
		[19:19h]	dem Material Helligkeit-Array eine  solche /\ Funktion eingetragen, 
				welche natürlich Falsch ist, da der Cosinus(0)=1 ist und 1*256=256 ist, dadurch 
				kam ein Beleuchtungseffekt zustande, der die dunkelsten, wie auch die hellsten
				Stellen ganz dunkel machte, die mittleren aber total Hell!! Ich habe auch ein 
				Specular eingebaut, Ich Meine damit, das das Face Weiss wird, wenn das Licht 
				genau senkrecht einfällt, dazu muss als Helligkeit einfach nur 255 eigetragen 
				werden und dann wird das Licht einfach auf Weiss gestellt. Später kann man 
				die Spekular-farbe, wie auch die Ambient und Hilight-farbe frei wählen...
- 26.Sept 99:	Die Transformation stimmte noch nicht, (irgendein scheiss Bug) aber nun läuft 
		[18:23h]	das Baby wieder. Die Camera habe ich nun auch implementiert und setze Sie nun in 
				das Transformationsmodul ein. Danach sollten keine Darstellungsfehler mehr 
				zu sehen sein...Ich arbeite nun seit exakt 1 Monat an dieser Engine und sie ist jetzt
				Schon Leistungsfähiger als meine PascalEngine..Mal sehen, wie es weitergeht
- 03.Okt 99:	Schon wieder eine Woche ins Land gegangen...Wenig geschafft, aber die Transformation
		[17:41h]	wie auch die Projektion ist Korrekt. Ich bin nun mit allen Modulen in der 
				Betaphase fertig. An dem NormalenCheck saß ich noch ziemlich lange, da die Camera ja
				um -128 Pkt (AUG) hinter ihrem eigentlichen Standort steht. Deswegen klappte das 
				alles letzte Woche noch nicht. Na ja, nun steht dieser Teil schonmal, aber noch 
				nicht Performance.orientiert. Ich baue nun erst das DirectX[Draw]-Modul, damit Jens
				Dort seine PolyFill Routinen reinhängen kann. Ansonsten werde ich danach noch 
				eine PolySort Routine in das Render Modul setzen, da ich dies bisher 
				noch völlig vernachlässigt habe. Das Modul All3DEngine wird nun implementiert 
				mit der endgültigen Rendering-Pipeline...
- 22.Okt 99:	Nun habe ich aber echt geschludert... Die DirectX-Implementierung war total Scheisse
		[08:45h]	schwer. ich habe zwei Wochen darauf verschwendet... Nun haben wir aber ein 
				Klasse, die entweder einen Fullscreen, oder einen Windowed-Mode initialisiert und 
				dazwischen auch wechseln kann!! MAC hat auch eine gleiche DOS-Demo schon fertig und 
				wir treffen uns nächste Woche Donnerstag, 28.10.99 um nochmal Codingmässig alles auf 
				die Reihe zu bekommen. Ich muss nun die CAll3DEngine Klasse noch schreiben, die ein
				Dach über das alles setzt. Darein muss dann auch noch der Polygonsort(RADIX-Sort).
- 01.Nov 99: Nicht erschrecken.. ich habe eingíges getan, auch wenn es nicht soviel war wie 
		[17:26h]	ich wollte. Vorgestern habe ich einen PCX-Loader gerschrieben und er funktionierte 
				auf Anhieb. Nicht, dass das was besonderes Schwieriges ist, aber mit Filezugriffen hatte 
				Ich ja nochie etwas versucht...Ich habe Die Engine deswegen auf 640x480x32 ugestrickt. Und 
				was musste ich zuhause erkennen? Nichts funktionierte mehr: DirectX schafft es nicht, einen
				Screenvon 32-Bit auf 16 Bit zu Blitten!!! (wenigstens nicht auf meiner Karte) deswegen 
				muss ich nun eine ConverterRoutine schreiben, die ie Interne Darstellung von 640x480x32Bit 
				auf die geforderte erlaubte Bildschirmauflösung runterrechnet: entweder 32,24,16 oder 8 Bit:
				Das ist meiner meinung nicht aller schnellst. Ausserdem knn ich dann nicht PageFlipping 
				benutzen, denn es kann sein, das die Primary-Surface keine 32-Bit Farbtiefe unterstützt. Das
				Bedeutet aber einen 640x480 Blit mehr... namlich um den Internen 32-Bit auf den Anzeige 
				Modus zu Konvertieren...
- 07.Dez 99: Scheisse! Ich war nun so in yagma.com involviert, dass ich nicht mehr zu etwas gekommen bin..
		[7:45h]		aber am Wochenende habe ich die Architektur auf Hermes umgestellt, den schnelltsten 
				PixelConverter, der wohl da ist. Gestern war ich auch wieder nah am Zusammenbruch, da 
				plötzlich der FullScreenmodus nicht mehr lief. 3 Std. Ich baue nun eien Linienalgorithmus 
				ein und lass mac endlich den anderen Scheiss übernehmen.. Die PolyRoutine, die der gebracht hat,
				funct hinten und vorne nicht(Keine Variable Breite...)
				Das Gnaze läuft nun so ab, dass ich versuche, 2 32-Bit Buffer zu erhalten, falls die Karte 
				dass nicht darstellen kann, lege ich 2 andere Buffer an, die die Karte noch hinbekommt.
				und einen 32-Bit Buffer, genannt HermesBuffer in den Rendere ich alles schön rein und 
				kopiere alles später auf die DirectX-Surfaces. Falls ich 32-Bit unterstütze, sezte ich 
				den Backbuffer in eine CBitmapklasse ein(via Init(..)) und Render dann auf diesen direkt. 
				Dadurch spare ich einen Converter-vorgang und Hermes ganz..Nun bastel ich noch eine 
				Line-Routine und bis zum Ende der Woche will ich noch einen Wasser-FX und einen Bump-FX haben
		[20:39h]	Soo. Der Hermes Funktioniert nun.. nur noch nicht im Paletten-Modus, is aber eh egal...
				Nun die FXs Das Hermes Ding war echt kinderleicht...aber die Doku is sauschlecht, naja, wenns 
				kostenlos ist. Ich mache mich nun an Bump-Mapping und Fluidmapping ran, danach an Blittings...
*/			
#include "All3DVec.h"
#include "All3DMat.h"

class CAll3DMesh  
{
public:
	void SetNormals(CAll3DVector* norms);
	CAll3DVector* GetNormals();
	void CreateVertexNormals();
	void CreateFaceNormals();
	CAll3DMaterial* GetMaterial();
	void SetMaterial(CAll3DMaterial* Mat);
	CAll3DMesh(int numVertices, int NumFaces);
	int GetViewMode();
	void SetViewMode(int VMode);
	void SetNumVertices(int intNumber);
	int GetNumVertices();
	int GetNumFaces();
	void SetNumFaces(int intNumber);
	void SetVertices(CAll3DVector* ptrVertices);
	void SetFaces(int* ptrFace);
	int* GetFaces();
	CAll3DVector* GetVertices();
	CAll3DMesh();
	virtual ~CAll3DMesh();

private:
	CAll3DMaterial* Materials;			// ARRAY of Materials for each Face
	CAll3DVector* All3DVertexNormals;	// ARRAY of Normals of the Vertices
	CAll3DVector* All3DNormals;			// ARRAY of Normals of the Faces
	CAll3DVector* All3DVertices;		// ARRAY of Vertices
	int* Faces;							// Faces
	int ViewMode;						// ViewMode
	int NumFaces;						// Number of Faces
	int NumVertices;					// Number of Vertices
};

#endif // !defined(AFX_ALL3DMESH_H__82CA5161_5C07_11D3_A6F8_0080AD40DBC7__INCLUDED_)
