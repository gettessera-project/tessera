// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_QT_TRANSACTIONDESC_H
#define TESSERA_QT_TRANSACTIONDESC_H

#include <qt/tesseraunits.h>

#include <QObject>
#include <QString>

class TransactionRecord;

namespace interfaces {
class Node;
class Wallet;
struct WalletTx;
struct WalletTxStatus;
}

/** Provide a human-readable extended HTML description of a transaction.
 */
class TransactionDesc: public QObject
{
    Q_OBJECT

public:
    static QString toHTML(interfaces::Node& node, interfaces::Wallet& wallet, TransactionRecord* rec, TesseraUnit unit);

private:
    TransactionDesc() = default;

    static QString FormatTxStatus(const interfaces::WalletTxStatus& status, bool inMempool);
};

#endif // TESSERA_QT_TRANSACTIONDESC_H
