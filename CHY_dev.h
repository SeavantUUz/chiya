// 先不做原生指针
typedef enum {
    CHY_BOOLEAN_VALUE = 1,
    CHY_INT_VALUE,
    CHY_DOUBLE_VALUE,
    CHY_STRING_VALUE,
    CHY_NONE_VALUE
} CHY_ValueType;

typedef struct {
    CHY_ValueType type;
    union {
        CHY_Boolean boolean_value;
        int int_value;
        double double_value;
        CHY_String *string_value;
    } u;
} CHY_Value;
