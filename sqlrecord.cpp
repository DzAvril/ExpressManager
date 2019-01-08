#include "sqlrecord.h"

SqlRecord::SqlRecord() {
    m_field = "";
    m_value = QVariant();
}

void SqlRecord::setField(const QString &field) {
    m_field = field;
}

void SqlRecord::setValue(const QVariant &value) {
    m_value = value;
}

QString SqlRecord::field() const {
    return m_field;
}

QVariant SqlRecord::value() const {
    return m_value;
}
