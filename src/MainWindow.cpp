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


#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "core/Sheet.h"
#include "core/BasicColumns.h"
#include "core/InputDivColumn.h"
#include "core/InputCntColumn.h"
#include "core/OutputAutoSumColumn.h"
#include "core/OutputAutoSplitColumn.h"
#include "core/Invoice.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    SheetModel* pModel = new SheetModel(this);

    //24 rows
    pModel->m_sheet.insertRow();
    pModel->m_sheet.insertRow();
    pModel->m_sheet.insertRow();
    pModel->m_sheet.insertRow();
    pModel->m_sheet.insertRow();
    pModel->m_sheet.insertRow();
    pModel->m_sheet.insertRow();
    pModel->m_sheet.insertRow();
    pModel->m_sheet.insertRow();
    pModel->m_sheet.insertRow();
    pModel->m_sheet.insertRow();
    pModel->m_sheet.insertRow();
    pModel->m_sheet.insertRow();
    pModel->m_sheet.insertRow();
    pModel->m_sheet.insertRow();
    pModel->m_sheet.insertRow();
    pModel->m_sheet.insertRow();
    pModel->m_sheet.insertRow();
    pModel->m_sheet.insertRow();
    pModel->m_sheet.insertRow();
    pModel->m_sheet.insertRow();
    pModel->m_sheet.insertRow();
    pModel->m_sheet.insertRow();
    pModel->m_sheet.insertRow();


    pModel->m_sheet.insertColumn(new StringColumn(pModel->m_sheet, ColId(), "nume"));

    InputDivColumn* pPers = new InputDivColumn(pModel->m_sheet, ColId(), "p");
    pModel->m_sheet.insertColumn(pPers);

    InputCntColumn* pCnt = new InputCntColumn(pModel->m_sheet, ColId(), "ac1");
    pModel->m_sheet.insertColumn(pCnt);

    pModel->m_sheet.insertColumn(new OutputAutoSumColumn(pModel->m_sheet, ColId(), "restante"));

    OutputAutoSplitColumn* pCol = new OutputAutoSplitColumn(pModel->m_sheet, ColId(), "salubr");
    pModel->m_sheet.insertColumn(pCol);
    pCol->addCommonsInput(pPers);   pCol->setCommonsPercent(0);
    pCol->addCountedInput(pCnt);    pCol->setCountedUnits(0);
    pCol->addDividedInput(pPers);
    pCol->addInvoice(new Invoice(352.46, "retim"));
    pCol->splitAmount();

    pModel->m_sheet.insertColumn(new TotalColumn(pModel->m_sheet, ColId(), "TOTAL"));

    ui->tableView->setModel(pModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}
