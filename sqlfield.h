#ifndef SQLFIELD_H
#define SQLFIELD_H
#include <QString>

class SqlField {
  public:
    enum class FieldType {
        INT,
        FLOAT,
        STRING,
        BINARY,
        NONE
    };
    enum FieldConstranints {
        PRIMARY_KEY = 1,
        AUTO_INCREMENT = 2,
        NOT_NULL = 4,
        NONE = 0
    };
    SqlField();
    QString sql() const;
    void setName(const QString &name);
    void setType(FieldType type);
    void SetConstraints(int constraints);
    QString name() const;
    FieldType type() const;
    int constraints() const;
  private:
    QString m_name;
    FieldType m_type;
    int m_constraints;
};

#endif // SQLFIELD_H
