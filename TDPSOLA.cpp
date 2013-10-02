// TDPSOLA.cpp: implementation of the CTDPSOLA class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "nettalk2.h"
#include "TDPSOLA.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTDPSOLA::CTDPSOLA()
{
	m_fLog.Open("TDPSOLA.log",CFile::modeWrite|CFile::modeCreate);
}

CTDPSOLA::~CTDPSOLA()
{
	m_fLog.Close();
}
void CTDPSOLA::LowpassFilter(vector<short> & vSignal)
{

	long i;
	double a = 0.5;
	for(i=1;i<vSignal.size();i++)
	{
		vSignal[i] = vSignal[i] - (a*vSignal[i-1]);
	}
}

vector<short> CTDPSOLA::Block(vector<short> vData, long start, long end)
{
	vector<short> vResult;
	long len = vData.size();
	for (long i=start;i<=end;i++)
	{
		if(i<len)
		{
			short val = vData[i];
			vResult.push_back(val);
		}else
			break;
	}
	return vResult;
}

short CTDPSOLA::Round(float d)
{
	return ((d-(int)d>=0.5) ? ceil(d) : floor(d));
}

short CTDPSOLA::Max(vector<short> vData, long & pos)
{
	vector<short>::iterator iter;
	short max = -32766 ;
	long minpos=0;
	float val;
	for (iter=vData.begin();iter!=vData.end();iter++)
	{
		val = (short)*iter;
		if(val>max)
		{
			max	= val;
			pos = minpos;
		}
		minpos++;
	}
	return max;
}
void CTDPSOLA::PitchDetector()
{
	if(m_vSignal.size()==0)
		return;
	
	long blocksize = 300;
	long len = m_vSignal.size();	
	long i;
	long lastpos = 0;	
	long maxpos=0;
	i=0;
	
	while ((lastpos+floor(blocksize*1.7))<len)
	{	
		//ambil short-time analysis signal  mulai lokasi lastpos+50 sampai lastpos + blocksize*1.7
		vector<short> vBlock = Block(m_vSignal,lastpos+50,lastpos+floor(blocksize*1.7));
		m_vPitchMarks.push_back(0);
		//cari amplitudo tertinggi pada block		
		short max = Max(vBlock,maxpos);
		//cek apakah nilai amplitudo memenuhi untuk menjadi pitch    	
		BOOL bAda;		
		if(max<100)
		{
			maxpos = vBlock.size()-1;
			bAda = FALSE;
			m_vPitchMarks[i] = maxpos+lastpos+50;			
		}else
			bAda = TRUE;
		// ambil short-time analysis signal kedua  
		while (bAda)
		{
			//cari amplitudo terbesar dalam block dari index pertama sampai akhir
			vector<short> vBlock2 = Block(vBlock,0,maxpos-50);
			long maxpos2;
			short max2 = Max(vBlock2,maxpos2);
			//cek apakah ada nilai amplitudo yang memenuhi nilai threshold
			if(max2>(0.9*max))
			{
				max = max2;
				maxpos = maxpos2;
			}else
			{
				bAda = FALSE;
				//catat lokasi pitch
				m_vPitchMarks[i] = maxpos+lastpos+50;
			}
		}
		//pindah ke block selanjutnya
		blocksize = maxpos+50;
		//cek apakah ukuran blok mencukupi
		if(blocksize<150)
		{
			blocksize = 150;
		}
		lastpos = maxpos+lastpos+50;
		i++;
	}
}

void CTDPSOLA::Normalize()
{	
	long lSum = 0,n=m_vSignal.size();
	for(long i=0;i<n;i++)
	{
		lSum += m_vSignal[i];
	}
	long x,lAvg = lSum/n;	
	for( i = 0; i < n; i++ )
	{ 
		x = (long)(m_vSignal[i] - lAvg); 
		if(abs(x) <= 32766L)
			m_vSignal[i] = x;
		else
		{ 
		   if(x>32766L)
			   m_vSignal[i] = 32766;
		   else              
			   m_vSignal[i] = -32766;
		}		
	}
}

void CTDPSOLA::PSOLA(double dAlpha /*time stretching factor*/,double dBeta/*pitch shifting factor*/)
{	
	if(m_vSignal.size()==0)
		return;
	LowpassFilter(m_vSignal);	
	Normalize();		
	PitchDetector();	
	
	//cari pitch periode
	vector<long> vPitchPeriode;
	long i,len=m_vPitchMarks.size();
	for(i=1;i<len;i++)
	{
		long val1 = m_vPitchMarks[i-1];
		long val2 = m_vPitchMarks[i];		
		vPitchPeriode.push_back((long)(val2-val1));	
		
	}
	//jika mark kurang dari pitch periode hapus pitch mark pertama
	if ((m_vPitchMarks.size()==0)||(vPitchPeriode.size()==0))
	{
		return;
	}
	vector<long>::iterator iter1;
	vector<long>::iterator iter2;

	while(m_vPitchMarks[0]<=vPitchPeriode[0])
	{
		iter1 = m_vPitchMarks.begin();
		iter2 = vPitchPeriode.begin();

		m_vPitchMarks.erase(iter1);
		vPitchPeriode.erase(iter2);
		if((m_vPitchMarks.size()==0)||(vPitchPeriode.size()==0))
			break;
	}	
	
	long lastpos = m_vPitchMarks.size()-1;
	long lastpos2 = vPitchPeriode.size()-1;
	long inputsignalsize = m_vSignal.size();
	//jika pitch tambah peride terakhir lebih dari data -> hapus pitch mark yang terakhir
	if((m_vPitchMarks[lastpos]+vPitchPeriode[lastpos2])>m_vSignal.size())
	{
		m_vPitchMarks.pop_back();
	}else
	{
		long last = vPitchPeriode.size()-1;
		vPitchPeriode.push_back(vPitchPeriode[last]);
	}
	
	//output signal
	long outputsize = ceil(m_vSignal.size()*dAlpha);
	m_vResult.resize(outputsize);	
	for(i=0;i<outputsize;i++)
	{		
		m_vResult[i] = 0;
	}
	
	//output pitch mark
	double outputpitch = vPitchPeriode[0]+1;
	vector<double> vHann;
	vector<long>::iterator iter;		
	while(Round(outputpitch)<outputsize)
	{
		//indexmin = index dari pitch terdekat ke outputpitch	
		double min = 32766;
		long indexmin=0,index=0;
		for (iter=m_vPitchMarks.begin();iter!=m_vPitchMarks.end();iter++)
		{
			long mark = *iter;
			double dMinAbs = fabs(dAlpha*mark-outputpitch);
			if(min>dMinAbs)
			{
				min=dMinAbs;
				indexmin = index;
			}			
			index++;
		}
		////short time analysis segment start..end 
		
		long pit = vPitchPeriode[indexmin];	
		long start = m_vPitchMarks[indexmin]-pit;
		long end = m_vPitchMarks[indexmin]+pit;		
		vector<short> vResultSegment;
		//superposition short time analysis segment  dengan hanning window
		long i=0,j=0;
		long m = 2*pit+1;		
		vHann.resize(m);
		//double factor = 8.0 * atan( 1.0 ) / ( m - 1 );
		for (i=0;i<m;i++)
		{
			vHann[i] = (double)(0.5 - 0.5*cos((double)(2.0*PI*i)/(m-1)));
			//vHann[i] = (double)(0.5 - 0.5*cos(factor*i));
		}				
		for (i=start;i<=end;i++)
		{			
			if((i<inputsignalsize)&&(j<m))
			{
				short val = m_vSignal[i];					
				vResultSegment.push_back((short)(val*vHann[j]));						
			}else
				break;
			j++;
		}				
	
		//lokasi dari short-time synthesis signal
		long start2, end2;
		start2 = Round(outputpitch)-pit;
		end2 = Round(outputpitch)+pit;
		long segmentsize = vResultSegment.size();
		
		if((end2>outputsize)||(start2<0))
			break;
		j=0;
		
		for (i=start2;i<=end2;i++)
		{					
			if((i<outputsize)&&(j<segmentsize)){
				short d = m_vResult[i];
				long temp = abs(d + vResultSegment[j]);
				if(temp<32766L)
					m_vResult[i] = d + vResultSegment[j];
				else
					m_vResult[i] = 32766;
			}
			else
				break;
			j++;
		}		
		outputpitch = outputpitch+pit/dBeta;				
	}	
	//SmootResult();
}

void CTDPSOLA::AddSignalFromFile(CString sFile)
{	
	CFile finput;
	if(!finput.Open(sFile,CFile::modeRead))
	{
		//MessageBox(NULL,sFile,"NETtalk2",MB_OK);
		return;
	}	
	WAVE_HDR m_win1;
    CHUNK_HDR m_chunkIn1;
    DATA_HDR m_din1;
    WAVEFORMATPE m_wavin1;
	long number_of_samples = GetHeader(&finput,&m_win1,&m_chunkIn1,&m_wavin1,&m_din1);		
	
    GetData(&finput,&m_wavin1,number_of_samples);		
	finput.Close();	
}

long CTDPSOLA::GetHeader(CFile * file, WAVE_HDR *whdr, CHUNK_HDR *chdr, WAVEFORMATPE *wfmt,DATA_HDR * dthdr)
{
	
	file->Read(whdr,sizeof(WAVE_HDR));	
	if(strnicmp(whdr->chunk_id,"RIFF",4) != 0) {		
		MessageBox(NULL,"Error in RIFF header","NETtalk2",MB_OK);
		return 0;
	}	
	file->Read(chdr,sizeof(CHUNK_HDR));
	if(strnicmp(chdr->form_type,"WAVEfmt ",8) != 0) {		
		MessageBox(NULL,"Error in WAVEfmt header","NETtalk2",MB_OK);
		return 0;
	}
	if(chdr->hdr_size != sizeof(WAVEFORMATPE)) {		
		MessageBox(NULL,"Error in WAVEfmt header","NETtalk2",MB_OK);
		return 0;
	}
	file->Read(wfmt,sizeof(WAVEFORMATPE));
	if(wfmt->wFormatTag != WAVE_FORMAT_PCM) {		
		MessageBox(NULL,"Error in WAVEfmt header - not PCM","NETtalk2",MB_OK);
		return 0;
	}
	if(wfmt->nChannels > 4 || wfmt->nBlockAlign > 8) {		
		MessageBox(NULL,"Error in WAVEfmt header - Channels/BlockSize","NETtalk2",MB_OK);
        return 0;
	}
	file->Read(dthdr,sizeof(DATA_HDR));
	char dataTypeCheck[5] = "data";
	for(int i = 0 ; i < 4 ; i++) {
		if(dthdr->data_type[i] != dataTypeCheck[i]) {
			MessageBox(NULL,"Data type not audio 'data'","NETtalk2",MB_OK);
			return 0;
		}
	}
	long number_of_samples = dthdr->data_size/wfmt->nBlockAlign;	
	return number_of_samples;
}

void CTDPSOLA::GetData(CFile *fin, WAVEFORMATPE * wavfmt,long nSamples)
{
	fin->Seek( 44, CFile::begin);		
	short int int_data[4];
	short int j;    	
	WORD nBitPerSample = wavfmt->wBitsPerSample;
	WORD nBlockAlign = wavfmt->nBlockAlign;		
	for(int x=0;x<nSamples;x++)
	{
		//16 bit per sample
		if(nBitPerSample==16){
			if(fin->Read(int_data,nBlockAlign)>0)
			{
				j = int_data[0];			
				m_vSignal.push_back(j);
			}				
		}		
	}					
}
void CTDPSOLA::Reset()
{
	m_vPitchMarks.clear();
	m_vSignal.clear();	
	m_vResult.clear();	
}

void CTDPSOLA::Save2File(CString sFile)
{		
	PSOLA(ALPHA,BETA);	
	CFile fout;
	if(!fout.Open(sFile,CFile::modeCreate|CFile::modeWrite)){		
		return;
	}
	WAVE_HDR wout = { 'R','I','F','F', 0L };
	CHUNK_HDR chunkout = { 'W','A','V','E','f','m','t',' ', sizeof(WAVEFORMATPE) };
	DATA_HDR dout = { 'd','a','t','a', 0L };
	WAVEFORMATPE wavout = { 1, 1, 0L, 0L, 1, 16 };
	wavout.wBitsPerSample = 16;	
	wavout.nChannels = 1;
	wavout.nBlockAlign = wavout.nChannels*wavout.wBitsPerSample/8 ;	
	wavout.nSamplesPerSec = 24000;
	long outputsize = m_vResult.size();
	wavout.nAvgBytesPerSec = wavout.nSamplesPerSec*wavout.nBlockAlign;
	dout.data_size=  outputsize * wavout.nBlockAlign;
	wout.chunk_size=dout.data_size+sizeof(DATA_HDR)+sizeof(CHUNK_HDR)+sizeof(WAVEFORMATPE);

	fout.Write(&wout,sizeof(WAVE_HDR));
	fout.Write(&chunkout,sizeof(CHUNK_HDR));
	fout.Write(&wavout,sizeof(WAVEFORMATPE));
	fout.Write(&dout,sizeof(DATA_HDR));

	int n = wavout.nBlockAlign;
	long i;
	
	for( i = 0 ; i < outputsize ; i++)
	{							
		fout.Write(&m_vResult[i],n);
	}	
	fout.Close();
	
	//sndPlaySound((char*)m_result,SND_MEMORY|SND_SYNC);
	Reset();
}

void CTDPSOLA::WriteLog(CString sText)
{	
	m_fLog.Write(sText,sText.GetLength());
}


