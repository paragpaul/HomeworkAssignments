// SmithWatermanCode.cpp : This is the code for a Dynamic Programming based implementation of HW#2 for CSE P527
// class for PMP. Winter 2018.
//

#include "stdafx.h"
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;
// Appendix Comments - minimize for smoother reading.
// Using Blosum matrix from the source : [ https://www.ncbi.nlm.nih.gov/Class/FieldGuide/BLOSUM62.txt ]
//  #  Matrix made by matblas from blosum62.iij
//  #  * column uses minimum score
//  #  BLOSUM Clustered Scoring Matrix in 1 / 2 Bit Units
//  #  Blocks Database = / data / blocks_5.0 / blocks.dat
//  #  Cluster Percentage : >= 62
//  #  Entropy = 0.6979, Expected = -0.5209
//     A  R  N  D  C  Q  E  G  H  I  L  K  M  F  P  S  T  W  Y  V  B  Z  X  *
//  A  4 - 1 - 2 - 2  0 - 1 - 1  0 - 2 - 1 - 1 - 1 - 1 - 2 - 1  1  0 - 3 - 2  0 - 2 - 1  0 - 4
//  R - 1  5  0 - 2 - 3  1  0 - 2  0 - 3 - 2  2 - 1 - 3 - 2 - 1 - 1 - 3 - 2 - 3 - 1  0 - 1 - 4
//  N - 2  0  6  1 - 3  0  0  0  1 - 3 - 3  0 - 2 - 3 - 2  1  0 - 4 - 2 - 3  3  0 - 1 - 4
//  D - 2 - 2  1  6 - 3  0  2 - 1 - 1 - 3 - 4 - 1 - 3 - 3 - 1  0 - 1 - 4 - 3 - 3  4  1 - 1 - 4
//  C  0 - 3 - 3 - 3  9 - 3 - 4 - 3 - 3 - 1 - 1 - 3 - 1 - 2 - 3 - 1 - 1 - 2 - 2 - 1 - 3 - 3 - 2 - 4
//  Q - 1  1  0  0 - 3  5  2 - 2  0 - 3 - 2  1  0 - 3 - 1  0 - 1 - 2 - 1 - 2  0  3 - 1 - 4
//  E - 1  0  0  2 - 4  2  5 - 2  0 - 3 - 3  1 - 2 - 3 - 1  0 - 1 - 3 - 2 - 2  1  4 - 1 - 4
//  G  0 - 2  0 - 1 - 3 - 2 - 2  6 - 2 - 4 - 4 - 2 - 3 - 3 - 2  0 - 2 - 2 - 3 - 3 - 1 - 2 - 1 - 4
//  H - 2  0  1 - 1 - 3  0  0 - 2  8 - 3 - 3 - 1 - 2 - 1 - 2 - 1 - 2 - 2  2 - 3  0  0 - 1 - 4
//  I - 1 - 3 - 3 - 3 - 1 - 3 - 3 - 4 - 3  4  2 - 3  1  0 - 3 - 2 - 1 - 3 - 1  3 - 3 - 3 - 1 - 4
//  L - 1 - 2 - 3 - 4 - 1 - 2 - 3 - 4 - 3  2  4 - 2  2  0 - 3 - 2 - 1 - 2 - 1  1 - 4 - 3 - 1 - 4
//  K - 1  2  0 - 1 - 3  1  1 - 2 - 1 - 3 - 2  5 - 1 - 3 - 1  0 - 1 - 3 - 2 - 2  0  1 - 1 - 4
//  M - 1 - 1 - 2 - 3 - 1  0 - 2 - 3 - 2  1  2 - 1  5  0 - 2 - 1 - 1 - 1 - 1  1 - 3 - 1 - 1 - 4
//  F - 2 - 3 - 3 - 3 - 2 - 3 - 3 - 3 - 1  0  0 - 3  0  6 - 4 - 2 - 2  1  3 - 1 - 3 - 3 - 1 - 4
//  P - 1 - 2 - 2 - 1 - 3 - 1 - 1 - 2 - 2 - 3 - 3 - 1 - 2 - 4  7 - 1 - 1 - 4 - 3 - 2 - 2 - 1 - 2 - 4
//  S  1 - 1  1  0 - 1  0  0  0 - 1 - 2 - 2  0 - 1 - 2 - 1  4  1 - 3 - 2 - 2  0  0  0 - 4
//  T  0 - 1  0 - 1 - 1 - 1 - 1 - 2 - 2 - 1 - 1 - 1 - 1 - 2 - 1  1  5 - 2 - 2  0 - 1 - 1  0 - 4
//  W - 3 - 3 - 4 - 4 - 2 - 2 - 3 - 2 - 2 - 3 - 2 - 3 - 1  1 - 4 - 3 - 2 11  2 - 3 - 4 - 3 - 2 - 4
//  Y - 2 - 2 - 2 - 3 - 2 - 1 - 2 - 3  2 - 1 - 1 - 2 - 1  3 - 3 - 2 - 2  2  7 - 1 - 3 - 2 - 1 - 4
//  V  0 - 3 - 3 - 3 - 1 - 2 - 2 - 3 - 3  3  1 - 2  1 - 1 - 2 - 2  0 - 3 - 1  4 - 3 - 2 - 1 - 4
//  B - 2 - 1  3  4 - 3  0  1 - 1  0 - 3 - 4  0 - 3 - 3 - 2  0 - 1 - 4 - 3 - 3  4  1 - 1 - 4
//  Z - 1  0  0  1 - 3  3  4 - 2  0 - 3 - 3  1 - 1 - 3 - 1  0 - 1 - 3 - 2 - 2  1  4 - 1 - 4
//  X  0 - 1 - 1 - 1 - 2 - 1 - 1 - 1 - 1 - 1 - 1 - 1 - 1 - 1 - 2  0  0 - 2 - 1 - 1 - 1 - 1 - 1 - 4
//  * -4 - 4 - 4 - 4 - 4 - 4 - 4 - 4 - 4 - 4 - 4 - 4 - 4 - 4 - 4 - 4 - 4 - 4 - 4 - 4 - 4 - 4 - 4  1

// FASTA values -- 
// Homo Sapiens (Human) MYOD1_HUMAN
//>sp|P15172|MYOD1_HUMAN Myoblast determination protein 1 OS=Homo sapiens GN=MYOD1 PE=1 SV=3
char P15172[] = "MELLSPPLRDVDLTAPDGSLCSFATTDDFYDDPCFDSPDLRFFEDLDPRLMHVGALLKPEEHSHFPAAVHPAPGAREDEHVRAPSGHHQAGRCLLWACKACKRKTTNADRRKAATMRERRRLSKVNEAFETLKRCTSSNPNQRLPKVEILRNAIRYIEGLQALLRDQDAAPPGAAAAFYAPGPLPPGRGGEHYSGDSDASSPRSNCSDGMMDYSGPPSGARRRNCYEGAYYNEAPSEPRPGKSAAVSSLDCLSSIVERISTESPAAPALLLADVPSESPPRRQEAAAPSEGESSGDPTQSPDAAPQCPAGANPNPIYQVL";

// Homo sapiens (Human) TAL1_HUMAN
// TAL1_HUMAN T-cell acute lymphocytic leukemia protein 1 OS=Homo sapiens GN=TAL1 PE=1 SV=2
char P17542[] = "MTERPPSEAARSDPQLEGRDAAEASMAPPHLVLLNGVAKETSRAAAAEPPVIELGARGGPGGGPAGGGGAARDLKGRDAATAEARHRVPTTELCRPPGPAPAPAPASVTAELPGDGRMVQLSPPALAAPAAPGRALLYSLSQPLASLGSGFFGEPDAFPMFTTNNRVKRRPSPYEMEITDGPHTKVVRRIFTNSRERWRQQNVNGAFAELRKLIPTHPPDKKLSKNEILRLAMKYINFLAKLLNDQEEEGTQRAKTGKDPVVGAGGGGGGGGGGAPPDDLLQDVLSPNSSCGSSLDGAASPDSYTEEPAPKHTARSLHPAMLPAADGAGPR";

// 	Mus musculus (Mouse) MYOD1_MOUSE
// >sp|P10085|MYOD1_MOUSE Myoblast determination protein 1 OS=Mus musculus GN=Myod1 PE=1 SV=2
char P10085[] = "MELLSPPLRDIDLTGPDGSLCSFETADDFYDDPCFDSPDLRFFEDLDPRLVHMGALLKPEEHAHFPTAVHPGPGAREDEHVRAPSGHHQAGRCLLWACKACKRKTTNADRRKAATMRERRRLSKVNEAFETLKRCTSSNPNQRLPKVEILRNAIRYIEGLQALLRDQDAAPPGAAAFYAPGPLPPGRGSEHYSGDSDASSPRSNCSDGMMDYSGPPSGPRRQNGYDTAYYSEAARESRPGKSAAVSSLDCLSSIVERISTDSPAAPALLLADAPPESPPGPPEGASLSDTEQGTQTPSPDAAPQCPAGSNPNAIYQVL";

// Gallus gallus (Chicken) 	MYOD1_CHICK
//>sp|P16075|MYOD1_CHICK Myoblast determination protein 1 homolog OS=Gallus gallus GN=MYOD1 PE=2 SV=2
char P16075[] = "MDLLGPMEMTEGSLCSFTAADDFYDDPCFNTSDMHFFEDLDPRLVHVGGLLKPEEHPHTRAPPREPTEEEHVRAPSGHHQAGRCLLWACKACKRKTTNADRRKAATMRERRRLSKVNEAFETLKRCTSTNPNQRLPKVEILRNAIRYIESLQALLREQEDAYYPVLEHYSGESDASSPRSNCSDGMMEYSGPPCSSRRRNSYDSSYYTESPNDPKHGKSSVVSSLDCLSSIVERISTDNSTCPILPPAEAVAEGSPCSPQEGGNLSDSGAQIPSPTNCTPLPQESSSSSSSNPIYQVL";

// Xenopus laevis (African clawed frog) MYODA_XENLA
// >sp|P13904|MYODA_XENLA Myoblast determination protein 1 homolog A OS=Xenopus laevis GN=myod1-a PE=2 SV=1
char P13904[] = "MELLPPPLRDMEVTEGSLCAFPTPDDFYDDPCFNTSDMSFFEDLDPRLVHVTLLKPEEPHHNEDEHVRAPSGHHQAGRCLLWACKACKRKTTNADRRKAATMRERRRLSKVNEAFETLKRYTSTNPNQRLPKVEILRNAIRYIESLQALLHDQDEAFYPVLEHYSGDSDASSPRSNCSDGMMDYNSPPCGSRRRNSYDSSFYSDSPNDSRLGKSSVISSLDCLSSIVERISTQSPSCPVPTAVDSGSEGSPCSPLQGETLSERVITIPSPSNTCTQLSQDPSSTIYHVL";

// Danio rerio (Zebrafish)MYOD1_DANRE
// >sp|Q90477|MYOD1_DANRE Myoblast determination protein 1 homolog OS=Danio rerio GN=myod1 PE=1 SV=2
char Q90477[] = "MELSDIPFPIPSADDFYDDPCFNTNDMHFFEDLDPRLVHVSLLKPDEHHHIEDEHVRAPSGHHQAGRCLLWACKACKRKTTNADRRKAATMRERRRLSKVNDAFETLKRCTSTNPNQRLPKVEILRNAISYIESLQALLRSQEDNYYPVLEHYSGDSDASSPRSNCSDGMMDFMGPTCQTRRRNSYDSSYFNDTPNADARNNKNSVVSSLDCLSSIVERISTETPACPVLSVPEGHEESPCSPHEGSVLSDTGTTAPSPTSCPQQQAQETIYQVL";

//Branchiostoma belcheri (Amphioxus) Q8IU24_BRABE
// >tr|Q8IU24|Q8IU24_BRABE MyoD-related OS=Branchiostoma belcheri GN=Amphi-MyoD1 PE=2 SV=1
char Q8IU24[] = "MEFVELSSCRFDATPTFCDRPAAPNATVLPGEHFPVPNGSYEDQGDGHVLAPGPSFHGPGRCLLWACKACKKKTVPIDRRKAATMRERRRLVKVNEAFDILKKKSCANPNQRLPKVEILRNAISYIEQLHKLLRDSKENSSGEVSDTSAPSPGSCSDGMAAHSPHSFCTDTSGNSSWEQGDGQPGNGYENQSCGNTVSSLDCLSLIVQSISTIEGEENNNASNTPR";

// Drosophila melanogaster (Fruit fly) MYOD_DROME
// >sp|P22816|MYOD_DROME Myogenic-determination protein OS=Drosophila melanogaster GN=nau PE=2 SV=3
char P22816[] = "MTKYNSGSSEMPAAQTIKQEYHNGYGQPTHPGYGFSAYSQQNPIAHPGQNPHQTLQNFFSRFNAVGDASAGNGGAASISANGSGSSCNYSHANHHPAELDKPLGMNMTPSPIYTTDYDDENSSLSSEEHVLAPLVCSSAQSSRPCLTWACKACKKKSVTVDRRKAATMRERRRLRKVNEAFEILKRRTSSNPNQRLPKVEILRNAIEYIESLEDLLQESSTTRDGDNLAPSLSGKSCQSDYLSSYAGAYLEDKLSFYNKHMEKYGQFTDFDGNANGSSLDCLNLIVQSINKSTTSPIQNKATPSASDTQSPPSSGATAPTSLHVNFKRKCST";

// Caenorhabditis elegans LIN32_CAEEL
// >sp|Q10574|LIN32_CAEEL Protein lin-32 OS=Caenorhabditis elegans GN=lin-32 PE=1 SV=2
char Q10574[] = "MSWEQYQMYVPQCHPSFMYQGSIQSTMTTPLQSPNFSLDSPNYPDSLSNGGGKDDKKKCRRYKTPSPQLLRMRRSAANERERRRMNTLNVAYDELREVLPEIDSGKKLSKFETLQMAQKYIECLSQILKQDSKNENLKSKSG";

//Homo sapiens (Human) SYFM_HUMAN
// >sp|O95363|SYFM_HUMAN Phenylalanine--tRNA ligase, mitochondrial OS=Homo sapiens GN=FARS2 PE=1 SV=1
char O95363[] = "MVGSALRRGAHAYVYLVSKASHISRGHQHQAWGSRPPAAECATQRAPGSVVELLGKSYPQDDHSNLTRKVLTRVGRNLHNQQHHPLWLIKERVKEHFYKQYVGRFGTPLFSVYDNLSPVVTTWQNFDSLLIPADHPSRKKGDNYYLNRTHMLRAHTSAHQWDLLHAGLDAFLVVGDVYRRDQIDSQHYPIFHQLEAVRLFSKHELFAGIKDGESLQLFEQSSRSAHKQETHTMEAVKLVEFDLKQTLTRLMAHLFGDELEIRWVDCYFPFTHPSFEMEINFHGEWLEVLGCGVMEQQLVNSAGAQDRIGWAFGLGLERLAMILYDIPDIRLFWCEDERFLKQFCVSNINQKVKFQPLSKYPAVINDISFWLPSENYAENDFYDLVRTIGGDLVEKVDLIDKFVHPKTHKTSHCYRITYRHMERTLSQREVRHIHQALQEAAVQLLGVEGRF";

// All the proteins in one single array for faster access and combination generation.
char* ProteinArray[10] = { P15172 , P17542 ,P10085,  P16075 , P13904 , Q90477, Q8IU24, P22816,  Q10574, O95363 };
const char* ProteinName[10] = { "P15172" , "P17542" ,"P10085",  "P16075" , "P13904" , "Q90477", "Q8IU24", "P22816",  "Q10574", "O95363" };

// Ignoring the B, Z, X and * columns from this matrix and working on the 20 standard Amino acids in the input
// After some experiments on Dictionaries and other data types, this representation is the fastest that I found the 
// representation to be reach the Blosum values that we will use in the calculation. 
// Here the 26 characters represent the 26 characters in the Alphabet. 'A'- 'A' will be 0 and and we can look at the 
// character matrix directly. BlosumMat['A']['C'] will be Blosum[0][2] and that makes it easy.
static int BlosumMat[26][26] =
{ { 4,0,0,-2,-1,-2,0,-2,-1,0,-1,-1,-1,-2,0,-1,-1,-1,1,0,0,0,-3,0,-2,0 }, // A
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, // B
{ 0,0,9,-3,-4,-2,-3,-3,-1,0,-3,-1,-1,-3,0,-3,-3,-3,-1,-1,0,-1,-2,0,-2,0 }, // C
{ -2,0,-3,6,2,-3,-1,-1,-3,0,-1,-4,-3,1,0,-1,0,-2,0,-1,0,-3,-4,0,-3,0 }, // D
{ -1,0,-4,2,5,-3,-2,0,-3,0,1,-3,-2,0,0,-1,2,0,0,-1,0,-2,-3,0,-2,0 }, // E
{ -2,0,-2,-3,-3,6,-3,-1,0,0,-3,0,0,-3,0,-4,-3,-3,-2,-2,0,-1,1,0,3,0 }, // F
{ 0,0,-3,-1,-2,-3,6,-2,-4,0,-2,-4,-3,0,0,-2,-2,-2,0,-2,0,-3,-2,0,-3,0 }, // G
{ -2,0,-3,-1,0,-1,-2,8,-3,0,-1,-3,-2,1,0,-2,0,0,-1,-2,0,-3,-2,0,2,0 }, // H
{ -1,0,-1,-3,-3,0,-4,-3,4,0,-3,2,1,-3,0,-3,-3,-3,-2,-1,0,3,-3,0,-1,0 }, // I
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, // J
{ -1,0,-3,-1,1,-3,-2,-1,-3,0,5,-2,-1,0,0,-1,1,2,0,-1,0,-2,-3,0,-2,0 }, // K
{ -1,0,-1,-4,-3,0,-4,-3,2,0,-2,4,2,-3,0,-3,-2,-2,-2,-1,0,1,-2,0,-1,0 }, // L
{ -1,0,-1,-3,-2,0,-3,-2,1,0,-1,2,5,-2,0,-2,0,-1,-1,-1,0,1,-1,0,-1,0 }, // M
{ -2,0,-3,1,0,-3,0,1,-3,0,0,-3,-2,6,0,-2,0,0,1,0,0,-3,-4,0,-2,0 }, // N
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, // O
{ -1,0,-3,-1,-1,-4,-2,-2,-3,0,-1,-3,-2,-2,0,7,-1,-2,-1,-1,0,-2,-4,0,-3,0 }, // P
{ -1,0,-3,0,2,-3,-2,0,-3,0,1,-2,0,0,0,-1,5,1,0,-1,0,-2,-2,0,-1,0 }, // Q
{ -1,0,-3,-2,0,-3,-2,0,-3,0,2,-2,-1,0,0,-2,1,5,-1,-1,0,-3,-3,0,-2,0 }, // R
{ 1,0,-1,0,0,-2,0,-1,-2,0,0,-2,-1,1,0,-1,0,-1,4,1,0,-2,-3,0,-2,0 }, // S
{ 0,0,-1,-1,-1,-2,-2,-2,-1,0,-1,-1,-1,0,0,-1,-1,-1,1,5,0,0,-2,0,-2,0 }, //T
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, // U
{ 0,0,-1,-3,-2,-1,-3,-3,3,0,-2,1,1,-3,0,-2,-2,-3,-2,0,0,4,-3,0,-1,0 }, // V
{ -3,0,-2,-4,-3,1,-2,-2,-3,0,-3,-2,-1,-4,0,-4,-2,-3,-3,-2,0,-3,11,0,2,0 }, //W
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, // X
{ -2,0,-2,-3,-2,3,-3,2,-1,0,-2,-1,-1,-2,0,-3,-1,-2,-2,-2,0,-1,2,0,7,0 }, //Y
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 } // Z
};

// Helper definitions
typedef enum
{
	SameCell,
	Top,
	Left,
	TopLeft
} Location;

typedef enum
{
	NonSequence, /// This is not part of the aligned sequence
	Match, // match 
	GapTop, // A gap is there on top
	GapBelow, // A gap in the btottom Seq
	Plus  // A non match but positive Blosum Score.
} CharRep;

// The struct will be the representation of the cells in each box.
// If the cell contains the value of the previous cell that it came from
// the traceback will easier. The space penalty is a little large here. But
// by choosing a signed short int [ max matrix score of +16K , which is 
// reasonable for the protein sequences that we are looking at ], we are reducing
// the size per node of the matrix to 4 bytes. 2 bytes for the shortint and the enum
// is going to use up one byte. The struct will be 4 byte aligned to make it use 4 bytes.
typedef struct ScoreMatrixNodeStruct
{
	short int matVal;
	Location prevCell;
	CharRep rep;
}ScoreMatrixNode;


// ScorePrintout will print to the console and to the temp file for score matrix.
// This is for the intermediate score table. For combination cases we need other solutions.
void ScorePrintout(int lengthA, int lengthB, string seqA, string seqB, ScoreMatrixNode ** scorematrix, ofstream& myfile)
{
	
	// print the scoring matrix to console
	myfile << "The Score matrix is as follows" << endl;
	// For smaller matrices, we will also printout on the screen.
	bool printOut = lengthA < 15 && lengthB < 15;
	if (printOut)
	{
		cout << "The Score matrix is as follows" << endl;
	}
	else
	{
		return;
	}
	for (int i = 0; i <= lengthA; i++)
	{
		if (i == 0)
		{
			myfile << "           ";
			if (printOut) cout << "           ";
			for (int j = 0; j < lengthB; ++j)
			{
				myfile << std::setfill(' ') << std::setw(4) << seqB[j] << " ";
				if (printOut) cout << std::setfill(' ') << std::setw(4) << seqB[j] << " ";
			}
			myfile << endl;
			myfile << "      ";
			if (printOut)
			{
				cout << endl;
				cout << "      ";
			}
			for (int j = 0; j < lengthB + 1; ++j)
			{
				myfile << std::setfill(' ') << std::setw(4) << 0 << " ";
				if (printOut) cout << std::setfill(' ') << std::setw(4) << 0 << " ";
			}
		}
		else
		{
			myfile << std::setw(4) << seqA[i - 1] << "  ";
			if (printOut)
			{
				cout << std::setw(4) << seqA[i - 1] << "  ";
			}
			for (int j = 0; j <= lengthB; j++)
			{
				myfile << std::setfill(' ') << std::setw(4) << scorematrix[i][j].matVal << " ";
				if (printOut)
				{
					cout << std::setfill(' ') << std::setw(4) << scorematrix[i][j].matVal << " ";
				}
			}
		}
		myfile << endl;
		if (printOut)
		{
			cout << endl;
		}
	}
	myfile.close();
}

void DiffLinePrinting(int max_i, int max_j, int tick, int gapA, int gapB, string protNameA, string protNameB, 
	string diffStringA, string diffStringB, string diffLine, ofstream& myfile)
{
	// Diff line printing
	// Example:
	// P68871:  62  KAHGKKVLGAFSDGLAHLDN-- - LKGTFATLSELHCDKLHVDPENFRLLGNVLVCVLAHH
	//				+ AH   V + A++ + L + ++ + L E H  K     E F + L  V++ VL
	// Q14SN0 : 83  QAHCAGVITALNNVIDFLHDPGLMEASLIGLVERH - KKRGQTKEEFQNLKEVMLEVLRQA
	// 
	// P68871 : 119  FGKEFTPPVQAAYQKVV
	//				 GK++TP V  A + K +
	// Q14SN0 : 142  LGKQYTPEVAEAWNKTL

	cout << "Diff style printing in blocks of 60: " << endl;
	myfile << "Diff style printing in blocks of 60: " << endl;
	int total = 0;
	int startA = max_i - tick + 1 + gapA;
	int startB = max_j - tick + 1 + gapB;
	for (int i = 0; i < (tick / 60 + 1); i++)
	{
		int tmpTotal = total;
		cout << std::setw(10) << protNameA << std::setw(5) << startA << " ";
		myfile << std::setw(10) << protNameA << std::setw(5) << startA << " ";
		for (int j = 0; j < 60 && total < tick; j++, total++)
		{
			cout << diffStringA[i * 60 + j];
			myfile << diffStringA[i * 60 + j];
			if (diffStringA[i * 60 + j] != '-') startA++;
		}
		cout << " " << startA - 1 << endl;
		cout << std::setfill(' ') << std::setw(16);
		myfile << " " << startA - 1 << endl;
		myfile << std::setfill(' ') << std::setw(16);

		total = tmpTotal;
		cout << " ";
		myfile << " ";
		for (int j = 0; j < 60 && total < tick; j++, total++)
		{
			cout << diffLine[i * 60 + j];
			myfile << diffLine[i * 60 + j];
		}
		cout << endl;
		cout << std::setw(10) << protNameB << std::setw(5) << startB << " ";

		myfile << endl;
		myfile << std::setw(10) << protNameB << std::setw(5) << startB << " ";
		total = tmpTotal;
		for (int j = 0; j < 60 && total < tick; j++, total++)
		{
			cout << diffStringB[i * 60 + j];
			myfile << diffStringB[i * 60 + j];
			if (diffStringB[i * 60 + j] != '-') startB++;
		}
		cout << " " << startB - 1 << endl;
		cout << endl;

		myfile << " " << startB - 1 << endl;
		myfile << endl;
	}
}

// Utility routine to get the cpu time. 
// Needed for testing the performance of various runs.
double get_cpu_time() {
	FILETIME a, b, c, d;
	if (GetProcessTimes(GetCurrentProcess(), &a, &b, &c, &d) != 0) {
		//  Returns total user time.
		//  Can be tweaked to include kernel times as well.
		return
			(double)(d.dwLowDateTime |
			((unsigned long long)d.dwHighDateTime << 32)) * 0.0000001;
	}
	else {
		//  Handle error
		return 0;
	}
}



// Using the defatul penalty for -4 .
// There will be option to choose different penalty schemes as per the choice of the user 
static int GapPenalty = -4;


// Checks whether the new value qualifies.
void SetScoreMatrixCell(ScoreMatrixNode &node, short int currVal, Location lVal, string seqA, string seqB, int i, int j)
{
	node.prevCell = currVal > node.matVal ? lVal : node.prevCell;
	node.matVal = max(currVal, node.matVal);
	int row = (seqA[i - 1] > 'Z') ? (seqA[i - 1] - 'a') : (seqA[i - 1] - 'A');
	int col = (seqB[j - 1] > 'Z') ? (seqB[j - 1] - 'a') : (seqB[j - 1] - 'A');

	if (seqA.at(i - 1) == seqB.at(j - 1))
	{
		node.rep = Match;
	}
	else if (seqA.at(i - 1) == seqB.at(j - 1) && BlosumMat[row][col] > 0)
	{
		node.rep = Plus;
	}
}

char GetCharAtDiffLine(ScoreMatrixNode &node)
{
	switch (node.rep)
	{
	case Plus:
		return '+';
	default:
		return ' ';
	}
}

// For the single path, this is the move decision call function.
void Move(ScoreMatrixNode** scorematrix, int currRow, int currCol, bool& up, bool &left, string seqA, string seqB)
{
	up = false;
	left = false;

	int row = (seqA[currRow - 1] > 'Z') ? (seqA[currRow - 1] - 'a') : (seqA[currRow - 1] - 'A');
	int col = (seqB[currCol - 1] > 'Z') ? (seqB[currCol - 1] - 'a') : (seqB[currCol - 1] - 'A');

	short int valueup = scorematrix[currRow - 1][currCol].matVal + GapPenalty;
	short int valuediag = scorematrix[currRow - 1][currCol - 1].matVal + BlosumMat[seqA[currRow-1]][seqB[currCol-1]];
	short int valueleft = scorematrix[currRow][currCol - 1].matVal + GapPenalty;

	if (valueup >= valuediag && valueup >= valueleft && valueup != 0) {
		up = true;
	}
	if (valuediag >= valueup && valuediag >= valueleft && valuediag != 0)
	{
		up = true, left = true;
	}
	else if (valueleft >= valueup && valueleft >= valuediag && valueleft != 0)
	{
		left = true;
	}
}

// When the end condition is reached, then we print.
//
void FinalPrint(int lengthA, int lengthB, string alignedStringA, string alignedStringB, int tick, int gapA, int gapB,
	string protNameA, string protNameB, int maxScore, int max_i, int max_j, int seqCount, ofstream& myfile)
{
	char *diffLine = new char[lengthA + lengthB + 2];
	char *diffStringA = new char[lengthA + lengthB + 2];
	char *diffStringB = new char[lengthA + lengthB + 2];
	// Create char arrays for the matching portions.
	int index = 0;
	for (int i = tick - 1; i >= 0; i--)
	{
		diffStringA[index] = alignedStringA[i];
		diffStringB[index] = alignedStringB[i];

		int row = (diffStringA[index] > 'Z') ? (diffStringA[index] - 'a') : (diffStringA[index] - 'A');
		int col = (diffStringB[index] > 'Z') ? (diffStringB[index] - 'a') : (diffStringB[index] - 'A');
		int blosumScore = BlosumMat[row][col];

		diffLine[index] = (diffStringA[index] != diffStringB[index]) ? ((diffStringA[index] != '-' && diffStringB[index] != '-') ? ((blosumScore <0) ? ' ' : '+') : ' ') : diffStringA[index];
		index++;
	}

	diffStringA[index] = '\0';
	diffStringB[index] = '\0';
	diffLine[index] = '\0';

	// Now lets print the aligned sequences
	cout << endl;

	cout << "Alignment:" << endl << endl;
	for (int i = tick - 1; i >= 0; i--) cout << alignedStringA[i];
	cout << endl;

	for (int j = tick - 1; j >= 0; j--) cout << alignedStringB[j];
	cout << endl << endl;

	cout << "Score: " << maxScore << endl;
	cout << "Backtrack Path " << seqCount << endl;

	///------ For the file print
	// Now lets print the aligned sequences
	myfile << endl;

	myfile << "Alignment:" << endl << endl;
	for (int i = tick - 1; i >= 0; i--) myfile << alignedStringA[i];
	myfile << endl;

	for (int j = tick - 1; j >= 0; j--) myfile << alignedStringB[j];
	myfile << endl << endl;

	myfile << "Score: " << maxScore << endl;
	myfile << "Backtrack Path " << seqCount << endl;
	DiffLinePrinting(max_i, max_j, tick, gapA, gapB, protNameA, protNameB, diffStringA, diffStringB, diffLine, myfile);
}

// Internal routine for recursively going down all possible paths during backtracking when there are equal values.
void RecursiveRoutineInternal(string alignedStringA, string alignedStringB, int currentRow, int nextRow, int currentCol, int nextCol,
	ScoreMatrixNode ** scorematrix, int tick, int gapA, int gapB, string seqA, string seqB, int max_i, int max_j, string protNameA, string protNameB, int maxScore, int seqCount, ofstream& myfile)
{
	int lengthA = seqA.length(); int lengthB = seqB.length();

	if (((currentRow != nextRow) || (currentCol != nextCol)) && (nextCol != 0) && (nextRow != 0))
	{
		if (nextRow == currentRow)
		{
			alignedStringB[tick] = '-';
			scorematrix[nextRow][nextCol].rep = GapTop;
			gapB++;
		}
		else
		{
			alignedStringA[tick] = seqA[nextRow - 1]; // Match or Score in A
		}

		if (nextCol == currentCol)
		{
			alignedStringA[tick] = '-';
			scorematrix[nextRow][nextCol].rep = GapBelow;
			gapA++;
		}
		else
		{
			alignedStringB[tick] = seqB[nextCol - 1]; // Match or Score in B
		}

		currentRow = nextRow;
		currentCol = nextCol;

		int iterCount = 0;
		int row = (seqA[currentRow - 1] > 'Z') ? (seqA[currentRow - 1] - 'a') : (seqA[currentRow - 1] - 'A');
		int col = (seqB[currentCol - 1] > 'Z') ? (seqB[currentCol - 1] - 'a') : (seqB[currentCol - 1] - 'A');

		short int valueup = scorematrix[currentRow - 1][currentCol].matVal + GapPenalty;
		short int valuediag = scorematrix[currentRow - 1][currentCol - 1].matVal + BlosumMat[seqA[currentRow - 1]][seqB[currentCol - 1]];
		short int valueleft = scorematrix[currentRow][currentCol - 1].matVal + GapPenalty;
		short int maxVal = max(max(valueup, valuediag), valueleft);
		bool move[3]; for (int i = 0; i < 3; i++) move[i] = false;
		if (valueup == maxVal && maxVal != 0) { iterCount++; move[0] = true; }
		if (valueleft == maxVal && maxVal != 0) { iterCount++; move[1] = true; }
		if (valuediag == maxVal && maxVal != 0) { iterCount++; move[2] = true; }
		tick++;
		if (iterCount == 0) FinalPrint(lengthA, lengthB, alignedStringA, alignedStringB, tick, gapA, gapB,
			protNameA, protNameB, maxScore, max_i, max_j, ++seqCount, myfile);

		for (int i = 0; i < 3; i++)
		{
			if (move[i])
			{
				nextRow = currentRow - ((((i == 0) && move[0]) || ((i == 2) && move[2])) ? 1 : 0);
				nextCol = currentCol - ((((i == 1) && move[1]) || ((i == 2) && move[2])) ? 1 : 0);
				RecursiveRoutineInternal(alignedStringA, alignedStringB, currentRow, nextRow, currentCol, nextCol,
					scorematrix, tick, gapA, gapB, seqA, seqB, max_i, max_j, protNameA, protNameB, maxScore, ++seqCount, myfile);
			}
		}
	}
	else
	{
		FinalPrint(lengthA, lengthB, alignedStringA, alignedStringB, tick, gapA, gapB,
			protNameA, protNameB, maxScore, max_i, max_j, ++seqCount, myfile);
	}

}

// ---- RecursiveTraceback-----
// The reason we need a recurisve routine is because we will need to find out all the paths possible in some cases and it might be interesting to 
// see the number of possible paths. This could also point towards the fact that higher P numbers could come from permutations landing up in the 
// other paths. We will need to find pathological conditions that would achieve that.
void RecursivetraceBack(int max_i, int max_j, ScoreMatrixNode** scorematrix, int lengthA, int lengthB, 
	string seqA, string seqB, string protNameA, string protNameB, int maxScore, ofstream& myfile)
{
	cout << "Proteins:" << endl;
	for (int i = 0; i < lengthA; i++) { cout << seqA[i]; }; cout << endl << endl;
	for (int i = 0; i < lengthB; i++) { cout << seqB[i]; }; cout << endl << endl << endl;

	myfile << "Proteins:" << endl;
	for (int i = 0; i < lengthA; i++) { myfile << seqA[i]; }; myfile << endl << endl;
	for (int i = 0; i < lengthB; i++) { myfile << seqB[i]; }; myfile << endl << endl << endl;


	int currentRow = max_i, currentCol = max_j;
	bool up = false, left = false;
	int tick = 0;

	int iterCount = 0;
	int row = (seqA[currentRow - 1] > 'Z') ? (seqA[currentRow - 1] - 'a') : (seqA[currentRow - 1] - 'A');
	int col = (seqB[currentCol - 1] > 'Z') ? (seqB[currentCol - 1] - 'a') : (seqB[currentCol - 1] - 'A');
	short int valueup = scorematrix[currentRow - 1][currentCol].matVal + GapPenalty;
	short int valuediag = scorematrix[currentRow - 1][currentCol - 1].matVal + BlosumMat[seqA[currentRow - 1]][seqB[currentCol - 1]];
	short int valueleft = scorematrix[currentRow][currentCol - 1].matVal + GapPenalty;
	short int maxVal = max(max(valueup, valuediag), valueleft);
	bool move[3]; for (int i = 0; i < 3; i++) move[i] = false;
	if (valueup == maxVal && maxVal != 0) { iterCount++; move[0] = true; }
	if (valueleft == maxVal && maxVal != 0) { iterCount++; move[1] = true; }
	if (valuediag == maxVal && maxVal != 0) { iterCount++; move[2] = true; }
	if (iterCount == 0) return;
	int seqCount = 0;
	for (int i = 0; i < 3; i++)
	{
		if (move[i]) {
			// Since the aligned strings could have '-'s all the way, for two absolutely different sequences, the best match will be
			// all gaps in one of the sequences [ arbitrary choice ] and in that case the length of the string that is possible will be
			// lengthA + lengthB.
			char *alignedStringA = new char[lengthA + lengthB + 2];
			char *alignedStringB = new char[lengthA + lengthB + 2];
			alignedStringB[tick] = seqB[currentCol - 1];
			alignedStringA[tick] = seqA[currentRow - 1];
			scorematrix[currentRow][currentCol].rep = Match;

			int nextRow = currentRow - ((((i == 0) && move[0]) || ((i == 2) && move[2])) ? 1 : 0);
			int nextCol = currentCol - ((((i == 1) && move[1]) || ((i == 2) && move[2])) ? 1 : 0);
			int tick = 0;
			tick++;
			int gapA = 0, gapB = 0;
			RecursiveRoutineInternal(alignedStringA, alignedStringB, currentRow, nextRow, currentCol, nextCol,
				scorematrix, tick, gapA, gapB, seqA, seqB, max_i, max_j, protNameA, protNameB, maxScore, ++seqCount, myfile);
		}
	}

}

// This is the solution to find the first best alignment. 
// This not recursive as there is only one single back track path that is needed. For simple proteins, it might be interesting to find all paths
// and also do permutation logics to take into account those other paths.
void SingleAlignmentSolution(int max_i, int max_j, ScoreMatrixNode** scorematrix, int lengthA, int lengthB, 
	string seqA, string seqB, string protNameA, string protNameB, int maxScore, ofstream& myfile, bool suppressPrint = false)
{
	// Lets do the traceback now.
	int currentRow = max_i, currentCol = max_j;
	bool up = false, left = false;
	Move(scorematrix, currentRow, currentCol, up, left, seqA ,seqB);
	int nextRow = currentRow - (up ? 1 : 0);
	int nextCol = currentCol - (left ? 1 : 0);
	int tick = 0;

	// Since the aligned strings could have '-'s all the way, for two absolutely different sequences, the best match will be
	// all gaps in one of the sequences [ arbitrary choice ] and in that case the length of the string that is possible will be
	// lengthA + lengthB
	char *alignedStringA = new char[lengthA + lengthB + 2];
	char *alignedStringB = new char[lengthA + lengthB + 2];
	alignedStringB[tick] = seqB[currentCol - 1];
	alignedStringA[tick] = seqA[currentRow - 1];
	scorematrix[currentRow][currentCol].rep = Match;
	tick++;
	int gapA = 0, gapB = 0;

	while (((currentRow != nextRow) || (currentCol != nextCol)) && (nextCol != 0) && (nextRow != 0))
	{
		if (nextRow == currentRow)
		{
			alignedStringA[tick] = '-';
			scorematrix[nextRow][nextCol].rep = GapTop;
			gapA++;
		}
		else
		{
			alignedStringA[tick] = seqA[nextRow - 1]; // Match or Score in A
		}

		if (nextCol == currentCol)
		{
			alignedStringB[tick] = '-';
			scorematrix[nextRow][nextCol].rep = GapBelow;
			gapB++;
		}
		else
		{
			alignedStringB[tick] = seqB[nextCol - 1]; // Match or Score in B
		}

		currentRow = nextRow;
		currentCol = nextCol;
		up = false; left = false;
		Move(scorematrix, currentRow, currentCol, up, left, seqA, seqB);
		nextRow = currentRow - (up ? 1 : 0);
		nextCol = currentCol - (left ? 1 : 0);
		tick++;
	}

	if (!suppressPrint)
	{
		char *diffLine = new char[lengthA + lengthB + 2];
		char *diffStringA = new char[lengthA + lengthB + 2];
		char *diffStringB = new char[lengthA + lengthB + 2];
		// Create char arrays for the matching portions.
		int index = 0;
		for (int i = tick - 1; i >= 0; i--)
		{
			diffStringA[index] = alignedStringA[i];
			diffStringB[index] = alignedStringB[i];

			diffLine[index] = (diffStringA[index] != diffStringB[index]) ? ((diffStringA[index] != '-' && diffStringB[index] != '-') ? '+' : ' ') : diffStringA[index];
			index++;
		}

		diffStringA[index] = '\0';
		diffStringB[index] = '\0';
		diffLine[index] = '\0';

		// Now lets print the aligned sequences
		cout << endl << " " << endl;
		cout << "Proteins:" << endl;
		myfile << endl << " " << endl;
		myfile << "Proteins:" << endl;
		for (int i = 0; i < lengthA; i++) { cout << seqA[i]; }; cout << endl << endl;
		for (int i = 0; i < lengthB; i++) { cout << seqB[i]; }; cout << endl << endl << endl;

		for (int i = 0; i < lengthA; i++) { myfile << seqA[i]; }; myfile << endl << endl;
		for (int i = 0; i < lengthB; i++) { myfile << seqB[i]; }; myfile << endl << endl << endl;

		FinalPrint(lengthA, lengthB, alignedStringA, alignedStringB, tick, gapA, gapB,
			protNameA, protNameB, maxScore, max_i, max_j, 1 /* for the sequence number */, myfile);
	}
}

// This is the implementation of the SmithWaterman algorithm
int SmithWatermanAlgorithmDP(string protNameA, string protNameB, string seqA, string seqB, bool allPath, ofstream& myfile, bool suppressPrint = false)
{
	// initialize some variables
	int lengthA = seqA.length();
	int lengthB = seqB.length();

	// initialize Score matrix. 
	ScoreMatrixNode** scorematrix = new ScoreMatrixNode*[lengthA + 1];
	for (int i = 0; i < lengthA + 1; ++i)
	{
		scorematrix[i] = new ScoreMatrixNode[lengthB + 1];
		for (int j = 0; j < lengthB + 1; j++)
		{
			scorematrix[i][j].matVal = 0;
			scorematrix[i][j].prevCell = SameCell;// default value is the same cell or start of a '0' length aligned string.
			scorematrix[i][j].rep = NonSequence; // Start with the value that none of them are in sequence.
		}
	}

	int max_i = 0;
	int max_j = 0;
	int maxVal = INT_MIN;

	//Populate the score matrix.
	for (int i = 1; i <= lengthA; i++)
	{
		for (int j = 1; j <= lengthB; j++)
		{
			int row = (seqA[i - 1] > 'Z') ? (seqA[i - 1] - 'a') : (seqA[i - 1] - 'A');
			int col = (seqB[j - 1] > 'Z') ? (seqB[j - 1] - 'a') : (seqB[j - 1] - 'A');
			short int currVal = scorematrix[i - 1][j - 1].matVal + BlosumMat[row][col];

			SetScoreMatrixCell(scorematrix[i][j], currVal, TopLeft, seqA, seqB, i, j);

			currVal = scorematrix[i - 1][j].matVal + GapPenalty;
			SetScoreMatrixCell(scorematrix[i][j], currVal, Top, seqA, seqB, i, j);

			currVal = scorematrix[i][j - 1].matVal + GapPenalty;
			SetScoreMatrixCell(scorematrix[i][j], currVal, Left, seqA, seqB, i, j);

			if (scorematrix[i][j].matVal > maxVal)
			{
				maxVal = scorematrix[i][j].matVal;
				max_i = i;
				max_j = j;
			}
		}
	}

	if (!suppressPrint)
	ScorePrintout(lengthA, lengthB, seqA, seqB, scorematrix, myfile);

	if (allPath)
	{
		// Recursive traceback routine that will print all paths that lead to the highest score.
		RecursivetraceBack(max_i, max_j, scorematrix, lengthA, lengthB, seqA, seqB, protNameA, protNameB, maxVal, myfile);
	}
	else
	{
		SingleAlignmentSolution(max_i, max_j, scorematrix, lengthA, lengthB, seqA, seqB, protNameA, protNameB, maxVal, myfile, suppressPrint);
	}
	return maxVal;
}

// Pretty print the lower triangualr matrix of the protein max scores.
void PrettyPrintTriangular(int **TriangResult)
{
	cout << "         ";
	for (int i = 0; i < 10; i++)
	{
		cout << std::setfill(' ') << std::setw(7) << ProteinName[i];
	}
	cout << endl;
	
	for (int i = 0; i < 10; i++)
	{
		cout << std::setw(9) << ProteinName[i] ;
		for (int j = 0; j <10; j++)
		{
			
			cout << std::setw(7) <<( (i == j) ? 0 : TriangResult[i][j]);
		}
		cout << endl;
	}
}

char aminoacid[] = { 'A', 'R', 'N', 'D', 'C', 'Q', 'E', 'G', 'H', 'I', 'L', 'K', 'M', 'F', 'P', 'S', 'T', 'W', 'Y', 'V' };

// Read a file containing the protein sequence in a fasta format. 
// Sample: 
// >tr|Q14SN0|Q14SN0_APIME Globin 1 OS=Apis mellifera GN=glob1 PE=2 SV=1
// MGTFLRFLGISSSDDNRIDQATGLTERQKKLVQNTWAVVRKDEVASGIAVMTAFFKKYPE
// YQRYFTAFMDTPLNELPANKRFQAHCAGVITALNNVIDFLHDPGLMEASLIGLVERHKKR
// GQTKEEFQNLKEVMLEVLRQALGKQYTPEVAEAWNKTLDMMFGKIYQVFAS
string ReadFastaFile(string file, string &protName)
{
	std::ifstream input(file);
	if (!input.good()) {
		std::cerr << "Error opening '" << file << "'. Stopping the execution." << std::endl;
		return nullptr;
	}

	std::string line, name, content;
	while (std::getline(input, line).good()) {
		if (line.empty() || line[0] == '>') { // Identifier marker
			if (!name.empty()) { // Print out what we read from the last entry
				std::cout << name << " : " << content << std::endl;
				name.clear();
			}
			if (!line.empty()) {
				name = line.substr(1);
			}
			content.clear();
		}
		else if (!name.empty()) {
			if (line.find(' ') != std::string::npos) { // Invalid sequence--no spaces allowed
				name.clear();
				content.clear();
			}
			else {
				content += line;
			}
		}
	}
	if (!name.empty()) { // Print out what we read from the last entry
		protName = name;
		return content;
	}
	return string();
}

void Shuffle(int n, char* arr)
{
	int* ind = new int[n];
	memset(ind, 0, 4 * n);
	int index;
	for (int i = 0; i < n; i++)
	{
		do
		{
			index = rand() % n;
		} while (ind[index] != 0);

		ind[index] = 1;
		arr[i] = *(arr + index);
	}
}

void KFYPermute(int n, char* arr)
{
	// To shuffle an array a of n elements(indices 0..n - 1) :
	for (int i = n-1; i > 0; --i)
	{
		int j = rand() % (i+1);
		int tmp = arr[i];
		arr[i] = arr[j]; arr[j] = tmp;
	}
}

// Need to remove any occurence of BZX and * from the protein sequence
void CleanupString(string str)
{
	str.erase(std::remove(str.begin(), str.end(), 'B'), str.end());
	str.erase(std::remove(str.begin(), str.end(), 'X'), str.end());
	str.erase(std::remove(str.begin(), str.end(), 'Z'), str.end());
	str.erase(std::remove(str.begin(), str.end(), '*'), str.end());
}

int main(int argc, char **argv)
{
	// Test Samples Code 
	// Uncomment the SmitWatermanAlgorithmDP code lines to see the results.

	string seqA = "GCCCTAGCG"; // sequence A
	string seqB = "CGTATT"; // sequence B

	string P68871 = "MVHLTPEEKSAVTALWGKVNVDEVGGEALGRLLVVYPWTQRFFESFGDLSTPDAVMGNPKVKAHGKKVLGAFSDGLAHLDNLKGTFATLSELHCDKLHVDPENFRLLGNVLVCVLAHHFGKEFTPPVQAAYQKVVAGVANALAHKYH";
	string Q14SN0 = "MGTFLRFLGISSSDDNRIDQATGLTERQKKLVQNTWAVVRKDEVASGIAVMTAFFKKYPEYQRYFTAFMDTPLNELPANKRFQAHCAGVITALNNVIDFLHDPGLMEASLIGLVERHKKRGQTKEEFQNLKEVMLEVLRQALGKQYTPEVAEAWNKTLDMMFGKIYQVFAS";


	// Commented test section
	// Self Example
	//SmithWatermanAlgorithmDP("Prot1", "Prot2", seqA, seqB);
	// Lecture Note Example
	//SmithWatermanAlgorithmDP("KEVLAR", "KNIEVIL", "KEVLAR", "KNIEVIL");

	// ofstream myfiletemp;
	// myfiletemp.open(".\\smithwatermanhelper1.txt");
	// SmithWatermanAlgorithmDP( "Prot1","Prot2", "ddgearlyk", "deadly", false, myfiletemp);
	// myfiletemp.close();
	
	// Alternate sample:
	// MELLSLCSWFAAATTYDADFYDDP vs MSNWTATSSDSTS
	// Result from the prorgram
	//  LC - SWFAAATTYDA
	//  
	//  M  - SNWTA--TSSDS
	//  
	//  	Diff style printing in blocks of 60:
	//      M1    6 LC - SWFAAATTYDA 18
	//  	        + +W + A  T++D +
	//  	M2    1 M - SNWTA--TSSDS 11
	//  
	//ofstream myfiletemp;
	//myfiletemp.open(".\\smithwatermanhelper1.txt");
	////SmithWatermanAlgorithmDP("M1", "M2", "MELLSLCSWFAAATTYDADFYDDP", "MSNWTATSSDSTS",true, myfiletemp);


	//// Lecture note sample for diff testing.
	//SmithWatermanAlgorithmDP("P68871", "Q14SN0", Q14SN0, P68871, true, myfiletemp);


	// This is the portion of the code that will take in 2 files [ Fasta format files ] and then responds back with the 
	// results.
	//if (argc <= 2) {
	//	std::cerr << "Usage: " << argv[0] << " [Fasta File1] [Fasta File2] " << std::endl;
	//	std::cerr << " Other options include -all for all best sequences that match the score." << endl;
	//	return -1;
	//}
	//
	//string protDetailsA; string protDetailsB;
	//string protA = ReadFastaFile(argv[1], protDetailsA);
	//string protB = ReadFastaFile(argv[2], protDetailsB);
	//
	//CleanupString(protA);
	//CleanupString(protB);
	//
	//bool allPath = false;
	//if (argc > 3 && !strcmp(argv[3], "-all"))
	//{
	//	allPath = true;
	//}
	//cout << "Result will be printed in console as well as result.txt" << endl;
	//
	//ofstream myfile;
	//myfile.open(".\\smithwaterman.txt");
	//
	//SmithWatermanAlgorithmDP("Prot1", "Prot2", protA, protB, allPath, myfile);
	//
	// Do p-value calculation for the combination.

	//"ddgearlyk", "deadly"
	ofstream myfile;
	myfile.open(".\\smithwaterman.txt");
	string tmpPrm = "deadly";
	int n = tmpPrm.length();
	int maxVal = SmithWatermanAlgorithmDP("Prot1", "Prot2", "ddgearlyk", "deadly", false, myfile);;
	// declaring character array
	char *chararray = new char[n+1];
	strcpy_s(chararray, n + 1, tmpPrm.c_str());
	int count = 0;
	int matchCount = 0;
	for (int i = 0; i < 999; i++)
	{
		char *tmpArray = new char[n+1];
		// copying the contents of the 
		// string to char array
		strcpy_s(tmpArray,n+1,  chararray);
		KFYPermute(n, chararray);
		string permStr = string(chararray);
		if (strcmp(permStr.c_str(), tmpPrm.c_str()) == 0)
		{
			cout << "Found a match " << endl;
			matchCount++;
		}
		cout << permStr << " ";
		int max = SmithWatermanAlgorithmDP("Prot1", "Prot2", "ddgearlyk", permStr, false, myfile, true);
		if (max >= maxVal)
		{
			cout << "Score: " << max << endl;
			count++;
		}
	}

	// All the diagnostics printing for all the matches.
	cout << "Number of permutations that matched with the original string " << matchCount << endl;
	// The number of permutations possible for deadly is 6*6*6*6*6*6  and that  is 46456. So numer of 
	// permutations more than that would not make sense. 100K perms would repeat the actual code it self 
	// multiple times.
	cout << "Number of matches found " << count << endl;
	double pval1000 = (count + 1) / 1000;
	cout << "The p1000 val is " << pval1000;
	myfile.close();


	// Getting the Traingular matrix score for the 10 proteins
	// This will only print the lower side of the matrix.
	// int **TriangResult = new int*[10];
	// for (int i = 0; i < 10; i++)
	// {
	// 	TriangResult[i] = new int[10];
	// 	memset(TriangResult[i], 0, 40);
	// }
	// ofstream myfile;
	// myfile.open(".\\smithwaterman.txt");
	// for (int i = 0; i < 10; i++)
	// {
	// 	for (int j = i; j < 10; j++)
	// 	{
	// 		TriangResult [i][j] = SmithWatermanAlgorithmDP(ProteinName[i], ProteinName[j], ProteinArray[i], ProteinArray[j], false, myfile);
	// 	}						  
	// }	
	// myfile.close();
	// PrettyPrintTriangular(TriangResult);

	return 0;
}

