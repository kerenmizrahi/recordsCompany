#ifndef DATA_STRUCTURES_2_WET_2_CUSTOMER_H
#define DATA_STRUCTURES_2_WET_2_CUSTOMER_H


class Customer {
private:
    int c_id;
    int phone;
    bool is_member;
    double month_spendings;

public:
    Customer(int c_id, int phone) : c_id(c_id), phone(phone), is_member(false), month_spendings(0) {};
    int getId() const;
    int getPhone() const;
    bool getIsMember() const;
    void setMember();
    double getMonthSpendings() const;
    void addMonthSpendings(double to_add);
    void setMonthSpendings(double updated);
};



#endif //DATA_STRUCTURES_2_WET_2_CUSTOMER_H
