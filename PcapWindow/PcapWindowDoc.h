
// PcapWindowDoc.h : CPcapWindowDoc ��Ľӿ�
//


#pragma once
#include"include.h"


class CPcapWindowDoc : public CDocument
{
protected: // �������л�����
	CPcapWindowDoc();
	DECLARE_DYNCREATE(CPcapWindowDoc)

// ����
public:
	PcapAnalyzer CACap;
	CSessions	mSessions;
	CSyncStream *curStream;
	void *curPacket;
	std::map<std::string, std::string> curResult;
// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CPcapWindowDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	static void _stream_call_handler(void* uParam, void* stream, unsigned int code);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
public:
	void AddStream2StreamView(CSyncStream*);
	void AddPacket2StreamView(CSyncStream*);
	void AddPacket2PacketView(CSyncStream*);
	void Packet2HexView(CSyncPacket*);
	void Result2AnalysisView(std::map<std::string, std::string>);
};
