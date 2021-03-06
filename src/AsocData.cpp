#include "AsocData.h"

#include "qjson/src/parser.h"
#include "qjson/src/serializer.h"

#include <QtCore/QVariant>
#include <QtCore/QVariantMap>
#include <QtCore/QFile>

//struct QVariantConvertException
//{
//  QVariantConvertException(const QString & err) : m_err(err) {}
//private:
//  QString m_err;
//};

////throw exception if can't convert variant to desired type
//QVariantMap toMap(QVariantMap & parent, const QString & name)
//{
//  QVariant v = parent[name];
//  if (!v.isValid())
//    throw QVariantConvertException(name + " field not found");

//  if (!v.canConvert<QVariantMap>())
//    throw QVariantConvertException("can't convert " + name + " to map");

//  return v.toMap();
//}

bool load(const QString & json, AsocData& asoc)
{
  QJson::Parser parser;
  bool ok = true;

  //root object
  QVariantMap root = parser.parse(json.toAscii(), &ok).toMap();
  if (!ok) {
    qFatal("An error occurred during parsing");
    return false;
  }

  QList<QString> inputColumnsText;
  QList<QString> inputColumnsValues;

  //header
  QVariantMap asocHeader  = root["asoc.header"].toMap();
  asoc.headerLeft      = asocHeader["left"].toString();
  asoc.headerCenter    = asocHeader["center"].toString();
  asoc.headerRight     = asocHeader["right"].toString();

  //footer
  QVariantMap asocFooter  = root["asoc.footer"].toMap();
  asoc.footerLeft      = asocFooter["left"].toString();
  asoc.footerCenter    = asocFooter["center"].toString();
  asoc.footerRight     = asocFooter["right"].toString();

  QVariantList sheetsList = root["sheets"].toList();
  foreach (QVariant s, sheetsList)
  {
    QVariantMap sheets = s.toMap(); //array of 'id -> sheet' pairs

    //scan all 'id->sheet' pairs
    QMapIterator<QString, QVariant> it(sheets);
    while(it.hasNext())
    {
      it.next();

      const QString sheetId = it.key();
      QVariantMap sheet = it.value().toMap();

      SheetData sh;
      sh.id = sheetId;

      //sheet.invoices
      foreach (QVariant inv, sheet["invoices"].toList())
      {
        const QVariantMap invoices = inv.toMap();
        for (QVariantMap::const_iterator it = invoices.begin(), itEnd = invoices.end(); it != itEnd; ++it)
        {
          QString invName = it.key();
          double invValue = it.value().toDouble();

          sh.invoices.insert(invName, invValue);
        }
      }

      //sheet.columns
      foreach (QVariant col, sheet["columns"].toList())
      {
        QVariantMap column = col.toMap();

        if (column.isEmpty())
          continue;

        ColumnData cd;
        cd.id = column["id"].toString();
        cd.name = column["name"].toString();
        cd.type = column["type"].toString();
        foreach (QVariant inv, column["invoices"].toList())
        {
          cd.invoices.append(inv.toString());
        }

        if (cd.type.startsWith("input.text"))
        {
          inputColumnsText.append(cd.name);
        }
        else if (cd.type.startsWith("input."))
        {
          inputColumnsValues.append(cd.name);
        }

        QVariantMap commons = column["commons"].toMap();
        if (!commons.isEmpty())
        {
          cd.commonsPercent = commons["percent"].toDouble();
          foreach (QVariant by, commons["by"].toList())
            cd.commonsBy.append(by.toString());
        }

        QVariantMap counted = column["counted"].toMap();
        if (!counted.isEmpty())
        {
          cd.countedUnits = counted["units"].toDouble();
          foreach (QVariant by, counted["by"].toList())
            cd.countedBy.append(by.toString());
        }

        QVariantMap divided = column["divided"].toMap();
        if (!divided.isEmpty())
        {
          foreach (QVariant by, divided["by"].toList())
            cd.dividedBy.append(by.toString());
        }

        sh.columns.insert(cd.id, cd);
      }

      //asoc.lodgers
      foreach (QVariant row, sheet["asoc.lodgers"].toList())
      {
        QVariantMap lodgerItem = row.toMap();

        if (lodgerItem.isEmpty())
          continue;

        LodgerData lodger;
        lodger.id = lodgerItem["id"].toString();
        //lodger.name = lodgerItem["name"].toString();

        foreach(QString inp, inputColumnsText)
        {
          QVariant val = lodgerItem[inp];
          if (!val.isNull())
            lodger.inputText.insert(inp, val.toString());
        }
        foreach(QString inp, inputColumnsValues)
        {
          QVariant val = lodgerItem[inp];
          if (!val.isNull())
            lodger.inputValues.insert(inp, val.toDouble());
        }

        sh.lodgers.append(lodger);
      }

      //store sheet
      asoc.sheets.insert(sh.id, sh);
    }
  }

  return ok;
}

QByteArray save(const AsocData& asoc)
{
  QVariantMap root; //asoc data

  //QList<QString> inputColumnsText;
  //QList<QString> inputColumnsValues;

  //header
  QVariantMap asocHeader;
  asocHeader["left"] = asoc.headerLeft;
  asocHeader["center"] = asoc.headerCenter;
  asocHeader["right"] = asoc.headerRight;

  root["asoc.header"] = asocHeader;

  //footer
  QVariantMap asocFooter;
  asocFooter["left"] = asoc.footerLeft;
  asocFooter["center"] = asoc.footerCenter;
  asocFooter["right"] = asoc.footerRight;

  root["asoc.footer"] = asocFooter;

  //sheets
  QVariantList sheetsList;
  QMapIterator<QString, SheetData> it(asoc.sheets);
  while(it.hasNext())
  {
    it.next();

    const QString & id = it.key();
    const SheetData & sheetData = it.value();

    QVariantMap sheet;

    //sheet.invoices
    QVariantMap invoices;
    QMapIterator<QString, double> its(sheetData.invoices);
    while(its.hasNext())
    {
      its.next();

      const QString & invName = its.key();
      const double & invValue = its.value();

      invoices[invName] = invValue;
    }
    sheet["invoices"] = invoices;


    //sheet.columns
    QVariantList columns;
    QMapIterator<QUuid, ColumnData> itc(sheetData.columns);
    while(itc.hasNext())
    {
      itc.next();

      const ColumnData & cd = itc.value();

      QVariantMap column;
      column["id"] = cd.id.toString();
      column["name"] = cd.name;
      column["type"] = cd.type;

      QVariantList invoices;
      QListIterator<QString> itci(cd.invoices);
      while(itci.hasNext())
      {
        const QString & inv = itci.next();

        invoices.append(inv);
      }
      column["invoices"] = invoices;

//      if (cd.type.startsWith("input.text"))
//      {
//        inputColumnsText.append(cd.name);
//      }
//      else if (cd.type.startsWith("input."))
//      {
//        inputColumnsValues.append(cd.name);
//      }


      if (!cd.commonsBy.isEmpty())
      {
        QVariantMap commons;
        commons["percent"] = cd.commonsPercent;

        QVariantList by;
        foreach (QString b, cd.commonsBy)
          by.append(b);

        commons["by"] = by;

        column["commons"] = commons;
      }

      if (!cd.countedBy.isEmpty())
      {
        QVariantMap counted;

        counted["units"] = cd.countedUnits;

        QVariantList by;
        foreach (QString b, cd.countedBy)
          by.append(b);

        counted["by"] = by;

        column["counted"] = counted;
      }

      if (!cd.dividedBy.isEmpty())
      {
        QVariantMap divided;

        QVariantList by;
        foreach (QString b, cd.dividedBy)
          by.append(b);

        divided["by"] = by;

        column["divided"] = divided;
      }

      columns.append(column);
    }
    sheet["columns"] = columns;



    //asoc.lodgers
    QVariantList lodgers;
    foreach(const LodgerData & lodger, sheetData.lodgers)
    {
      QVariantMap lodgerItem;

      lodgerItem["id"] = lodger.id.toString();

      QMapIterator<QString, QString> itit(lodger.inputText);
      while (itit.hasNext())
      {
        itit.next();

        const QString & inp = itit.key();
        const QString & v = itit.value();

        lodgerItem[inp] = v;
      }

      QMapIterator<QString, double> itiv(lodger.inputValues);
      while (itiv.hasNext())
      {
        itiv.next();

        const QString & inp = itiv.key();
        const double & v = itiv.value();

        lodgerItem[inp] = v;
      }

      lodgers.append(lodgerItem);
    }
    sheet["asoc.lodgers"] = lodgers;


    //add sheet
    QVariantMap item;
    item[id] = sheet;

    sheetsList.append(item);
  }
  root["sheets"] = sheetsList;


  //save
  QJson::Serializer serializer;
  return serializer.serialize(root);
}

InvoiceData::InvoiceData()
  : amount(0)
{
}

ColumnData::ColumnData()
  : commonsPercent(0)
  , countedUnits(0)
{
}

bool AsocData::load(const QString &jsonFilename)
{
  QFile file(jsonFilename);
  if (file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    const QString & asocJson = file.readAll();
    return ::load(asocJson, *this);
  }

  return false;
}

bool AsocData::save(const QString &jsonFilename) const
{
  QFile file(jsonFilename);
  if (file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    const QByteArray & asocJson = ::save(*this);
    return 0 < file.write(asocJson);
  }

  return false;
}
