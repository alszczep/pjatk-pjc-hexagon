#ifndef PJC_HEXAGON_FIELD_H
#define PJC_HEXAGON_FIELD_H

#include "../Consts.h"

namespace Game {
    enum FieldState {
        Empty = 0,
        Blocked = 1,
        Red = 2,
        Blue = 3,
    };

    class Field {
    private:
        FieldState state;
        unsigned short row;
        unsigned short column;
        // index of a column which can be seen in the UI (but 0 based), makes operations on fields easier
        unsigned short uiColumn;
    public:
        Field() = default;

        Field(FieldState state, unsigned short row, unsigned short column);

        void setState(FieldState _state);

        FieldState getState() const;

        unsigned short getRow() const;

        unsigned short getColumn() const;

        unsigned short getUiColumn() const;

        static bool isRowValid(short row);

        static bool isColumnInRowValid(short column, short row);

        static bool isUiColumnValid(short uiColumn);

        /**
         * @return Count of fields in the row, does not take into account empty spaces indexed with uiColumn
         */
        static short columnsInRowByRowIndex(short row);
    };
}


#endif //PJC_HEXAGON_FIELD_H
