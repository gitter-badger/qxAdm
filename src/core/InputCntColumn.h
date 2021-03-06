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


#ifndef INPUTCNTCOLUMN_H
#define INPUTCNTCOLUMN_H

#include "BasicColumns.h"

struct ICell;

struct InputCntColumn : public InputColumn
{
  InputCntColumn(const Sheet& sheet, const ColId & colId, const QString & title);
protected:
  virtual ICell* createCell(const RowId& rowId, int index) const;
};

#endif // INPUTCNTCOLUMN_H
