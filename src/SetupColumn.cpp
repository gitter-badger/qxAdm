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


#include "SetupColumn.h"
#include "ui_SetupColumn.h"

SetupColumn::SetupColumn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupColumn)
{
    ui->setupUi(this);
}

SetupColumn::~SetupColumn()
{
    delete ui;
}
