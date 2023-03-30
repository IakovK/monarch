
// CntrItem.cpp : implementation of the CTextEditorCntrItem class
//

#include "pch.h"
#include "framework.h"
#include "TextEditor.h"

#include "TextEditorDoc.h"
#include "TextEditorView.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTextEditorCntrItem implementation

IMPLEMENT_SERIAL(CTextEditorCntrItem, CRichEditCntrItem, 0)

CTextEditorCntrItem::CTextEditorCntrItem(REOBJECT* preo, CTextEditorDoc* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
	// TODO: add one-time construction code here
}

CTextEditorCntrItem::~CTextEditorCntrItem()
{
	// TODO: add cleanup code here
}


// CTextEditorCntrItem diagnostics

#ifdef _DEBUG
void CTextEditorCntrItem::AssertValid() const
{
	CRichEditCntrItem::AssertValid();
}

void CTextEditorCntrItem::Dump(CDumpContext& dc) const
{
	CRichEditCntrItem::Dump(dc);
}
#endif

