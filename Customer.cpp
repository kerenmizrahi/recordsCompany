#include "Customer.h"

int Customer::getId() const {
    return c_id;
}

int Customer::getPhone() const {
    return phone;
}

bool Customer::getIsMember() const {
    return is_member;
}

void Customer::setMember(){
    is_member = true;
}

double Customer::getMonthSpendings() const {
    return month_spendings;
}

void Customer::addMonthSpendings(double to_add){
    month_spendings += to_add;
}

void Customer::setMonthSpendings(double updated) {
    month_spendings = updated;
}