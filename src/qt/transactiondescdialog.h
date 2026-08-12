// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_QT_TRANSACTIONDESCDIALOG_H
#define TESSERA_QT_TRANSACTIONDESCDIALOG_H

#include <QDialog>

namespace Ui {
    class TransactionDescDialog;
}

QT_BEGIN_NAMESPACE
class QModelIndex;
QT_END_NAMESPACE

/** Dialog showing transaction details. */
class TransactionDescDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransactionDescDialog(const QModelIndex &idx, QWidget *parent = nullptr);
    ~TransactionDescDialog();

private:
    Ui::TransactionDescDialog *ui;
};

#endif // TESSERA_QT_TRANSACTIONDESCDIALOG_H
