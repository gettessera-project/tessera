// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_QT_TESSERAADDRESSVALIDATOR_H
#define TESSERA_QT_TESSERAADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class TesseraAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit TesseraAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

/** Tessera address widget validator, checks for a valid tessera address.
 */
class TesseraAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit TesseraAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

#endif // TESSERA_QT_TESSERAADDRESSVALIDATOR_H
