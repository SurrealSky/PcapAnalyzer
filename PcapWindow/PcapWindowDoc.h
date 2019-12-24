
// PcapWindowDoc.h : CPcapWindowDoc 类的接口
//


#pragma once
#include"include.h"


class CPcapWindowDoc : public CDocument
{
protected: // 仅从序列化创建
	CPcapWindowDoc();
	DECLARE_DYNCREATE(CPcapWindowDoc)

// 特性
public:
	PcapAnalyzer CACap;
	CSessions	mSessions;
	CSyncStream *curStream;
	void *curPacket;
	std::map<std::string, std::string> curResult;
// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CPcapWindowDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
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
