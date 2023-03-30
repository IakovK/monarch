
// CntrItem.h : interface of the CTextEditorCntrItem class
//

#pragma once

class CTextEditorDoc;
class CTextEditorView;

class CTextEditorCntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(CTextEditorCntrItem)

// Constructors
public:
	CTextEditorCntrItem(REOBJECT* preo = nullptr, CTextEditorDoc* pContainer = nullptr);
		// Note: pContainer is allowed to be null to enable IMPLEMENT_SERIALIZE
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-null document pointer

// Attributes
public:
	CTextEditorDoc* GetDocument()
		{ return reinterpret_cast<CTextEditorDoc*>(CRichEditCntrItem::GetDocument()); }
	CTextEditorView* GetActiveView()
		{ return reinterpret_cast<CTextEditorView*>(CRichEditCntrItem::GetActiveView()); }

// Implementation
public:
	~CTextEditorCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

