/*
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government have not placed any restriction on its use or reproduction.
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
*  Please cite the author in any work or product based on this material.
*
* ===========================================================================
*
*  FileName: CPanelLabSampleThresholds.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_PANEL_LAB_SAMPLE_THRESHOLDS_H__
#define __C_PANEL_LAB_SAMPLE_THRESHOLDS_H__

#include "CPanelLabSettingsUtils.h"
#include "CXMLmessageBook.h"
#include "CGridEdit.h"
#include <wx/checkbox.h>

//********************************************************************
//
//    CGridRFULimits
//

class CGridRFULimits : public nwxGrid
{
public:
  CGridRFULimits(wxWindow *parent, wxWindowID id);
  void SetData(CLabThresholds *pData);
  void SetAllReadOnly(bool bReadOnly = true);
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
private:
  enum
  {
    COL_SAMPLE = 0,
    COL_LADDER,
    COL_ILS,
    COL_COUNT
  };
  enum
  {
    ROW_RFU_MIN = 0,
    ROW_RFU_INTERLOCUS,
    ROW_RFU_MAX,
    ROW_DETECTION,
    ROW_COUNT
  };

  CLabRFU *m_apRFU[COL_COUNT];

  void _SetCellIntValue(int nRow, int nCol, int nValue);
  int _GetCellIntValue(int nRow, int nCol);
  bool _DisabledCell(int nRow, int nCol)
  {
    bool bRtn = 
      ((nCol == COL_ILS) && (nRow == ROW_RFU_INTERLOCUS)) ||
      ((nRow == ROW_DETECTION) && (nCol != COL_SAMPLE));
    return bRtn;
  }
  void _DisableUnused()
  {
    SetReadOnly(ROW_RFU_INTERLOCUS,COL_ILS,true);
    SetReadOnly(ROW_DETECTION,COL_ILS,true);
    SetReadOnly(ROW_DETECTION,COL_LADDER,true);
  }
  CLabThresholds *m_pData;
};

//********************************************************************
//
//    CGridSampleLimits
//
class CGridSampleLimits : public _CGridEdit
{
public:
  CGridSampleLimits(wxWindow *parent, wxWindowID id);
  void SetData(CLabThresholds *pData,
      const CXMLmessageBook *pBook);
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  void SetAllReadOnly(bool bReadOnly)
  {
    SetTableReadOnly(bReadOnly);
  }
  void SetMessageBookRows(int n);
private:
  enum
  {
    ROW_PULLUP = 0,
    ROW_STUTTER,
    ROW_ADENYLATION,
    ROW_OFF_LADDER,
    ROW_RESIDUAL,
    ROW_EXCESSIVE_RESIDUAL,
    ROW_RFU_INCOMPLETE_SAMPLE,
    ROW_MIN_BPS_ARTIFACTS,
    ROW_MESSAGE_BOOK_OFFSET
  };
  nwxGridCellValidator *GetPctValidator()
  {
    if(m_pValidatePct == NULL)
    {
      m_pValidatePct = new nwxGridCellUIntRangeValidator(0,100,true);
    }
    return m_pValidatePct;
  }
  nwxGridCellUIntRangeValidator *m_pValidatePct;
  wxColour m_clrBackground;
  CLabThresholds *m_pData;
  const CXMLmessageBook *m_pMessageBook;
  vector<const CXMLmessageBookSM *> m_vpMsgBookSM;
  //bool m_bReadOnly;
};


//********************************************************************
//
//    CPanelLabSampleThresholds
//

class CPanelLabSampleThresholds : public nwxPanel
{
public:
  CPanelLabSampleThresholds(wxWindow *parent, wxWindowID id);
  virtual void SetReadOnly(bool b = true)
  {
    m_pGrid->SetAllReadOnly(b);
    m_pGridSample->SetAllReadOnly(b);
    m_pAllowOverride->Enable(!b);
  }
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  void SetData(CLabThresholds *pData,
    const CXMLmessageBook *pBook)
  {
    m_pData = pData;
    m_pGrid->SetData(pData);
    m_pGridSample->SetData(pData,pBook);
    Layout();
    Refresh();
  }
private:
  wxCheckBox *m_pAllowOverride;
  CGridRFULimits *m_pGrid;
  CGridSampleLimits *m_pGridSample;
  CLabThresholds *m_pData;
};


#endif
