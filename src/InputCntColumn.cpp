/*
 * Copyright (c) 2011, DidiSoft, Timisoara
 * All rights reserved.
 *
 * Acquisition and use of this software and related materials for any
 * purpose requires a written licence agreement from DidiSoft,
 * or a written licence from an organisation licenced by DidiSoft
 * to grant such a licence.
 *
 * $Id:$
 * $Date:$
 * $Rev:$
 */



#include "InputCntColumn.h"

#include "Cell.h"

struct CntCell : public ICell
{
  CntCell(const InputCntColumn& col, const RowId& rowId) : ICell(col, rowId), m_cnt(5), m_diff(4) {}

  virtual bool isEditable() const {return true;}
  virtual bool isNumeric() const {return true;}
  virtual void setData(const variant_t & v) {m_cnt = v.toDouble(); m_diff = m_cnt - 1.0; m_column.notify(m_rowId);} //TODO: compute real delta
  virtual variant_t getData() const {return m_cnt;}

  virtual bool isPartOfTotal() const {return false;}
  virtual numeric_t getValue(const OutputColumn* pColumn) const {return static_cast<const InputCntColumn &>(m_column).getInputValue(m_diff, pColumn, m_rowId);}

private:
  numeric_t m_cnt; //current reading
  numeric_t m_diff; //consumed delta
};


InputCntColumn::InputCntColumn(const Sheet& sheet, const ColId & colId, const QString & title)
  : InputColumn(sheet, colId, title)
{
}

ICell* InputCntColumn::createCell(const RowId& rowId, int index) const
{
  return new CntCell(*this, rowId);
}
