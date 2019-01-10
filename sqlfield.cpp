#include "sqlfield.h"

SqlField::SqlField() {
    m_name = "";
    m_type = FieldType::NONE;
    m_constraints = FieldConstranints::NONE;
}

QString SqlField::sql() const {
    QString sqlString;
    sqlString += m_name;
    switch (m_type) {
        case FieldType::INT: {
            sqlString += " integer";
            break;
        }
        case FieldType::FLOAT: {
            sqlString += " real";
            break;
        }
        case FieldType::STRING: {
            sqlString += " text";
            break;
        }
        case FieldType::BINARY: {
            sqlString += " blob";
            break;
        }
        case FieldType::NONE: {
            return "";
        }
    }
    if (m_constraints & FieldConstranints::PRIMARY_KEY) {
        sqlString += " primary key";
    }
    if (m_constraints & FieldConstranints::AUTO_INCREMENT) {
        sqlString += " autoincrement";
    }
    if (m_constraints & FieldConstranints::NOT_NULL) {
        sqlString += " not null";
    }
    return sqlString;
}

void SqlField::setName(const QString &name) {
    m_name = name;
}

void SqlField::setType(SqlField::FieldType type) {
    m_type = type;
}

void SqlField::SetConstraints(int constraints) {
    m_constraints = constraints;
}

QString SqlField::name() const {
    return m_name;
}

SqlField::FieldType SqlField::type() const {
    return m_type;
}

int SqlField::constraints() const {
    return m_constraints;
}
