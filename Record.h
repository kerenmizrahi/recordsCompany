#ifndef DATA_STRUCTURES_2_WET_2_RECORD_H
#define DATA_STRUCTURES_2_WET_2_RECORD_H
#include <memory>

class Record {
private:
    int r_id;
    int copies;
    int num_purchases;
    int aux;
    int height;
    int record_types;
    int column;
    std::shared_ptr<Record> father;

public:
    Record(int id, int copies):
            r_id(id),
            copies(copies),
            num_purchases(0),
            aux(0), // was =copies
            height(copies),
            record_types(1),
            column(id),
            father(nullptr) {};

    ~Record() { father = nullptr; }

    int getCopies() const;
    int getNumPurchases() const;
    void addPurchase();
    int getAux() const;
    void setAux(int new_aux);
    int getHeight() const;
    void setHeight(int new_height);
    int getRecordTypes() const;
    void setRecordTypes(int types);
    int getColumn() const;
    void setColumn(int col);
    void setFather(std::shared_ptr<Record> new_father);
    std::shared_ptr<Record> getFather() const;
    void resetFather();
    int getId() const;
};

#endif //DATA_STRUCTURES_2_WET_2_RECORD_H
