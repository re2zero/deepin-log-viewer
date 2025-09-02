// SPDX-FileCopyrightText: 2021 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef LOGALLEXPORTTHREAD_H
#define LOGALLEXPORTTHREAD_H

#include "structdef.h"
#include "zip.h"

#include <QObject>
#include <QAtomicInt>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    #include <atomic>
#endif

class LogAllExportThread : public QObject
{
    Q_OBJECT
public:
    explicit LogAllExportThread(const QStringList &types, const QString &outfile, QObject *parent = nullptr);

public slots:
    void slot_cancelExport() {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        m_cancel.storeRelaxed(1);
#else
        m_cancel.store(1);
#endif
    }
    void run();

signals:
    void updatecurrentProcess(int current);
    void updateTolProcess(int tol);
    void exportFinsh(bool success = true);

private:
    bool addFileToZip(const QString &filePath, const QString &zipEntryName);
    bool addDataToZip(const QByteArray &data, const QString &zipEntryName);
    bool addProcessOutputToZip(const QString &command, const QStringList &args, const QString &zipEntryName);

    QStringList m_types;
    QString m_outfile {""};
    zipFile m_zipFile {nullptr};

    QAtomicInt m_cancel;

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
private:
    // Qt6兼容性辅助函数
    int atomicLoad() const { return m_cancel.loadRelaxed(); }
    void atomicStore(int value) { m_cancel.storeRelaxed(value); }
#else
private:
    // Qt5兼容性辅助函数
    int atomicLoad() const { return m_cancel.load(); }
    void atomicStore(int value) { m_cancel.store(value); }
#endif
};

#endif // LOGALLEXPORTTHREAD_H